/*
    nanogui/common.h -- common definitions used by NanoGUI

    NanoGUI was developed by Wenzel Jakob <wenzel@inf.ethz.ch>.
    The widget drawing code is based on the NanoVG demo application
    by Mikko Mononen.

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE.txt file.
*/

#pragma once

#include "cinder/Vector.h"
#include "cinder/Color.h"
#include <stdint.h>
#include <array>
#include <vector>

/* Set to 1 to draw boxes around widgets */
//#define NANOGUI_SHOW_WIDGET_BOUNDS 1

#if !defined(NAMESPACE_BEGIN)
   #define NAMESPACE_BEGIN(name) namespace name {
#endif
#if !defined(NAMESPACE_END)
   #define NAMESPACE_END(name) }
#endif

/* Forward declarations */
struct NVGcontext;
struct NVGcolor;
struct NVGglyphPosition;

NAMESPACE_BEGIN (nanogui)

// Looks like no cursor support in Cinder?
/* Cursor shapes */
enum class Cursor
{
   Arrow = 0,
   IBeam,
   Crosshair,
   Hand,
   HResize,
   VResize,
   CursorCount
};

/* Import some common glm types */
using ci::ivec2;
using ci::ivec3;
using ci::ivec4;
using ci::vec2;
using ci::vec3;
using ci::vec4;
using ci::ColorA;

// Eigen ops not available in glm
inline ivec2 cwiseMin (ivec2 v1, ivec2 v2)
{
   return ivec2 (std::min (v1.x, v2.x), std::min (v1.y, v2.y));
}

inline ivec2 cwiseMax (ivec2 v1, ivec2 v2)
{
   return ivec2 (std::max (v1.x, v2.x), std::max (v1.y, v2.y));
}

/// Stores an RGBA color value
class Colour : public ColorA
{
   public:
      Colour() : Colour (0, 0, 0, 0) {}
      Colour (const vec4 & color) : ColorA (color) { }
      Colour (const vec3 & color, float alpha)
         : Colour (color.x, color.y, color.z, alpha) { }
      Colour (const vec3 & color) : Colour (color, 1.0f) {}
      Colour (float r, float g, float b, float a) : Colour (vec4 (r, g, b, a)) { }
      Colour (float intensity, float alpha)
         : Colour (vec3 (intensity), alpha) { }
      Colour (int intensity, int alpha)
         : Colour (ivec3 (intensity), alpha) { }
      Colour (const ivec3 & color, int alpha)
         : Colour ((float)color.x / 255.0f, (float)color.y / 255.0f, (float)color.z / 255.0f, (float)alpha / 255.0f) {}
      Colour (const ivec4 & color)
         : Colour (vec4 ((float)color.x / 255.0f, (float)color.y / 255.0f, (float)color.z / 255.0f, (float)color.w / 255.0f)) { }
      Colour (int r, int g, int b, int a) : Colour (ivec4 (r, g, b, a)) { }

      /// Return a reference to the red channel
      float & red()
      {
         return r;
      }

      /// Return a reference to the red channel (const version)
      const float & red() const
      {
         return r;
      }

      /// Return a reference to the green channel
      float & green()
      {
         return g;
      }

      /// Return a reference to the green channel (const version)
      const float & green() const
      {
         return g;
      }

      /// Return a reference to the blue channel
      float & blue()
      {
         return b;
      }

      /// Return a reference to the blue channel (const version)
      const float & blue() const
      {
         return b;
      }

      inline operator const NVGcolor & () const
      {
         return reinterpret_cast<const NVGcolor &> (*this);
      }
};

/* Forward declarations */
template <typename T> class ref;
class AdvancedGridLayout;
class BoxLayout;
class Button;
class CheckBox;
class ColorWheel;
class ColorPicker;
class ComboBox;
class GLFramebuffer;
class GLShader;
class GridLayout;
class GroupLayout;
class ImagePanel;
class Label;
class Layout;
class MessageDialog;
class Object;
class Popup;
class PopupButton;
class ProgressBar;
class Screen;
class Slider;
class TextBox;
class Theme;
class ToolButton;
class VScrollPanel;
class Widget;
class Window;

/// Determine whether an icon ID is a texture loaded via nvgImageIcon
inline bool nvgIsImageIcon (int value)
{
   return value < 1024;
}

/// Determine whether an icon ID is a font-based icon (e.g. from the entypo.ttf font)
inline bool nvgIsFontIcon (int value)
{
   return value >= 1024;
}

inline std::array<char, 8> utf8 (int c)
{
   std::array<char, 8> seq;
   int n = 0;
   if (c < 0x80) n = 1;
   else
      if (c < 0x800) n = 2;
      else
         if (c < 0x10000) n = 3;
         else
            if (c < 0x200000) n = 4;
            else
               if (c < 0x4000000) n = 5;
               else
                  if (c <= 0x7fffffff) n = 6;
   seq[n] = '\0';
   switch (n)
   {
      case 6:
         seq[5] = 0x80 | (c & 0x3f);
         c = c >> 6;
         c |= 0x4000000;
      case 5:
         seq[4] = 0x80 | (c & 0x3f);
         c = c >> 6;
         c |= 0x200000;
      case 4:
         seq[3] = 0x80 | (c & 0x3f);
         c = c >> 6;
         c |= 0x10000;
      case 3:
         seq[2] = 0x80 | (c & 0x3f);
         c = c >> 6;
         c |= 0x800;
      case 2:
         seq[1] = 0x80 | (c & 0x3f);
         c = c >> 6;
         c |= 0xc0;
      case 1:
         seq[0] = c;
   }
   return seq;
}

NAMESPACE_END (nanogui)

// mouse events
#define RELEASE                0
#define PRESS                  1
#define REPEAT				   2
#define MOUSE_BUTTON_1         0
#define MOUSE_BUTTON_2         1
#define MOUSE_BUTTON_3         2
#define MOUSE_BUTTON_LEFT      MOUSE_BUTTON_1
#define MOUSE_BUTTON_RIGHT     MOUSE_BUTTON_2
#define MOUSE_BUTTON_MIDDLE    MOUSE_BUTTON_3