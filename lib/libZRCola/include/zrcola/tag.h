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
#include <ostream>
#include <vector>
#include <string>

#pragma warning(push)
#pragma warning(disable: 4200)
#pragma warning(disable: 4251)
#pragma warning(disable: 4512)


namespace ZRCola {
    typedef unsigned __int16 tagid_t;

    ///
    /// Character Tag Database
    ///
    class ZRCOLA_API chrtag_db {
    public:
#pragma pack(push)
#pragma pack(2)
        ///
        /// Character tag data
        ///
        struct chrtag {
            wchar_t chr;    ///> Character
            tagid_t tag;    ///< Tag ID
        };
#pragma pack(pop)

        ///
        /// Character Index
        ///
        class indexChar : public index<unsigned __int16, unsigned __int32, chrtag>
        {
        public:
            ///
            /// Constructs the index
            ///
            /// \param[in] h  Reference to vector holding the data
            ///
            indexChar(_In_ std::vector<unsigned __int16> &h) : index<unsigned __int16, unsigned __int32, chrtag>(h) {}

            ///
            /// Compares two character tags by character (for searching)
            ///
            /// \param[in] a  Pointer to first element
            /// \param[in] b  Pointer to second element
            ///
            /// \returns
            /// - <0 when a <  b
            /// - =0 when a == b
            /// - >0 when a >  b
            ///
            virtual int compare(_In_ const chrtag &a, _In_ const chrtag &b) const
            {
                     if (a.chr < b.chr) return -1;
                else if (a.chr > b.chr) return  1;

                return 0;
            }

            ///
            /// Compares two character tags by character (for sorting)
            ///
            /// \param[in] a  Pointer to first element
            /// \param[in] b  Pointer to second element
            ///
            /// \returns
            /// - <0 when a <  b
            /// - =0 when a == b
            /// - >0 when a >  b
            ///
            virtual int compare_sort(_In_ const chrtag &a, _In_ const chrtag &b) const
            {
                     if (a.chr < b.chr) return -1;
                else if (a.chr > b.chr) return  1;

                     if (a.tag < b.tag) return -1;
                else if (a.tag > b.tag) return  1;

                return 0;
            }
        } idxChr;      ///< Character index


        ///
        /// Tag Index
        ///
        class indexTag : public index<unsigned __int16, unsigned __int32, chrtag>
        {
        public:
            ///
            /// Constructs the index
            ///
            /// \param[in] h  Reference to vector holding the data
            ///
            indexTag(_In_ std::vector<unsigned __int16> &h) : index<unsigned __int16, unsigned __int32, chrtag>(h) {}

            ///
            /// Compares two character tags by tag (for searching)
            ///
            /// \param[in] a  Pointer to first element
            /// \param[in] b  Pointer to second element
            ///
            /// \returns
            /// - <0 when a <  b
            /// - =0 when a == b
            /// - >0 when a >  b
            ///
            virtual int compare(_In_ const chrtag &a, _In_ const chrtag &b) const
            {
                     if (a.tag < b.tag) return -1;
                else if (a.tag > b.tag) return  1;

                return 0;
            }

            ///
            /// Compares two character tags by tag (for sorting)
            ///
            /// \param[in] a  Pointer to first element
            /// \param[in] b  Pointer to second element
            ///
            /// \returns
            /// - <0 when a <  b
            /// - =0 when a == b
            /// - >0 when a >  b
            ///
            virtual int compare_sort(_In_ const chrtag &a, _In_ const chrtag &b) const
            {
                     if (a.tag < b.tag) return -1;
                else if (a.tag > b.tag) return  1;

                     if (a.chr < b.chr) return -1;
                else if (a.chr > b.chr) return  1;

                return 0;
            }
        } idxTag;      ///< Tag index

        std::vector<unsigned __int16> data;     ///< Character tags data

    public:
        ///
        /// Constructs the database
        ///
        inline chrtag_db() : idxChr(data), idxTag(data) {}
    };


    typedef ZRCOLA_API stdex::idrec::record<chrtag_db, recordid_t, recordsize_t, ZRCOLA_RECORD_ALIGN> chrtag_rec;


    ///
    /// Tag name database
    ///
    class ZRCOLA_API tagname_db {
    public:
#pragma pack(push)
#pragma pack(2)
        ///
        /// Tag name data
        ///
        struct tagname {
            tagid_t tag;                ///< Tag ID
            LCID lang;                  ///< Language ID
            unsigned __int16 name_len;  ///< \c name length (in characters)
            wchar_t name[];             ///< Tag localized name

