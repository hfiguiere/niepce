/*
 * niepce - fwk/movieutils.cpp
 *
 * Copyright (C) 2011 Hub Figuiere
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


#include <stdlib.h>

#include <boost/format.hpp>


namespace fwk {

  // TODO don't harcode command
  // TODO check errors
  // TODO be smarter
bool thumbnail_movie(const std::string &src, int w, int h, const std::string &dest)
{
  std::string command = str(boost::format("totem-video-thumbnailer -s %1% %2% %3%") 
			    % std::max(w,h) % src % dest);
  return system(command.c_str()) != -1;
}

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
