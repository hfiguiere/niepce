/*
 * niepce - ui/workspacecontroller.cpp
 *
 * Copyright (C) 2007-2013 Hubert Figuiere
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

#include <boost/any.hpp>

#include <glibmm/i18n.h>

#include <gtkmm/icontheme.h>
#include <gtkmm/box.h>

#include "fwk/base/debug.hpp"
#include "niepce/notifications.hpp"
#include "engine/db/librarytypes.hpp"
#include "libraryclient/libraryclient.hpp"
#include "fwk/toolkit/application.hpp"
#include "niepcewindow.hpp"
#include "workspacecontroller.hpp"


using fwk::Application;

namespace ui {


WorkspaceController::WorkspaceController()
    : fwk::UiController()
    , m_vbox(Gtk::ORIENTATION_VERTICAL)
{
    static struct _Icons {
        int icon_id;
        const char *icon_name;
    } icons[] = {
        { ICON_FOLDER, "folder" },
        { ICON_PROJECT, "applications-accessories" },
        { ICON_ROLL, "emblem-photos" },
        { ICON_TRASH, "user-trash" },
        { ICON_KEYWORD, "application-certificate" },
        { 0, nullptr }
    };

    Glib::RefPtr< Gtk::IconTheme > icon_theme(Application::app()->getIconTheme());
    int i = 0;
    while(icons[i].icon_name) {
        try {
            m_icons[icons[i].icon_id] = icon_theme->load_icon(
                Glib::ustring(icons[i].icon_name), 16,
                Gtk::ICON_LOOKUP_USE_BUILTIN);
        }
        catch(const Gtk::IconThemeError & e)
        {
            ERR_OUT("Exception %s.", e.what().c_str());
        }
        i++;
    }
}

libraryclient::LibraryClientPtr WorkspaceController::getLibraryClient() const
{
    return std::dynamic_pointer_cast<NiepceWindow>(m_parent.lock())->getLibraryClient();
}

fwk::Configuration::Ptr WorkspaceController::getLibraryConfig() const
{
    return std::dynamic_pointer_cast<NiepceWindow>(m_parent.lock())->getLibraryConfig();
}

void WorkspaceController::on_lib_notification(const eng::LibNotification &ln)
{
    DBG_OUT("notification for workspace");
    switch(ln.type) {
    case eng::LibNotification::Type::ADDED_FOLDERS:
    {
        auto l = ln.get<eng::LibNotification::Type::ADDED_FOLDERS>().folders;
        DBG_OUT("received added folders # %lu", l->size());
        for_each(l->cbegin(), l->cend(),
                 [this] (const eng::LibFolder::Ptr& f) {
                     this->add_folder_item(f);
                 });
        break;
    }
    case eng::LibNotification::Type::ADDED_KEYWORD:
    {
        auto k = ln.get<eng::LibNotification::Type::ADDED_KEYWORD>().keyword;
        DBG_ASSERT(static_cast<bool>(k), "keyword must not be NULL");
        add_keyword_item(k);
        break;
    }
    case eng::LibNotification::Type::ADDED_KEYWORDS:
    {
        auto l = ln.get<eng::LibNotification::Type::ADDED_KEYWORDS>().keywords;
        DBG_ASSERT(static_cast<bool>(l), "keyword list must not be NULL");
        for_each(l->cbegin(), l->cend(),
                 [this] (const eng::KeywordPtr& k) {
                     this->add_keyword_item(k);
                 });
        break;
    }
    case eng::LibNotification::Type::FOLDER_COUNTED:
    {
        auto count = ln.get<eng::LibNotification::Type::FOLDER_COUNTED>();
        DBG_OUT("count for folder %Ld is %d", (long long)count.folder, count.count);
        std::map<eng::library_id_t, Gtk::TreeIter>::const_iterator iter
            = m_folderidmap.find(count.folder);
        if(iter != m_folderidmap.cend()) {
            Gtk::TreeRow row = *(iter->second);
            row[m_librarycolumns.m_count_n] = count.count;
            row[m_librarycolumns.m_count] = std::to_string(count.count);
        }

        break;
    }
    case eng::LibNotification::Type::FOLDER_COUNT_CHANGE:
    {
        auto count = ln.get<eng::LibNotification::Type::FOLDER_COUNT_CHANGE>();
        DBG_OUT("count change for folder %Ld is %d", (long long)count.folder, count.count);
        std::map<eng::library_id_t, Gtk::TreeIter>::const_iterator iter
            = m_folderidmap.find(count.folder);
        if(iter != m_folderidmap.cend()) {
            Gtk::TreeRow row = *(iter->second);
            int new_count = row[m_librarycolumns.m_count_n] + count.count;
            row[m_librarycolumns.m_count_n] = new_count;
            row[m_librarycolumns.m_count] = std::to_string(new_count);
        }

        break;
    }
    default:
        break;
    }
}

void WorkspaceController::on_count_notification(int)
{
    DBG_OUT("received NOTIFICATION_COUNT");
}


void WorkspaceController::on_libtree_selection()
{
    Glib::RefPtr<Gtk::TreeSelection> selection = m_librarytree.get_selection();
    auto selected = selection->get_selected();
    int type = (*selected)[m_librarycolumns.m_type];
    eng::library_id_t id = (*selected)[m_librarycolumns.m_id];

    switch(type) {

    case FOLDER_ITEM:
        getLibraryClient()->queryFolderContent(id);
        break;

    case KEYWORD_ITEM:
        getLibraryClient()->queryKeywordContent(id);
        break;

    default:
        DBG_OUT("selected something not a folder");
    }
}

void WorkspaceController::on_row_expanded_collapsed(const Gtk::TreeIter& iter,
                                                    const Gtk::TreePath& /*path*/,
                                                    bool expanded)
{
    int type = (*iter)[m_librarycolumns.m_type];
    fwk::Configuration::Ptr cfg = getLibraryConfig();
    const char* key = nullptr;
    switch(type) {
    case FOLDERS_ITEM:
        key = "workspace_folders_expanded";
        break;
    case PROJECTS_ITEM:
        key = "workspace_projects_expanded";
        break;
    case KEYWORDS_ITEM:
        key = "workspace_keywords_expanded";
        break;
    }
    if(cfg && key) {
        cfg->setValue(key, std::to_string(expanded));
    }
}

