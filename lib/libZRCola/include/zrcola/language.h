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
    ///
    /// Language Character Database
    ///
    class ZRCOLA_API langchar_db {
    public:
#pragma pack(push)
#pragma pack(2)
        ///
        /// Character data
        ///
        struct langchar {
            wchar_t chr;                ///> Character
            langid_t lang;              ///< Language ID
        };
#pragma pack(pop)

        ///
        /// Character index
        ///
        class indexChar : public index<unsigned __int16, unsigned __int32, langchar>
        {
        public:
            ///
            /// Constructs the index
            ///
            /// \param[in] h  Reference to vector holding the data
            ///
            indexChar(_In_ std::vector<unsigned __int16> &h) : index<unsigned __int16, unsigned __int32, langchar>(h) {}

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
            virtual int compare(_In_ const langchar &a, _In_ const langchar &b) const
            {
                     if (a.chr < b.chr) return -1;
                else if (a.chr > b.chr) return  1;

                return 0;
            }

            ///
            /// Compares two characters by ID (for sorting)
            ///
            /// \param[in] a  Pointer to first element
            /// \param[in] b  Pointer to second element
            ///
            /// \returns
            /// - <0 when a <  b
            /// - =0 when a == b
            /// - >0 when a >  b
            ///
            virtual int compare_sort(_In_ const langchar &a, _In_ const langchar &b) const
            {
                     if (a.chr < b.chr) return -1;
                else if (a.chr > b.chr) return  1;

                     if (a.lang < b.lang) return -1;
                else if (a.lang > b.lang) return  1;

                return 0;
            }
        } idxChr;      ///< Character index


#ifdef ZRCOLA_LANGCHAR_LANG_IDX
        ///
        /// Character Language Index
        ///
        class indexCharLang : public index<unsigned __int16, unsigned __int32, langchar>
        {
        public:
            ///
            /// Constructs the index
            ///
            /// \param[in] h  Reference to vector holding the data
            ///
            indexCharLang(_In_ std::vector<unsigned __int16> &h) : index<unsigned __int16, unsigned __int32, langchar>(h) {}

            ///
            /// Compares two languages by ID (for searching)
            ///
            /// \param[in] a  Pointer to first element
            /// \param[in] b  Pointer to second element
            ///
            /// \returns
            /// - <0 when a <  b
            /// - =0 when a == b
            /// - >0 when a >  b
            ///
            virtual int compare(_In_ const langchar &a, _In_ const langchar &b) const
            {
                int r = memcmp(a.lang, b.lang, sizeof(langid_t));
                if (r != 0) return r;

                return 0;
            }

            ///
            /// Compares two languages by ID (for sorting)
            ///
            /// \param[in] a  Pointer to first element
            /// \param[in] b  Pointer to second element
            ///
            /// \returns
            /// - <0 when a <  b
            /// - =0 when a == b
            /// - >0 when a >  b
            ///
            virtual int compare_sort(_In_ const langchar &a, _In_ const langchar &b) const
            {
                int r = memcmp(a.lang, b.lang, sizeof(langid_t));
                if (r != 0) return r;

                     if (a.chr < b.chr) return -1;
                else if (a.chr > b.chr) return  1;

                return 0;
            }
        } idxLng;      ///< Character language index
#endif

        std::vector<unsigned __int16> data;     ///< Character data

    public:
        ///
        /// Constructs the database
        ///
        inline langchar_db() : idxChr(data)
#ifdef ZRCOLA_LANGCHAR_LANG_IDX
            , idxLng(data)
#endif
        {}

        ///
        /// Clears the database
        ///
        inline void clear()
        {
            idxChr.clear();
#ifdef ZRCOLA_LANGCHAR_LANG_IDX
            idxLng.clear();
#endif
            data  .clear();
        }

        ///
        /// Tests presence of character in the given language
        ///
        /// \param[in]  chr   Character (UTF-16)
        /// \param[in]  lang  Language
        ///
        /// \returns
        /// - \c true when character is used in language
        /// - \c false otherwise
        bool IsLocalCharacter(_In_ wchar_t chr, _In_ langid_t lang) const;
    };


    typedef ZRCOLA_API stdex::idrec::record<langchar_db, recordid_t, recordsize_t, ZRCOLA_RECORD_ALIGN> langchar_rec;


    ///
    /// Language database
    ///
    class ZRCOLA_API language_db {
    public:
#pragma pack(push)
#pragma pack(2)
        ///
        /// Language data
        ///
        struct language {
            langid_t id;                ///< Language ID
            unsigned __int16 name_len;  ///< \c name length (in characters)
            wchar_t name[];             ///< Language name
        };
#pragma pack(pop)

        ///
        /// Language index
        ///
        class indexLang : public index<unsigned __int16, unsigned __int32, language>
        {
        public:
            ///
            /// Constructs the index
            ///
            /// \param[in] h  Reference to vector holding the data
            ///
            indexLang(_In_ std::vector<unsigned __int16> &h) : index<unsigned __int16, unsigned __int32, language>(h) {}

            ///
            /// Compares two languages by ID (for searching)
            ///
            /// \param[in] a  Pointer to first element
            /// \param[in] b  Pointer to second element
            ///
            /// \returns
            /// - <0 when a <  b
            /// - =0 when a == b
            /// - >0 when a >  b
            ///
            virtual int compare(_In_ const language &a, _In_ const language &b) const
            {
                     if (a.id < b.id) return -1;
                else if (a.id > b.id) return  1;

                return 0;
            }
        } idxLng;      ///< Language index

        std::vector<unsigned __int16> data;     ///< Language data

    public:
        ///
        /// Constructs the database
        ///
        inline language_db() : idxLng(data) {}

        ///
        /// Clears the database
        ///
        inline void clear()
        {
            idxLng.clear();
            data  .clear();
        }
    };


    typedef ZRCOLA_API stdex::idrec::record<language_db, recordid_t, recordsize_t, ZRCOLA_RECORD_ALIGN> language_rec;
};


