/*
   nanogui/button.h -- [Normal/Toggle/Radio/Popup] Button widget

   NanoGUI was developed by Wenzel Jakob <wenzel@inf.ethz.ch>.
   The widget drawing code is based on the NanoVG demo application
   by Mikko Mononen.

   All rights reserved. Use of this source code is governed by a
   BSD-style license that can be found in the LICENSE.txt file.
*/

// This is a modified version of Nanogui!

#pragma once

#include "widget.h"

NAMESPACE_BEGIN (nanogui)

class ImagePanel : public Widget
{
   public:
      typedef std::vector<std::pair<int, std::string>> Images;
   public:
      ImagePanel (Widget * parent);

      void setImages (const Images & data)
      {
         mImages = data;
      }
      const Images & images() const
      {
         return mImages;
      }

      std::function<void (int)> callback() const
      {
         return mCallback;
      }
      void setCallback (const std::function<void (int)> & callback)
      {
         mCallback = callback;
      }

      virtual bool mouseMotionEvent (const ivec2 & p, const ivec2 & rel, int button, int modifiers);
      virtual bool mouseButtonEvent (const ivec2 & p, int button, bool down, int modifiers);
      virtual ivec2 preferredSize (NVGcontext * ctx) const;
      virtual void draw (NVGcontext * ctx);
   protected:
      ivec2 gridSize() const;
      int indexForPosition (const ivec2 & p) const;
   protected:
      Images mImages;
      std::function<void (int)> mCallback;
      int mThumbSize;
      int mSpacing;
      int mMargin;
      int mMouseIndex;
};



NAMESPACE_END (nanogui)