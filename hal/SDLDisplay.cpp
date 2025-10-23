#ifndef ARDUINO
#include "SDLDisplay.hpp"
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_version.h>
#include <iostream>

// --- Definiera de statiska buffertarna utanför klassen ---
lv_color_t hal::SDLDisplay::buf1[800 * hal::SDLDisplay::BUF_LINES];
lv_color_t hal::SDLDisplay::buf2[800 * hal::SDLDisplay::BUF_LINES];

namespace hal {

SDLDisplay::SDLDisplay()
    : screen_width(800),
      screen_height(600),
      window(nullptr),
      renderer(nullptr),
      texture(nullptr),
      disp_drv(nullptr)  // Initiera driver-pekaren
{}

SDLDisplay::~SDLDisplay() {
  // LVGL-städning skulle hända separat om det behövs
  if (texture)
    SDL_DestroyTexture(texture);
  if (renderer)
    SDL_DestroyRenderer(renderer);
  if (window)
    SDL_DestroyWindow(window);
  SDL_Quit();
  std::cout << "SDLDisplay destroyed." << std::endl;
}

bool SDLDisplay::init() {
  std::cout << "Initializing SDLDisplay..." << std::endl;

  // --- SDL-Initialisering ---
  // SDL2: SDL_Init returnerar 0 vid framgång, negativt vid fel.
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    SDL_Log("SDL could not initialize! SDL_Error: %s", SDL_GetError());
    return false;
  }

  // --- Logga SDL-Version EFTER lyckad init ---
  SDL_version ver;
  SDL_VERSION(&ver);  // Använd SDL_VERSION-makrot för SDL2
  SDL_Log("Using SDL Version: %d.%d.%d", ver.major, ver.minor, ver.patch);

  // SDL2: SDL_CreateWindow kräver X, Y-positioner.
  window = SDL_CreateWindow("LVGL SDL2 Simulation", SDL_WINDOWPOS_CENTERED,
                            SDL_WINDOWPOS_CENTERED, screen_width, screen_height,
                            SDL_WINDOW_SHOWN);  // Använd SDL_WINDOW_SHOWN
  if (!window) {
    SDL_Log("Window could not be created! SDL_Error: %s", SDL_GetError());
    SDL_Quit();
    return false;
  }

  // SDL2: Använd -1 för att auto-välja driver, 0 för inga flaggor.
  renderer = SDL_CreateRenderer(window, -1, 0);
  if (!renderer) {
    SDL_Log("Renderer could not be created! SDL_Error: %s", SDL_GetError());
    SDL_DestroyWindow(window);
    SDL_Quit();
    return false;
  } else {
    // SDL2: Hämta info om renderaren
    SDL_RendererInfo info;
    if (SDL_GetRendererInfo(renderer, &info) == 0) {
      SDL_Log("--- Using Renderer: %s ---", info.name ? info.name : "Unknown");
    }
  }

  // SDL2: Använd SDL_TEXTUREACCESS_STREAMING
  texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB565,
                              SDL_TEXTUREACCESS_STREAMING, screen_width,
                              screen_height);

  SDL_Log("SDL_CreateTexture result: %p", (void*)texture);
  if (!texture) {
    SDL_Log("Texture could not be created! SDL_Error: %s", SDL_GetError());
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return false;
  }

  // --- LVGL-Initialisering ---
  std::cout << "Initializing LVGL..." << std::endl;
  lv_init();
  std::cout << "LVGL Initialized." << std::endl;

  // --- Skapa LVGL Display Driver (EFTER lv_init) ---
  disp_drv = lv_display_create(screen_width, screen_height);
  if (!disp_drv) {
    SDL_Log("LVGL display create failed!");
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return false;
  }
  std::cout << "LVGL Display Driver Created." << std::endl;

  // --- Sätt upp display-drivern i RÄTT ORDNING (v9) ---

  // RÄTTELSE: v9 förväntar sig buffertstorleken i BYTES, inte pixlar
  uint32_t buf_size_bytes = screen_width * BUF_LINES * sizeof(lv_color_t);

  // 1. Sätt flush callback FÖRST
  lv_display_set_flush_cb(disp_drv, flush_cb_static);

  // 2. Sätt buffertarna SEN
  // RÄTTELSE: Använd FULL-mode eftersom vi har fullstora buffertar
  lv_display_set_buffers(disp_drv, buf1, buf2, buf_size_bytes,
                         LV_DISPLAY_RENDER_MODE_FULL);

  // 3. Sätt användardata
  lv_display_set_user_data(disp_drv, this);  // v9-metod
  SDL_Log("Setting user_data in driver to: %p", (void*)this);

  // I v9 är disp_drv *redan* lv_disp_t*. Ingen separat registrering.
  lv_disp_t* disp = disp_drv;

  std::cout << "LVGL Display Driver Setup Complete." << std::endl;
  SDL_Log("Init running on thread ID: %lu",
          SDL_ThreadID());  // SDL2: SDL_ThreadID()

  // Skapa en default-grupp
  lv_group_t* group = lv_group_create();
  lv_group_set_default(group);

  // Registrera SDL-mus (v9)
  lv_indev_t* mouse_indev = lv_sdl_mouse_create();
  if (mouse_indev) {
    lv_indev_set_group(mouse_indev, group);
    lv_indev_set_disp(mouse_indev, disp);
  } else {
    std::cerr << "Error: Failed to create SDL mouse input device!" << std::endl;
  }

  // Valfritt: Registrera mushjul (v9)
  lv_indev_t* mousewheel_indev = lv_sdl_mousewheel_create();
  if (mousewheel_indev) {
    lv_indev_set_group(mousewheel_indev, group);
    lv_indev_set_disp(mousewheel_indev, disp);
  } else {
    std::cerr << "Warning: Failed to create SDL mousewheel input device."
              << std::endl;
  }

  // Valfritt: Registrera tangentbord (v9)
  lv_indev_t* keyboard_indev = lv_sdl_keyboard_create();
  if (keyboard_indev) {
    lv_indev_set_group(keyboard_indev, group);
    lv_indev_set_disp(keyboard_indev, disp);
  } else {
    std::cerr << "Warning: Failed to create SDL keyboard input device."
              << std::endl;
  }
  // --- Slut LVGL Input ---
  return true;
}

