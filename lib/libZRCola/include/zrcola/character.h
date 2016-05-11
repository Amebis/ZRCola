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
#pragma warning(disable: 4200)
#pragma warning(disable: 4251)
#pragma warning(disable: 4512)


namespace ZRCola {
    ///
    /// Character category ID type
    /// Two letter abbreviation, non-terminated
    ///
    typedef char chrcatid_t[2];

    ///
    /// Character Database
    ///
    class ZRCOLA_API character_db {
    public:
#pragma pack(push)
#pragma pack(2)
        ///
        /// Character data
        ///
        struct character {
            wchar_t chr;                ///> Character
            chrcatid_t cat;             ///> Category ID
            unsigned __int16 desc_len;  ///< Character description length in \c data
            unsigned __int16 rel_len;   ///< Related character count in \c data
            wchar_t data[];             ///< Character description and list of related characters
        };
#pragma pack(pop)

        ///
        /// Character index
        ///
        class indexChar : public index<unsigned __int16, unsigned __int32, character>
        {
        public:
            ///
            /// Constructs the index
            ///
            /// \param[in] h  Reference to vector holding the data
            ///
            indexChar(_In_ std::vector<unsigned __int16> &h) : index<unsigned __int16, unsigned __int32, character>(h) {}

            ///
            /// Compares two characters by ID (for searching)
            ///
            /// \param[in] a  Pointer to first element
            /// \param[in] b  Pointer to second element
            ///
            /// \returns
            /// - <0 when a <  b
            /// - =0 when a == b
            /// - >0 when a >  b
            ///
            virtual int compare(_In_ const character &a, _In_ const character &b) const
            {
                     if (a.chr < b.chr) return -1;
                else if (a.chr > b.chr) return  1;

                return 0;
            }
        } idxChr;      ///< Character index

        std::vector<unsigned __int16> data;     ///< Character data

    public:
        ///
        /// Constructs the database
        ///
        inline character_db() : idxChr(data) {}
    };


    typedef ZRCOLA_API stdex::idrec::record<character_db, recordid_t, recordsize_t, ZRCOLA_RECORD_ALIGN> character_rec;


    ///
    /// Character category database
    ///
    class ZRCOLA_API chrcat_db {
    public:
#pragma pack(push)
#pragma pack(2)
        ///
        /// Character category data
        ///
        struct chrcat {
            chrcatid_t id;              ///< Character category ID
            unsigned __int16 rank;      ///< Character category rank
            unsigned __int16 name_len;  ///< \c name length (in characters)
            wchar_t name[];             ///< Character category name
        };
#pragma pack(pop)

        ///
        /// Character category index
        ///
        class indexChrCat : public index<unsigned __int16, unsigned __int32, chrcat>
        {
        public:
            ///
            /// Constructs the index
            ///
            /// \param[in] h  Reference to vector holding the data
            ///
            indexChrCat(_In_ std::vector<unsigned __int16> &h) : index<unsigned __int16, unsigned __int32, chrcat>(h) {}

            ///
            /// Compares two character categories by ID (for searching)
            ///
            /// \param[in] a  Pointer to first element
            /// \param[in] b  Pointer to second element
            ///
            /// \returns
            /// - <0 when a <  b
            /// - =0 when a == b
            /// - >0 when a >  b
            ///
            virtual int compare(_In_ const chrcat &a, _In_ const chrcat &b) const
            {
                int r = memcmp(a.id, b.id, sizeof(chrcatid_t));
                if (r != 0) return r;

                return 0;
            }
        } idxChrCat;      ///< Character category index

        ///
        /// Rank index
        ///
        class indexRank : public index<unsigned __int16, unsigned __int32, chrcat>
        {
        public:
            ///
            /// Constructs the index
            ///
            /// \param[in] h  Reference to vector holding the data
            ///
            indexRank(_In_ std::vector<unsigned __int16> &h) : index<unsigned __int16, unsigned __int32, chrcat>(h) {}

