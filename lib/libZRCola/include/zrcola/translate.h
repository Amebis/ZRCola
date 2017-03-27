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
#include <ostream>
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
        public:
            unsigned __int16 com_rank; ///< Composed character rank
            unsigned __int16 dec_rank; ///< Decomposed character rank

        protected:
            unsigned __int16 com_to;   ///< Composed character end in \c data
            unsigned __int16 dec_to;   ///< Decomposed string end in \c data
            wchar_t data[];            ///< Decomposed string and composed character

        private:
            inline translation(_In_ const translation &other);
            inline translation& operator=(_In_ const translation &other);

        public:
            ///
            /// Constructs the translation
            ///
            /// \param[in] com_rank Composed character rank
            /// \param[in] com      Composed character
            /// \param[in] com_len  Number of UTF-16 characters in \p com
            /// \param[in] dec_rank Decomposed character rank
            /// \param[in] dec      Decomposed character
            /// \param[in] dec_len  Number of UTF-16 characters in \p dec
            ///
            inline translation(
                _In_opt_                        unsigned __int16  com_rank = 0,
                _In_opt_z_count_(com_len) const wchar_t          *com      = NULL,
                _In_opt_                        size_t            com_len  = 0,
                _In_opt_                        unsigned __int16  dec_rank = 0,
                _In_opt_z_count_(dec_len) const wchar_t          *dec      = NULL,
                _In_opt_                        size_t            dec_len  = 0)
            {
                this->com_rank = com_rank;
                this->dec_rank = dec_rank;
                this->com_to = static_cast<unsigned __int16>(com_len);
                if (com_len) memcpy(this->data, com, sizeof(wchar_t)*com_len);
                this->dec_to = static_cast<unsigned __int16>(this->com_to + dec_len);
                if (dec_len) memcpy(this->data + this->com_to, dec, sizeof(wchar_t)*dec_len);
            }

            inline const wchar_t*         com    () const { return data;          };
            inline       wchar_t*         com    ()       { return data;          };
            inline const wchar_t*         com_end() const { return data + com_to; };
            inline       wchar_t*         com_end()       { return data + com_to; };
            inline       unsigned __int16 com_len() const { return com_to;        };

            inline wchar_t com_at(_In_ size_t i) const
            {
                return i < com_to ? data[i] : 0;
            }

            inline const wchar_t*         dec    () const { return data + com_to;   };
            inline       wchar_t*         dec    ()       { return data + com_to;   };
            inline const wchar_t*         dec_end() const { return data + dec_to;   };
            inline       wchar_t*         dec_end()       { return data + dec_to;   };
            inline       unsigned __int16 dec_len() const { return dec_to - com_to; };

            inline wchar_t dec_at(_In_ size_t i) const
            {
                size_t ii = i + com_to; // absolute index
                return ii < dec_to ? data[ii] : 0;
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
                int r = ZRCola::CompareString(a.dec(), a.dec_len(), b.dec(), b.dec_len());
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
                int r = ZRCola::CompareString(a.dec(), a.dec_len(), b.dec(), b.dec_len());
                if (r != 0) return r;

                     if (a.dec_rank < b.dec_rank) return -1;
                else if (a.dec_rank > b.dec_rank) return +1;

                r = ZRCola::CompareString(a.com(), a.com_len(), b.com(), b.com_len());
                if (r != 0) return r;

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
                int r = ZRCola::CompareString(a.com(), a.com_len(), b.com(), b.com_len());
                if (r != 0) return r;

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
                int r = ZRCola::CompareString(a.com(), a.com_len(), b.com(), b.com_len());
                if (r != 0) return r;

                     if (a.com_rank < b.com_rank) return -1;
                else if (a.com_rank > b.com_rank) return +1;

                r = ZRCola::CompareString(a.dec(), a.dec_len(), b.dec(), b.dec_len());
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
        /// Clears the database
        ///
        inline void clear()
        {
            idxComp  .clear();
            idxDecomp.clear();
            data     .clear();
        }

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
            Decompose(input, inputMax, NULL, langid_t::blank, output, map);
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
/// Writes translation database to a stream
///
/// \param[in] stream  Output stream
/// \param[in] db      Translation database
///
/// \returns The stream \p stream
///
inline std::ostream& operator <<(_In_ std::ostream& stream, _In_ const ZRCola::translation_db &db)
{
    // Write composition index.
    if (stream.fail()) return stream;
    stream << db.idxComp;

    // Write decomposition index.
    if (stream.fail()) return stream;
    stream << db.idxDecomp;

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
/// Reads translation database from a stream
///
/// \param[in ] stream  Input stream
/// \param[out] db      Translation database
///
/// \returns The stream \p stream
///
inline std::istream& operator >>(_In_ std::istream& stream, _Out_ ZRCola::translation_db &db)
{
    // Read composition index.
    stream >> db.idxComp;
    if (!stream.good()) return stream;

    // Read decomposition index.
    stream >> db.idxDecomp;
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
