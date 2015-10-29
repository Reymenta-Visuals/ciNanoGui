/*
   src/widget.cpp -- Base class of all widgets

   NanoGUI was developed by Wenzel Jakob <wenzel@inf.ethz.ch>.
   The widget drawing code is based on the NanoVG demo application
   by Mikko Mononen.

   All rights reserved. Use of this source code is governed by a
   BSD-style license that can be found in the LICENSE.txt file.
*/

#include "widget.h"
#include "theme.h"
#include "layout.h"
#include "window.h"
#include "../nanovg/nanovg.h"
#include "screen.h"
using namespace ci;

NAMESPACE_BEGIN (nanogui)

// ctor
Widget::Widget (Widget * parent)
   : mParent (nullptr),
     mTheme (nullptr),
     mLayout (nullptr),
     mPos (ivec2 (0)),
     mSize (ivec2 (0)),
     mFixedSize (ivec2 (0)),
     mVisible (true),
     mEnabled (true),
     mFocused (false),
     mMouseFocus (false),
     mTooltip (""),
     mFontSize (-1.0f)
{
   if (parent)
   {
      parent->addChild (this);
      mTheme = parent->mTheme;
   }
}

// dtor
Widget::~Widget()
{
}

void Widget::addChild (Widget * widget)
{
   mChildren.push_back (widget);
   widget->incRef();
   widget->setParent (this);
}

void Widget::removeChild (const Widget * widget)
{
   mChildren.erase (std::remove (mChildren.begin(), mChildren.end(), widget), mChildren.end());
   widget->decRef();
}

void Widget::removeChild (int index)
{
   Widget * widget = mChildren[index];
   mChildren.erase (mChildren.begin() + index);
   widget->decRef();
}

ivec2 Widget::preferredSize (NVGcontext * ctx) const
{
   if (mLayout)
      return mLayout->preferredSize (ctx, this);
   else
      return mSize;
}

void Widget::performLayout (NVGcontext * ctx)
{
   if (mLayout)
      mLayout->performLayout (ctx, this);
   else
   {
      for (auto c : mChildren)
      {
         ivec2 pref = c->preferredSize (ctx), fix = c->fixedSize();
         c->setSize (ivec2
                     (
                        fix[0] ? fix[0] : pref[0],
                        fix[1] ? fix[1] : pref[1]
                     ));
         c->performLayout (ctx);
      }
   }
}

int Widget::fontSize() const
{
   return mFontSize < 0 ? mTheme->mStandardFontSize : mFontSize;
}

void Widget::draw (NVGcontext * ctx)
{
#if NANOGUI_SHOW_WIDGET_BOUNDS
   nvgStrokeWidth (ctx, 1.0f);
   nvgBeginPath (ctx);
   nvgRect (ctx, mPos.x - 0.5f, mPos.y - 0.5f, mSize.x + 1, mSize.y + 1);
   nvgStrokeColor (ctx, nvgRGBA (255, 0, 0, 255));
   nvgStroke (ctx);
#endif
   if (mChildren.empty())
      return;
   nvgTranslate (ctx, mPos.x, mPos.y);
   for (auto child : mChildren)
      if (child->visible())
         child->draw (ctx);
   nvgTranslate (ctx, -mPos.x, -mPos.y);
}

Widget * Widget::findWidget (const ivec2 & p)
{
   for (auto it = mChildren.rbegin(); it != mChildren.rend(); ++it)
   {
      Widget * child = *it;
      if (child->visible() && child->contains (p - mPos))
         return child->findWidget (p - mPos);
   }
   return contains (p) ? this : nullptr;
}

bool Widget::mouseEnterEvent (const ivec2 & p, bool enter)
{
   mMouseFocus = enter;
   return false;
}

bool Widget::mouseDragEvent (const ivec2 & p, const ivec2 & rel, int button, int modifiers)
{
   return false;
}

bool Widget::mouseButtonEvent (const ivec2 & p, int button, bool down, int modifiers)
{
   for (auto it = mChildren.rbegin(); it != mChildren.rend(); ++it)
   {
      Widget * child = *it;
      if (child->visible() && child->contains (p - mPos) &&
            child->mouseButtonEvent (p - mPos, button, down, modifiers))
         return true;
   }
   if (button == MOUSE_BUTTON_1 && down && !mFocused)
      requestFocus();
   return false;
}

bool Widget::focusEvent (bool focused)
{
   mFocused = focused;
   return false;
}

bool Widget::mouseMotionEvent (const ivec2 & p, const ivec2 & rel, int button, int modifiers)
{
   for (auto it = mChildren.rbegin(); it != mChildren.rend(); ++it)
   {
      Widget * child = *it;
      if (!child->visible())
         continue;
      bool contained = child->contains (p - mPos), prevContained = child->contains (p - mPos - rel);
      if (contained != prevContained)
         child->mouseEnterEvent (p, contained);
      if ((contained || prevContained) &&
            child->mouseMotionEvent (p - mPos, rel, button, modifiers))
         return true;
   }
   return false;
}

void Widget::requestFocus()
{
   Widget * widget = this;
   while (widget->parent())
      widget = widget->parent();
   ((Screen *)widget)->updateFocus (this);
}

Window * Widget::window()
{
   Widget * widget = this;
   while (true)
   {
      if (!widget)
         throw std::runtime_error (
            "Widget:internal error (could not find parent window)");
      Window * window = dynamic_cast<Window *> (widget);
      if (window)
         return window;
      widget = widget->parent();
   }
}


NAMESPACE_END (nanogui)
