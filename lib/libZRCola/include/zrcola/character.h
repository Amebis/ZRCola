/*
    SPDX-License-Identifier: GPL-3.0-or-later
    Copyright © 2015-2022 Amebis
*/

#pragma once

#include "common.h"

#include <stdex/idrec.h>
#include <assert.h>
#include <algorithm>
#include <istream>
#include <map>
#include <ostream>
#include <vector>
#include <set>
#include <string>

#pragma warning(push)
#pragma warning(disable: 4200)
#pragma warning(disable: 4251)
#pragma warning(disable: 4512)


namespace ZRCola {
    ///
    /// Character rank type
    ///
    typedef double charrank_t;

    inline bool ispua(_In_ wchar_t c)
    {
        return L'\ue000' <= c && c <= L'\uf8ff';
    }

#pragma pack(push)
#pragma pack(2)
    ///
    /// Character category ID type
    /// Two letter abbreviation, non-terminated
    ///
    struct chrcatid_t {
        char data[2];

        inline chrcatid_t& operator=(const chrcatid_t &src)
        {
            data[0] = src.data[0];
            data[1] = src.data[1];
            return *this;
        }

        inline chrcatid_t& operator=(const char *src)
        {
            data[1] = (data[0] = src[0]) != 0 ? src[1] : 0;
            return *this;
        }

        ///
        /// Blank character category
        ///
        static const chrcatid_t blank;
    };
#pragma pack(pop)


    ///
    /// Compares two character category IDs
    ///
    /// \param[in] a  First character category ID
    /// \param[in] b  Second character category ID
    ///
    /// \returns
    /// - true when \p a == \p b
    /// - false otherwise
    ///
    inline bool operator==(const chrcatid_t &a, const chrcatid_t & b)
    {
        return
                               a.data[0] == b.data[0]  &&
            (a.data[0] == 0 || a.data[1] == b.data[1]);
    }


    ///
    /// Compares two character category IDs
    ///
    /// \param[in] a  First character category ID
    /// \param[in] b  Second character category ID
    ///
    /// \returns
    /// - true when \p a != \p b
    /// - false otherwise
    ///
    inline bool operator!=(const chrcatid_t &a, const chrcatid_t & b)
    {
        return !operator==(a, b);
    }


    ///
    /// Compares two character category IDs
    ///
    /// \param[in] a  First character category ID
    /// \param[in] b  Second character category ID
    ///
    /// \returns
    /// - true when \p a < \p b
    /// - false otherwise
    ///
    inline bool operator<(const chrcatid_t& a, const chrcatid_t& b)
    {
             if (a.data[0] < b.data[0]) return true;
        else if (a.data[0] > b.data[0]) return false;
        else if (a.data[1] < b.data[1]) return true;
        else                            return false;
    }


    ///
    /// Compares two character category IDs
    ///
    /// \param[in] a  First character category ID
    /// \param[in] b  Second character category ID
    ///
    /// \returns
    /// - true when \p a > \p b
    /// - false otherwise
    ///
    inline bool operator>(const chrcatid_t& a, const chrcatid_t& b)
    {
        return operator<(b, a);
    }


    ///
    /// Compares two character category IDs
    ///
    /// \param[in] a  First character category ID
    /// \param[in] b  Second character category ID
    ///
    /// \returns
    /// - true when \p a <= \p b
    /// - false otherwise
    ///
    inline bool operator<=(const chrcatid_t &a, const chrcatid_t & b)
    {
        return !operator>(a, b);
    }


    ///
    /// Compares two character category IDs
    ///
    /// \param[in] a  First character category ID
    /// \param[in] b  Second character category ID
    ///
    /// \returns
    /// - true when \p a >= \p b
    /// - false otherwise
    ///
    inline bool operator>=(const chrcatid_t &a, const chrcatid_t & b)
    {
        return !operator<(a, b);
    }


    ///
    /// Character Database
    ///
    class character_db {
    public:
#pragma pack(push)
#pragma pack(2)
        ///
        /// Character data
        ///
        struct character {
        public:
            chrcatid_t cat;     ///> Character category ID

        protected:
            uint16_t chr_to;    ///< Character end in \c data
            uint16_t desc_to;   ///< Character description end in \c data
            uint16_t rel_to;    ///< Related characters end in \c data
            wchar_t data[];     ///< Character, character description

        private:
            inline character(_In_ const character &other);
            inline character& operator=(_In_ const character &other);

