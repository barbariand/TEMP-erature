#pragma once

extern "C" {
#include "lvgl.h"
}

namespace LVGL_Wrapper {

namespace Align {
enum Type : uint8_t {
  Default = LV_ALIGN_DEFAULT,
  TopLeft = LV_ALIGN_TOP_LEFT,
  TopMid = LV_ALIGN_TOP_MID,
  TopRight = LV_ALIGN_TOP_RIGHT,
  MidLeft = LV_ALIGN_LEFT_MID,
  Center = LV_ALIGN_CENTER,
  MidRight = LV_ALIGN_RIGHT_MID,
  BottomLeft = LV_ALIGN_BOTTOM_LEFT,
  BottomMid = LV_ALIGN_BOTTOM_MID,
  BottomRight = LV_ALIGN_BOTTOM_RIGHT,
  OutTopLeft = LV_ALIGN_OUT_TOP_LEFT,
  OutTopMid = LV_ALIGN_OUT_TOP_MID,
  OutTopRight = LV_ALIGN_OUT_TOP_RIGHT,
  OutMidLeft = LV_ALIGN_OUT_LEFT_MID,
  OutMidRight = LV_ALIGN_OUT_RIGHT_MID,
  OutBottomLeft = LV_ALIGN_OUT_BOTTOM_LEFT,
  OutBottomMid = LV_ALIGN_OUT_BOTTOM_MID,
  OutBottomRight = LV_ALIGN_OUT_BOTTOM_RIGHT,
};
}

namespace Flag {
enum Type : uint32_t {
  Hidden = LV_OBJ_FLAG_HIDDEN,
  Clickable = LV_OBJ_FLAG_CLICKABLE,
  ClickFocusable = LV_OBJ_FLAG_CLICK_FOCUSABLE,
  Checkable = LV_OBJ_FLAG_CHECKABLE,
  Scrollable = LV_OBJ_FLAG_SCROLLABLE,
  ScrollElastic = LV_OBJ_FLAG_SCROLL_ELASTIC,
  ScrollMomentum = LV_OBJ_FLAG_SCROLL_MOMENTUM,
  ScrollOnFocus = LV_OBJ_FLAG_SCROLL_ON_FOCUS,
  ScrollChain = LV_OBJ_FLAG_SCROLL_CHAIN,
  ScrollWithArrow = LV_OBJ_FLAG_SCROLL_WITH_ARROW,
  GestureBubble = LV_OBJ_FLAG_GESTURE_BUBBLE,
  EventBubble = LV_OBJ_FLAG_EVENT_BUBBLE,
  Widget1 = LV_OBJ_FLAG_WIDGET_1,
  Widget2 = LV_OBJ_FLAG_WIDGET_2,
  User1 = LV_OBJ_FLAG_USER_1,
  User2 = LV_OBJ_FLAG_USER_2,
  User3 = LV_OBJ_FLAG_USER_3,
  User4 = LV_OBJ_FLAG_USER_4,
};
}

namespace State {
enum Type : uint16_t {
  Default = LV_STATE_DEFAULT,
  Checked = LV_STATE_CHECKED,
  Focused = LV_STATE_FOCUSED,
  FocusKey = LV_STATE_FOCUS_KEY,
  Edited = LV_STATE_EDITED,
  Hovered = LV_STATE_HOVERED,
  Pressed = LV_STATE_PRESSED,
  Scrolled = LV_STATE_SCROLLED,
  Disabled = LV_STATE_DISABLED,
  User1 = LV_STATE_USER_1,
  User2 = LV_STATE_USER_2,
  User3 = LV_STATE_USER_3,
  User4 = LV_STATE_USER_4,
  Any = LV_STATE_ANY,
};
}

namespace Part {
enum Type : uint32_t {
  Main = LV_PART_MAIN,
  Scrollbar = LV_PART_SCROLLBAR,
  Indicator = LV_PART_INDICATOR,
  Knob = LV_PART_KNOB,
  Selected = LV_PART_SELECTED,
  Items = LV_PART_ITEMS,
  Cursor = LV_PART_CURSOR,
  CustomFirst = LV_PART_CUSTOM_FIRST,
  Any = LV_PART_ANY,
};
}

namespace Event {
enum Type : int32_t {
  Pressed = LV_EVENT_PRESSED,
  Pressing = LV_EVENT_PRESSING,
  PressLost = LV_EVENT_PRESS_LOST,
  ShortClicked = LV_EVENT_SHORT_CLICKED,
  LongPressed = LV_EVENT_LONG_PRESSED,
  LongPressedRepeat = LV_EVENT_LONG_PRESSED_REPEAT,
  Clicked = LV_EVENT_CLICKED,
  Released = LV_EVENT_RELEASED,
  ScrollBegin = LV_EVENT_SCROLL_BEGIN,
  ScrollEnd = LV_EVENT_SCROLL_END,
  Scroll = LV_EVENT_SCROLL,
  Gesture = LV_EVENT_GESTURE,
  Key = LV_EVENT_KEY,
  Focused = LV_EVENT_FOCUSED,
  Defocused = LV_EVENT_DEFOCUSED,
  Leave = LV_EVENT_LEAVE,
  HitTest = LV_EVENT_HIT_TEST,
  ValueChanged = LV_EVENT_VALUE_CHANGED,
  Insert = LV_EVENT_INSERT,
  Refresh = LV_EVENT_REFRESH,
  Ready = LV_EVENT_READY,
  Cancel = LV_EVENT_CANCEL,
  Delete = LV_EVENT_DELETE,
  ChildChanged = LV_EVENT_CHILD_CHANGED,
  DrawMainBegin = LV_EVENT_DRAW_MAIN_BEGIN,
  DrawMain = LV_EVENT_DRAW_MAIN,
  DrawMainEnd = LV_EVENT_DRAW_MAIN_END,
  DrawPostBegin = LV_EVENT_DRAW_POST_BEGIN,
  DrawPost = LV_EVENT_DRAW_POST,
  DrawPostEnd = LV_EVENT_DRAW_POST_END,
};
}

namespace Direction {
enum Type : uint8_t {
  None = LV_DIR_NONE,
  Left = LV_DIR_LEFT,
  Right = LV_DIR_RIGHT,
  Top = LV_DIR_TOP,
  Bottom = LV_DIR_BOTTOM,
  Horizontal = LV_DIR_HOR,
  Vertical = LV_DIR_VER,
  All = LV_DIR_ALL,
};
}

namespace ScrollbarMode {
enum Type : uint8_t {
  Off = LV_SCROLLBAR_MODE_OFF,
  On = LV_SCROLLBAR_MODE_ON,
  Active = LV_SCROLLBAR_MODE_ACTIVE,
  Auto = LV_SCROLLBAR_MODE_AUTO,
};
}

namespace Opa {
enum Type : uint8_t {
  Transparent = LV_OPA_TRANSP,
  Opa0 = LV_OPA_0,
  Opa10 = LV_OPA_10,
  Opa20 = LV_OPA_20,
  Opa30 = LV_OPA_30,
  Opa40 = LV_OPA_40,
  Opa50 = LV_OPA_50,
  Opa60 = LV_OPA_60,
  Opa70 = LV_OPA_70,
  Opa80 = LV_OPA_80,
  Opa90 = LV_OPA_90,
  Opa100 = LV_OPA_100,
  Cover = LV_OPA_COVER,
};
}

}  // namespace LVGL_Wrapper
