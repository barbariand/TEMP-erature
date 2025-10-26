// This file is compiled with the `wasm32-unknown-emscripten` target.
// It CANNOT use `web-sys`, `js-sys`, or `wasm-bindgen` for DOM interop.
// Instead, it relies on a custom JavaScript library (`library.js`)
// that is linked by Emscripten.

// ---
// 1. Include generated bindings for C/C++
// ---
#[allow(non_upper_case_globals)]
#[allow(non_camel_case_types)]
#[allow(non_snake_case)]
#[allow(dead_code)]
mod lvgl_sys {
    include!(concat!(env!("OUT_DIR"), "/bindings.rs"));
    // manual declaration because bindgen/emscripten might miss it
    #[link(name = "lvgl")] // Link against the C library you built
    unsafe extern "C" {
        pub fn lv_display_flush_ready(disp: *mut lv_display_t);
    }
}

// ---
// 2. Import C++ Functions (from app_setup/app_loop)
// ---
// These are linked by Emscripten from the `.a` file
// created by your `build.rs` script.
unsafe extern "C" {
    pub unsafe fn app_setup();
    pub unsafe fn app_loop();
}

// ---
// 3. Import JavaScript "library" functions
// ---
// These functions are defined in `library.js` and linked by Emscripten
// via the `--js-library` flag. This is the *only* way to
// interact with JavaScript in this environment.
unsafe extern "C" {
    // Initializes canvas listeners and sets up the JS context
    unsafe fn js_bridge_init_listeners();

    // Gets input state from the JS library
    // We pass pointers to write the values back to
    unsafe fn js_bridge_get_input_state(pressed_ptr: *mut bool, x_ptr: *mut i32, y_ptr: *mut i32);

    // Flushes a raw pixel buffer to the HTML canvas
    unsafe fn js_bridge_flush_canvas(x1: i32, y1: i32, width: i32, height: i32, color_p: *const u8);
}

// ---
// 4. WASM Entry Point (Called by JS)
// ---
/// This is the main entry point for our Rust/C++ application.
/// It is called by `Module.onRuntimeInitialized` in `index.html`.
#[unsafe(no_mangle)]
pub unsafe extern "C" fn run_app() {
    unsafe {
        app_setup();
    }
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn wasm_tick_inc() {
    unsafe {
        lvgl_sys::lv_tick_inc;
    }
}

// ---
// 5. Exported Rust Bridge Functions (Called by C++)
// ---
// These are the functions that your C++ `WASMDisplay` class calls.
// They are the "HAL" for LVGL.

/// Called by C++ to initialize the display driver.
/// This function calls *into* JavaScript to set up canvas listeners.
#[unsafe(no_mangle)]
pub unsafe extern "C" fn wasm_bridge_init() {
    // Call the JS function to set up mousedown, mousemove, etc.
    unsafe { js_bridge_init_listeners() };
}

/// Called by C++ (LVGL) to flush a portion of the display buffer
/// to the screen. This function calls *into* JavaScript to draw.
#[unsafe(no_mangle)]
pub unsafe extern "C" fn wasm_bridge_flush(
    disp: *mut lvgl_sys::lv_display_t,
    area: *const lvgl_sys::lv_area_t,
    color_p: *mut u8,
) {
    let (width, height, x1, y1) = {
        let area_ref = unsafe { &*area };
        let w = (area_ref.x2 - area_ref.x1 + 1) as i32;
        let h = (area_ref.y2 - area_ref.y1 + 1) as i32;
        (w, h, area_ref.x1 as i32, area_ref.y1 as i32)
    };

    // Call the JS function to draw the raw pixels
    unsafe { js_bridge_flush_canvas(x1, y1, width, height, color_p) };

    // Tell LVGL we are done flushing
    unsafe { lvgl_sys::lv_display_flush_ready(disp) };
}

/// Called by C++ (LVGL) to read the input device state (mouse).
/// This function calls *into* JavaScript to get mouse coordinates.
#[unsafe(no_mangle)]
pub unsafe extern "C" fn wasm_bridge_read(
    _indev: *mut lvgl_sys::lv_indev_t,
    data: *mut lvgl_sys::lv_indev_data_t,
) {
    // Create variables on the stack to hold the JS state
    let mut pressed: bool = false;
    let mut x: i32 = 0;
    let mut y: i32 = 0;

    // Call JS, passing pointers to our stack variables
    unsafe { js_bridge_get_input_state(&mut pressed, &mut x, &mut y) };

    // Update the LVGL data structure with the new state
    (unsafe { *data }).state = if pressed {
        lvgl_sys::lv_indev_state_t_LV_INDEV_STATE_PRESSED as u32
    } else {
        lvgl_sys::lv_indev_state_t_LV_INDEV_STATE_RELEASED as u32
    };

    (unsafe { *data }).point.x = x as lvgl_sys::lv_coord_t;
    (unsafe { *data }).point.y = y as lvgl_sys::lv_coord_t;
}
