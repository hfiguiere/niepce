/* *************************************************************************
 * Copyright (c) 2005 VMware, Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 * *************************************************************************/

/*
 * header.cc --
 *
 *      Header widget. Draw text inside a (hopefully) attractive frame.
 */

#include <gtkmm/settings.h>

#include <gtk/gtk.h>

#include <libview/header.hh>


namespace view {


/*
 *-----------------------------------------------------------------------------
 *
 * view::Header::Header --
 *
 *      Constructor.
 *
 * Results:
 *      None
 *
 * Side effects:
 *      None
 *
 *-----------------------------------------------------------------------------
 */

Header::Header(const Glib::ustring &markup, // IN: Markup text
               Alignment align)             // IN: Default alignment
   : Gtk::MenuItem(),
     mLabel()
{
   mLabel.show();
   add(mLabel);

   SetMarkup(markup);
   SetAlignment(align);

   select();
}


/*
 *-----------------------------------------------------------------------------
 *
 * view::Header::SetMarkup --
 *
 *      Set the markup text of the header.
 *
 * Results:
 *      None
 *
 * Side effects:
 *      None
 *
 *-----------------------------------------------------------------------------
 */

void
Header::SetMarkup(const Glib::ustring& markup) // IN: The markup text
{
   mLabel.set_markup(markup);
}


/*
 *-----------------------------------------------------------------------------
 *
 * view::Header::SetAlignment --
 *
 *      Sets the alignment of the text inside the header.
 *
 * Results:
 *      None.
 *
 * Side effects:
 *      None.
 *
 *-----------------------------------------------------------------------------
 */

void
Header::SetAlignment(Alignment align) // IN: The new alignment
{
   switch (align) {
   case Alignment::LEFT:
      mLabel.set_xalign(0);
      mLabel.set_yalign(0.5);
      mLabel.set_justify(Gtk::JUSTIFY_LEFT);
      break;

   case Alignment::CENTER:
      mLabel.set_xalign(0.5);
      mLabel.set_yalign(0.5);
      mLabel.set_justify(Gtk::JUSTIFY_CENTER);
      break;

   case Alignment::RIGHT:
      mLabel.set_xalign(1);
      mLabel.set_yalign(0.5);
      mLabel.set_justify(Gtk::JUSTIFY_RIGHT);
      break;

   default:
      g_assert_not_reached();
   }
}


/*
 *-----------------------------------------------------------------------------
 *
 * view::Header::on_style_changed --
 *
 *      Override method to handle style changes. Simply apply our style to
 *      the child label.
 *
 *      This method should not be necessary but many theme engines are broken
 *      so that a menu item in an unexpected place is not drawn correctly.
 *      Examples include Bluecurve and Industrial. The Mist theme is so broken
 *      that this workaround won't fix it. Such is life.
 *
 * Results:
 *      None
 *
 * Side effects:
 *      Label may redraw/resize.
 *
 *-----------------------------------------------------------------------------
 */

void
Header::on_style_updated() // IN
{
   Gtk::MenuItem::on_style_updated();

#if GTKMM2
   mLabel.set_style(get_style());
#endif
}


} // namespace view