            ///
            /// Compares two names
            ///
            /// \param[in] lcid     Locale ID to use for compare
            /// \param[in] str_a    First name
            /// \param[in] count_a  Number of characters in string \p str_a
            /// \param[in] str_b    Second name
            /// \param[in] count_b  Number of characters in string \p str_b
            ///
            /// \returns
            /// - <0 when str_a <  str_b
            /// - =0 when str_a == str_b
            /// - >0 when str_a >  str_b
            ///
            /// \note
            /// The function does not treat \\0 characters as terminators for performance reasons.
            /// Therefore \p count_a and \p count_b must represent exact string lengths.
            ///
            static inline int CompareName(LCID lcid, const wchar_t *str_a, unsigned __int16 count_a, const wchar_t *str_b, unsigned __int16 count_b)
            {
                switch (CompareString(lcid, LINGUISTIC_IGNORECASE | LINGUISTIC_IGNOREDIACRITIC | NORM_LINGUISTIC_CASING | NORM_IGNOREWIDTH, str_a, count_a, str_b, count_b)) {
                    case CSTR_LESS_THAN   : return -1;
                    case CSTR_EQUAL       : return  0;
                    case CSTR_GREATER_THAN: return  1;
                    default               : assert(0); return -1;
                }
            }
        };
#pragma pack(pop)

        ///
        /// Name index
        ///
        class indexName : public index<unsigned __int16, unsigned __int32, tagname>
        {
        public:
            ///
            /// Constructs the index
            ///
            /// \param[in] h     Reference to vector holding the data
            /// \param[in] lcid  Locale used to perform tag name comparison
            ///
            indexName(_In_ std::vector<unsigned __int16> &h) : index<unsigned __int16, unsigned __int32, tagname>(h) {}

            ///
            /// Compares two tag names by name (for searching)
            ///
            /// \param[in] a  Pointer to first element
            /// \param[in] b  Pointer to second element
            ///
            /// \returns
            /// - <0 when a <  b
            /// - =0 when a == b
            /// - >0 when a >  b
            ///
            virtual int compare(_In_ const tagname &a, _In_ const tagname &b) const
            {
                     if (a.lang < b.lang) return -1;
                else if (a.lang > b.lang) return  1;

                int r = tagname::CompareName(a.lang, a.name, a.name_len, b.name, b.name_len);
                if (r != 0) return r;

                return 0;
            }

            ///
            /// Compares two tag names by name (for sorting)
            ///
            /// \param[in] a  Pointer to first element
            /// \param[in] b  Pointer to second element
            ///
            /// \returns
            /// - <0 when a <  b
            /// - =0 when a == b
            /// - >0 when a >  b
            ///
            virtual int compare_sort(_In_ const tagname &a, _In_ const tagname &b) const
            {
                     if (a.lang < b.lang) return -1;
                else if (a.lang > b.lang) return  1;

                int r = tagname::CompareName(a.lang, a.name, a.name_len, b.name, b.name_len);
                if (r != 0) return r;

                     if (a.tag < b.tag) return -1;
                else if (a.tag > b.tag) return  1;

                return 0;
            }
        } idxName;      ///< Name index

        std::vector<unsigned __int16> data;     ///< Tag data

    public:
        ///
        /// Constructs the database
        ///
        /// \param[in] lcid  Locale used to perform tag name comparison
        ///
        inline tagname_db() : idxName(data) {}
    };


    typedef ZRCOLA_API stdex::idrec::record<tagname_db, recordid_t, recordsize_t, ZRCOLA_RECORD_ALIGN> tagname_rec;
};


const ZRCola::recordid_t stdex::idrec::record<ZRCola::chrtag_db, ZRCola::recordid_t, ZRCola::recordsize_t, ZRCOLA_RECORD_ALIGN>::id = *(ZRCola::recordid_t*)"C-T";
const ZRCola::recordid_t stdex::idrec::record<ZRCola::tagname_db, ZRCola::recordid_t, ZRCola::recordsize_t, ZRCOLA_RECORD_ALIGN>::id = *(ZRCola::recordid_t*)"TGN";


///
/// Writes character tag database to a stream
///
/// \param[in] stream  Output stream
/// \param[in] db      Character tag database
///
/// \returns The stream \p stream
///
inline std::ostream& operator <<(_In_ std::ostream& stream, _In_ const ZRCola::chrtag_db &db)
{
    // Write character index.
    if (stream.fail()) return stream;
    stream << db.idxChr;

    // Write tag index.
    if (stream.fail()) return stream;
    stream << db.idxTag;

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
/// Reads character tag database from a stream
///
/// \param[in ] stream  Input stream
/// \param[out] db      Character tag database
///
/// \returns The stream \p stream
///
inline std::istream& operator >>(_In_ std::istream& stream, _Out_ ZRCola::chrtag_db &db)
{
    // Read character index.
    stream >> db.idxChr;
    if (!stream.good()) return stream;

    // Read tag index.
    stream >> db.idxTag;
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


///
/// Writes tag database to a stream
///
/// \param[in] stream  Output stream
/// \param[in] db      Tag database
///
/// \returns The stream \p stream
///
inline std::ostream& operator <<(_In_ std::ostream& stream, _In_ const ZRCola::tagname_db &db)
{
    // Write tag index.
    if (stream.fail()) return stream;
    stream << db.idxName;

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
/// Reads tag database from a stream
///
/// \param[in ] stream  Input stream
/// \param[out] db      Tag database
///
/// \returns The stream \p stream
///
inline std::istream& operator >>(_In_ std::istream& stream, _Out_ ZRCola::tagname_db &db)
{
    // Read tag index.
    stream >> db.idxName;
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
