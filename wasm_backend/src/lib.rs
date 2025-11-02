use std::sync::atomic::{AtomicBool, AtomicI32, Ordering};

#[allow(non_upper_case_globals)]
#[allow(non_camel_case_types)]
#[allow(non_snake_case)]
#[allow(dead_code)]
mod lvgl_sys {
    include!(concat!(env!("OUT_DIR"), "/bindings.rs"));

    #[link(name = "lvgl")]
    unsafe extern "C" {
        pub unsafe fn lv_display_flush_ready(disp: *mut lv_display_t);

        pub unsafe fn lv_tick_inc(tick_period: u32);
    }
}

static DRIVER_STATE: AtomicBool = AtomicBool::new(false);
static DRIVER_X: AtomicI32 = AtomicI32::new(0);
static DRIVER_Y: AtomicI32 = AtomicI32::new(0);

unsafe extern "C" {
    pub unsafe fn app_setup();
    pub unsafe fn app_loop();
}

unsafe extern "C" {

    unsafe fn js_bridge_init_listeners();

    unsafe fn js_bridge_get_input_state(pressed_ptr: *mut u8, x_ptr: *mut i32, y_ptr: *mut i32);

    unsafe fn js_bridge_flush_canvas(x1: i32, y1: i32, width: i32, height: i32, color_p: *const u8);
}

#[unsafe(no_mangle)]
pub unsafe extern "C" fn run_loop() {
    unsafe {
        app_loop();
    }
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn run_app() {
    unsafe {
        app_setup();
    }
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn wasm_tick_inc() {
    unsafe {
        lvgl_sys::lv_tick_inc(5);
    }
}

#[unsafe(no_mangle)]
pub unsafe extern "C" fn wasm_bridge_init() {
    unsafe { js_bridge_init_listeners() };
}

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

    unsafe { js_bridge_flush_canvas(x1, y1, width, height, color_p) };

    unsafe { lvgl_sys::lv_display_flush_ready(disp) };
}

#[unsafe(no_mangle)]
pub unsafe extern "C" fn wasm_bridge_get_input_data(
    out_state: *mut u32,
    out_x: *mut i32,
    out_y: *mut i32,
) {
    let mut js_pressed_raw: u8 = 0;
    let mut js_x: i32 = 0;
    let mut js_y: i32 = 0;
    unsafe { js_bridge_get_input_state(&mut js_pressed_raw, &mut js_x, &mut js_y) };
    let js_pressed: bool = js_pressed_raw != 0;

    if js_pressed {
        DRIVER_STATE.store(true, Ordering::Relaxed);
        DRIVER_X.store(js_x, Ordering::Relaxed);
        DRIVER_Y.store(js_y, Ordering::Relaxed);
    } else {
        DRIVER_STATE.store(false, Ordering::Relaxed);
    }

    let is_pressed = DRIVER_STATE.load(Ordering::Relaxed);
    let final_x = DRIVER_X.load(Ordering::Relaxed);
    let final_y = DRIVER_Y.load(Ordering::Relaxed);

    unsafe {
        *out_x = final_x;
        *out_y = final_y;
        *out_state = if is_pressed {
            lvgl_sys::lv_indev_state_t_LV_INDEV_STATE_PRESSED as u32
        } else {
            lvgl_sys::lv_indev_state_t_LV_INDEV_STATE_RELEASED as u32
        };
    }
}
