#pragma once
#include <lvgl.h>
#include <cstdint>

// Interface for any display hardware
class IDisplay {
public:
    virtual ~IDisplay() = default;
    virtual void init() = 0;
    virtual void flush(lv_disp_drv_t* drv, const lv_area_t* area, lv_color_t* color_p) = 0;
    virtual void setBrightness(uint8_t brightness) = 0;
    virtual void sleep() = 0;
    virtual void wakeup() = 0;
};
