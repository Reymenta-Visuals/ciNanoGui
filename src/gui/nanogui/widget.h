/*
   nanogui/widget.h -- Base class of all widgets

   NanoGUI was developed by Wenzel Jakob <wenzel@inf.ethz.ch>.
   The widget drawing code is based on the NanoVG demo application
   by Mikko Mononen.

   All rights reserved. Use of this source code is governed by a
   BSD-style license that can be found in the LICENSE.txt file.
*/

// This is a modified version of Nanogui!

#pragma  once

#include "object.h"

NAMESPACE_BEGIN (nanogui)

class Widget : public Object
{

   public:
      /// Construct a new widget with the given parent widget
      Widget (Widget * parent);
      ~Widget();

      /// Return the parent widget
      Widget * parent()
      {
         return mParent;
      }

      /// Return the parent widget
      const Widget * parent() const
      {
         return mParent;
      }

      /// Set the parent widget
      void setParent (Widget * parent)
      {
         mParent = parent;
      }

      /// Return the number of child widgets
      int childCount() const
      {
         return (int)mChildren.size();
      }

      /// Return the list of child widgets of the current widget
      const std::vector<Widget *> & children() const
      {
         return mChildren;
      }

      /**
         \brief Add a child widget to the current widget

         This function almost never needs to be called by hand,
         since the constructor of \ref Widget automatically
         adds the current widget to its parent
      */
      void addChild (Widget * widget);

      /// Remove a child widget by index
      void removeChild (int index);

      /// Remove a child widget by value
      void removeChild (const Widget * widget);

      /// Associate this widget with an ID value (optional)
      void setId (const std::string & id)
      {
         mId = id;
      }

      /// Return the ID value associated with this widget, if any
      const std::string & id() const
      {
         return mId;
      }

      /// Return whether or not this widget is currently enabled
      bool enabled() const
      {
         return mEnabled;
      }

      /// Set whether or not this widget is currently enabled
      void setEnabled (bool enabled)
      {
         mEnabled = enabled;
      }

      /// Return the \ref Theme used to draw this widget
      Theme * theme()
      {
         return mTheme;
      }

      /// Return the \ref Theme used to draw this widget
      const Theme * theme() const
      {
         return mTheme.get();
      }

      /// Set the \ref Theme used to draw this widget
      void setTheme (Theme * theme)
      {
         mTheme = theme;
      }

      /// Return the used \ref Layout generator
      Layout * layout()
      {
         return mLayout;
      }

      /// Return the used \ref Layout generator
      const Layout * layout() const
      {
         return mLayout.get();
      }

      /// Set the used \ref Layout generator
      void setLayout (Layout * layout)
      {
         mLayout = layout;
      }

      /// Return whether or not the widget is currently visible (assuming all parents are visible)
      bool visible() const
      {
         return mVisible;
      }

      /// Set whether or not the widget is currently visible (assuming all parents are visible)
      void setVisible (bool visible)
      {
         mVisible = visible;
      }

      /// Check if this widget is currently visible, taking parent widgets into account
      bool visibleRecursive() const
      {
         bool visible = true;
         const Widget * widget = this;
         while (widget)
         {
            visible &= widget->visible();
            widget = widget->parent();
         }
         return visible;
      }

      /// Compute the preferred size of the widget
      virtual ivec2 preferredSize (NVGcontext * ctx) const;

      /// Return the fixed size (see \ref setFixedSize())
      const ivec2 & fixedSize() const
      {
         return mFixedSize;
      }

      /// Return the width of the widget
      int width() const
      {
         return mSize.x;
      }

      /// Set the width of the widget
      void setWidth (int width)
      {
         mSize.x = width;
      }

      /// Return the height of the widget
      int height() const
      {
         return mSize.y;
      }

      /// Set the height of the widget
      void setHeight (int height)
      {
         mSize.y = height;
      }

