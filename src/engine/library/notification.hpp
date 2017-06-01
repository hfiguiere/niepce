/* -*- mode: C++; tab-width: 4; c-basic-offset: 4; indent-tabs-mode:nil; -*- */
/*
 * niepce - engine/library/notification.hpp
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

#pragma once

#include <boost/variant.hpp>

#include "engine/library/clienttypes.hpp"

namespace eng {

class LibNotification
{
public:
    enum class Type {
        NONE = 0,
        NEW_LIBRARY_CREATED,
        ADDED_FOLDERS,
        ADDED_FILES,
        ADDED_KEYWORDS,
        ADDED_KEYWORD,
        ADDED_LABELS,
        FOLDER_CONTENT_QUERIED,
        KEYWORD_CONTENT_QUERIED,
        METADATA_QUERIED,
        METADATA_CHANGED,
        LABEL_CHANGED,
        LABEL_DELETED,
        XMP_NEEDS_UPDATE,
        FOLDER_COUNTED,
        FOLDER_COUNT_CHANGE,
        FILE_MOVED
    };

    struct None {
    };

    struct Id {
        library_id_t id;
    };

    struct AddedFolders {
        LibFolder::ListPtr folders;
    };

    struct AddedKeyword {
        Keyword::Ptr keyword;
    };

    struct AddedKeywords {
        Keyword::ListPtr keywords;
    };

    struct AddedLabels {
        Label::ListPtr labels;
    };

    struct FileMoved {
        library_id_t file;
        library_id_t from;
        library_id_t to;
    };

    struct QueriedContent {
        library_id_t container;
        LibFile::ListPtr files;
    };

    struct MetadataContent {
        library_id_t file;
        LibMetadata::Ptr metadata;
    };

    typedef metadata_desc_t MetadataChange;

    struct FolderCount {
        library_id_t folder;
        int32_t count;
    };

    struct LabelChange {
        eng::Label::Ptr label;
    };

    // specialise this class template to map the notification parameter type
    // with the notification type.
    // By default type is None.
    template<Type t> struct ParamType {
        typedef None Type;
    };

    typedef boost::variant<None, Id, AddedFolders,
                           AddedLabels, AddedKeyword, AddedKeywords,
                           QueriedContent,
                           MetadataContent, MetadataChange,
                           LabelChange,
                           FileMoved, FolderCount> Param;

    Type type;

    // Instanciate a notification. Will use the ParamType template to enforce the
    // parameter type.
    template<LibNotification::Type t>
    static LibNotification make(typename LibNotification::ParamType<t>::Type&& p)
        {
            return LibNotification(t, p);
        }
    // Extract the parameter.
    template<LibNotification::Type t>
    const typename LibNotification::ParamType<t>::Type& get() const
        {
            return boost::get<typename LibNotification::ParamType<t>::Type>(param);
        }
private:
    Param param;

    LibNotification(Type t, Param&& p)
        : type(t), param(p) {}
};

template<>
struct LibNotification::ParamType<LibNotification::Type::ADDED_FOLDERS> {
    typedef AddedFolders Type;
};

template<>
struct LibNotification::ParamType<LibNotification::Type::ADDED_KEYWORDS> {
    typedef AddedKeywords Type;
};

template<>
struct LibNotification::ParamType<LibNotification::Type::ADDED_KEYWORD> {
    typedef AddedKeyword Type;
};

template<>
struct LibNotification::ParamType<LibNotification::Type::ADDED_LABELS> {
    typedef AddedLabels Type;
};

template<>
struct LibNotification::ParamType<LibNotification::Type::FOLDER_CONTENT_QUERIED> {
    typedef QueriedContent Type;
};

template<>
struct LibNotification::ParamType<LibNotification::Type::KEYWORD_CONTENT_QUERIED> {
    typedef QueriedContent Type;
};

template<>
struct LibNotification::ParamType<LibNotification::Type::METADATA_QUERIED> {
    typedef MetadataContent Type;
};

template<>
struct LibNotification::ParamType<LibNotification::Type::METADATA_CHANGED> {
    typedef MetadataChange Type;
};

template<>
struct LibNotification::ParamType<LibNotification::Type::LABEL_CHANGED> {
    typedef LabelChange Type;
};

template<>
struct LibNotification::ParamType<LibNotification::Type::LABEL_DELETED> {
    typedef Id Type;
};

template<>
struct LibNotification::ParamType<LibNotification::Type::FOLDER_COUNTED> {
    typedef FolderCount Type;
};

template<>
struct LibNotification::ParamType<LibNotification::Type::FOLDER_COUNT_CHANGE> {
    typedef FolderCount Type;
};

template<>
struct LibNotification::ParamType<LibNotification::Type::FILE_MOVED> {
    typedef FileMoved Type;
};


}
