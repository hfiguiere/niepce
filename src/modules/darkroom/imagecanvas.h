/*
 * niepce - darkroom/imagecanvas.h
 *
 * Copyright (C) 2008 Hubert Figuiere
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


#include <gdkmm/pixbuf.h>
#include <gtkmm/bin.h>
#include <goocanvasmm/canvas.h>
#include <goocanvasmm/image.h>
#include <goocanvasmm/rect.h>

namespace darkroom {

class ImageCanvas
    : public Goocanvas::Canvas
{
public:
    ImageCanvas();

    void set_image(const Glib::RefPtr<Gdk::Pixbuf> & img);

private:
    Glib::RefPtr<Goocanvas::Image> m_imageitem;
    Glib::RefPtr<Goocanvas::Rect>  m_frameitem;
    Goocanvas::Canvas*             m_imagecanvas;
};

}

/*
  Local Variables:
  mode:c++
  c-file-style:"stroustrup"
  c-file-offsets:((innamespace . 0))
  indent-tabs-mode:nil
  fill-column:99
  End:
*/
