/*
    SPDX-License-Identifier: GPL-3.0-or-later
    Copyright © 2015-2022 Amebis
*/

#pragma once

#include "common.h"

#include <assert.h>
#include <istream>
#include <ostream>
#include <vector>
#include <string>

#pragma warning(push)
#pragma warning(disable: 4200)
#pragma warning(disable: 4251)
#pragma warning(disable: 4512)


namespace ZRCola {
    typedef uint16_t tagid_t;

    ///
    /// Character Tag Database
    ///
    class chrtag_db {
    public:
#pragma pack(push)
#pragma pack(2)
        ///
        /// Character tag data
        ///
        struct chrtag {
        public:
            tagid_t tag;        ///< Tag ID

        protected:
            uint16_t chr_to;    ///< Character end in \c data
            char_t data[];      ///< Character

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
                _In_opt_z_count_(chr_len) const char_t  *chr      = NULL,
                _In_opt_                        size_t   chr_len  = 0,
                _In_opt_                        tagid_t  tag      = 0)
            {
                this->tag    = tag;
                this->chr_to = static_cast<uint16_t>(chr_len);
                if (chr && chr_len) memcpy(this->data, chr, sizeof(char_t)*chr_len);
            }

            inline const char_t*  chr    () const { return data;          };
            inline       char_t*  chr    ()       { return data;          };
            inline const char_t*  chr_end() const { return data + chr_to; };
            inline       char_t*  chr_end()       { return data + chr_to; };
            inline       uint16_t chr_len() const { return chr_to;        };
        };
#pragma pack(pop)

        ///
        /// Character Index
        ///
        class indexChr : public index<uint16_t, uint32_t, chrtag>
        {
        public:
            ///
            /// Constructs the index
            ///
            /// \param[in] h  Reference to vector holding the data
            ///
            indexChr(_In_ std::vector<uint16_t> &h) : index<uint16_t, uint32_t, chrtag>(h) {}

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
        class indexTag : public index<uint16_t, uint32_t, chrtag>
        {
        public:
            ///
            /// Constructs the index
            ///
            /// \param[in] h  Reference to vector holding the data
            ///
            indexTag(_In_ std::vector<uint16_t> &h) : index<uint16_t, uint32_t, chrtag>(h) {}

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
                if (a.tag > b.tag) return  1;

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

        std::vector<uint16_t> data; ///< Character tags data

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
        bool Search(_In_ const std::map<tagid_t, uint16_t> &tags, _In_ const character_db &ch_db, _In_ const std::set<chrcatid_t> &cats, _Inout_ std::map<string_t, charrank_t> &hits, _In_opt_ bool (__cdecl *fn_abort)(void *cookie) = NULL, _In_opt_ void *cookie = NULL) const;


        ///
        /// Writes character tag database to a stream
        ///
        /// \param[in] stream  Output stream
        /// \param[in] db      Character tag database
        ///
        /// \returns The stream \p stream
        ///
        friend std::ostream& operator <<(_In_ std::ostream& stream, _In_ const chrtag_db& db)
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
            uint32_t count = (uint32_t)data_count;
            stream.write((const char*)&count, sizeof(count));

            // Write data.
            if (stream.fail()) return stream;
            stream.write((const char*)db.data.data(), sizeof(uint16_t) * static_cast<std::streamsize>(count));

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
        friend std::istream& operator >>(_In_ std::istream& stream, _Out_ chrtag_db& db)
        {
            // Read character index.
            stream >> db.idxChr;
            if (!stream.good()) return stream;

            // Read tag index.
            stream >> db.idxTag;
            if (!stream.good()) return stream;

            // Read data count.
            uint32_t count;
            stream.read((char*)&count, sizeof(count));
            if (!stream.good()) return stream;

            if (count) {
                // Read data.
                db.data.resize(count);
                stream.read((char*)db.data.data(), sizeof(uint16_t) * static_cast<std::streamsize>(count));
            }
            else
                db.data.clear();

            return stream;
        }
    };


    ///
    /// Tag name database
    ///
    class tagname_db {
    public:
#pragma pack(push)
#pragma pack(2)
        ///
        /// Tag name data
        ///
        struct tagname {
        public:
            tagid_t tag;        ///< Tag ID
            uint32_t locale;    ///< Locale ID