        public:
            ///
            /// Constructs the character
            ///
            /// \param[in] chr       Character
            /// \param[in] chr_len   Number of UTF-16 characters in \p chr
            /// \param[in] cat       Category
            /// \param[in] desc      Description
            /// \param[in] desc_len  Number of UTF-16 characters in \p desc
            /// \param[in] rel       Related characters list (zero delimited)
            /// \param[in] rel_len   Number of UTF-16 characters in \p rel (including zero delimiters)
            ///
            inline character(
                _In_opt_z_count_(chr_len)  const wchar_t    *chr      = NULL,
                _In_opt_                         size_t      chr_len  = 0,
                _In_opt_                         chrcatid_t  cat      = chrcatid_t::blank,
                _In_opt_z_count_(desc_len) const wchar_t    *desc     = NULL,
                _In_opt_                         size_t      desc_len = 0,
                _In_opt_z_count_(rel_len)  const wchar_t    *rel      = NULL,
                _In_opt_                         size_t      rel_len  = 0)
            {
                this->cat = cat;
                this->chr_to = static_cast<uint16_t>(chr_len);
                if (chr && chr_len) memcpy(this->data, chr, sizeof(wchar_t)*chr_len);
                this->desc_to = static_cast<uint16_t>(this->chr_to + desc_len);
                if (desc && desc_len) memcpy(this->data + this->chr_to, desc, sizeof(wchar_t)*desc_len);
                this->rel_to = static_cast<uint16_t>(this->desc_to + rel_len);
                if (rel && rel_len) memcpy(this->data + this->desc_to, rel, sizeof(wchar_t)*rel_len);
            }

            inline const wchar_t* chr    () const { return data;          };
            inline       wchar_t* chr    ()       { return data;          };
            inline const wchar_t* chr_end() const { return data + chr_to; };
            inline       wchar_t* chr_end()       { return data + chr_to; };
            inline       uint16_t chr_len() const { return chr_to;        };

            inline const wchar_t* desc    () const { return data + chr_to;    };
            inline       wchar_t* desc    ()       { return data + chr_to;    };
            inline const wchar_t* desc_end() const { return data + desc_to;   };
            inline       wchar_t* desc_end()       { return data + desc_to;   };
            inline       uint16_t desc_len() const { return desc_to - chr_to; };

            inline const wchar_t* rel    () const { return data + desc_to;   };
            inline       wchar_t* rel    ()       { return data + desc_to;   };
            inline const wchar_t* rel_end() const { return data + rel_to;    };
            inline       wchar_t* rel_end()       { return data + rel_to;    };
            inline       uint16_t rel_len() const { return rel_to - desc_to; };
        };
#pragma pack(pop)

        ///
        /// Character index
        ///
        class indexChr : public index<uint16_t, uint32_t, character>
        {
        public:
            ///
            /// Constructs the index
            ///
            /// \param[in] h  Reference to vector holding the data
            ///
            indexChr(_In_ std::vector<uint16_t> &h) : index<uint16_t, uint32_t, character>(h) {}

            ///
            /// Compares two characters by ID (for searching)
            ///
            /// \param[in] a  Pointer to first element
            /// \param[in] b  Pointer to second element
            ///
            /// \returns
            /// - <0 when a <  b
            /// - =0 when a == b
            /// - >0 when a >  b
            ///
            virtual int compare(_In_ const character &a, _In_ const character &b) const
            {
                int r = ZRCola::CompareString(a.chr(), a.chr_len(), b.chr(), b.chr_len());
                if (r != 0) return r;

                return 0;
            }
        } idxChr;      ///< Character index

        textindex<wchar_t, wchar_t, uint32_t> idxDsc;    ///< Description index
        textindex<wchar_t, wchar_t, uint32_t> idxDscSub; ///< Description index (sub-terms)
        std::vector<uint16_t> data;                      ///< Character data

    public:
        ///
        /// Constructs the database
        ///
        inline character_db() : idxChr(data) {}

        ///
        /// Clears the database
        ///
        inline void clear()
        {
            idxChr   .clear();
            idxDsc   .clear();
            idxDscSub.clear();
            data     .clear();
        }

