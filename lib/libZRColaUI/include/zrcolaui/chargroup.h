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
#include <zrcola/common.h>

#include <stdex/idrec.h>
#include <istream>
#include <vector>

#pragma warning(push)
#pragma warning(disable: 4200)
#pragma warning(disable: 4251)
#pragma warning(disable: 4512)


namespace ZRCola {
    ///
    /// Character group database
    ///
    class ZRCOLAUI_API chrgrp_db {
    public:
#pragma pack(push)
#pragma pack(2)
        ///
        /// Character group data
        ///
        struct chrgrp {
            unsigned __int16 id;                ///< Character group id
            unsigned __int16 rank;              ///< Character group rank
            unsigned __int16 name_len;          ///< Character group name length in \c data
            unsigned __int16 char_len;          ///< Character list length in \c data
            wchar_t data[];                     ///< Character group name and character list
        };
#pragma pack(pop)

        ///
        /// Rank index
        ///
        class indexRank : public index<unsigned __int16, unsigned __int32, chrgrp>
        {
        public:
            ///
            /// Constructs the index
            ///
            /// \param[in] h  Reference to vector holding the data
            ///
            indexRank(_In_ std::vector<unsigned __int16> &h) : index<unsigned __int16, unsigned __int32, chrgrp>(h) {}

            ///
            /// Compares two character groups by rank (for searching)
            ///
            /// \param[in] a  Pointer to character group
            /// \param[in] b  Pointer to second character group
            ///
            /// \returns
            /// - <0 when a <  b
            /// - =0 when a == b
            /// - >0 when a >  b
            ///
            virtual int compare(_In_ const chrgrp &a, _In_ const chrgrp &b) const
            {
                     if (a.rank < b.rank) return -1;
                else if (a.rank > b.rank) return +1;

                return 0;
            }

            ///
            /// Compares two character groups by rank (for sorting)
            ///
            /// \param[in] a  Pointer to character group
            /// \param[in] b  Pointer to second character group
            ///
            /// \returns
            /// - <0 when a <  b
            /// - =0 when a == b
            /// - >0 when a >  b
            ///
            virtual int compare_sort(_In_ const chrgrp &a, _In_ const chrgrp &b) const
            {
                     if (a.rank < b.rank) return -1;
                else if (a.rank > b.rank) return +1;

                int r = _wcsncoll(a.data, b.data, std::min<unsigned __int16>(a.name_len, b.name_len));
                if (r != 0) return r;
                     if (a.name_len < b.name_len) return -1;
                else if (a.name_len > b.name_len) return +1;

                return 0;
            }
        } idxRnk;   ///< Rank index

        std::vector<unsigned __int16> data;     ///< Character groups data

    public:
        ///
        /// Constructs the database
        ///
        inline chrgrp_db() : idxRnk(data) {}
    };


    typedef ZRCOLAUI_API stdex::idrec::record<chrgrp_db, recordid_t, recordsize_t, ZRCOLA_RECORD_ALIGN> chrgrp_rec;
};


const ZRCola::recordid_t stdex::idrec::record<ZRCola::chrgrp_db, ZRCola::recordid_t, ZRCola::recordsize_t, ZRCOLA_RECORD_ALIGN>::id = *(ZRCola::recordid_t*)"CGR";


///
/// Reads character group database from a stream
///
/// \param[in]  stream  Input stream
/// \param[out] db      Character group database
///
/// \returns The stream \p stream
///
inline std::istream& operator >>(_In_ std::istream& stream, _Out_ ZRCola::chrgrp_db &db)
{
    unsigned __int32 count;

    // Read index count.
    stream.read((char*)&count, sizeof(count));
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
