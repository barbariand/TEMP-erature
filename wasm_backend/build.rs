use std::env;
use std::path::{Path, PathBuf};

fn main() {
    // ---
    // Part 1: Get Paths
    // ---
    let emscripten_system_path_str = env::var("EMSCRIPTEN_SYSTEM_PATH")
        .expect("EMSCRIPTEN_SYSTEM_PATH not set. Did you update flake.nix shellHook?");
    let emscripten_system_path = PathBuf::from(emscripten_system_path_str);

    let em_include_path = emscripten_system_path.join("include");
    let em_include_path_cxx = emscripten_system_path.join("lib/libcxx/include");
    let em_musl_include_path = emscripten_system_path.join("lib/libc/musl/include");
    let em_musl_arch_include_path = emscripten_system_path.join("lib/libc/musl/arch/emscripten");

    let project_root = PathBuf::from(env::var("CARGO_MANIFEST_DIR").unwrap())
        .canonicalize()
        .unwrap();

    let hal_dir = project_root.join("hal");

    let pio_lvgl_path = project_root.join(Path::new(".pio/libdeps/native/lvgl"));

    if !pio_lvgl_path.is_dir() {
        panic!(
            "PlatformIO LVGL source directory NOT FOUND at: {}. \nIt might be a broken symlink or missing. \nPlease run `platformio run -e native` to ensure dependencies are downloaded correctly.",
            pio_lvgl_path.display()
        );
    }

    let out_dir = PathBuf::from(env::var("OUT_DIR").unwrap());
    let target_dir = project_root.join("target");
    let lvgl_vendor_path = target_dir.join("vendor/lvgl");

    // Copy LVGL source to target dir
    if !lvgl_vendor_path.exists() {
        std::fs::create_dir_all(&lvgl_vendor_path)
            .expect("Failed to create vendor/lvgl dir in target");

        let mut copy_options = fs_extra::dir::CopyOptions::new();
        copy_options.overwrite = true;
        copy_options.content_only = true;

        fs_extra::dir::copy(&pio_lvgl_path, &lvgl_vendor_path, &copy_options)
            .expect("Failed to copy LVGL source contents from .pio to target");
    }

    // ---
    // Part 2: Compile LVGL (C)
    // ---
    let mut lvgl_build = cc::Build::new();
    lvgl_build
        .target("wasm32-unknown-emscripten")
        .compiler("emcc")
        .warnings(false) // <-- Silence C warnings
        .flag("-nostdinc")
        .include(&em_include_path)
        .include(&em_musl_include_path)
        .include(&em_musl_arch_include_path)
        .opt_level(1)
        .include(&lvgl_vendor_path)
        .include(&project_root)
        .define("LV_CONF_PATH", "\"hal/lv_conf.h\"")
        .define("WASM_BUILD", None);

    let c_src_pattern = lvgl_vendor_path.join("src/**/*.c");
    for entry in glob::glob(c_src_pattern.to_str().unwrap()).unwrap() {
        lvgl_build.file(entry.unwrap());
    }

    lvgl_build.compile("lvgl"); // Creates liblvgl.a

    // ---
    // Part 3: Generate Bindings
    // ---
    let bindings = bindgen::Builder::default()
        .header(lvgl_vendor_path.join("lvgl.h").to_str().unwrap())
        .header(
            // Keep explicit header add
            lvgl_vendor_path
                .join("src/display/lv_display.h")
                .to_str()
                .unwrap(),
        )
        .clang_arg(format!("-I{}", project_root.display()))
        .clang_arg(format!("-I{}", lvgl_vendor_path.display()))
        .clang_arg("-DLV_CONF_PATH=\"hal/lv_conf.h\"")
        .clang_arg("-DWASM_BUILD")
        .clang_arg("--target=wasm32-unknown-emscripten")
        .clang_arg(format!("-isystem{}", em_include_path.display()))
        .clang_arg(format!("-isystem{}", em_include_path_cxx.display()))
        .clang_arg(format!("-isystem{}", em_musl_include_path.display()))
        .clang_arg(format!("-isystem{}", em_musl_arch_include_path.display()))
        .clang_arg("-nostdinc")
        .clang_arg("-nostdinc++")
        .use_core()
        .layout_tests(false)
        .ctypes_prefix("core::ffi")
        .allowlist_function("lv_.*")
        .allowlist_type("lv_.*")
        .allowlist_var("lv_.*")
        .allowlist_function("lv_tick_inc") // Keep explicit allowlist
        .allowlist_function("lv_display_flush_ready") // Keep explicit allowlist
        .parse_callbacks(Box::new(bindgen::CargoCallbacks::new())) // Let this handle lvgl linking
        .generate()
        .expect("Unable to generate bindings");

    bindings
        .write_to_file(out_dir.join("bindings.rs"))
        .expect("Couldn't write bindings!");

    // ---
    // Part 4: Compile C++ App
    // ---
    cc::Build::new()
        .cpp(true)
        .target("wasm32-unknown-emscripten")
        .compiler("em++")
        .warnings(false) // <-- Silence C++ warnings
        .opt_level(1)
        .flag("-fno-exceptions")
        .flag("-fno-rtti")
        .flag("-nostdinc++")
        .include(&em_include_path)
        .include(&em_include_path_cxx)
        .include(&em_musl_include_path)
        .include(&em_musl_arch_include_path)
        .define("WASM_BUILD", None)
        .define("LV_CONF_PATH", "\"hal/lv_conf.h\"")
        .include(&lvgl_vendor_path)
        .include(&project_root)
        .include(&hal_dir)
        .include(project_root.join("wasm_backend/include_stubs"))
        .file(project_root.join("src/main.cpp"))
        .file(project_root.join("hal/WASMDisplay.cpp"))
        .compile("app"); // Creates libapp.a

    // --- Linker Instructions ---
    // Ensure search path is added (CargoCallbacks might also do this)
    println!("cargo:rustc-link-search=native={}", out_dir.display());
    // Explicitly link app (CargoCallbacks won't know about this one)
    println!("cargo:rustc-link-lib=app");
    // REMOVED explicit link for lvgl -> let CargoCallbacks handle it
    println!("cargo:rustc-link-lib=lvgl");

    // ---
    // Part 5: Rerun Triggers
    // ---
    println!(
        "cargo:rerun-if-changed={}",
        project_root.join("hal").display()
    );
    println!(
        "cargo:rerun-if-changed={}",
        project_root.join("src").display()
    );
    println!(
        "cargo:rerun-if-changed={}",
        project_root.join("gui").display()
    );
    println!(
        "cargo:rerun-if-changed={}",
        project_root.join("wasm_backend/include_stubs").display()
    );
    // You might still want this if your lv_conf.h source changes
    println!("cargo:rerun-if-changed=../hal/lv_conf.h");
}
