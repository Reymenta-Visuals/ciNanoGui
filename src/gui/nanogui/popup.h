/*
   nanogui/popup.h -- Simple popup widget which is attached to another given
   window (can be nested)

   NanoGUI was developed by Wenzel Jakob <wenzel@inf.ethz.ch>.
   The widget drawing code is based on the NanoVG demo application
   by Mikko Mononen.

   All rights reserved. Use of this source code is governed by a
   BSD-style license that can be found in the LICENSE.txt file.
*/

// This is a modified version of Nanogui!

#pragma once

#include "window.h"

NAMESPACE_BEGIN (nanogui)

/**
   \brief Popup window for combo boxes, popup buttons, nested dialogs etc.

   Usually the Popup instance is constructed by another widget (e.g. \ref PopupButton)
   and does not need to be created by hand.
*/

class Popup : public Window
{

   public:
      /// Create a new popup parented to a screen (first argument) and a parent window
      Popup (Widget * parent, Window * parentWindow);

      /// Return the anchor position in the parent window; the placement of the popup is relative to it
      void setAnchorPos (const ivec2 & anchorPos)
      {
         mAnchorPos = anchorPos;
      }
      /// Set the anchor position in the parent window; the placement of the popup is relative to it
      const ivec2 & anchorPos() const
      {
         return mAnchorPos;
      }

      /// Set the anchor height; this determines the vertical shift relative to the anchor position
      void setAnchorHeight (int anchorHeight)
      {
         mAnchorHeight = anchorHeight;
      }
      /// Return the anchor height; this determines the vertical shift relative to the anchor position
      int anchorHeight() const
      {
         return mAnchorHeight;
      }

      /// Return the parent window of the popup
      Window * parentWindow()
      {
         return mParentWindow;
      }
      /// Return the parent window of the popup
      const Window * parentWindow() const
      {
         return mParentWindow;
      }

      /// Invoke the associated layout generator to properly place child widgets, if any
      virtual void performLayout (NVGcontext * ctx);

      /// Draw the popup window
      virtual void draw (NVGcontext * ctx);
   protected:
      /// Internal helper function to maintain nested window position values
      virtual void refreshRelativePlacement();

   protected:
      Window * mParentWindow;
      ivec2 mAnchorPos;
      int mAnchorHeight;


}; // end class Popup

NAMESPACE_END (nanogui)