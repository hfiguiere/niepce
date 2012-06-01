/*
 * niepce - darkroom/imagecanvas.h
 *
 * Copyright (C) 2008-2009 Hubert Figuiere
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



#include <gdk/gdk.h>
#include <gdkmm/pixbuf.h>
#include <gtkmm/drawingarea.h>
#include <cairomm/surface.h>

#include "ncr/image.hpp"

namespace dr {

class ImageCanvas
    : public Gtk::DrawingArea
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

    void set_image(const ncr::Image::Ptr & img);
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
protected:
    virtual bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr);
private:
    void on_image_reloaded();

    void _calc_image_frame(int img_w, int img_h,
                           double & x, double & y,
                           double & width, double & height);
    double _calc_image_scale(int img_w, int img_h);
    /** cause to "recalulate" the content. */
    void _redisplay();

    bool                           m_need_redisplay;
    ZoomMode                       m_zoom_mode;
    ncr::Image::Ptr                m_image;
    Cairo::RefPtr<Cairo::Surface>  m_backingstore;

    sigc::connection               m_image_reloaded_cid;
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