void WorkspaceController::on_row_expanded(const Gtk::TreeIter& iter,
                                          const Gtk::TreePath& path)
{
    on_row_expanded_collapsed(iter, path, true);
}

void WorkspaceController::on_row_collapsed(const Gtk::TreeIter& iter,
                                           const Gtk::TreePath& path)
{
    on_row_expanded_collapsed(iter, path, false);
}


void WorkspaceController::add_keyword_item(const eng::KeywordPtr & k)
{
    auto children = m_keywordsNode->children();
    bool was_empty = children.empty();
    auto iter = add_item(m_treestore, children,
                         m_icons[ICON_KEYWORD],
                         engine_db_keyword_keyword(k.get()),
                         engine_db_keyword_id(k.get()), KEYWORD_ITEM);
//		getLibraryClient()->countKeyword(f->id());
    m_keywordsidmap[engine_db_keyword_id(k.get())] = iter;
    if(was_empty) {
        expand_from_cfg("workspace_keywords_expanded", m_keywordsNode);
    }
}

void WorkspaceController::add_folder_item(const eng::LibFolder::Ptr & f)
{
    int icon_idx = ICON_ROLL;
    if(f->virtual_type() == eng::LibFolder::VirtualType::TRASH) {
        icon_idx = ICON_TRASH;
        getLibraryClient()->set_trash_id(f->id());
    }
    auto children = m_folderNode->children();
    bool was_empty = children.empty();
    auto iter = add_item(m_treestore, children,
                         m_icons[icon_idx],
                         f->name(), f->id(), FOLDER_ITEM);
    if(f->is_expanded()) {
        m_librarytree.expand_row(m_treestore->get_path(iter), false);
    }
    getLibraryClient()->countFolder(f->id());
    m_folderidmap[f->id()] = iter;
    // expand if needed. Because Gtk is stupid and doesn't expand empty
    if(was_empty) {
        expand_from_cfg("workspace_folders_expanded", m_folderNode);
    }
}

