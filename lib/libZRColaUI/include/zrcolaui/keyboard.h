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

#include <wx/string.h>

#include <istream>
#include <ostream>
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

        ///
        /// Clears the database
        ///
        inline void clear()
        {
            idxChr.clear();
            idxKey.clear();
            data  .clear();
        }

        ///
        /// Get text representation of a given key sequence
        ///
        /// \param[in]  seq      Key sequence
        /// \param[in]  seq_len  Number of elements in \p seq
        /// \param[out] str      Text representation of a \p seq key sequence
        ///
        /// \returns
        /// - \c true if conversion succeeded
        /// - \c false otherwise
        ///
        static bool GetSequenceAsText(_In_count_(seq_len) const keyseq::key_t *seq, _In_ size_t seq_len, _Out_ wxString& str);

        ///
        /// Get text representation of a given key sequence
        ///
        /// \param[in]  seq      Key sequence
        /// \param[in]  seq_len  Number of elements in \p seq
        ///
        /// \returns Text representation of a \p seq key sequence
        ///
        static inline wxString GetSequenceAsText(_In_count_(seq_len) const keyseq_db::keyseq::key_t *seq, _In_ size_t seq_len)
        {
            wxString str;
            return GetSequenceAsText(seq, seq_len, str) ? str : wxEmptyString;
        }
    };


    typedef ZRCOLAUI_API stdex::idrec::record<keyseq_db, recordid_t, recordsize_t, ZRCOLA_RECORD_ALIGN> keyseq_rec;
};


const ZRCola::recordid_t stdex::idrec::record<ZRCola::keyseq_db, ZRCola::recordid_t, ZRCola::recordsize_t, ZRCOLA_RECORD_ALIGN>::id = *(ZRCola::recordid_t*)"KEY";


///
/// Writes key sequence database to a stream
///
/// \param[in] stream  Output stream
/// \param[in] db      Key sequence database
///
/// \returns The stream \p stream
///
inline std::ostream& operator <<(_In_ std::ostream& stream, _In_ const ZRCola::keyseq_db &db)
{
    // Write character index.
    if (stream.fail()) return stream;
    stream << db.idxChr;

    // Write key index.
    if (stream.fail()) return stream;
    stream << db.idxKey;

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
/// Reads key sequence database from a stream
///
/// \param[in]  stream  Input stream
/// \param[out] db      Key sequence database
///
/// \returns The stream \p stream
///
inline std::istream& operator >>(_In_ std::istream& stream, _Out_ ZRCola::keyseq_db &db)
{
    // Read character index.
    stream >> db.idxChr;
    if (!stream.good()) return stream;

    // Read key index.
    stream >> db.idxKey;
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
