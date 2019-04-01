/*
    Copyright 2015-2019 Amebis

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
#include <assert.h>
#include <algorithm>
#include <istream>
#include <ostream>
#include <vector>

#pragma warning(push)
#pragma warning(disable: 4200)
#pragma warning(disable: 4251)
#pragma warning(disable: 4512)


namespace ZRCola {
    ///
    /// Character group ID
    ///
    typedef unsigned __int16 chrgrpid_t;


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
        public:
            chrgrpid_t grp;                     ///< Character group ID
            unsigned __int16 rank;              ///< Character group rank

        protected:
            unsigned __int16 name_to;           ///< Character group name end in \c data
            unsigned __int16 chrlst_to;         ///< Character list end in \c data
            wchar_t data[];                     ///< Character group name, character list, bit vector if particular character is displayed initially

        public:
            ///
            /// Constructs the character group
            ///
            /// \param[in] grp         Character group ID
            /// \param[in] rank        Character group rank
            /// \param[in] name        Character group name
            /// \param[in] name_len    Number of UTF-16 characters in \p name
            /// \param[in] chrlst      Character list (zero delimited)
            /// \param[in] chrlst_len  Number of UTF-16 characters in \p chrlst (including zero delimiters)
            /// \param[in] chrshow     Binary vector which particular character is displayed initially
            ///
            inline chrgrp(
                _In_opt_                                     chrgrpid_t        grp        = 0,
                _In_opt_                                     unsigned __int16  rank       = 0,
                _In_opt_z_count_(name_len)             const wchar_t          *name       = NULL,
                _In_opt_                                     size_t            name_len   = 0,
                _In_opt_z_count_(chrlst_len)           const wchar_t          *chrlst     = NULL,
                _In_opt_                                     size_t            chrlst_len = 0,
                _In_opt_count_x_((chrlst_len + 15)/16) const unsigned __int16 *chrshow    = NULL)
            {
                this->grp  = grp;
                this->rank = rank;
                this->name_to = static_cast<unsigned __int16>(name_len);
                if (name_len) memcpy(this->data, name, sizeof(wchar_t)*name_len);
                this->chrlst_to = static_cast<unsigned __int16>(this->name_to + chrlst_len);
                if (chrlst_len) {
                    memcpy(this->data + this->name_to, chrlst, sizeof(wchar_t)*chrlst_len);
                    memcpy(this->data + this->chrlst_to, chrshow, (chrlst_len + sizeof(*data)*8 - 1)/8);
                }
            }

            inline const wchar_t*         name    () const { return data;           };
            inline       wchar_t*         name    ()       { return data;           };
            inline const wchar_t*         name_end() const { return data + name_to; };
            inline       wchar_t*         name_end()       { return data + name_to; };
            inline       unsigned __int16 name_len() const { return name_to;        };

            inline const wchar_t*         chrlst    () const { return data + name_to;      };
            inline       wchar_t*         chrlst    ()       { return data + name_to;      };
            inline const wchar_t*         chrlst_end() const { return data + chrlst_to;    };
            inline       wchar_t*         chrlst_end()       { return data + chrlst_to;    };
            inline       unsigned __int16 chrlst_len() const { return chrlst_to - name_to; };

            inline const unsigned __int16* chrshow    () const { return reinterpret_cast<const unsigned __int16*>(data + chrlst_to                ); };
            inline       unsigned __int16* chrshow    ()       { return reinterpret_cast<      unsigned __int16*>(data + chrlst_to                ); };
            inline const unsigned __int16* chrshow_end() const { return reinterpret_cast<const unsigned __int16*>(data + chrlst_to + chrshow_len()); };
            inline       unsigned __int16* chrshow_end()       { return reinterpret_cast<      unsigned __int16*>(data + chrlst_to + chrshow_len()); };
            inline       unsigned __int16  chrshow_len() const { return (chrlst_len() + sizeof(*data)*8 - 1)/(sizeof(*data)*8);                      };
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

                unsigned __int16
                    a_name_len = a.name_len(),
                    b_name_len = b.name_len();
                int r = _wcsncoll(a.name(), b.name(), std::min<unsigned __int16>(a_name_len, b_name_len));
                if (r != 0) return r;
                     if (a_name_len < b_name_len) return -1;
                else if (a_name_len > b_name_len) return +1;

                return 0;
            }
        } idxRank;  ///< Rank index

        std::vector<unsigned __int16> data;     ///< Character groups data

    public:
        ///
        /// Constructs the database
        ///
        inline chrgrp_db() : idxRank(data) {}
    };


    typedef ZRCOLAUI_API stdex::idrec::record<chrgrp_db, recordid_t, recordsize_t, ZRCOLA_RECORD_ALIGN> chrgrp_rec;
};


const ZRCola::recordid_t ZRCola::chrgrp_rec::id = *(ZRCola::recordid_t*)"CGR";


///
/// Writes character group database to a stream
///
/// \param[in] stream  Output stream
/// \param[in] db      Character group database
///
/// \returns The stream \p stream
///
inline std::ostream& operator <<(_In_ std::ostream& stream, _In_ const ZRCola::chrgrp_db &db)
{
    // Write rank index.
    if (stream.fail()) return stream;
    stream << db.idxRank;

    // Write data count.
    auto data_count = db.data.size();
#if defined(_WIN64) || defined(__x86_64__) || defined(__ppc64__)
    // 4G check
    if (data_count > 0xffffffff) {
        stream.setstate(std::ios_base::failbit);
        return stream;
    }
#endif
    if (stream.fail()) return stream;
    unsigned __int32 count = (unsigned __int32)data_count;
    stream.write((const char*)&count, sizeof(count));

    // Write data.
    if (stream.fail()) return stream;
    stream.write((const char*)db.data.data(), sizeof(unsigned __int16)*count);

    return stream;
}


///
/// Reads character group database from a stream
///
/// \param[in ] stream  Input stream
/// \param[out] db      Character group database
///
/// \returns The stream \p stream
///
inline std::istream& operator >>(_In_ std::istream& stream, _Out_ ZRCola::chrgrp_db &db)
{
    // Read rank index.
    stream >> db.idxRank;
    if (!stream.good()) return stream;

    // Read data count.
    unsigned __int32 count;
    stream.read((char*)&count, sizeof(count));
    if (!stream.good()) return stream;

    if (count) {
        // Read data.
        db.data.resize(count);
        stream.read((char*)db.data.data(), sizeof(unsigned __int16)*count);
    } else
        db.data.clear();

    return stream;
}

#pragma warning(pop)
