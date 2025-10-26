mergeInto(LibraryManager.library, {
  js_bridge_init_listeners: function() {
    const state = Module.JS_BRIDGE_STATE;
    const canvas = Module.canvas;
    if (!canvas) {
      console.error("JS Bridge: Canvas not found on Module!");
      return;
    }
    state.ctx = Module.canvas.getContext('2d');
    const updateState = (e, isPressed) => {
      const rect = canvas.getBoundingClientRect();
      state.x = Math.round(((e.clientX - rect.left) / rect.width) * canvas.width);
      state.y = Math.round(((e.clientY - rect.top) / rect.height) * canvas.height);
      state.pressed = isPressed;
    };
    canvas.addEventListener('mousedown', (e) => updateState(e, true));
    canvas.addEventListener('mousemove', (e) => {

      if (state.pressed) updateState(e, true);
    });
    canvas.addEventListener('mouseup', (e) => updateState(e, false));
    canvas.addEventListener('mouseout', (e) => updateState(e, false));
    const updateTouchState = (e, isPressed) => {
      e.preventDefault();
      if (e.touches.length > 0) {
        const touch = e.touches[0];

        const rect = canvas.getBoundingClientRect();
        state.x = Math.round(((touch.clientX - rect.left) / rect.width) * canvas.width);
        state.y = Math.round(((touch.clientY - rect.top) / rect.height) * canvas.height);
        state.pressed = isPressed;
      } else {


        if (e.changedTouches.length > 0) {
            updateState(e.changedTouches[0], isPressed);
        } else {
            state.pressed = isPressed;
        }
      }
    };
    canvas.addEventListener('touchstart', (e) => updateTouchState(e, true));
    canvas.addEventListener('touchmove', (e) => updateTouchState(e, true));
    canvas.addEventListener('touchend', (e) => updateTouchState(e, false));
    canvas.addEventListener('touchcancel', (e) => updateTouchState(e, false));
    console.log("JS Bridge: Listeners initialized.");
  },
  js_bridge_get_input_state: function(pressed_ptr, x_ptr, y_ptr) {
    const state = Module.JS_BRIDGE_STATE;
    Module.HEAP8[pressed_ptr] = state.pressed ? 1 : 0;
    Module.HEAP32[x_ptr >> 2] = state.x;
    Module.HEAP32[y_ptr >> 2] = state.y;
  },

  js_bridge_flush_canvas: function(x1, y1, width, height, color_p) {
    const state = Module.JS_BRIDGE_STATE;
    if (!state.ctx) {
      console.warn("JS Bridge: trying to flush, but context is null.");
      return;
    }
    const size = width * height;
    const rgba_data = new Uint8ClampedArray(size * 4);
    const color_data = Module.HEAPU8.subarray(color_p, color_p + size * 3);

    for (let i = 0; i < size; i++) {
      rgba_data[i * 4 + 0] = color_data[i * 3 + 2];
      rgba_data[i * 4 + 1] = color_data[i * 3 + 1];
      rgba_data[i * 4 + 2] = color_data[i * 3 + 0];
      rgba_data[i * 4 + 3] = 255;
    }

    const image_data = new ImageData(rgba_data, width, height);
    state.ctx.putImageData(image_data, x1, y1);
  },
});
