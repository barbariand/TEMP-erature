#pragma once

extern "C" {
#include "lvgl.h"
}

#include <algorithm>
#include <functional>
#include <map>
#include <memory>
#include <vector>

namespace LVGL_Wrapper {

using LvCreateFunc = lv_obj_t* (*)(lv_obj_t* parent);

class BaseWidget : public std::enable_shared_from_this<BaseWidget> {
  friend class Screen;

 protected:
  lv_obj_t* m_obj;
  std::weak_ptr<BaseWidget> m_parent;
  std::vector<std::shared_ptr<BaseWidget>> m_children;
  std::map<lv_event_code_t, std::function<void(lv_event_t*)>> m_callbacks;

  void wrap(lv_obj_t* obj) {
    m_obj = obj;
    if (m_obj) {
      lv_obj_set_user_data(m_obj, this);

      lv_obj_add_event_cb(m_obj, universal_event_handler, LV_EVENT_DELETE,
                          nullptr);
    }
  }

  BaseWidget(LvCreateFunc create_func, BaseWidget* parent_raw)
      : m_obj(nullptr), m_parent() {
    lv_obj_t* c_obj = create_func(parent_raw ? parent_raw->raw() : nullptr);
    this->wrap(c_obj);
  }

  explicit BaseWidget(lv_obj_t* obj_to_wrap) : m_obj(nullptr), m_parent() {
    this->wrap(obj_to_wrap);
  }

  void remove_child_from_list(BaseWidget* child_ptr) {
    auto it = std::find_if(
        m_children.begin(), m_children.end(),
        [child_ptr](const auto& p) { return p.get() == child_ptr; });

    if (it != m_children.end()) {
      m_children.erase(it);
    }
  }

  BaseWidget(lv_obj_t* obj_to_wrap, BaseWidget* parent)
      : m_obj(nullptr), m_parent() {
    this->wrap(obj_to_wrap);
  }
  static void universal_event_handler(lv_event_t* e) {
    BaseWidget* widget = static_cast<BaseWidget*>(lv_event_get_user_data(e));
    if (!widget)
      return;

    lv_event_code_t code = lv_event_get_code(e);
    if (code == LV_EVENT_DELETE) {
      widget->m_obj = nullptr;

      if (auto parent_ptr = widget->m_parent.lock()) {
        parent_ptr->remove_child_from_list(widget);
      }
      return;
    }

    auto it = widget->m_callbacks.find(code);
    if (it != widget->m_callbacks.end()) {
      it->second(e);
    }
  }

 public:
  virtual ~BaseWidget() {
    m_children.clear();

    if (m_obj) {
      if (m_obj != lv_scr_act()) {
        lv_obj_del(m_obj);
      }
      m_obj = nullptr;
    }
  }

  BaseWidget(const BaseWidget&) = delete;
  BaseWidget& operator=(const BaseWidget&) = delete;

  lv_obj_t* raw() const { return m_obj; }
  std::shared_ptr<BaseWidget> parent() const { return m_parent.lock(); }

  void register_child(std::shared_ptr<BaseWidget> child) {
    m_children.push_back(child);
    child->m_parent = this->shared_from_this();
  }

  // Removed destroy_child - lifetime is managed by shared_ptr
};

}  // namespace LVGL_Wrapper
