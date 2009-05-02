/*
 * niepce - niepce/ui/dialogs/importdialog.h
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




#ifndef __UI_IMPORTDIALOG_H__
#define __UI_IMPORTDIALOG_H__

#include <list>
#include <string>

#include <glibmm/refptr.h>

#include "fwk/toolkit/gtkutils.hpp"
#include "fwk/toolkit/dialog.hpp"

namespace Gtk {
	class Dialog;
	class ComboBox;
	class CheckButton;
	class TreeView;
}

namespace ui {

class ImportDialog 
	: public fwk::Dialog
{
public:
  typedef std::tr1::shared_ptr<ImportDialog> Ptr;

	ImportDialog();

 	virtual void setup_widget();

	const std::list<std::string> & to_import() const
		{ return m_list_to_import; }
	void set_to_import(const Glib::SListHandle<Glib::ustring> & l);

private:
	class ImportParam;

	void do_select_directories();
	
	std::list<std::string> m_list_to_import;
	Gtk::ComboBox *m_date_tz_combo;
	Gtk::CheckButton *m_ufraw_import_check;
	Gtk::CheckButton *m_rawstudio_import_check;
	Gtk::Label *m_directory_name;
	Gtk::TreeView *m_folderList;
	fwk::TextModelRecord m_folderListModelRecord;
	Glib::RefPtr<Gtk::ListStore> m_folderListModel;
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

#endif