            ///
            /// Compares two character categories by ID (for searching)
            ///
            /// \param[in] a  Pointer to first element
            /// \param[in] b  Pointer to second element
            ///
            /// \returns
            /// - <0 when a <  b
            /// - =0 when a == b
            /// - >0 when a >  b
            ///
            virtual int compare(_In_ const chrcat &a, _In_ const chrcat &b) const
            {
                     if (a.rank < b.rank) return -1;
                else if (a.rank > b.rank) return +1;

                return 0;
            }

            ///
            /// Compares two character categories by rank (for sorting)
            ///
            /// \param[in] a  Pointer to character category
            /// \param[in] b  Pointer to second character category
            ///
            /// \returns
            /// - <0 when a <  b
            /// - =0 when a == b
            /// - >0 when a >  b
            ///
            virtual int compare_sort(_In_ const chrcat &a, _In_ const chrcat &b) const
            {
                     if (a.rank < b.rank) return -1;
                else if (a.rank > b.rank) return +1;

                int r = _wcsncoll(a.name, b.name, std::min<unsigned __int16>(a.name_len, b.name_len));
                if (r != 0) return r;
                     if (a.name_len < b.name_len) return -1;
                else if (a.name_len > b.name_len) return +1;

                return 0;
            }
        } idxRnk;      ///< Rank index

        std::vector<unsigned __int16> data;     ///< Character category data

    public:
        ///
        /// Constructs the database
        ///
        inline chrcat_db() : idxChrCat(data), idxRnk(data) {}
    };


    typedef ZRCOLA_API stdex::idrec::record<chrcat_db, recordid_t, recordsize_t, ZRCOLA_RECORD_ALIGN> chrcat_rec;
};


const ZRCola::recordid_t stdex::idrec::record<ZRCola::character_db, ZRCola::recordid_t, ZRCola::recordsize_t, ZRCOLA_RECORD_ALIGN>::id = *(ZRCola::recordid_t*)"CHR";
const ZRCola::recordid_t stdex::idrec::record<ZRCola::chrcat_db, ZRCola::recordid_t, ZRCola::recordsize_t, ZRCOLA_RECORD_ALIGN>::id = *(ZRCola::recordid_t*)"CCT";


///
/// Reads character database from a stream
///
/// \param[in]  stream  Input stream
/// \param[out] db      Character database
///
/// \returns The stream \p stream
///
inline std::istream& operator >>(_In_ std::istream& stream, _Out_ ZRCola::character_db &db)
{
    unsigned __int32 count;

    // Read index count.
    stream.read((char*)&count, sizeof(count));
    if (!stream.good()) return stream;

    // Read character index.
    db.idxChr.resize(count);
    stream.read((char*)db.idxChr.data(), sizeof(unsigned __int32)*count);
    if (!stream.good()) return stream;

    // Read data count.
    stream.read((char*)&count, sizeof(count));
    if (!stream.good()) return stream;

    // Read data.
    db.data.resize(count);
    stream.read((char*)db.data.data(), sizeof(unsigned __int16)*count);

    return stream;
}


///
/// Reads character category database from a stream
///
/// \param[in]  stream  Input stream
/// \param[out] db      Character category database
///
/// \returns The stream \p stream
///
inline std::istream& operator >>(_In_ std::istream& stream, _Out_ ZRCola::chrcat_db &db)
{
    unsigned __int32 count;

    // Read index count.
    stream.read((char*)&count, sizeof(count));
    if (!stream.good()) return stream;

    // Read character category index.
    db.idxChrCat.resize(count);
    stream.read((char*)db.idxChrCat.data(), sizeof(unsigned __int32)*count);
    if (!stream.good()) return stream;

    // Read rank index.
    db.idxRnk.resize(count);
    stream.read((char*)db.idxRnk.data(), sizeof(unsigned __int32)*count);
    if (!stream.good()) return stream;

    // Read data count.
    stream.read((char*)&count, sizeof(count));
    if (!stream.good()) return stream;

    // Read data.
    db.data.resize(count);
    stream.read((char*)db.data.data(), sizeof(unsigned __int16)*count);

    return stream;
}

#pragma warning(pop)
