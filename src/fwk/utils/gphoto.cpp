/* -*- mode: C++; tab-width: 4; c-basic-offset: 4; indent-tabs-mode:nil; -*- */
/*
 * niepce - fwk/utils/gphoto.cpp
 *
 * Copyright (C) 2009-2017 Hubert Figuiere
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

#include <string.h>
#include <string>
#include <vector>

#include <gphoto2-port-info-list.h>
#include <gphoto2-abilities-list.h>
#include <gphoto2-camera.h>
#include <gphoto2-context.h>
#include <gphoto2-port-result.h>

#include <gdkmm/pixbufloader.h>

#include "fwk/base/debug.hpp"
#include "fwk/utils/pathutils.hpp"
#include "fwk/toolkit/thumbnail.hpp"
#include "gphoto.hpp"


namespace fwk {


#define GP_CHECK(x, op)   if (x < GP_OK) { \
    DBG_OUT("%s failed with %d", #op, x); }


GpDevice::GpDevice(const std::string& model, const std::string& path)
    : m_model(model)
    , m_path(path)
{
}



GpDeviceList::GpDeviceList()
    : m_abilities(nullptr)
    , m_ports(nullptr)
{
    reload();
}

GpDeviceList::~GpDeviceList()
{
    gp_cleanup();
}


void GpDeviceList::gp_cleanup()
{
    if (m_abilities) {
        ::gp_abilities_list_free(m_abilities);
        m_abilities = nullptr;
    }
    if (m_ports) {
        ::gp_port_info_list_free(m_ports);
        m_ports = nullptr;
    }
}

void GpDeviceList::reload()
{
    gp_cleanup();

    int ret;
    ret = ::gp_port_info_list_new(&m_ports);
    GP_CHECK(ret, gp_port_info_list_new);
    ret = ::gp_port_info_list_load(m_ports);
    GP_CHECK(ret, gp_port_list_load);
	ret = ::gp_abilities_list_new(&m_abilities);
    GP_CHECK(ret, gp_abilities_list_new);
    ret = ::gp_abilities_list_load(m_abilities, NULL);
    GP_CHECK(ret, gp_abilities_list_load);
}


void GpDeviceList::detect()
{
    if((!m_ports) || (!m_abilities)) {
        reload();
    }
    ::CameraList *camera_list;
    int ret;

    ret = ::gp_list_new(&camera_list);
    GP_CHECK(ret, gp_list_new);
    ret = ::gp_abilities_list_detect(m_abilities, m_ports, camera_list, nullptr);
    GP_CHECK(ret, gp_abilities_list_detect);

    int count = ::gp_list_count(camera_list);
    for (int i = 0; i < count; i++) {
        const char * name = nullptr;
        const char * value = nullptr;

        ret = ::gp_list_get_name(camera_list, i, &name);
        GP_CHECK(ret, gp_list_get_name);

        ret = ::gp_list_get_value(camera_list, i, &value);
        GP_CHECK(ret, gp_list_get_value);

        if ((count > 1) && (strcmp(value, "usb:") == 0)) {
            continue;
        }
        DBG_OUT("found %s %s", name, value);
        push_back(GpDevicePtr(new GpDevice(name, value)));
    }
    ::gp_list_free(camera_list);
}

fwk::Option<GpDevicePtr> GpDeviceList::get_device(const std::string& device)
{
    for (auto d : fwk::GpDeviceList::obj()) {
        if (d->get_path() == device) {
            return fwk::Option<GpDevicePtr>(d);
        }
    }
    return fwk::Option<GpDevicePtr>();
}

class GpCamera::Priv {
public:
    Priv()
        : camera(nullptr)
        , context(::gp_context_new())
        {
        }
    ~Priv()
        {
            if (camera) {
                ::gp_camera_unref(camera);
            }
            ::gp_context_unref(context);
        }
    ::Camera* camera;
    ::GPContext* context;
};

GpCamera::GpCamera(const GpDevicePtr& device)
    : m_device(device)
    , m_priv(new Priv)
{
}

GpCamera::~GpCamera()
{
    delete m_priv;
}

bool GpCamera::open()
{
    bool success = false;
    if (m_priv->camera) {
        close();
    }
    ::gp_camera_new(&m_priv->camera);

    CameraAbilities abilities;
    auto al = fwk::GpDeviceList::obj().get_abilities_list();
    int model_index = ::gp_abilities_list_lookup_model(al, m_device->get_model().c_str());
	::gp_abilities_list_get_abilities(al, model_index, &abilities);
    ::gp_camera_set_abilities(m_priv->camera, abilities);

	GPPortInfo info;
    auto info_list = ::fwk::GpDeviceList::obj().get_port_info_list();
    DBG_OUT("looking up port %s",  m_device->get_path().c_str());
    int port_index = ::gp_port_info_list_lookup_path(info_list, m_device->get_path().c_str());
    DBG_OUT("port index = %d", port_index);
    if (port_index >= 0) {
        ::gp_port_info_list_get_info(info_list, port_index, &info);
        ::gp_camera_set_port_info(m_priv->camera, info);

        int result = ::gp_camera_init(m_priv->camera, m_priv->context);
        DBG_OUT("camera init returned %d", result);
        switch (result) {
        case GP_OK:
            DBG_OUT("camera open");
            success = true;
            break;
        case GP_ERROR_CANCEL:
            break;
        default:
            close();
        }
    }

    return success;
}

bool GpCamera::close()
{
    ::gp_camera_unref(m_priv->camera);
    m_priv->camera = nullptr;
    return true;
}

void GpCamera::process_folders(const std::vector<std::string>& folders,
                               std::list<std::pair<std::string, std::string>>& files) const
{
    for (auto folder : folders) {
        CameraList *flist;
        ::gp_list_new(&flist);
        int result = ::gp_camera_folder_list_files(m_priv->camera, folder.c_str(), flist,
                                                m_priv->context);
        DBG_OUT("listed folder %d", result);
        if (result == GP_OK) {
            int count = ::gp_list_count(flist);
            DBG_OUT("processing folder %s, count %d", folder.c_str(), count);
            for (int i = 0; i < count; i++) {
                const char *name = nullptr;
                ::gp_list_get_name(flist, i, &name);
                files.push_back(std::make_pair(folder, name));
            }
        }
        gp_list_unref(flist);
    }
}

std::list<std::pair<std::string, std::string>> GpCamera::list_content() const
{
    std::list<std::pair<std::string, std::string>> files;

    DBG_OUT("list content");
    CameraList *list = nullptr;

    // XXX fixme this should not be hardcoded.
    std::string root_folder = "/store_00010001/DCIM";
    ::gp_list_new(&list);
    int result = gp_camera_folder_list_folders(m_priv->camera, root_folder.c_str(), list,
                                               m_priv->context);

    DBG_OUT("initial folder list %d", result);
    if (result == GP_OK) {
        std::vector<std::string> folders;
        int count = gp_list_count(list);
        DBG_OUT("list count %d", count);

        for (int i = 0; i < count; i++) {
            const char* name = nullptr;
            ::gp_list_get_name(list, i, &name);
            DBG_OUT("found folder %s", name);
            folders.push_back(root_folder + "/" + name);
        }

        process_folders(folders, files);
    }
    ::gp_list_unref(list);
    return files;
}

fwk::Thumbnail GpCamera::get_preview(const std::string& path) const
{
    fwk::Thumbnail thumbnail;
    CameraFile* file;
    std::string folder = fwk::path_dirname(path);
    std::string name = fwk::path_basename(path);

    ::gp_file_new(&file);
    int result = ::gp_camera_file_get (m_priv->camera, folder.c_str(), name.c_str(),
                                       GP_FILE_TYPE_PREVIEW, file,
                                       m_priv->context);
    DBG_OUT("file_get %s %d", path.c_str(), result);
    if (result >= 0) {
        const char *fd;
        unsigned long fs;
        ::gp_file_get_data_and_size(file, &fd, &fs);

        Glib::RefPtr<Gdk::PixbufLoader> loader = Gdk::PixbufLoader::create();
        loader->write(reinterpret_cast<const guint8*>(fd), fs);
        loader->close();
        thumbnail = fwk::Thumbnail(loader->get_pixbuf());
    }
    ::gp_file_unref(file);
    return thumbnail;
}

bool GpCamera::download_file(const std::string& folder, const std::string& file,
                             const std::string& dest)
{
    CameraFile *camerafile;
    DBG_OUT("importing into %s", dest.c_str());
    gp_file_new(&camerafile);
    int result = gp_camera_file_get(m_priv->camera, folder.c_str(), file.c_str(),
                                    GP_FILE_TYPE_NORMAL, camerafile,
                                    m_priv->context);
    if (result == GP_OK) {
        gp_file_save(camerafile, dest.c_str());
    }
    gp_file_unref(camerafile);

    return (result == GP_OK);
}

}