        ///
        /// Search for characters by description in given categories
        ///
        /// \param[in   ] str       Search string
        /// \param[in   ] cats      Set of categories, character must be a part of
        /// \param[inout] hits      (character, count) map to append full-word hits to
        /// \param[inout] hits_sub  (character, count) map to append partial-word hits to
        /// \param[in   ] fn_abort  Pointer to function to periodically test for search cancellation
        /// \param[in   ] cookie    Cookie for \p fn_abort call
        ///
        bool Search(_In_z_ const wchar_t *str, _In_ const std::set<chrcatid_t> &cats, _Inout_ std::map<std::wstring, charrank_t> &hits, _Inout_ std::map<std::wstring, charrank_t> &hits_sub, _In_opt_ bool (__cdecl *fn_abort)(void *cookie) = NULL, _In_opt_ void *cookie = NULL) const;

        ///
        /// Get character category
        ///
        /// \param[in] chr  Character
        /// \param[in] len  Number of UTF-16 characters in \p chr
        ///
        /// \returns
        /// - Character category if character found
        /// - `ZRCola::chrcatid_t::blank` otherwise
        ///
        inline chrcatid_t GetCharCat(_In_z_count_(len) const wchar_t *chr, _In_ const size_t len) const
        {
            assert(len <= 0xffff);
            std::unique_ptr<character> c((character*)new char[sizeof(character) + sizeof(wchar_t)*len]);
            c->character::character(chr, len);
            indexChr::size_type start;
            return idxChr.find(*c, start) ? idxChr[start].cat : chrcatid_t::blank;
        }
    };


    typedef stdex::idrec::record<character_db, recordid_t, recordsize_t, ZRCOLA_RECORD_ALIGN> character_rec;


    ///
    /// Character category database
    ///
    class chrcat_db {
    public:
#pragma pack(push)
#pragma pack(2)
        ///
        /// Character category data
        ///
        struct chrcat {
        public:
            chrcatid_t cat;     ///< Character category ID
            uint16_t rank;      ///< Character category rank

        protected:
            uint16_t name_to;   ///< Character category name end in \c data
            wchar_t data[];     ///< Character category name

        private:
            inline chrcat(_In_ const chrcat &other);
            inline chrcat& operator=(_In_ const chrcat &other);

        public:
            ///
            /// Constructs the character category
            ///
            /// \param[in] cat       Character category ID
            /// \param[in] rank      Character category rank
            /// \param[in] name      Character category name
            /// \param[in] name_len  Number of UTF-16 characters in \p name
            ///
            inline chrcat(
                _In_opt_                         chrcatid_t        cat      = chrcatid_t::blank,
                _In_opt_                         uint16_t  rank     = 0,
                _In_opt_z_count_(name_len) const wchar_t          *name     = NULL,
                _In_opt_                         size_t            name_len = 0)
            {
                this->cat  = cat;
                this->rank = rank;
                this->name_to = static_cast<uint16_t>(name_len);
                if (name && name_len) memcpy(this->data, name, sizeof(wchar_t)*name_len);
            }

            inline const wchar_t*         name    () const { return data;           };
            inline       wchar_t*         name    ()       { return data;           };
            inline const wchar_t*         name_end() const { return data + name_to; };
            inline       wchar_t*         name_end()       { return data + name_to; };
            inline       uint16_t name_len() const { return name_to;        };
        };
#pragma pack(pop)

        ///
        /// Character category index
        ///
        class indexChrCat : public index<uint16_t, uint32_t, chrcat>
        {
        public:
            ///
            /// Constructs the index
            ///
            /// \param[in] h  Reference to vector holding the data
            ///
            indexChrCat(_In_ std::vector<uint16_t> &h) : index<uint16_t, uint32_t, chrcat>(h) {}

            ///
            /// Compares two character categories by ID (for searching)
            ///
            /// \param[in] a  Pointer to first element
            /// \param[in] b  Pointer to second element
            ///
            /// \returns
            /// - <0 when a <  b
            /// - =0 when a == b
            /// - >0 when a >  b
            ///
            virtual int compare(_In_ const chrcat &a, _In_ const chrcat &b) const
            {
                     if (a.cat < b.cat) return -1;
                else if (a.cat > b.cat) return  1;

                return  0;
            }
        } idxChrCat;    ///< Character category index

        ///
        /// Rank index
        ///
        class indexRank : public index<uint16_t, uint32_t, chrcat>
        {
        public:
            ///
            /// Constructs the index
            ///
            /// \param[in] h  Reference to vector holding the data
            ///
            indexRank(_In_ std::vector<uint16_t> &h) : index<uint16_t, uint32_t, chrcat>(h) {}

