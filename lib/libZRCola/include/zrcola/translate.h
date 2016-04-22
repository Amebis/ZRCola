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
#include "language.h"

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
    /// Translation database
    ///
    class ZRCOLA_API translation_db {
    public:
#pragma pack(push)
#pragma pack(2)
        ///
        /// Translation data
        ///
        struct translation {
            wchar_t chr;                ///< Composed character
            unsigned __int16 rank;      ///< Decomposition rank
            unsigned __int16 str_len;   ///< \c str length (in characters)
            wchar_t str[];              ///< Decomposed string

            ///
            /// Binary compares two strings
            ///
            /// \param[in] str_a    First string
            /// \param[in] count_a  Number of characters in string \p str_a
            /// \param[in] str_b    Second string
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
            static inline int CompareString(const wchar_t *str_a, unsigned __int16 count_a, const wchar_t *str_b, unsigned __int16 count_b)
            {
                for (unsigned __int16 i = 0; ; i++) {
                         if (i >= count_a && i >= count_b) return  0;
                    else if (i >= count_a && i <  count_b) return -1;
                    else if (i <  count_a && i >= count_b) return +1;
                    else if (str_a[i] < str_b[i]) return -1;
                    else if (str_a[i] > str_b[i]) return +1;
                }
            }
        };
#pragma pack(pop)

        ///
        /// Composition index
        ///
        class indexComp : public index<unsigned __int16, unsigned __int32, translation>
        {
        public:
            ///
            /// Constructs the index
            ///
            /// \param[in] h  Reference to vector holding the data
            ///
            indexComp(_In_ std::vector<unsigned __int16> &h) : index<unsigned __int16, unsigned __int32, translation>(h) {}

            ///
            /// Compares two transformations by string (for searching)
            ///
            /// \param[in] a  Pointer to first element
            /// \param[in] b  Pointer to second element
            ///
            /// \returns
            /// - <0 when a <  b
            /// - =0 when a == b
            /// - >0 when a >  b
            ///
            virtual int compare(_In_ const translation &a, _In_ const translation &b) const
            {
                int r = translation::CompareString(a.str, a.str_len, b.str, b.str_len);
                if (r != 0) return r;

                return 0;
            }

            ///
            /// Compares two transformations by string (for sorting)
            ///
            /// \param[in] a  Pointer to first element
            /// \param[in] b  Pointer to second element
            ///
            /// \returns
            /// - <0 when a <  b
            /// - =0 when a == b
            /// - >0 when a >  b
            ///
            virtual int compare_sort(_In_ const translation &a, _In_ const translation &b) const
            {
                int r = translation::CompareString(a.str, a.str_len, b.str, b.str_len);
                if (r != 0) return r;

                     if (a.chr < b.chr) return -1;
                else if (a.chr > b.chr) return +1;

                return 0;
            }
        } idxComp;      ///< Composition index


        ///
        /// Decomposition index
        ///
        class indexDecomp : public index<unsigned __int16, unsigned __int32, translation>
        {
        public:
            ///
            /// Constructs the index
            ///
            /// \param[in] h  Reference to vector holding the data
            ///
            indexDecomp(_In_ std::vector<unsigned __int16> &h) : index<unsigned __int16, unsigned __int32, translation>(h) {}

            ///
            /// Compares two transformations by character (for searching)
            ///
            /// \param[in] a  Pointer to first element
            /// \param[in] b  Pointer to second element
            ///
            /// \returns
            /// - <0 when a <  b
            /// - =0 when a == b
            /// - >0 when a >  b
            ///
            virtual int compare(_In_ const translation &a, _In_ const translation &b) const
            {
                     if (a.chr < b.chr) return -1;
                else if (a.chr > b.chr) return +1;

                return 0;
            }

            ///
            /// Compares two transformations by character (for sorting)
            ///
            /// \param[in] a  Pointer to first element
            /// \param[in] b  Pointer to second element
            ///
            /// \returns
            /// - <0 when a <  b
            /// - =0 when a == b
            /// - >0 when a >  b
            ///
            virtual int compare_sort(_In_ const translation &a, _In_ const translation &b) const
            {
                     if (a.chr < b.chr) return -1;
                else if (a.chr > b.chr) return +1;

                     if (a.rank < b.rank) return -1;
                else if (a.rank > b.rank) return +1;

                int r = translation::CompareString(a.str, a.str_len, b.str, b.str_len);
                if (r != 0) return r;

                return 0;
            }
        } idxDecomp;    ///< Decomposition index


        std::vector<unsigned __int16> data;         ///< Transformation data

    public:
        ///
        /// Constructs the database
        ///
        inline translation_db() : idxComp(data), idxDecomp(data) {}

        ///
        /// Composes string
        ///
        /// \param[in]  input     Input string (UTF-16)
        /// \param[in]  inputMax  Length of the input string in characters. Can be (size_t)-1 if \p input is zero terminated.
        /// \param[out] output    Output string (UTF-16)
        /// \param[out] map       The vector of source to destination index mappings (optional)
        ///
        void Compose(_In_z_count_(inputMax) const wchar_t* input, _In_ size_t inputMax, _Out_ std::wstring &output, _Out_opt_ std::vector<mapping>* map = NULL) const;

        ///
        /// Decomposes string
        ///
        /// \param[in]  input     Input string (UTF-16)
        /// \param[in]  inputMax  Length of the input string in characters. Can be (size_t)-1 if \p input is zero terminated.
        /// \param[out] output    Output string (UTF-16)
        /// \param[out] map       The vector of source to destination index mappings (optional)
        ///
        inline void Decompose(_In_z_count_(inputMax) const wchar_t* input, _In_ size_t inputMax, _Out_ std::wstring &output, _Out_opt_ std::vector<mapping>* map = NULL) const
        {
            Decompose(input, inputMax, NULL, ZRCOLA_LANG_VOID, output, map);
        }

        ///
        /// Decomposes string according ommiting language specific characters
        ///
        /// \param[in]  input     Input string (UTF-16)
        /// \param[in]  inputMax  Length of the input string in characters. Can be (size_t)-1 if \p input is zero terminated.
        /// \param[in]  lc_db     Language character database
        /// \param[in]  lang      Language ID
        /// \param[out] output    Output string (UTF-16)
        /// \param[out] map       The vector of source to destination index mappings (optional)
        ///
        void Decompose(_In_z_count_(inputMax) const wchar_t* input, _In_ size_t inputMax, _In_opt_ const langchar_db *lc_db, _In_opt_ langid_t lang, _Out_ std::wstring &output, _Out_opt_ std::vector<mapping>* map = NULL) const;
    };


    typedef ZRCOLA_API stdex::idrec::record<translation_db, recordid_t, recordsize_t, ZRCOLA_RECORD_ALIGN> translation_rec;
};


const ZRCola::recordid_t stdex::idrec::record<ZRCola::translation_db, ZRCola::recordid_t, ZRCola::recordsize_t, ZRCOLA_RECORD_ALIGN>::id = *(ZRCola::recordid_t*)"TRN";


///
/// Reads translation database from a stream
///
/// \param[in]  stream  Input stream
/// \param[out] db      Translation database
///
/// \returns The stream \p stream
///
inline std::istream& operator >>(_In_ std::istream& stream, _Out_ ZRCola::translation_db &db)
{
    unsigned __int32 count;

    // Read index count.
    stream.read((char*)&count, sizeof(count));
    if (!stream.good()) return stream;

    // Read composition index.
    db.idxComp.resize(count);
    stream.read((char*)db.idxComp.data(), sizeof(unsigned __int32)*count);
    if (!stream.good()) return stream;

    // Read decomposition index.
    db.idxDecomp.resize(count);
    stream.read((char*)db.idxDecomp.data(), sizeof(unsigned __int32)*count);
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
