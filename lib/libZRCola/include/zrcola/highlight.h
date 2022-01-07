/*
    SPDX-License-Identifier: GPL-3.0-or-later
    Copyright © 2021-2022 Amebis
*/

#pragma once

#include "common.h"

#include <stdex/idrec.h>

#include <functional>

#pragma warning(push)
#pragma warning(disable: 4200)

///
/// Default highlight
///
#define ZRCOLA_HLGHTSETID_DEFAULT                           ((ZRCola::hlghtsetid_t)0x0000)


///
/// ZRCola Unicode Composed Issues
///
#define ZRCOLA_HLGHTSETID_ZRCOLA_UNICODE_COMPOSED_ISSUES    ((ZRCola::hlghtsetid_t)0x0001)

namespace ZRCola {
    ///
    /// Highlight set ID
    ///
    typedef unsigned __int16 hlghtsetid_t;

    ///
    /// Highlight database
    ///
    class highlight_db {
    public:
#pragma pack(push)
#pragma pack(2)
        ///
        /// Highlight data
        ///
        struct highlight {
        public:
            hlghtsetid_t set;           ///< Highlight set ID

        protected:
            unsigned __int16 chr_to;    ///< Character end in \c data
            wchar_t data[];             ///< Character

        private:
            inline highlight(_In_ const highlight &other);
            inline highlight& operator=(_In_ const highlight &other);

        public:
            ///
            /// Constructs the highlight
            ///
            /// \param[in] set      Highlight set ID
            /// \param[in] chr      Character
            /// \param[in] chr_len  Number of UTF-16 characters in \p chr
            ///
            inline highlight(
                _In_opt_                        hlghtsetid_t  set     = 0,
                _In_opt_z_count_(chr_len) const wchar_t      *chr     = NULL,
                _In_opt_                        size_t        chr_len = 0)
            {
                this->set    = set;
                this->chr_to = static_cast<unsigned __int16>(chr_len);
                if (chr && chr_len) memcpy(this->data, chr, sizeof(wchar_t)*chr_len);
            }

            inline const wchar_t*         chr    () const { return data;          };
            inline       wchar_t*         chr    ()       { return data;          };
            inline const wchar_t*         chr_end() const { return data + chr_to; };
            inline       wchar_t*         chr_end()       { return data + chr_to; };
            inline       unsigned __int16 chr_len() const { return chr_to;        };

            inline wchar_t chr_at(_In_ size_t i) const
            {
                return i < chr_to ? data[i] : 0;
            }
        };
#pragma pack(pop)

        ///
        /// Highlight index
        ///
        class indexChr : public index<unsigned __int16, unsigned __int32, highlight>
        {
        public:
            ///
            /// Constructs the index
            ///
            /// \param[in] h  Reference to vector holding the data
            ///
            indexChr(_In_ std::vector<unsigned __int16> &h) : index<unsigned __int16, unsigned __int32, highlight>(h) {}

            ///
            /// Compares two highlights by string (for searching)
            ///
            /// \param[in] a  Pointer to first element
            /// \param[in] b  Pointer to second element
            ///
            /// \returns
            /// - <0 when a <  b
            /// - =0 when a == b
            /// - >0 when a >  b
            ///
            virtual int compare(_In_ const highlight &a, _In_ const highlight &b) const
            {
                int r = ZRCola::CompareString(a.chr(), a.chr_len(), b.chr(), b.chr_len());
                if (r != 0) return r;

                     if (a.set < b.set) return -1;
                else if (a.set > b.set) return +1;

                return 0;
            }

            ///
            /// Compares two highlights by string (for sorting)
            ///
            /// \param[in] a  Pointer to first element
            /// \param[in] b  Pointer to second element
            ///
            /// \returns
            /// - <0 when a <  b
            /// - =0 when a == b
            /// - >0 when a >  b
            ///
            virtual int compare_sort(_In_ const highlight &a, _In_ const highlight &b) const
            {
                // Revert to `compare()` by default.
                return compare(a, b);
            }
        } idxChr;   ///< Highlight index


        std::vector<unsigned __int16> data;         ///< Highlight data

    public:
        ///
        /// Constructs the database
        ///
        inline highlight_db() : idxChr(data) {}

        ///
        /// Clears the database
        ///
        inline void clear()
        {
            idxChr.clear();
            data  .clear();
        }

        ///
        /// Highlights string
        ///
        /// \param[in] input     Input string (UTF-16)
        /// \param[in] inputMax  Length of the input string in characters. Can be (size_t)-1 if \p input is zero terminated.
        /// \param[in] callback  Function to be called on highlight switch
        ///
        void Highlight(_In_z_count_(inputMax) const wchar_t* input, _In_ size_t inputMax, _In_ std::function<void (hlghtsetid_t set, size_t start, size_t end)> callback) const;
    };


    typedef stdex::idrec::record<highlight_db, recordid_t, recordsize_t, ZRCOLA_RECORD_ALIGN> highlight_rec;
};


const ZRCola::recordid_t ZRCola::highlight_rec::id = *(ZRCola::recordid_t*)"HGH";


///
/// Writes highlight database to a stream
///
/// \param[in] stream  Output stream
/// \param[in] db      Highlight database
///
/// \returns The stream \p stream
///
inline std::ostream& operator <<(_In_ std::ostream& stream, _In_ const ZRCola::highlight_db &db)
{
    // Write highlight index.
    if (stream.fail()) return stream;
    stream << db.idxChr;

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
    stream.write((const char*)db.data.data(), sizeof(unsigned __int16)*static_cast<std::streamsize>(count));

    return stream;
}


///
/// Reads highlight database from a stream
///
/// \param[in ] stream  Input stream
/// \param[out] db      Highlight database
///
/// \returns The stream \p stream
///
inline std::istream& operator >>(_In_ std::istream& stream, _Out_ ZRCola::highlight_db &db)
{
    // Read highlight index.
    stream >> db.idxChr;
    if (!stream.good()) return stream;

    // Read data count.
    unsigned __int32 count;
    stream.read((char*)&count, sizeof(count));
    if (!stream.good()) return stream;

    if (count) {
        // Read data.
        db.data.resize(count);
        stream.read((char*)db.data.data(), sizeof(unsigned __int16)*static_cast<std::streamsize>(count));
    } else
        db.data.clear();

    return stream;
}

#pragma warning(pop)
