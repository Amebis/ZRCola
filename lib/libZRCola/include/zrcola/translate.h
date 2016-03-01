/*
    Copyright 2015-2016 Amebis

    This file is part of ZRCola.

    ZRCola is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    ZRCola is distributed in the hope that it will be useful, but
    WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with ZRCola. If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include "common.h"

#include <stdex/idrec.h>
#include <istream>
#include <vector>
#include <string>

#pragma warning(push)
#pragma warning(disable: 4251)


namespace ZRCola {
    ///
    /// Translation database
    ///
    class ZRCOLA_API translation_db {
    public:
#pragma pack(push)
#pragma pack(4)
        ///
        /// Translation index
        ///
        struct index {
            unsigned __int32 start; ///< Composed character offset
            unsigned __int32 end;   ///< Decomposed string end offset

            ///
            /// Returns translation string start offset
            ///
            inline unsigned __int32 GetChrStart() const { return start; }

            ///
            /// Returns translation string start offset
            ///
            inline unsigned __int32 GetStrStart() const { return start + 1; }

            ///
            /// Returns translation string length
            ///
            inline unsigned __int32 GetStrLength() const { return end - (start + 1); }
        };
#pragma pack(pop)

        std::vector<index> idxComp;      ///< Composition index
        std::vector<index> idxDecomp;    ///< Decomposition index
        std::vector<wchar_t> data;          ///< Transformation data

    public:
        ///
        /// Composes string
        ///
        /// \param[in]  input     Input string (UTF-16)
        /// \param[in]  inputMax  Length of the input string in characters. Can be (size_t)-1 if \p input is zero terminated.
        /// \param[out] output    Output string (UTF-16)
        /// \param[out] map       The vector of source to destination index mappings (optional)
        ///
        void Compose(_In_z_count_(inputMax) const wchar_t* input, _In_ size_t inputMax, _Out_ std::wstring &output, _Out_opt_ std::vector<mapping>* map = NULL) const;

        ///
        /// Decomposes string
        ///
        /// \param[in]  input     Input string (UTF-16)
        /// \param[in]  inputMax  Length of the input string in characters. Can be (size_t)-1 if \p input is zero terminated.
        /// \param[out] output    Output string (UTF-16)
        /// \param[out] map       The vector of source to destination index mappings (optional)
        ///
        void Decompose(_In_z_count_(inputMax) const wchar_t* input, _In_ size_t inputMax, _Out_ std::wstring &output, _Out_opt_ std::vector<mapping>* map = NULL) const;
    };


    typedef ZRCOLA_API stdex::idrec::record<translation_db, recordid_t, recordsize_t, ZRCOLA_RECORD_ALIGN> translation_rec;
};


const ZRCola::recordid_t stdex::idrec::record<ZRCola::translation_db, ZRCola::recordid_t, ZRCola::recordsize_t, ZRCOLA_RECORD_ALIGN>::id = ZRCOLA_DB_TRANSLATIONS_ID;


///
/// Reads translation database from a stream
///
/// \param[in]  stream  Input stream
/// \param[out] t_db    Translation database
///
/// \returns The stream \p stream
///
inline std::istream& operator >>(_In_ std::istream& stream, _Out_ ZRCola::translation_db &t_db)
{
    unsigned __int32 count;

    // Read index count.
    stream.read((char*)&count, sizeof(count));
    if (!stream.good()) return stream;

    // Read composition index.
    t_db.idxComp.resize(count);
    stream.read((char*)t_db.idxComp.data(), sizeof(ZRCola::translation_db::index)*count);
    if (!stream.good()) return stream;

    // Read decomposition index.
    t_db.idxDecomp.resize(count);
    stream.read((char*)t_db.idxDecomp.data(), sizeof(ZRCola::translation_db::index)*count);
    if (!stream.good()) return stream;

    // Read data count.
    stream.read((char*)&count, sizeof(count));
    if (!stream.good()) return stream;

    // Read data.
    t_db.data.resize(count);
    stream.read((char*)t_db.data.data(), sizeof(wchar_t)*count);

    return stream;
}

#pragma warning(pop)