// --- Statisk Callback Bridge (LVGL v9) ---
void SDLDisplay::flush_cb_static(
    lv_display_t* drv, const lv_area_t* area,
    uint8_t* color_p) {  // Korrekt typ per LVGL typedef
  // v9-metod för att hämta användardata
  SDLDisplay* display = static_cast<SDLDisplay*>(lv_display_get_user_data(drv));
  if (display) {
    display->flush_display(drv, area, color_p);
  } else {
    SDL_Log("Error: user_data not found in flush_cb_static!");
    lv_display_flush_ready(drv);  // v9 flush ready
  }
}

// --- Medlems-Flush-Funktion ---
void SDLDisplay::flush_display(
    lv_display_t* drv, const lv_area_t* area,
    uint8_t* color_p) {  // Korrekt typ per LVGL typedef

  if (!renderer || !texture || !drv || !area || !color_p) {
    SDL_Log(
        "Error: NULL pointer passed to flush_display (renderer=%p, "
        "texture=%p)!",
        (void*)renderer, (void*)texture);
    if (drv)
      lv_display_flush_ready(drv);  // v9 flush ready
    return;
  }

  SDL_ClearError();

  int tex_w, tex_h;  // SDL2 använder int
  // SDL2: Använd SDL_QueryTexture, returnerar 0 vid framgång.
  int query_result = SDL_QueryTexture(texture, NULL, NULL, &tex_w, &tex_h);
  if (query_result != 0) {  // Kolla efter icke-noll (fel)
    const char* sdl_error = SDL_GetError();
    SDL_Log("SDL_QueryTexture failed! Error: %s",
            sdl_error ? sdl_error : "None");
    lv_display_flush_ready(drv);
    return;
  }

  // Ingen float-casting behövs
  int x1 = (int)area->x1;
  int y1 = (int)area->y1;
  int x2 = (int)area->x2;
  int y2 = (int)area->y2;

  if (x1 >= tex_w || y1 >= tex_h || x2 < 0 || y2 < 0) {
    lv_display_flush_ready(drv);
    return;
  }

  x1 = (x1 < 0) ? 0 : x1;
  y1 = (y1 < 0) ? 0 : y1;
  x2 = (x2 >= tex_w) ? tex_w - 1 : x2;
  y2 = (y2 >= tex_h) ? tex_h - 1 : y2;

  int w = x2 - x1 + 1;
  int h = y2 - y1 + 1;

  if (w <= 0 || h <= 0) {
    lv_display_flush_ready(drv);
    return;
  }

  SDL_Rect r = {x1, y1, w, h};

  // RÄTTELSE: Pitchen (stride) för källdatan (color_p) är ALLTID
  // den fulla bredden på bufferten den ritades till (buf1/buf2),
  // vilket är `screen_width` pixlar brett.
  int source_pitch = screen_width * sizeof(lv_color_t);

  // SDL2: SDL_UpdateTexture returnerar 0 vid framgång.
  if (SDL_UpdateTexture(texture, &r, (const void*)color_p, source_pitch) != 0) {
    SDL_Log("SDL_UpdateTexture failed: %s", SDL_GetError());
    lv_display_flush_ready(drv);
    return;
  }

  // SDL2: Använd SDL_RenderCopy, returnerar 0 vid framgång.
  if (SDL_RenderCopy(renderer, texture, NULL, NULL) != 0) {
    SDL_Log("SDL_RenderCopy failed: %s", SDL_GetError());
    lv_display_flush_ready(drv);
    return;
  }

  // SDL2: SDL_RenderPresent returnerar void.
  SDL_RenderPresent(renderer);

  lv_display_flush_ready(drv);  // v9 flush ready
}

// --- Andra Metoder ---
void SDLDisplay::setBrightness(uint8_t brightness) {
  (void)brightness;
}
void SDLDisplay::sleep() { /* SDL_HideWindow(window); */ }
void SDLDisplay::wakeup() { /* SDL_ShowWindow(window); */ }
SDL_Renderer* SDLDisplay::getRenderer() {
  return renderer;
}
int SDLDisplay::handle_events() {

  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    // SDL2: Använd SDL_QUIT
    if (event.type == SDL_QUIT) {
      return 1;  // Signalera att programmet ska avslutas
    }
    // LVGL:s SDL-funktioner (lv_sdl_..._create) sätter upp en egen
    // timer/task som anropar SDL_PollEvent internt.
    // Att polla här igen kan störa dem.
    // Men om LVGL *inte* pollar, måste du skicka events till LVGL här.
    // För nu låter vi den vara, men om musen inte funkar, titta här.
  }
  return 0;  // Signalera att fortsätta
}

}  // namespace hal
#endif  // ARDUINO