        protected:
            uint16_t name_to;   ///< Tag name end in \c data
            char_t data[];      ///< Tag name

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
                _In_opt_                         tagid_t   tag      = 0,
                _In_opt_                         uint32_t  locale   = 0,
                _In_opt_z_count_(name_len) const char_t   *name     = NULL,
                _In_opt_                         size_t    name_len = 0)
            {
                this->tag    = tag;
                this->locale = locale;
                this->name_to = static_cast<uint16_t>(name_len);
                if (name && name_len) memcpy(this->data, name, sizeof(char_t)*name_len);
            }

            inline const char_t*  name    () const { return data; };
            inline       char_t*  name    ()       { return data; };
            inline const char_t*  name_end() const { return data + name_to; };
            inline       char_t*  name_end()       { return data + name_to; };
            inline       uint16_t name_len() const { return name_to; };

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
            static inline int CompareName(_In_ uint32_t locale, _In_z_count_(count_a) const char_t *str_a, _In_ uint16_t count_a, _In_z_count_(count_b) const char_t *str_b, _In_ uint16_t count_b)
            {
#ifdef _WIN32
                switch (::CompareString(locale, SORT_STRINGSORT | NORM_IGNORECASE, str_a, count_a, str_b, count_b)) {
                    case CSTR_LESS_THAN   : return -1;
                    case CSTR_EQUAL       : return  0;
                    case CSTR_GREATER_THAN: return  1;
                    default               : assert(0); return -1;
                }
#else
                assert(0); // TODO: 1. Should honour locale. 2. Should use ICU for lowercase conversion. 3. Should be UTF-16-aware.
                string_t
                    a(str_a, count_a),
                    b(str_b, count_b);
                auto tolower = [](char_t c){ return std::towlower(c); };
                std::transform(a.begin(), a.end(), a.begin(), tolower);
                std::transform(b.begin(), b.end(), b.begin(), tolower);
                auto &coll = std::use_facet<std::collate<char_t>>(std::locale());
                return coll.compare(&*a.cbegin(), &*a.cend(), &*b.cbegin(), &*b.cend());
#endif
            }
        };
#pragma pack(pop)

        ///
        /// Name index
        ///
        class indexName : public index<uint16_t, uint32_t, tagname>
        {
        public:
            ///
            /// Constructs the index
            ///
            /// \param[in] h       Reference to vector holding the data
            /// \param[in] locale  Locale used to perform tag name comparison
            ///
            indexName(_In_ std::vector<uint16_t> &h) : index<uint16_t, uint32_t, tagname>(h) {}

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
                if (a.locale > b.locale) return  1;

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
        class indexTag : public index<uint16_t, uint32_t, tagname>
        {
        public:
            ///
            /// Constructs the index
            ///
            /// \param[in] h       Reference to vector holding the data
            /// \param[in] locale  Locale used to perform tag name comparison
            ///
            indexTag(_In_ std::vector<uint16_t> &h) : index<uint16_t, uint32_t, tagname>(h) {}

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
                if (a.locale > b.locale) return  1;

                if (a.tag < b.tag) return -1;
                if (a.tag > b.tag) return  1;

                return 0;
            }
        } idxTag;      ///< Tag index

        std::vector<uint16_t> data; ///< Tag data

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
        bool Search(_In_z_ const char_t *str, _In_ uint32_t locale, _Inout_ std::map<tagid_t, uint16_t> &hits, _In_opt_ bool (__cdecl *fn_abort)(void *cookie) = NULL, _In_opt_ void *cookie = NULL) const;


        ///
        /// Writes tag database to a stream
        ///
        /// \param[in] stream  Output stream
        /// \param[in] db      Tag database
        ///
        /// \returns The stream \p stream
        ///
        friend std::ostream& operator <<(_In_ std::ostream& stream, _In_ const tagname_db& db)
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
            uint32_t count = (uint32_t)data_count;
            stream.write((const char*)&count, sizeof(count));

            // Write data.
            if (stream.fail()) return stream;
            stream.write((const char*)db.data.data(), sizeof(uint16_t) * static_cast<std::streamsize>(count));

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
        friend std::istream& operator >>(_In_ std::istream& stream, _Out_ tagname_db& db)
        {
            // Read name index.
            stream >> db.idxName;
            if (!stream.good()) return stream;

            // Read tag index.
            stream >> db.idxTag;
            if (!stream.good()) return stream;

            // Read data count.
            uint32_t count;
            stream.read((char*)&count, sizeof(count));
            if (!stream.good()) return stream;

            if (count) {
                // Read data.
                db.data.resize(count);
                stream.read((char*)db.data.data(), sizeof(uint16_t) * static_cast<std::streamsize>(count));
            }
            else
                db.data.clear();

            return stream;
        }
    };
};

#pragma warning(pop)
