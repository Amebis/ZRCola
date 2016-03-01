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

#include <stdex/idrec.h>
#include <istream>
#include <vector>


///
/// Public function calling convention
///
#ifdef LIBZRCOLA
#define ZRCOLA_API      __declspec(dllexport)
#else
#define ZRCOLA_API      __declspec(dllimport)
#endif
#define ZRCOLA_NOVTABLE __declspec(novtable)
#pragma warning(push)
#pragma warning(disable: 4251)


///
/// Data records alignment
///
#define ZRCOLA_RECORD_ALIGN 1

///
/// Database IDs
///
#define ZRCOLA_DB_ID                ((ZRCola::recordid_t)0x0043525a)
#define ZRCOLA_DB_TRANSLATIONS_ID   ((ZRCola::recordid_t)0x00000001)


namespace ZRCola {
    typedef unsigned __int32 recordid_t;
    typedef unsigned __int32 recordsize_t;


    ///
    /// Translation database
    ///
    class translation_db {
    public:
#pragma pack(push)
#pragma pack(4)
        ///
        /// Translation index
        ///
        struct index {
            unsigned __int32 start; ///< Composed character offset
            unsigned __int32 end;   ///< Decomposed string end offset
        };
#pragma pack(pop)

        std::vector<index> comp_index;      ///< Composition index
        std::vector<index> decomp_index;    ///< Decomposition index
        std::vector<wchar_t> data;          ///< Transformation data
    };


    typedef ZRCOLA_API stdex::idrec::record<translation_db, recordid_t, recordsize_t, ZRCOLA_RECORD_ALIGN> translation_rec;


    ///
    /// Composed-decomposed index transformation mapping
    ///
    class ZRCOLA_NOVTABLE ZRCOLA_API mapping {
    public:
        size_t cmp;     ///< Character index in composed string
        size_t decmp;   ///< Character index in decomposed string

        inline mapping() {};
        inline mapping(_In_ size_t c, _In_ size_t d) : cmp(c), decmp(d) {}
    };


    ///
    /// A vector for composed-decomposed index transformation mapping
    ///
    class ZRCOLA_API mapping_vector : public std::vector<mapping> {
    public:
        ///
        /// Transforms character index of decomposed to composed string
        ///
        /// \param[in] decmp  Character index in decomposed string
        ///
        /// \returns  Character index in composed string
        ///
        size_t to_composed(_In_ size_t decmp) const;

        ///
        /// Transforms destination index to source index
        ///
        /// \param[in] cmp  Character index in composed string
        ///
        /// \returns  Character index in decomposed string
        ///
        size_t to_decomposed(_In_ size_t cmp) const;
    };
};

#pragma warning(pop)


const ZRCola::recordid_t stdex::idrec::record<ZRCola::translation_db, ZRCola::recordid_t, ZRCola::recordsize_t, ZRCOLA_RECORD_ALIGN>::id = ZRCOLA_DB_TRANSLATIONS_ID;


///
/// Reads translation database from a stream
///
/// \param[in]  stream  Input stream
/// \param[out] t_db    Translation database
///
/// \returns The stream \p stream
///
inline std::istream& operator >>(std::istream& stream, ZRCola::translation_db &t_db)
{
    unsigned __int32 count;

    // Read index count.
    stream.read((char*)&count, sizeof(count));
    if (!stream.good()) return stream;

    // Read composition index.
    t_db.comp_index.resize(count);
    stream.read((char*)t_db.comp_index.data(), sizeof(ZRCola::translation_db::index)*count);
    if (!stream.good()) return stream;

    // Read decomposition index.
    t_db.decomp_index.resize(count);
    stream.read((char*)t_db.decomp_index.data(), sizeof(ZRCola::translation_db::index)*count);
    if (!stream.good()) return stream;

    // Read data count.
    stream.read((char*)&count, sizeof(count));
    if (!stream.good()) return stream;

    // Read data.
    t_db.data.resize(count);
    stream.read((char*)t_db.data.data(), sizeof(wchar_t)*count);

    return stream;
}