      /**
         \brief Set the fixed size of this widget

         If nonzero, components of the fixed size attribute override any values
         computed by a layout generator associated with this widget. Note that
         just setting the fixed size alone is not enough to actually change its
         size; this is done with a call to \ref setSize or a call to \ref performLayout()
         in the parent widget.
      */
      void setFixedSize (const ivec2 & fixedSize)
      {
         mFixedSize = fixedSize;
      }

      // Return the fixed width (see \ref setFixedSize())
      int fixedWidth() const
      {
         return mFixedSize.x;
      }

      // Return the fixed height (see \ref setFixedSize())
      int fixedHeight() const
      {
         return mFixedSize.y;
      }

      /// Set the fixed width (see \ref setFixedSize())
      void setFixedWidth (int width)
      {
         mFixedSize.x = width;
      }

      /// Set the fixed height (see \ref setFixedSize())
      void setFixedHeight (int height)
      {
         mFixedSize.y = height;
      }

      /// Return the position relative to the parent widget
      const ivec2 & position() const
      {
         return mPos;
      }

      /// Set the position relative to the parent widget
      void setPosition (const ivec2 & pos)
      {
         mPos = pos;
      }

      /// Return the absolute position on screen
      ivec2 absolutePosition() const
      {
         return mParent ?
                (parent()->absolutePosition() + mPos) : mPos;
      }

      /// Return the size of the widget
      const ivec2 & size() const
      {
         return mSize;
      }

      /// set the size of the widget
      void setSize (const ivec2 & size)
      {
         mSize = size;
      }

      /// Return current font size. If not set the default of the current theme will be returned
      int fontSize() const;

      /// Set the font size of this widget
      void setFontSize (int fontSize)
      {
         mFontSize = fontSize;
      }

      /// Return whether the font size is explicitly specified for this widget
      bool hasFontSize() const
      {
         return mFontSize > 0;
      }

      /// Invoke the associated layout generator to properly place child widgets, if any
      virtual void performLayout (NVGcontext * ctx);

      /// Draw the widget (and all child widgets)
      virtual void draw (NVGcontext * ctx);

      /// Determine the widget located at the given position value (recursive)
      Widget * findWidget (const ivec2 & p);

      /// Check if the widget contains a certain position
      bool contains (const ivec2 & p) const
      {
         ivec2 d = p - mPos;
         return ((d.x >= 0 && d.y >= 0) && (d.x < mSize.x && d.y < mSize.y));
      }

      /// Handle a mouse motion event (default implementation: propagate to children)
      virtual bool mouseMotionEvent (const ivec2 & p, const ivec2 & rel, int button, int modifiers);

      /// Handle a mouse enter/leave event (default implementation: record this fact, but do nothing)
      virtual bool mouseEnterEvent (const ivec2 & p, bool enter);

      /// Handle a mouse drag event (default implementation: do nothing)
      virtual bool mouseDragEvent (const ivec2 & p, const ivec2 & rel, int button, int modifiers);

      /// Handle a mouse button event (default implementation: propagate to children)
      virtual bool mouseButtonEvent (const ivec2 & p, int button, bool down, int modifiers);

      /// Handle a focus change event (default implementation: record the focus status, but do nothing)
      virtual bool focusEvent (bool focused);

      /// Return whether or not this widget is currently focused
      bool focused() const
      {
         return mFocused;
      }
      /// Set whether or not this widget is currently focused
      void setFocused (bool focused)
      {
         mFocused = focused;
      }
      /// Request the focus to be moved to this widget
      void requestFocus();

      // Walk up the hierarchy and return the parent window
      Window * window();

   protected:
      Widget * mParent;
      ref<Theme> mTheme;
      ref<Layout> mLayout;
      std::string mId;
      std::vector<Widget *> mChildren;

      ivec2 mPos, mSize, mFixedSize;
      bool mVisible, mEnabled;
      bool mFocused, mMouseFocus;
      std::string mTooltip;
      int mFontSize;

}; // end class Widget

NAMESPACE_END (nanogui)
