mergeInto(LibraryManager.library, {
js_bridge_init_listeners: function() {
    const canvas = Module.canvas;
    const state = Module.JS_BRIDGE_STATE;
    if (!canvas) {
      console.error("JS Bridge: FATAL: Module.canvas is null! Check index.html.");
      return;
    }

    state.ctx = canvas.getContext('2d');
    if (!state.ctx) {
        console.error("JS Bridge: FATAL: canvas.getContext('2d') failed!");
        return;
    }

    const updateCoords = (e) => {
      const rect = canvas.getBoundingClientRect();
      state.x = Math.round(((e.clientX - rect.left) / rect.width) * canvas.width);
      state.y = Math.round(((e.clientY - rect.top) / rect.height) * canvas.height);
    };

    canvas.addEventListener('mousedown', (e) => {
        updateCoords(e);
        state.pressed = true;
    });

    window.addEventListener('mousemove', (e) => {
        if (state.pressed) {
            updateCoords(e);
        }
    });


    window.addEventListener('mouseup', (e) => {
        if (state.pressed) {
            updateCoords(e);
            state.pressed = false;
        }
    });

    canvas.addEventListener('mousemove', (e) => {
        if (!state.pressed) {
            updateCoords(e);
        }
    });

    const updateTouchState = (e, isPressed) => {
      e.preventDefault();
      let touch;
      if (e.touches.length > 0) {
        touch = e.touches[0];
      } else if (e.changedTouches.length > 0) {
        touch = e.changedTouches[0];
      } else {
        state.pressed = isPressed;
        return;
      }

      const rect = canvas.getBoundingClientRect();
      state.x = Math.round(((touch.clientX - rect.left) / rect.width) * canvas.width);
      state.y = Math.round(((touch.clientY - rect.top) / rect.height) * canvas.height);
      state.pressed = isPressed;
    };

    canvas.addEventListener('touchstart', (e) => updateTouchState(e, true));
    canvas.addEventListener('touchmove', (e) => updateTouchState(e, true));
    canvas.addEventListener('touchend', (e) => updateTouchState(e, false));
    canvas.addEventListener('touchcancel', (e) => updateTouchState(e, false));

    console.log("JS Bridge: Listeners (Manual State Logic) initialized.");
  },

  js_bridge_get_input_state: function(pressed_ptr, x_ptr, y_ptr) {
const state = Module.JS_BRIDGE_STATE;

    HEAP8[pressed_ptr] = state.pressed ? 1 : 0;
    HEAP32[x_ptr >> 2] = state.x;
    HEAP32[y_ptr >> 2] = state.y;
  },

  js_bridge_flush_canvas: function(x1, y1, width, height, color_p) {

    const state = Module.JS_BRIDGE_STATE;
    if (!state.ctx) {
      console.warn("JS Bridge: trying to flush, but context is null.");
      return;
    }

    const size = width * height;
    const rgba_data = new Uint8ClampedArray(size * 4);
    const color_data = HEAPU8.subarray(color_p, color_p + size * 3);

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
