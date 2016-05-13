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
    };


    ///
    /// Blank character category
    ///
    const chrcatid_t chrcatid_t_blank = {};


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
    class ZRCOLA_API character_db {
    public:
#pragma pack(push)
#pragma pack(2)
        ///
        /// Character data
        ///
        struct character {
            wchar_t chr;                ///> Character
            chrcatid_t cat;             ///> Category ID
            unsigned __int16 desc_len;  ///< Character description length in \c data
            unsigned __int16 rel_len;   ///< Related character count in \c data
            wchar_t data[];             ///< Character description and list of related characters
        };
#pragma pack(pop)

        ///
        /// Character index
        ///
        class indexChar : public index<unsigned __int16, unsigned __int32, character>
        {
        public:
            ///
            /// Constructs the index
            ///
            /// \param[in] h  Reference to vector holding the data
            ///
            indexChar(_In_ std::vector<unsigned __int16> &h) : index<unsigned __int16, unsigned __int32, character>(h) {}

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
                     if (a.chr < b.chr) return -1;
                else if (a.chr > b.chr) return  1;

                return 0;
            }
        } idxChr;      ///< Character index

        textindex<wchar_t, wchar_t, unsigned __int32> idxDsc;    ///< Description index
        textindex<wchar_t, wchar_t, unsigned __int32> idxDscSub; ///< Description index (sub-terms)
        std::vector<unsigned __int16> data;                      ///< Character data

    public:
        ///
        /// Constructs the database
        ///
        inline character_db() : idxChr(data) {}

        ///
        /// Search for characters by description in given categories
        ///
        /// \param[in   ] str       Search string
        /// \param[in   ] cats      Set of categories, character must be a part of
        /// \param[inout] hits      (character, count) map to append full-word hits to
        /// \param[inout] hits_sub  (character, count) map to append partial-word hits to
        /// \param[in]    fn_abort  Pointer to function to periodically test for search cancellation
        /// \param[in]    cookie    Cookie for \p fn_abort call
        ///
        bool Search(_In_z_ const wchar_t *str, _In_ const std::set<chrcatid_t> &cats, _Inout_ std::map<wchar_t, unsigned long> &hits, _Inout_ std::map<wchar_t, unsigned long> &hits_sub, _In_opt_ bool (__cdecl *fn_abort)(void *cookie) = NULL, _In_opt_ void *cookie = NULL) const;

        ///
        /// Get character category
        ///
        /// \param[in] c  Character
        ///
        /// \returns
        /// - Character category if character found
        /// - `ZRCola::chrcatid_t_blank` otherwise
        ///
        inline chrcatid_t GetCharCat(wchar_t c) const
        {
            char _chr[sizeof(character)];
            ((character *)_chr)->chr = c;
            indexChar::size_type start;
            return idxChr.find(*((character *)_chr), start) ? idxChr[start].cat : chrcatid_t_blank;
        }
    };


    typedef ZRCOLA_API stdex::idrec::record<character_db, recordid_t, recordsize_t, ZRCOLA_RECORD_ALIGN> character_rec;


    ///
    /// Character category database
    ///
    class ZRCOLA_API chrcat_db {
    public:
#pragma pack(push)
#pragma pack(2)
        ///
        /// Character category data
        ///
        struct chrcat {
            chrcatid_t id;              ///< Character category ID
            unsigned __int16 rank;      ///< Character category rank
            unsigned __int16 name_len;  ///< \c name length (in characters)
            wchar_t name[];             ///< Character category name
        };
#pragma pack(pop)

        ///
        /// Character category index
        ///
        class indexChrCat : public index<unsigned __int16, unsigned __int32, chrcat>
        {
        public:
            ///
            /// Constructs the index
            ///
            /// \param[in] h  Reference to vector holding the data
            ///
            indexChrCat(_In_ std::vector<unsigned __int16> &h) : index<unsigned __int16, unsigned __int32, chrcat>(h) {}

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
                     if (a.id < b.id) return -1;
                else if (a.id > b.id) return  1;
                else                  return  0;
            }
        } idxChrCat;      ///< Character category index

        ///
        /// Rank index
        ///
        class indexRank : public index<unsigned __int16, unsigned __int32, chrcat>
        {
        public:
            ///
            /// Constructs the index
            ///
            /// \param[in] h  Reference to vector holding the data
            ///
            indexRank(_In_ std::vector<unsigned __int16> &h) : index<unsigned __int16, unsigned __int32, chrcat>(h) {}

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

                int r = _wcsncoll(a.name, b.name, std::min<unsigned __int16>(a.name_len, b.name_len));
                if (r != 0) return r;
                     if (a.name_len < b.name_len) return -1;
                else if (a.name_len > b.name_len) return +1;

                return 0;
            }
        } idxRnk;      ///< Rank index

        std::vector<unsigned __int16> data;     ///< Character category data

    public:
        ///
        /// Constructs the database
        ///
        inline chrcat_db() : idxChrCat(data), idxRnk(data) {}
    };


    typedef ZRCOLA_API stdex::idrec::record<chrcat_db, recordid_t, recordsize_t, ZRCOLA_RECORD_ALIGN> chrcat_rec;
};


const ZRCola::recordid_t stdex::idrec::record<ZRCola::character_db, ZRCola::recordid_t, ZRCola::recordsize_t, ZRCOLA_RECORD_ALIGN>::id = *(ZRCola::recordid_t*)"CHR";
const ZRCola::recordid_t stdex::idrec::record<ZRCola::chrcat_db, ZRCola::recordid_t, ZRCola::recordsize_t, ZRCOLA_RECORD_ALIGN>::id = *(ZRCola::recordid_t*)"CCT";


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
    std::vector<unsigned __int16>::size_type data_count = db.data.size();
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
    stream << db.idxRnk;

    // Write data count.
    std::vector<unsigned __int16>::size_type data_count = db.data.size();
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
    stream >> db.idxRnk;
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
