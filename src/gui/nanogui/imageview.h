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

class  ImageView : public Widget
{
   public:
      ImageView (Widget * parent, int image = 0);

      void setImage (int img)
      {
         mImage = img;
      }
      int  image() const
      {
         return mImage;
      }

      virtual ivec2 preferredSize (NVGcontext * ctx) const;
      virtual void draw (NVGcontext * ctx);

   protected:
      int mImage;
};


NAMESPACE_END (nanogui)