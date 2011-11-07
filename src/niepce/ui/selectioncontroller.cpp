/*
 * niepce - niepce/ui/selectioncontroller.cpp
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

#include <boost/bind.hpp>

#include <gtkmm/iconview.h>
#include <gtkmm/treeiter.h>
#include <glibmm/i18n.h>

#include "fwk/base/autoflag.hpp"
#include "fwk/utils/boost.hpp"
#include "fwk/base/debug.hpp"
#include "fwk/toolkit/undo.hpp"
#include "fwk/toolkit/command.hpp"
#include "fwk/toolkit/application.hpp"
#include "engine/db/metadata.hpp"
#include "libraryclient/libraryclient.hpp"
#include "niepcewindow.hpp"
#include "selectioncontroller.hpp"

namespace ui {


SelectionController::SelectionController()
	: m_in_handler(false)
{
	m_imageliststore = ImageListStore::create();
}

void SelectionController::_added()
{
	m_imageliststore->set_parent_controller(m_parent);
}

void SelectionController::add_selectable(IImageSelectable * selectable)
{ 
    DBG_OUT("added %p", (void*)selectable);
    m_selectables.push_back(selectable);
    selectable->image_list()->signal_selection_changed().connect(
        sigc::bind(sigc::mem_fun(*this, &SelectionController::selected),
                   selectable));
    selectable->image_list()->signal_item_activated().connect(
        sigc::bind(sigc::mem_fun(*this, &SelectionController::activated),
                   selectable));
}


void SelectionController::activated(const Gtk::TreeModel::Path & path,
                                    IImageSelectable * /*selectable*/)
{
    fwk::AutoFlag f(m_in_handler);
    Gtk::TreeIter iter = m_imageliststore->get_iter(path);
    if(iter) {
        eng::LibFile::Ptr file = (*iter)[m_imageliststore->columns().m_libfile];
        if(file) {
            eng::library_id_t selection = file->id();
            DBG_OUT("item activated %Ld", selection);
            signal_activated(selection);
        }
    }
}

void SelectionController::selected(IImageSelectable * selectable)
{
	if(m_in_handler) {
		DBG_OUT("%p already in handler", (void*)this);
		return;
	}

	fwk::AutoFlag f(m_in_handler);

	eng::library_id_t selection = selectable->get_selected();
	std::vector<IImageSelectable *>::iterator iter;
	for(iter = m_selectables.begin(); iter != m_selectables.end(); iter++) {
		if(*iter != selectable) {
			(*iter)->select_image(selection);
		}
	}
	signal_selected(selection);
}


libraryclient::LibraryClient::Ptr SelectionController::getLibraryClient()
{
    ModuleShell::Ptr shell = std::tr1::dynamic_pointer_cast<ModuleShell>(m_parent.lock());
    DBG_ASSERT(shell, "parent not a ModuleShell");
    return	shell->getLibraryClient();
}

inline eng::library_id_t SelectionController::get_selection()
{
    DBG_ASSERT(!m_selectables.empty(), "selectables list can't be empty");
    return m_selectables[0]->get_selected();
}


void SelectionController::_selection_move(bool backwards)
{
	eng::library_id_t selection = get_selection();
    Gtk::TreeIter iter = m_imageliststore->get_iter_from_id(selection);
    if(iter) {
        if(backwards) {
            if(iter != m_imageliststore->children().begin()) {
                --iter;
            }
        }
        else {
            iter++;
        }
        if(iter) {
            // make sure the iterator is valid...
            eng::LibFile::Ptr libfile 
                = (*iter)[m_imageliststore->columns().m_libfile];
            selection = libfile->id();

            fwk::AutoFlag f(m_in_handler);
        
            std::for_each(m_selectables.begin(), m_selectables.end(),
                          boost::bind(&IImageSelectable::select_image, _1,  
                                      selection));
            signal_selected(selection);
        }
    }
}

/** select the previous image. Emit the signal */
void SelectionController::select_previous()
{
    _selection_move(true);
}


/** select the next image. Emit the signal */
void SelectionController::select_next()
{
    _selection_move(false);
}


void SelectionController::rotate(int angle)
{
    DBG_OUT("angle = %d", angle);
	eng::library_id_t selection = get_selection();
    if(selection >= 0) {
        Gtk::TreeIter iter = m_imageliststore->get_iter_from_id(selection);
        if(iter) {
            // @todo
        }
    }
}


bool SelectionController::_set_metadata(const std::string & undo_label, const eng::LibFile::Ptr & file,
                                        int meta, int old_value, int new_value)
{
    fwk::UndoTransaction *undo = fwk::Application::app()->begin_undo(undo_label);
    undo->new_command<void>(
        boost::bind(&libraryclient::LibraryClient::setMetadata,
                    getLibraryClient(), file->id(), meta, new_value),
        boost::bind(&libraryclient::LibraryClient::setMetadata,
                    getLibraryClient(), file->id(), meta, old_value)
        );
    undo->execute();
    return true;
}


void SelectionController::set_label(int label)
{
    DBG_OUT("label = %d", label);
    eng::library_id_t selection = get_selection();
    if(selection >= 0) {
        Gtk::TreeIter iter = m_imageliststore->get_iter_from_id(selection);
        if(iter) {
            eng::LibFile::Ptr file = (*iter)[m_imageliststore->columns().m_libfile];
            DBG_OUT("old label is %d", file->label());
            int old_value = file->label();
            _set_metadata(_("Set Label"), file, 
                          MAKE_METADATA_IDX(eng::META_NS_XMPCORE, eng::META_XMPCORE_LABEL), 
                          old_value, label);
            // we need to set the label here so that undo/redo works
            // consistently.
            file->setLabel(label);
        }
    }
}


void SelectionController::set_rating(int rating)
{
    DBG_OUT("rating = %d", rating);
    eng::library_id_t selection = get_selection();
    if(selection >= 0) {
        Gtk::TreeIter iter = m_imageliststore->get_iter_from_id(selection);
        if(iter) {
            eng::LibFile::Ptr file = (*iter)[m_imageliststore->columns().m_libfile];
            DBG_OUT("old rating is %d", file->rating());
            int old_value = file->rating();
            _set_metadata(_("Set Rating"), file, 
                          MAKE_METADATA_IDX(eng::META_NS_XMPCORE, eng::META_XMPCORE_RATING), 
                          old_value, rating);
            // we need to set the rating here so that undo/redo works
            // consistently.
            file->setRating(rating);
        }
    }
}

void SelectionController::set_flag(int flag)
{
    DBG_OUT("flag = %d", flag);
    eng::library_id_t selection = get_selection();
    if(selection >= 0) {
        Gtk::TreeIter iter = m_imageliststore->get_iter_from_id(selection);
        if(iter) {
            eng::LibFile::Ptr file = (*iter)[m_imageliststore->columns().m_libfile];
            DBG_OUT("old flag is %d", file->flag());
            int old_value = file->flag();
            _set_metadata(_("Set Flag"), file, 
                          MAKE_METADATA_IDX(eng::META_NS_NIEPCE, eng::META_NIEPCE_FLAG), 
                          old_value, flag);
            // we need to set the rating here so that undo/redo works
            // consistently.
            file->setFlag(flag);
        }
    }
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
