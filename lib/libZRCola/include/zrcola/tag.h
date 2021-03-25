/*
    Copyright © 2015-2021 Amebis

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
        public:
            tagid_t tag;    ///< Tag ID

        protected:
            unsigned __int16 chr_to;    ///< Character end in \c data
            wchar_t data[];             ///< Character

        private:
            inline chrtag(_In_ const chrtag &other);
            inline chrtag& operator=(_In_ const chrtag &other);

        public:
            ///
            /// Constructs the character tag
            ///
            /// \param[in] chr      Character
            /// \param[in] chr_len  Number of UTF-16 characters in \p chr
            /// \param[in] tag      Tag
            ///
            inline chrtag(
                _In_opt_z_count_(chr_len) const wchar_t *chr      = NULL,
                _In_opt_                        size_t   chr_len  = 0,
                _In_opt_                        tagid_t  tag      = 0)
            {
                this->tag    = tag;
                this->chr_to = static_cast<unsigned __int16>(chr_len);
                if (chr && chr_len) memcpy(this->data, chr, sizeof(wchar_t)*chr_len);
            }

            inline const wchar_t*         chr    () const { return data;          };
            inline       wchar_t*         chr    ()       { return data;          };
            inline const wchar_t*         chr_end() const { return data + chr_to; };
            inline       wchar_t*         chr_end()       { return data + chr_to; };
            inline       unsigned __int16 chr_len() const { return chr_to;        };
        };
#pragma pack(pop)

        ///
        /// Character Index
        ///
        class indexChr : public index<unsigned __int16, unsigned __int32, chrtag>
        {
        public:
            ///
            /// Constructs the index
            ///
            /// \param[in] h  Reference to vector holding the data
            ///
            indexChr(_In_ std::vector<unsigned __int16> &h) : index<unsigned __int16, unsigned __int32, chrtag>(h) {}

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
                int r = ZRCola::CompareString(a.chr(), a.chr_len(), b.chr(), b.chr_len());
                if (r != 0) return r;

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
                int r = ZRCola::CompareString(a.chr(), a.chr_len(), b.chr(), b.chr_len());
                if (r != 0) return r;

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

                int r = ZRCola::CompareString(a.chr(), a.chr_len(), b.chr(), b.chr_len());
                if (r != 0) return r;

                return 0;
            }
        } idxTag;      ///< Tag index

        std::vector<unsigned __int16> data;     ///< Character tags data

    public:
        ///
        /// Constructs the database
        ///
        inline chrtag_db() : idxChr(data), idxTag(data) {}

        ///
        /// Clears the database
        ///
        inline void clear()
        {
            idxChr.clear();
            idxTag.clear();
            data  .clear();
        }

        ///
        /// Search for characters by tags
        ///
        /// \param[in   ] tags      Search tags
        /// \param[in   ] ch_db     Character database
        /// \param[in   ] cats      Set of categories from \p ch_db, character must be a part of
        /// \param[inout] hits      (character, count) map to append hits to
        /// \param[in   ] fn_abort  Pointer to function to periodically test for search cancellation
        /// \param[in   ] cookie    Cookie for \p fn_abort call
        ///
        bool Search(_In_ const std::map<tagid_t, unsigned __int16> &tags, _In_ const character_db &ch_db, _In_ const std::set<chrcatid_t> &cats, _Inout_ std::map<std::wstring, charrank_t> &hits, _In_opt_ bool (__cdecl *fn_abort)(void *cookie) = NULL, _In_opt_ void *cookie = NULL) const;
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
        public:
            tagid_t tag;                ///< Tag ID
            LCID locale;                ///< Locale ID

        protected:
            unsigned __int16 name_to;   ///< Tag name end in \c data
            wchar_t data[];             ///< Tag name

        private:
            inline tagname(_In_ const tagname &other);
            inline tagname& operator=(_In_ const tagname &other);

        public:
            ///
            /// Constructs the localized tag name
            ///
            /// \param[in] tag       Tag
            /// \param[in] locale    Locale
            /// \param[in] name      Tag name
            /// \param[in] name_len  Number of UTF-16 characters in \p name
            ///
            inline tagname(
                _In_opt_                         tagid_t  tag      = 0,
                _In_opt_                         LCID     locale   = MAKELCID(MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL), SORT_DEFAULT),
                _In_opt_z_count_(name_len) const wchar_t *name     = NULL,
                _In_opt_                         size_t   name_len = 0)
            {
                this->tag    = tag;
                this->locale = locale;
                this->name_to = static_cast<unsigned __int16>(name_len);
                if (name && name_len) memcpy(this->data, name, sizeof(wchar_t)*name_len);
            }

            inline const wchar_t*         name    () const { return data; };
            inline       wchar_t*         name    ()       { return data; };
            inline const wchar_t*         name_end() const { return data + name_to; };
            inline       wchar_t*         name_end()       { return data + name_to; };
            inline       unsigned __int16 name_len() const { return name_to; };

            ///
            /// Compares two names
            ///
            /// \param[in] locale   Locale ID to use for compare
            /// \param[in] str_a    First name
            /// \param[in] count_a  Number of UTF-16 characters in \p str_a
            /// \param[in] str_b    Second name
            /// \param[in] count_b  Number of UTF-16 characters in \p str_b
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
            static inline int CompareName(LCID locale, const wchar_t *str_a, unsigned __int16 count_a, const wchar_t *str_b, unsigned __int16 count_b)
            {
                switch (::CompareString(locale, SORT_STRINGSORT | NORM_IGNORECASE, str_a, count_a, str_b, count_b)) {
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
            /// \param[in] h       Reference to vector holding the data
            /// \param[in] locale  Locale used to perform tag name comparison
            ///
            indexName(_In_ std::vector<unsigned __int16> &h) : index<unsigned __int16, unsigned __int32, tagname>(h) {}

            ///
            /// Compares two tag names by locale and name (for searching)
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
                     if (a.locale < b.locale) return -1;
                else if (a.locale > b.locale) return  1;

                int r = tagname::CompareName(a.locale, a.name(), a.name_len(), b.name(), b.name_len());
                if (r != 0) return r;

                return 0;
            }

            ///
            /// Compares two tag names by locale and name (for sorting)
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
                     if (a.locale < b.locale) return -1;
                else if (a.locale > b.locale) return  1;

                int r = tagname::CompareName(a.locale, a.name(), a.name_len(), b.name(), b.name_len());
                if (r != 0) return r;

                     if (a.tag < b.tag) return -1;
                else if (a.tag > b.tag) return  1;

                return 0;
            }
        } idxName;      ///< Name index

        ///
        /// Tag index
        ///
        class indexTag : public index<unsigned __int16, unsigned __int32, tagname>
        {
        public:
            ///
            /// Constructs the index
            ///
            /// \param[in] h       Reference to vector holding the data
            /// \param[in] locale  Locale used to perform tag name comparison
            ///
            indexTag(_In_ std::vector<unsigned __int16> &h) : index<unsigned __int16, unsigned __int32, tagname>(h) {}

            ///
            /// Compares two tag names by tag (for searching)
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
                     if (a.locale < b.locale) return -1;
                else if (a.locale > b.locale) return  1;

                     if (a.tag < b.tag) return -1;
                else if (a.tag > b.tag) return  1;

                return 0;
            }
        } idxTag;      ///< Tag index

        std::vector<unsigned __int16> data;     ///< Tag data

    public:
        ///
        /// Constructs the database
        ///
        inline tagname_db() : idxName(data), idxTag(data) {}

        ///
        /// Clears the database
        ///
        inline void clear()
        {
            idxName.clear();
            idxTag .clear();
            data   .clear();
        }

        ///
        /// Search for tags by names
        ///
        /// \param[in   ] str       Search string
        /// \param[in   ] locale    Locale to search tags in
        /// \param[inout] hits      (tag, count) map to append hits to
        /// \param[in   ] fn_abort  Pointer to function to periodically test for search cancellation
        /// \param[in   ] cookie    Cookie for \p fn_abort call
        ///
        bool Search(_In_z_ const wchar_t *str, _In_ LCID locale, _Inout_ std::map<tagid_t, unsigned __int16> &hits, _In_opt_ bool (__cdecl *fn_abort)(void *cookie) = NULL, _In_opt_ void *cookie = NULL) const;
    };


    typedef ZRCOLA_API stdex::idrec::record<tagname_db, recordid_t, recordsize_t, ZRCOLA_RECORD_ALIGN> tagname_rec;
};


const ZRCola::recordid_t ZRCola::chrtag_rec ::id = *(ZRCola::recordid_t*)"C-T";
const ZRCola::recordid_t ZRCola::tagname_rec::id = *(ZRCola::recordid_t*)"TGN";


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
    stream.write((const char*)db.data.data(), sizeof(unsigned __int16)*static_cast<std::streamsize>(count));

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
        stream.read((char*)db.data.data(), sizeof(unsigned __int16)*static_cast<std::streamsize>(count));
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
    // Write name index.
    if (stream.fail()) return stream;
    stream << db.idxName;

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
    stream.write((const char*)db.data.data(), sizeof(unsigned __int16)*static_cast<std::streamsize>(count));

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
    // Read name index.
    stream >> db.idxName;
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
        stream.read((char*)db.data.data(), sizeof(unsigned __int16)*static_cast<std::streamsize>(count));
    } else
        db.data.clear();

    return stream;
}

#pragma warning(pop)
