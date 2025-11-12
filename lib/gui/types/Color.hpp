#pragma once

extern "C" {
#include "lvgl.h"
}

namespace LVGL_Wrapper {

struct Color {
  uint8_t r;
  uint8_t g;
  uint8_t b;
  uint8_t a;

  constexpr Color() : r(0), g(0), b(0), a(0) {}

 private:
  constexpr Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
      : r(r), g(g), b(b), a(a) {}

 public:
  static constexpr Color from_rgb(uint8_t r, uint8_t g, uint8_t b) {
    return Color(r, g, b, 255);
  }

  static constexpr Color from_rgba(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    return Color(r, g, b, a);
  }

  static constexpr Color from_hex(uint32_t hex24) {
    return Color((hex24 >> 16) & 0xFF, (hex24 >> 8) & 0xFF, hex24 & 0xFF, 255);
  }

  static constexpr Color from_hex32(uint32_t hex32) {
    return Color((hex32 >> 16) & 0xFF, (hex32 >> 8) & 0xFF, hex32 & 0xFF,
                 (hex32 >> 24) & 0xFF);
  }

  static Color from_lv_color(lv_color_t c, uint8_t opa) {
    lv_color32_t c32 = lv_color_to_32(c, opa);
    return Color(c32.red, c32.green, c32.blue, opa);
  }

  static Color from_lv_color32(lv_color32_t c) {
    return Color(c.red, c.green, c.blue, c.alpha);
  }

  static const Color Black;
  static const Color White;
  static const Color Red;
  static const Color Lime;
  static const Color Blue;
  static const Color Yellow;
  static const Color Cyan;
  static const Color Magenta;
  static const Color Gray;
  static const Color Green;
  static const Color LightGray;
  static const Color DarkGray;
  static const Color Orange;
  static const Color Purple;
  static const Color Transparent;

  operator lv_color_t() const { return lv_color_make(r, g, b); }

  operator lv_color32_t() const {
    lv_color32_t c32;
    c32.red = r;
    c32.green = g;
    c32.blue = b;
    c32.alpha = a;
    return c32;
  }
};

inline const Color Color::Black = from_rgb(0, 0, 0);
inline const Color Color::White = from_rgb(255, 255, 255);
inline const Color Color::Red = from_rgb(255, 0, 0);
inline const Color Color::Lime = from_rgb(0, 255, 0);
inline const Color Color::Blue = from_rgb(0, 0, 255);
inline const Color Color::Yellow = from_rgb(255, 255, 0);
inline const Color Color::Cyan = from_rgb(0, 255, 255);
inline const Color Color::Magenta = from_rgb(255, 0, 255);
inline const Color Color::Gray = from_rgb(128, 128, 128);
inline const Color Color::Green = from_rgb(0, 128, 0);
inline const Color Color::LightGray = from_rgb(211, 211, 211);
inline const Color Color::DarkGray = from_rgb(169, 169, 169);
inline const Color Color::Orange = from_rgb(255, 165, 0);
inline const Color Color::Purple = from_rgb(128, 0, 128);
inline const Color Color::Transparent = from_rgba(0, 0, 0, 0);

}  // namespace LVGL_Wrapper
