
#ifndef LVGL_WIDGET_HPP
#define LVGL_WIDGET_HPP

extern "C" {
#include "lvgl.h"
}

namespace LVGL_Wrapper {

class BaseWidget {
 protected:
  lv_obj_t* m_obj;
  explicit BaseWidget(lv_obj_t* obj);

 public:
  // Virtual destructor for proper polymorphic cleanup
  virtual ~BaseWidget();

  // Getter for the raw LVGL object pointer
  lv_obj_t* raw() const;
};
class Widget : public BaseWidget {
 protected:
  Widget(lv_obj_t* m_obj);
};

}  // namespace LVGL_Wrapper

#endif  // LVGL_WIDGET_HPP
