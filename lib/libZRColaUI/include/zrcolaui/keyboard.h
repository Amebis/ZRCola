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
    /// Key sequence database
    ///
    class ZRCOLAUI_API keyseq_db {
    public:
#pragma pack(push)
#pragma pack(2)
        ///
        /// Key sequence data
        ///
        struct keyseq {
            enum modifiers_t {
                SHIFT   = 1<<0,                 ///< SHIFT key was pressed
                CTRL    = 1<<1,                 ///< CTRL key was pressed
                ALT     = 1<<2,                 ///< ALT key was pressed
            };

            wchar_t chr;                        ///< Character
            unsigned __int16 seq_len;           ///< \c seq length
            struct key_t {
                wchar_t key;                    ///< Key
                unsigned __int16 modifiers;     ///< Modifiers (bitwise combination of SHIFT, CTRL and ALT)
            } seq[];                            ///< Key sequence


            ///
            /// Compares two key sequences
            ///
            /// \param[in] seq_a    First key sequence
            /// \param[in] count_a  Number of keys in sequence \p seq_a
            /// \param[in] seq_b    Second key sequence
            /// \param[in] count_b  Number of keys in sequence \p seq_b
            ///
            /// \returns
            /// - <0 when seq_a <  seq_b
            /// - =0 when seq_a == seq_b
            /// - >0 when seq_a >  seq_b
            ///
            static inline int CompareSequence(const key_t *seq_a, unsigned __int16 count_a, const key_t *seq_b, unsigned __int16 count_b)
            {
                for (unsigned __int16 i = 0; ; i++) {
                         if (i >= count_a && i >= count_b) return  0;
                    else if (i >= count_a && i <  count_b) return -1;
                    else if (i <  count_a && i >= count_b) return +1;
                    else if (seq_a[i].key       < seq_b[i].key      ) return -1;
                    else if (seq_a[i].key       > seq_b[i].key      ) return +1;
                    else if (seq_a[i].modifiers < seq_b[i].modifiers) return -1;
                    else if (seq_a[i].modifiers > seq_b[i].modifiers) return +1;
                }
            }
        };
#pragma pack(pop)

        ///
        /// Character index
        ///
        class indexChr : public index<unsigned __int16, unsigned __int32, keyseq>
        {
        public:
            ///
            /// Constructs the index
            ///
            /// \param[in] h  Reference to vector holding the data
            ///
            indexChr(_In_ std::vector<unsigned __int16> &h) : index<unsigned __int16, unsigned __int32, keyseq>(h) {}

            ///
            /// Compares two key sequences by character (for searching)
            ///
            /// \param[in] a  Pointer to key sequence
            /// \param[in] b  Pointer to second key sequence
            ///
            /// \returns
            /// - <0 when a <  b
            /// - =0 when a == b
            /// - >0 when a >  b
            ///
            virtual int compare(_In_ const keyseq &a, _In_ const keyseq &b) const
            {
                     if (a.chr < b.chr) return -1;
                else if (a.chr > b.chr) return +1;

                return 0;
            }

            ///
            /// Compares two key sequences by character (for sorting)
            ///
            /// \param[in] a  Pointer to key sequence
            /// \param[in] b  Pointer to second key sequence
            ///
            /// \returns
            /// - <0 when a <  b
            /// - =0 when a == b
            /// - >0 when a >  b
            ///
            virtual int compare_sort(_In_ const keyseq &a, _In_ const keyseq &b) const
            {
                     if (a.chr < b.chr) return -1;
                else if (a.chr > b.chr) return +1;

                int r = keyseq::CompareSequence(a.seq, a.seq_len, b.seq, b.seq_len);
                if (r != 0) return r;

                return 0;
            }
        } idxChr;   ///< Character index


        ///
        /// Key index
        ///
        class indexKey : public index<unsigned __int16, unsigned __int32, keyseq>
        {
        public:
            ///
            /// Constructs the index
            ///
            /// \param[in] h  Reference to vector holding the data
            ///
            indexKey(_In_ std::vector<unsigned __int16> &h) : index<unsigned __int16, unsigned __int32, keyseq>(h) {}

            ///
            /// Compares two key sequences by key (for searching)
            ///
            /// \param[in] a  Pointer to key sequence
            /// \param[in] b  Pointer to second key sequence
            ///
            /// \returns
            /// - <0 when a <  b
            /// - =0 when a == b
            /// - >0 when a >  b
            ///
            virtual int compare(_In_ const keyseq &a, _In_ const keyseq &b) const
            {
                int r = keyseq::CompareSequence(a.seq, a.seq_len, b.seq, b.seq_len);
                if (r != 0) return r;

                return 0;
            }

            ///
            /// Compares two key sequences by key (for sorting)
            ///
            /// \param[in] a  Pointer to key sequence
            /// \param[in] b  Pointer to second key sequence
            ///
            /// \returns
            /// - <0 when a <  b
            /// - =0 when a == b
            /// - >0 when a >  b
            ///
            virtual int compare_sort(_In_ const keyseq &a, _In_ const keyseq &b) const
            {
                int r = keyseq::CompareSequence(a.seq, a.seq_len, b.seq, b.seq_len);
                if (r != 0) return r;

                     if (a.chr < b.chr) return -1;
                else if (a.chr > b.chr) return +1;

                return 0;
            }
        } idxKey;   ///< Key index

        std::vector<unsigned __int16> data;     ///< Key sequences data

    public:
        ///
        /// Constructs the database
        ///
        inline keyseq_db() : idxChr(data), idxKey(data) {}
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
