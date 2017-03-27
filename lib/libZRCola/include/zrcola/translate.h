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
            unsigned __int16 dst_rank; ///< Destination character rank
            unsigned __int16 src_rank; ///< Source character rank

        protected:
            unsigned __int16 dst_to;   ///< Destination character end in \c data
            unsigned __int16 src_to;   ///< Source string end in \c data
            wchar_t data[];            ///< Destination string and source character

        private:
            inline translation(_In_ const translation &other);
            inline translation& operator=(_In_ const translation &other);

        public:
            ///
            /// Constructs the translation
            ///
            /// \param[in] dst_rank Destination character rank
            /// \param[in] dst      Destination character
            /// \param[in] dst_len  Number of UTF-16 characters in \p dst
            /// \param[in] src_rank Source character rank
            /// \param[in] src      Source character
            /// \param[in] src_len  Number of UTF-16 characters in \p src
            ///
            inline translation(
                _In_opt_                        unsigned __int16  dst_rank = 0,
                _In_opt_z_count_(dst_len) const wchar_t          *dst      = NULL,
                _In_opt_                        size_t            dst_len  = 0,
                _In_opt_                        unsigned __int16  src_rank = 0,
                _In_opt_z_count_(src_len) const wchar_t          *src      = NULL,
                _In_opt_                        size_t            src_len  = 0)
            {
                this->dst_rank = dst_rank;
                this->src_rank = src_rank;
                this->dst_to = static_cast<unsigned __int16>(dst_len);
                if (dst_len) memcpy(this->data, dst, sizeof(wchar_t)*dst_len);
                this->src_to = static_cast<unsigned __int16>(this->dst_to + src_len);
                if (src_len) memcpy(this->data + this->dst_to, src, sizeof(wchar_t)*src_len);
            }

            inline const wchar_t*         dst    () const { return data;          };
            inline       wchar_t*         dst    ()       { return data;          };
            inline const wchar_t*         dst_end() const { return data + dst_to; };
            inline       wchar_t*         dst_end()       { return data + dst_to; };
            inline       unsigned __int16 dst_len() const { return dst_to;        };

            inline wchar_t dst_at(_In_ size_t i) const
            {
                return i < dst_to ? data[i] : 0;
            }

            inline const wchar_t*         src    () const { return data + dst_to;   };
            inline       wchar_t*         src    ()       { return data + dst_to;   };
            inline const wchar_t*         src_end() const { return data + src_to;   };
            inline       wchar_t*         src_end()       { return data + src_to;   };
            inline       unsigned __int16 src_len() const { return src_to - dst_to; };

            inline wchar_t src_at(_In_ size_t i) const
            {
                size_t ii = i + dst_to; // absolute index
                return ii < src_to ? data[ii] : 0;
            }
        };
#pragma pack(pop)

        ///
        /// Translation index
        ///
        class indexTrans : public index<unsigned __int16, unsigned __int32, translation>
        {
        public:
            ///
            /// Constructs the index
            ///
            /// \param[in] h  Reference to vector holding the data
            ///
            indexTrans(_In_ std::vector<unsigned __int16> &h) : index<unsigned __int16, unsigned __int32, translation>(h) {}

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
                int r = ZRCola::CompareString(a.src(), a.src_len(), b.src(), b.src_len());
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
                int r = ZRCola::CompareString(a.src(), a.src_len(), b.src(), b.src_len());
                if (r != 0) return r;

                     if (a.src_rank < b.src_rank) return -1;
                else if (a.src_rank > b.src_rank) return +1;

                r = ZRCola::CompareString(a.dst(), a.dst_len(), b.dst(), b.dst_len());
                if (r != 0) return r;

                return 0;
            }
        } idxTrans;      ///< Translation index


        ///
        /// Inverse translation index
        ///
        class indexTransInv : public index<unsigned __int16, unsigned __int32, translation>
        {
        public:
            ///
            /// Constructs the index
            ///
            /// \param[in] h  Reference to vector holding the data
            ///
            indexTransInv(_In_ std::vector<unsigned __int16> &h) : index<unsigned __int16, unsigned __int32, translation>(h) {}

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
                int r = ZRCola::CompareString(a.dst(), a.dst_len(), b.dst(), b.dst_len());
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
                int r = ZRCola::CompareString(a.dst(), a.dst_len(), b.dst(), b.dst_len());
                if (r != 0) return r;

                     if (a.dst_rank < b.dst_rank) return -1;
                else if (a.dst_rank > b.dst_rank) return +1;

                r = ZRCola::CompareString(a.src(), a.src_len(), b.src(), b.src_len());
                if (r != 0) return r;

                return 0;
            }
        } idxTransInv;    ///< Inverse translation index


        std::vector<unsigned __int16> data;         ///< Transformation data

    public:
        ///
        /// Constructs the database
        ///
        inline translation_db() : idxTrans(data), idxTransInv(data) {}

        ///
        /// Clears the database
        ///
        inline void clear()
        {
            idxTrans   .clear();
            idxTransInv.clear();
            data       .clear();
        }

        ///
        /// Translates string
        ///
        /// \param[in]  input     Input string (UTF-16)
        /// \param[in]  inputMax  Length of the input string in characters. Can be (size_t)-1 if \p input is zero terminated.
        /// \param[out] output    Output string (UTF-16)
        /// \param[out] map       The vector of source to destination index mappings (optional)
        ///
        void Translate(_In_z_count_(inputMax) const wchar_t* input, _In_ size_t inputMax, _Out_ std::wstring &output, _Out_opt_ std::vector<mapping>* map = NULL) const;

        ///
        /// Inverse translates string
        ///
        /// \param[in]  input     Input string (UTF-16)
        /// \param[in]  inputMax  Length of the input string in characters. Can be (size_t)-1 if \p input is zero terminated.
        /// \param[out] output    Output string (UTF-16)
        /// \param[out] map       The vector of source to destination index mappings (optional)
        ///
        inline void TranslateInv(_In_z_count_(inputMax) const wchar_t* input, _In_ size_t inputMax, _Out_ std::wstring &output, _Out_opt_ std::vector<mapping>* map = NULL) const
        {
            TranslateInv(input, inputMax, NULL, langid_t::blank, output, map);
        }

        ///
        /// Inverse translates string according ommiting language specific characters
        ///
        /// \param[in]  input     Input string (UTF-16)
        /// \param[in]  inputMax  Length of the input string in characters. Can be (size_t)-1 if \p input is zero terminated.
        /// \param[in]  lc_db     Language character database
        /// \param[in]  lang      Language ID
        /// \param[out] output    Output string (UTF-16)
        /// \param[out] map       The vector of source to destination index mappings (optional)
        ///
        void TranslateInv(_In_z_count_(inputMax) const wchar_t* input, _In_ size_t inputMax, _In_opt_ const langchar_db *lc_db, _In_opt_ langid_t lang, _Out_ std::wstring &output, _Out_opt_ std::vector<mapping>* map = NULL) const;
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
    // Write translation index.
    if (stream.fail()) return stream;
    stream << db.idxTrans;

    // Write inverse translation index.
    if (stream.fail()) return stream;
    stream << db.idxTransInv;

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
    // Read translation index.
    stream >> db.idxTrans;
    if (!stream.good()) return stream;

    // Read inverse translation index.
    stream >> db.idxTransInv;
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