const ZRCola::recordid_t stdex::idrec::record<ZRCola::langchar_db, ZRCola::recordid_t, ZRCola::recordsize_t, ZRCOLA_RECORD_ALIGN>::id = *(ZRCola::recordid_t*)"L-C";
const ZRCola::recordid_t stdex::idrec::record<ZRCola::language_db, ZRCola::recordid_t, ZRCola::recordsize_t, ZRCOLA_RECORD_ALIGN>::id = *(ZRCola::recordid_t*)"LNG";


///
/// Writes language character database to a stream
///
/// \param[in] stream  Output stream
/// \param[in] db      Language character database
///
/// \returns The stream \p stream
///
inline std::ostream& operator <<(_In_ std::ostream& stream, _In_ const ZRCola::langchar_db &db)
{
    // Write character index.
    if (stream.fail()) return stream;
    stream << db.idxChr;

#ifdef ZRCOLA_LANGCHAR_LANG_IDX
    // Write language index.
    if (stream.fail()) return stream;
    stream << db.idxLng;
#endif

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
/// Reads language character database from a stream
///
/// \param[in ] stream  Input stream
/// \param[out] db      Language character database
///
/// \returns The stream \p stream
///
inline std::istream& operator >>(_In_ std::istream& stream, _Out_ ZRCola::langchar_db &db)
{
    // Read character index.
    stream >> db.idxChr;
    if (!stream.good()) return stream;

#ifdef ZRCOLA_LANGCHAR_LANG_IDX
    // Read language index.
    stream >> db.idxLng;
    if (!stream.good()) return stream;
#endif

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
/// Writes language database to a stream
///
/// \param[in] stream  Output stream
/// \param[in] db      Language database
///
/// \returns The stream \p stream
///
inline std::ostream& operator <<(_In_ std::ostream& stream, _In_ const ZRCola::language_db &db)
{
    // Write language index.
    if (stream.fail()) return stream;
    stream << db.idxLng;

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
/// Reads language database from a stream
///
/// \param[in ] stream  Input stream
/// \param[out] db      Language database
///
/// \returns The stream \p stream
///
inline std::istream& operator >>(_In_ std::istream& stream, _Out_ ZRCola::language_db &db)
{
    // Read language index.
    stream >> db.idxLng;
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
