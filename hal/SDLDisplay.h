#pragma once
#include "IDisplay.h"
#include <SDL3/SDL.h>

class SDLDisplay : public IDisplay {
public:
    SDLDisplay(int width, int height);
    ~SDLDisplay();
    void init() override;
    void flush(lv_disp_drv_t* drv, const lv_area_t* area, lv_color_t* color_p) override;

    // Helper for the main loop
    SDL_Renderer* getRenderer();

private:
    int screen_width;
    int screen_height;
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* texture;
};
