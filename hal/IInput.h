#pragma once
#include <lvgl.h>

// Interface for any input hardware (touch, mouse, etc.)
class IInput {
public:
    virtual ~IInput() = default;
    virtual void init() = 0;
    virtual void read(lv_indev_drv_t* drv, lv_indev_data_t* data) = 0;
};
