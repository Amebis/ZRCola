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
#pragma warning(disable: 4251)


namespace ZRCola {
    ///
    /// Key sequence database
    ///
    class ZRCOLAUI_API keyseq_db {
    public:
#pragma pack(push)
#pragma pack(2)
#pragma warning(push)
#pragma warning(disable: 4200)
        ///
        /// Key sequence data
        ///
        struct keyseq {
            enum modifiers_t {
                SHIFT   = 1<<0,             ///< SHIFT key was pressed
                CTRL    = 1<<1,             ///< CTRL key was pressed
                ALT     = 1<<2,             ///< ALT key was pressed
            };

            wchar_t chr;                        ///< Character
            unsigned __int16 seq_len;           ///< \c seq length
            struct key_t {
                wchar_t key;                    ///< Key
                unsigned __int16 modifiers;     ///< Modifiers (bitwise combination of SHIFT, CTRL and ALT)
            } seq[];                            ///< Key sequence
        };
#pragma warning(pop)
#pragma pack(pop)

        std::vector<unsigned __int32> idxChr;   ///< Character index
        std::vector<unsigned __int32> idxKey;   ///< Key index
        std::vector<unsigned __int16> data;     ///< Key sequences data

    public:
        ///
        /// Get text representation of a given key sequence
        ///
        /// \param[in]  seq      Key sequence
        /// \param[in]  seq_len  Number of elements in \p seq
        /// \param[out] str     Text representation of a \p seq key sequence
        ///
        static void GetSequenceAsText(_In_count_(seq_len) const keyseq::key_t *seq, _In_ size_t seq_len, _Out_ std::wstring& str);

        ///
        /// Get text representation of a given key sequence
        ///
        /// \param[in]  seq      Key sequence
        /// \param[in]  seq_len  Number of elements in \p seq
        ///
        /// \returns Text representation of a \p seq key sequence
        ///
        static inline std::wstring GetSequenceAsText(_In_count_(seq_len) const keyseq::key_t *seq, _In_ size_t seq_len)
        {
            std::wstring str;
            GetSequenceAsText(seq, seq_len, str);
            return str;
        }
    };


    typedef ZRCOLAUI_API stdex::idrec::record<keyseq_db, recordid_t, recordsize_t, ZRCOLA_RECORD_ALIGN> keyseq_rec;
};


const ZRCola::recordid_t stdex::idrec::record<ZRCola::keyseq_db, ZRCola::recordid_t, ZRCola::recordsize_t, ZRCOLA_RECORD_ALIGN>::id = *(ZRCola::recordid_t*)"KEY";


///
/// Reads key sequence database from a stream
///
/// \param[in]  stream  Input stream
/// \param[out] db   Key sequence database
///
/// \returns The stream \p stream
///
inline std::istream& operator >>(_In_ std::istream& stream, _Out_ ZRCola::keyseq_db &db)
{
    unsigned __int32 count;

    // Read index count.
    stream.read((char*)&count, sizeof(count));
    if (!stream.good()) return stream;

    // Read character index.
    db.idxChr.resize(count);
    stream.read((char*)db.idxChr.data(), sizeof(unsigned __int32)*count);
    if (!stream.good()) return stream;

    // Read key index.
    db.idxKey.resize(count);
    stream.read((char*)db.idxKey.data(), sizeof(unsigned __int32)*count);
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