Gtk::TreeModel::iterator
WorkspaceController::add_item(const Glib::RefPtr<Gtk::TreeStore> &treestore,
                              const Gtk::TreeNodeChildren & childrens,
                              const Glib::RefPtr<Gdk::Pixbuf> & icon,
                              const Glib::ustring & label, 
                              eng::library_id_t id, int type) const
{
    Gtk::TreeModel::iterator iter;
    Gtk::TreeModel::Row row;
    iter = treestore->append(childrens);
    row = *iter;
    row[m_librarycolumns.m_icon] = icon;
    row[m_librarycolumns.m_label] = label; 
    row[m_librarycolumns.m_id] = id;
    row[m_librarycolumns.m_type] = type;
    row[m_librarycolumns.m_count] = "--";
    return iter;
}


Gtk::Widget * WorkspaceController::buildWidget()
{
    if(m_widget) {
        return m_widget;
    }
    m_widget = &m_vbox;
    m_treestore = Gtk::TreeStore::create(m_librarycolumns);
    m_librarytree.set_model(m_treestore);
    DBG_ASSERT(m_treestore->get_flags() & Gtk::TREE_MODEL_ITERS_PERSIST,
        "Model isn't persistent");

    m_folderNode = add_item(m_treestore, m_treestore->children(),
                            m_icons[ICON_FOLDER], 
                            Glib::ustring(_("Pictures")), 0,
                            FOLDERS_ITEM);
    m_projectNode = add_item(m_treestore, m_treestore->children(),
                             m_icons[ICON_PROJECT], 
                             Glib::ustring(_("Projects")), 0,
                             PROJECTS_ITEM);
    m_keywordsNode = add_item(m_treestore, m_treestore->children(),
                              m_icons[ICON_KEYWORD],
                              Glib::ustring(_("Keywords")), 0,
                              KEYWORDS_ITEM);
    m_librarytree.set_headers_visible(false);
    m_librarytree.append_column("", m_librarycolumns.m_icon);
    int num = m_librarytree.append_column("", m_librarycolumns.m_label);
    Gtk::TreeViewColumn * col = m_librarytree.get_column(num - 1);
    col->set_expand(true);
    num = m_librarytree.append_column("", m_librarycolumns.m_count);
    col = m_librarytree.get_column(num - 1);
    col->set_alignment(1.0f);

    // TODO make it a mnemonic
    m_label.set_text_with_mnemonic(Glib::ustring(_("_Workspace")));
    m_label.set_mnemonic_widget(m_librarytree);
    m_vbox.pack_start(m_label, Gtk::PACK_SHRINK);
    Gtk::ScrolledWindow* scrolled = Gtk::manage(new Gtk::ScrolledWindow);
    m_vbox.pack_start(*scrolled);
    scrolled->add(m_librarytree);

    m_librarytree.get_selection()->signal_changed().connect (
        sigc::mem_fun(this, 
                      &WorkspaceController::on_libtree_selection));
    m_librarytree.signal_row_expanded().connect(
        sigc::mem_fun(this,
                      &WorkspaceController::on_row_expanded));
    m_librarytree.signal_row_collapsed().connect(
        sigc::mem_fun(this,
                      &WorkspaceController::on_row_collapsed));

    return m_widget;
}

/** Expand treenode from a cfg key */
void WorkspaceController::expand_from_cfg(const char* key,
                                          const Gtk::TreeIter& treenode)
{
    fwk::Configuration::Ptr cfg = getLibraryConfig();

    bool expanded = std::stoi(cfg->getValue(key, "1"));
    if(expanded) {
        m_librarytree.expand_row(m_treestore->get_path(treenode),
                                 false);
    }
}

void WorkspaceController::on_ready()
{
    libraryclient::LibraryClientPtr libraryClient = getLibraryClient();
    if (libraryClient) {
        libraryClient->getAllFolders();
        libraryClient->getAllKeywords();
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
