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

#include "framework/goocanvas_proxy_header.h"

namespace darkroom {

class ImageCanvas
    : public Goocanvas::Canvas
{
public:
    typedef enum {
        ZOOM_MODE_NONE = 0,
        ZOOM_MODE_FIT,
        ZOOM_MODE_FILL,
        ZOOM_MODE_100P,
        ZOOM_MODE_CUSTOM
    } ZoomMode;
    ImageCanvas();

    void set_image(const Glib::RefPtr<Gdk::Pixbuf> & img);
    void set_zoom_mode(ZoomMode mode)
        {
            if(m_zoom_mode != mode) {
                m_need_redisplay = true;
                m_zoom_mode = mode;
            }
        }
    ZoomMode get_zoom_mode() const
        {
            return m_zoom_mode;
        }
private:
    void _calc_image_frame(int img_w, int img_h,
                          double & x, double & y,
                          double & width, double & height);
    /** cause to "recalulate" the content. 
        Only if m_need_display of force */
    void _redisplay(bool force = false);

    bool                           m_need_redisplay;
    ZoomMode                       m_zoom_mode;
    Glib::RefPtr<Gdk::Pixbuf>      m_image;
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
  fill-column:80
  End:
*/
