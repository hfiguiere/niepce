/*
 * niepce - modules/darkroom/image.h
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



#ifndef _DARKROOM_IMAGE_H_
#define _DARKROOM_IMAGE_H_

#include <boost/shared_ptr.hpp>
#include <boost/filesystem/path.hpp>

#include <gdkmm/pixbuf.h>

namespace darkroom {

class Image {
public:
    typedef boost::shared_ptr<Image> Ptr;

    Image();
    virtual ~Image();

    Glib::RefPtr<Gdk::Pixbuf> pixbuf_for_display();
    void reload(const boost::filesystem::path & p);
    void set_scale(double scale);
    void set_scale_to_dim(int w, int h);
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
  fill-column:99
  End:
*/
