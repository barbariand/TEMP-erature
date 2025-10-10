#include "LilyGoInput.h"
#include <TFT_eSPI.h>

LilyGoInput::LilyGoInput(LilyGo_AMOLED* display) : amoled(display) {}

void LilyGoInput::init() {
    // Initialization, if any, for the touch controller
}

void LilyGoInput::read(lv_indev_drv_t* drv, lv_indev_data_t* data) {
    Point p;
    if (tft->getTouch(p.x, p.y)) {
        data->point.x = p.x;
        data->point.y = p.y;
        last_x = data->point.x;
        last_y = data->point.y;
        data->state = LV_INDEV_STATE_PR;
    } else {
        data->point.x = last_x;
        data->point.y = last_y;
        data->state = LV_INDEV_STATE_REL;
    }
}
