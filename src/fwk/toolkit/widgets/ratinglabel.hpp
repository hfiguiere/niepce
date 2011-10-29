/*
 * niepce - fwk/toolkit/widgets/ratinglabel.hpp
 *
 * Copyright (C) 2011 Hubert Figuiere
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

#ifndef __FWK_TOOLKIT_RATINGLABEL_HPP__
#define __FWK_TOOLKIT_RATINGLABEL_HPP__

#include <gtkmm/drawingarea.h>
#include <cairomm/surface.h>

namespace fwk {

class RatingLabel
  : public Gtk::DrawingArea
{
public:
  RatingLabel(int rating = 0);
  virtual ~RatingLabel();

  bool is_editable() const
  { return m_is_editable; }
  void set_editable(bool);

  void set_rating(int rating);
  static const Cairo::RefPtr<Cairo::ImageSurface> & get_star();
  static const Cairo::RefPtr<Cairo::ImageSurface> & get_unstar();

  // draw the rating at x and y in the context.
  // this can be called by anybody who wants to print these
  static void draw_rating(const Cairo::RefPtr<Cairo::Context> & cr, 
			  int32_t rating,
			  const Cairo::RefPtr<Cairo::ImageSurface> & star,
			  const Cairo::RefPtr<Cairo::ImageSurface> & unstar,
			  double x, double y);
  static void get_geometry(double & w, double & h);
  static int rating_value_from_hit_x(double x);

  // signal emitted when the rating is changed in the UI
  sigc::signal<void, int> signal_changed;
protected:
  virtual void  on_realize();
  virtual bool 	on_button_press_event (GdkEventButton* event);
  virtual bool 	on_button_release_event (GdkEventButton* event);
  virtual bool 	on_motion_notify_event (GdkEventMotion* event);

  virtual void 	on_size_request (Gtk::Requisition* requisition);
  virtual bool  on_expose_event (GdkEventExpose *event);
  virtual void 	on_state_changed (Gtk::StateType previous_state);
private:
  int m_rating;
  bool m_is_editable;
};

}

#endif