            ///
            /// Compares two character categories by ID (for searching)
            ///
            /// \param[in] a  Pointer to first element
            /// \param[in] b  Pointer to second element
            ///
            /// \returns
            /// - <0 when a <  b
            /// - =0 when a == b
            /// - >0 when a >  b
            ///
            virtual int compare(_In_ const chrcat &a, _In_ const chrcat &b) const
            {
                     if (a.rank < b.rank) return -1;
                else if (a.rank > b.rank) return +1;

                return 0;
            }

            ///
            /// Compares two character categories by rank (for sorting)
            ///
            /// \param[in] a  Pointer to character category
            /// \param[in] b  Pointer to second character category
            ///
            /// \returns
            /// - <0 when a <  b
            /// - =0 when a == b
            /// - >0 when a >  b
            ///
            virtual int compare_sort(_In_ const chrcat &a, _In_ const chrcat &b) const
            {
                     if (a.rank < b.rank) return -1;
                else if (a.rank > b.rank) return +1;

                uint16_t
                    a_name_len = a.name_len(),
                    b_name_len = b.name_len();
                int r = _wcsncoll(a.name(), b.name(), std::min<uint16_t>(a_name_len, b_name_len));
                if (r != 0) return r;
                     if (a_name_len < b_name_len) return -1;
                else if (a_name_len > b_name_len) return +1;

                return 0;
            }
        } idxRank;  ///< Rank index

        std::vector<uint16_t> data;     ///< Character category data

    public:
        ///
        /// Constructs the database
        ///
        inline chrcat_db() : idxChrCat(data), idxRank(data) {}

        ///
        /// Clears the database
        ///
        inline void clear()
        {
            idxChrCat.clear();
            idxRank  .clear();
            data     .clear();
        }
    };


    typedef stdex::idrec::record<chrcat_db, recordid_t, recordsize_t, ZRCOLA_RECORD_ALIGN> chrcat_rec;
};


const ZRCola::recordid_t ZRCola::character_rec::id = *(ZRCola::recordid_t*)"CHR";
const ZRCola::recordid_t ZRCola::chrcat_rec   ::id = *(ZRCola::recordid_t*)"CCT";


///
/// Reads character database from a stream
///
/// \param[in ] stream  Input stream
/// \param[out] db      Character database
///
/// \returns The stream \p stream
///
inline std::istream& operator >>(_In_ std::istream& stream, _Out_ ZRCola::character_db &db)
{
    // Read character index.
    stream >> db.idxChr;
    if (!stream.good()) return stream;

    // Read description index.
    stream >> db.idxDsc;
    if (!stream.good()) return stream;

    // Read sub-term description index.
    stream >> db.idxDscSub;
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


///
/// Writes character database to a stream
///
/// \param[in] stream  Output stream
/// \param[in] db      Character database
///
/// \returns The stream \p stream
///
inline std::ostream& operator <<(_In_ std::ostream& stream, _In_ const ZRCola::character_db &db)
{
    // Write character index.
    if (stream.fail()) return stream;
    stream << db.idxChr;

    // Write description index.
    if (!stream.good()) return stream;
    stream << db.idxDsc;

    // Write sub-term description index.
    if (!stream.good()) return stream;
    stream << db.idxDscSub;

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
/// Writes character category database to a stream
///
/// \param[in] stream  Output stream
/// \param[in] db      Character category database
///
/// \returns The stream \p stream
///
inline std::ostream& operator <<(_In_ std::ostream& stream, _In_ const ZRCola::chrcat_db &db)
{
    // Write character category index.
    if (stream.fail()) return stream;
    stream << db.idxChrCat;

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
    uint32_t count = (uint32_t)data_count;
    stream.write((const char*)&count, sizeof(count));

    // Write data.
    if (stream.fail()) return stream;
    stream.write((const char*)db.data.data(), sizeof(uint16_t)*static_cast<std::streamsize>(count));

    return stream;
}


///
/// Reads character category database from a stream
///
/// \param[in ] stream  Input stream
/// \param[out] db      Character category database
///
/// \returns The stream \p stream
///
inline std::istream& operator >>(_In_ std::istream& stream, _Out_ ZRCola::chrcat_db &db)
{
    // Read character category index.
    stream >> db.idxChrCat;
    if (!stream.good()) return stream;

    // Read rank index.
    stream >> db.idxRank;
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
