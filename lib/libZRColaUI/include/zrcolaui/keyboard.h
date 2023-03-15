/*
    SPDX-License-Identifier: GPL-3.0-or-later
    Copyright © 2015-2022 Amebis
*/

#pragma once

#include <zrcola/common.h>

#include <stdex/idrec.hpp>

#include <wxex/common.h>
#pragma warning(push)
#pragma warning(disable: WXWIDGETS_CODE_ANALYSIS_WARNINGS)
#include <wx/string.h>
#pragma warning(pop)

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
    class keyseq_db {
    public:
#pragma pack(push)
#pragma pack(2)
        ///
        /// Key sequence data
        ///
        struct keyseq {
        public:
            enum modifiers_t {
                SHIFT   = 1<<0,     ///< SHIFT key was pressed
                CTRL    = 1<<1,     ///< CTRL key was pressed
                ALT     = 1<<2,     ///< ALT key was pressed
            };

            struct key_t {
                char_t key;         ///< Key
                uint16_t modifiers; ///< Modifiers (bitwise combination of SHIFT, CTRL and ALT)
            };

        protected:
            uint16_t chr_to;        ///< Character end in \c data
            uint16_t seq_to;        ///< Key sequence end in \c data
            char_t data[];          ///< Character and key sequence

        public:
            ///
            /// Constructs the key sequence
            ///
            /// \param[in] seq        Key sequence
            /// \param[in] seq_count  Number of UTF-16 characters in \p seq
            /// \param[in] chr        Character
            /// \param[in] chr_len    Number of UTF-16 characters in \p chr
            ///
            inline keyseq(
                _In_opt_count_(seq_count) const key_t  *seq       = NULL,
                _In_opt_                        size_t  seq_count = 0,
                _In_opt_z_count_(chr_len) const char_t *chr       = NULL,
                _In_opt_                        size_t  chr_len   = 0)
            {
                this->chr_to = static_cast<uint16_t>(chr_len);
                if (chr && chr_len) memcpy(this->data, chr, sizeof(char_t)*chr_len);
                this->seq_to = static_cast<uint16_t>(this->chr_to + seq_count * sizeof(key_t) / sizeof(*data));
                if (seq && seq_count) memcpy(this->data + this->chr_to, seq, sizeof(key_t)*seq_count);
            }

            inline const char_t*  chr    () const { return data;          };
            inline       char_t*  chr    ()       { return data;          };
            inline const char_t*  chr_end() const { return data + chr_to; };
            inline       char_t*  chr_end()       { return data + chr_to; };
            inline       uint16_t chr_len() const { return chr_to;        };

            inline const key_t*   seq    () const { return reinterpret_cast<const key_t*>(data + chr_to);     };
            inline       key_t*   seq    ()       { return reinterpret_cast<      key_t*>(data + chr_to);     };
            inline const key_t*   seq_end() const { return reinterpret_cast<const key_t*>(data + seq_to);     };
            inline       key_t*   seq_end()       { return reinterpret_cast<      key_t*>(data + seq_to);     };
            inline       uint16_t seq_len() const { return (seq_to - chr_to) * sizeof(*data) / sizeof(key_t); };

            ///
            /// Compares two key sequences
            ///
            /// \param[in] seq_a  First key sequence
            /// \param[in] len_a  Number of keys in sequence \p seq_a
            /// \param[in] seq_b  Second key sequence
            /// \param[in] len_b  Number of keys in sequence \p seq_b
            ///
            /// \returns
            /// - <0 when seq_a <  seq_b
            /// - =0 when seq_a == seq_b
            /// - >0 when seq_a >  seq_b
            ///
            static inline int CompareSequence(_In_ const key_t *seq_a, _In_ size_t len_a, _In_ const key_t *seq_b, _In_ size_t len_b)
            {
                for (size_t i = 0; ; i++) {
                         if (i >= len_a && i >= len_b) return  0;
                    else if (i >= len_a && i <  len_b) return -1;
                    else if (i <  len_a && i >= len_b) return +1;
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
        class indexChr : public index<uint16_t, uint32_t, keyseq>
        {
        public:
            ///
            /// Constructs the index
            ///
            /// \param[in] h  Reference to vector holding the data
            ///
            indexChr(_In_ std::vector<uint16_t> &h) : index<uint16_t, uint32_t, keyseq>(h) {}

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
                int r = ZRCola::CompareString(a.chr(), a.chr_len(), b.chr(), b.chr_len());
                if (r != 0) return r;

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
                int r = ZRCola::CompareString(a.chr(), a.chr_len(), b.chr(), b.chr_len());
                if (r != 0) return r;

                r = keyseq::CompareSequence(a.seq(), a.seq_len(), b.seq(), b.seq_len());
                if (r != 0) return r;

                return 0;
            }
        } idxChr;   ///< Character index


        ///
        /// Key index
        ///
        class indexKey : public index<uint16_t, uint32_t, keyseq>
        {
        public:
            ///
            /// Constructs the index
            ///
            /// \param[in] h  Reference to vector holding the data
            ///
            indexKey(_In_ std::vector<uint16_t> &h) : index<uint16_t, uint32_t, keyseq>(h) {}

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
                int r = keyseq::CompareSequence(a.seq(), a.seq_len(), b.seq(), b.seq_len());
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
                int r = keyseq::CompareSequence(a.seq(), a.seq_len(), b.seq(), b.seq_len());
                if (r != 0) return r;

                r = ZRCola::CompareString(a.chr(), a.chr_len(), b.chr(), b.chr_len());
                if (r != 0) return r;

                return 0;
            }
        } idxKey;   ///< Key index

        std::vector<uint16_t> data; ///< Key sequences data

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


    typedef stdex::idrec::record<keyseq_db, recordid_t, 0x59454b /*"KEY"*/, recordsize_t, ZRCOLA_RECORD_ALIGN> keyseq_rec;
};


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
    uint32_t count = (uint32_t)data_count;
    stream.write((const char*)&count, sizeof(count));

    // Write data.
    if (stream.fail()) return stream;
    stream.write((const char*)db.data.data(), sizeof(uint16_t)*static_cast<std::streamsize>(count));

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
    uint32_t count;
    stream.read((char*)&count, sizeof(count));
    if (!stream.good()) return stream;

    if (count) {
        // Read data.
        db.data.resize(count);
        stream.read((char*)db.data.data(), sizeof(uint16_t)*static_cast<std::streamsize>(count));
    } else
        db.data.clear();

    return stream;
}

#pragma warning(pop)
