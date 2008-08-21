/*
 * niepce - ncr/image.h
 *
 * Copyright (C) 2008 Hubert Figuiere
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this program; if not, see 
 * <http://www.gnu.org/licenses/>.
 */



#ifndef _NCR_IMAGE_H_
#define _NCR_IMAGE_H_

#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/filesystem/path.hpp>

#include <gdkmm/pixbuf.h>

namespace ncr {

class Image 
    : public boost::enable_shared_from_this<Image>
{
public:
    typedef boost::shared_ptr<Image> Ptr;

    Image();
    virtual ~Image();

    Glib::RefPtr<Gdk::Pixbuf> pixbuf_for_display();
    void reload(const boost::filesystem::path & p, bool is_raw,
        int orientation);
    void set_scale(double scale);
    void set_scale_to_dim(int w, int h);


    void set_tilt(double angle);
    void set_color_temp(int temp);
    void set_exposure(double exposure);
    void set_brightness(int brightness);
    void set_contrast(int contrast);
    void set_saturation(int saturation);
    void set_vibrance(int vibrance);

    sigc::signal<void, Image::Ptr> signal_update;
private:
    class Private;
    Private *priv;
};

}

#endif

/*
  Local Variables:
  mode:c++
  c-file-style:"stroustrup"
  c-file-offsets:((innamespace . 0))
  indent-tabs-mode:nil
  fill-column:80
  End:
*/
