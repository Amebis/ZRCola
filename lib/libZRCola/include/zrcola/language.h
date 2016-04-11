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
#include <vector>
#include <string>

#pragma warning(push)
#pragma warning(disable: 4200)
#pragma warning(disable: 4251)
#pragma warning(disable: 4512)


namespace ZRCola {
    ///
    /// Language ID type
    /// Three letter abbreviation, zero terminated
    ///
    typedef char langid_t[4];

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

                int r = memcmp(a.lang, b.lang, sizeof(langid_t));
                if (r != 0) return r;

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
                int r = memcmp(a.id, b.id, sizeof(langid_t));
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
            virtual int compare_sort(_In_ const language &a, _In_ const language &b) const
            {
                int r = memcmp(a.id, b.id, sizeof(langid_t));
                if (r != 0) return r;

                // As the language ID must not duplicate, further comparison is pointless.

                return 0;
            }
        } idxLng;      ///< Language index

        std::vector<unsigned __int16> data;     ///< Language data

    public:
        ///
        /// Constructs the database
        ///
        inline language_db() : idxLng(data) {}
    };


    typedef ZRCOLA_API stdex::idrec::record<language_db, recordid_t, recordsize_t, ZRCOLA_RECORD_ALIGN> language_rec;
};


const ZRCola::recordid_t stdex::idrec::record<ZRCola::langchar_db, ZRCola::recordid_t, ZRCola::recordsize_t, ZRCOLA_RECORD_ALIGN>::id = *(ZRCola::recordid_t*)"L-C";
const ZRCola::recordid_t stdex::idrec::record<ZRCola::language_db, ZRCola::recordid_t, ZRCola::recordsize_t, ZRCOLA_RECORD_ALIGN>::id = *(ZRCola::recordid_t*)"LNG";


///
/// Reads language character database from a stream
///
/// \param[in]  stream  Input stream
/// \param[out] db      Language character database
///
/// \returns The stream \p stream
///
inline std::istream& operator >>(_In_ std::istream& stream, _Out_ ZRCola::langchar_db &db)
{
    unsigned __int32 count;

    // Read index count.
    stream.read((char*)&count, sizeof(count));
    if (!stream.good()) return stream;

    // Read character index.
    db.idxChr.resize(count);
    stream.read((char*)db.idxChr.data(), sizeof(unsigned __int32)*count);
    if (!stream.good()) return stream;

#ifdef ZRCOLA_LANGCHAR_LANG_IDX
    // Read language index.
    db.idxLng.resize(count);
    stream.read((char*)db.idxLng.data(), sizeof(unsigned __int32)*count);
    if (!stream.good()) return stream;
#endif

    // Read data count.
    stream.read((char*)&count, sizeof(count));
    if (!stream.good()) return stream;

    // Read data.
    db.data.resize(count);
    stream.read((char*)db.data.data(), sizeof(unsigned __int16)*count);

    return stream;
}


///
/// Reads language database from a stream
///
/// \param[in]  stream  Input stream
/// \param[out] db      Language database
///
/// \returns The stream \p stream
///
inline std::istream& operator >>(_In_ std::istream& stream, _Out_ ZRCola::language_db &db)
{
    unsigned __int32 count;

    // Read index count.
    stream.read((char*)&count, sizeof(count));
    if (!stream.good()) return stream;

    // Read language index.
    db.idxLng.resize(count);
    stream.read((char*)db.idxLng.data(), sizeof(unsigned __int32)*count);
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
