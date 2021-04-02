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
#include "language.h"

#include <stdex/idrec.h>
#include <algorithm>
#include <istream>
#include <ostream>
#include <vector>
#include <string>

#pragma warning(push)
#pragma warning(disable: 4200)
#pragma warning(disable: 4251)
#pragma warning(disable: 4512)

///
/// Translation disabled/ZRCola (De)Composition
///
#define ZRCOLA_TRANSEQID_DEFAULT    ((ZRCola::transeqid_t)0x0000)

///
/// Custom translation sequence ID
///
#define ZRCOLA_TRANSEQID_CUSTOM     ((ZRCola::transeqid_t)0xffff)

///
/// ZRCola to Unicode translation sequence ID
///
#define ZRCOLA_TRANSEQID_UNICODE    ((ZRCola::transeqid_t)0xfffe)


namespace ZRCola {
    ///
    /// Translation set ID
    ///
    typedef unsigned __int16 transetid_t;

    ///
    /// Translation sequence ID
    ///
    typedef unsigned __int16 transeqid_t;

    ///
    /// Translation database
    ///
    class translation_db {
    public:
#pragma pack(push)
#pragma pack(2)
        ///
        /// Translation data
        ///
        struct translation {
        public:
            transetid_t set;            ///< Translation set ID
            unsigned __int16 dst_rank;  ///< Destination character rank
            unsigned __int16 src_rank;  ///< Source character rank

        protected:
            unsigned __int16 dst_to;    ///< Destination character end in \c data
            unsigned __int16 src_to;    ///< Source string end in \c data
            wchar_t data[];             ///< Destination string and source character

        private:
            inline translation(_In_ const translation &other);
            inline translation& operator=(_In_ const translation &other);

        public:
            ///
            /// Constructs the translation
            ///
            /// \param[in] set      Translation set ID
            /// \param[in] dst_rank Destination character rank
            /// \param[in] dst      Destination character
            /// \param[in] dst_len  Number of UTF-16 characters in \p dst
            /// \param[in] src_rank Source character rank
            /// \param[in] src      Source character
            /// \param[in] src_len  Number of UTF-16 characters in \p src
            ///
            inline translation(
                _In_opt_                        transetid_t       set      = 0,
                _In_opt_                        unsigned __int16  dst_rank = 0,
                _In_opt_z_count_(dst_len) const wchar_t          *dst      = NULL,
                _In_opt_                        size_t            dst_len  = 0,
                _In_opt_                        unsigned __int16  src_rank = 0,
                _In_opt_z_count_(src_len) const wchar_t          *src      = NULL,
                _In_opt_                        size_t            src_len  = 0)
            {
                this->set      = set;
                this->dst_rank = dst_rank;
                this->src_rank = src_rank;
                this->dst_to = static_cast<unsigned __int16>(dst_len);
                if (dst && dst_len) memcpy(this->data, dst, sizeof(wchar_t)*dst_len);
                this->src_to = static_cast<unsigned __int16>(this->dst_to + src_len);
                if (src && src_len) memcpy(this->data + this->dst_to, src, sizeof(wchar_t)*src_len);
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
        class indexSrc : public index<unsigned __int16, unsigned __int32, translation>
        {
        public:
            ///
            /// Constructs the index
            ///
            /// \param[in] h  Reference to vector holding the data
            ///
            indexSrc(_In_ std::vector<unsigned __int16> &h) : index<unsigned __int16, unsigned __int32, translation>(h) {}

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
                     if (a.set < b.set) return -1;
                else if (a.set > b.set) return +1;

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
                     if (a.set < b.set) return -1;
                else if (a.set > b.set) return +1;

                int r = ZRCola::CompareString(a.src(), a.src_len(), b.src(), b.src_len());
                if (r != 0) return r;

                     if (a.src_rank < b.src_rank) return -1;
                else if (a.src_rank > b.src_rank) return +1;

                r = ZRCola::CompareString(a.dst(), a.dst_len(), b.dst(), b.dst_len());
                if (r != 0) return r;

                return 0;
            }
        } idxSrc;   ///< Translation index


        ///
        /// Inverse translation index
        ///
        class indexDst : public index<unsigned __int16, unsigned __int32, translation>
        {
        public:
            ///
            /// Constructs the index
            ///
            /// \param[in] h  Reference to vector holding the data
            ///
            indexDst(_In_ std::vector<unsigned __int16> &h) : index<unsigned __int16, unsigned __int32, translation>(h) {}

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
                     if (a.set < b.set) return -1;
                else if (a.set > b.set) return +1;

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
                     if (a.set < b.set) return -1;
                else if (a.set > b.set) return +1;

                int r = ZRCola::CompareString(a.dst(), a.dst_len(), b.dst(), b.dst_len());
                if (r != 0) return r;

                     if (a.dst_rank < b.dst_rank) return -1;
                else if (a.dst_rank > b.dst_rank) return +1;

                r = ZRCola::CompareString(a.src(), a.src_len(), b.src(), b.src_len());
                if (r != 0) return r;

                return 0;
            }
        } idxDst;   ///< Inverse translation index


        std::vector<unsigned __int16> data;         ///< Transformation data

    public:
        ///
        /// Constructs the database
        ///
        inline translation_db() : idxSrc(data), idxDst(data) {}

        ///
        /// Clears the database
        ///
        inline void clear()
        {
            idxSrc.clear();
            idxDst.clear();
            data  .clear();
        }

        ///
        /// Translates string
        ///
        /// \param[in]  set       Translation set ID
        /// \param[in]  input     Input string (UTF-16)
        /// \param[in]  inputMax  Length of the input string in characters. Can be (size_t)-1 if \p input is zero terminated.
        /// \param[out] output    Output string (UTF-16)
        /// \param[out] map       The vector of source to destination index mappings (optional)
        ///
        void Translate(_In_ transetid_t set, _In_z_count_(inputMax) const wchar_t* input, _In_ size_t inputMax, _Out_ std::wstring &output, _Out_opt_ std::vector<mapping>* map = NULL) const;

        ///
        /// Inverse translates string
        ///
        /// \param[in]  set       Translation set ID
        /// \param[in]  input     Input string (UTF-16)
        /// \param[in]  inputMax  Length of the input string in characters. Can be (size_t)-1 if \p input is zero terminated.
        /// \param[out] output    Output string (UTF-16)
        /// \param[out] map       The vector of source to destination index mappings (optional)
        ///
        inline void TranslateInv(_In_ transetid_t set, _In_z_count_(inputMax) const wchar_t* input, _In_ size_t inputMax, _Out_ std::wstring &output, _Out_opt_ std::vector<mapping>* map = NULL) const
        {
            TranslateInv(set, input, inputMax, NULL, langid_t::blank, output, map);
        }

        ///
        /// Inverse translates string according ommiting language specific characters
        ///
        /// \param[in]  set       Translation set ID
        /// \param[in]  input     Input string (UTF-16)
        /// \param[in]  inputMax  Length of the input string in characters. Can be (size_t)-1 if \p input is zero terminated.
        /// \param[in]  lc_db     Language character database
        /// \param[in]  lang      Language ID
        /// \param[out] output    Output string (UTF-16)
        /// \param[out] map       The vector of source to destination index mappings (optional)
        ///
        void TranslateInv(_In_ transetid_t set, _In_z_count_(inputMax) const wchar_t* input, _In_ size_t inputMax, _In_opt_ const langchar_db *lc_db, _In_opt_ langid_t lang, _Out_ std::wstring &output, _Out_opt_ std::vector<mapping>* map = NULL) const;
    };


    typedef stdex::idrec::record<translation_db, recordid_t, recordsize_t, ZRCOLA_RECORD_ALIGN> translation_rec;


    ///
    /// Translation set database
    ///
    class transet_db {
    public:
#pragma pack(push)
#pragma pack(2)
        ///
        /// Translation set data
        ///
        struct transet {
        public:
            transetid_t set;            ///< Translation set ID

        protected:
            unsigned __int16 src_to;    ///< Source name end in \c data
            unsigned __int16 dst_to;    ///< Sestination name end in \c data
            wchar_t data[];             ///< Source and destination names

        private:
            inline transet(_In_ const transet &other);
            inline transet& operator=(_In_ const transet &other);

        public:
            ///
            /// Constructs the translation set
            ///
            /// \param[in] set      Translation set ID
            /// \param[in] src      Translation set source
            /// \param[in] src_len  Number of UTF-16 characters in \p src
            /// \param[in] dst      Translation set destination
            /// \param[in] dst_len  Number of UTF-16 characters in \p dst
            ///
            inline transet(
                _In_opt_                        transetid_t  set     = 0,
                _In_opt_z_count_(src_len) const wchar_t     *src     = NULL,
                _In_opt_                        size_t       src_len = 0,
                _In_opt_z_count_(dst_len) const wchar_t     *dst     = NULL,
                _In_opt_                        size_t       dst_len = 0)
            {
                this->set = set;
                this->src_to = static_cast<unsigned __int16>(src_len);
                if (src && src_len) memcpy(this->data, src, sizeof(wchar_t)*src_len);
                this->dst_to = static_cast<unsigned __int16>(this->src_to + dst_len);
                if (dst && dst_len) memcpy(this->data + this->src_to, dst, sizeof(wchar_t)*dst_len);
            }

            inline const wchar_t*         src    () const { return data;          };
            inline       wchar_t*         src    ()       { return data;          };
            inline const wchar_t*         src_end() const { return data + src_to; };
            inline       wchar_t*         src_end()       { return data + src_to; };
            inline       unsigned __int16 src_len() const { return src_to;        };

            inline const wchar_t*         dst    () const { return data + src_to;   };
            inline       wchar_t*         dst    ()       { return data + src_to;   };
            inline const wchar_t*         dst_end() const { return data + dst_to;   };
            inline       wchar_t*         dst_end()       { return data + dst_to;   };
            inline       unsigned __int16 dst_len() const { return dst_to - src_to; };
        };
#pragma pack(pop)

        ///
        /// Translation set index
        ///
        class indexTranSet : public index<unsigned __int16, unsigned __int32, transet>
        {
        public:
            ///
            /// Constructs the index
            ///
            /// \param[in] h  Reference to vector holding the data
            ///
            indexTranSet(_In_ std::vector<unsigned __int16> &h) : index<unsigned __int16, unsigned __int32, transet>(h) {}

            ///
            /// Compares two translation sets by ID (for searching)
            ///
            /// \param[in] a  Pointer to first element
            /// \param[in] b  Pointer to second element
            ///
            /// \returns
            /// - <0 when a <  b
            /// - =0 when a == b
            /// - >0 when a >  b
            ///
            virtual int compare(_In_ const transet &a, _In_ const transet &b) const
            {
                     if (a.set < b.set) return -1;
                else if (a.set > b.set) return  1;

                return 0;
            }
        } idxTranSet;   ///< Translation set index

        std::vector<unsigned __int16> data;     ///< Translation set data

    public:
        ///
        /// Constructs the database
        ///
        inline transet_db() : idxTranSet(data) {}

        ///
        /// Clears the database
        ///
        inline void clear()
        {
            idxTranSet.clear();
            data      .clear();
        }
    };


    typedef stdex::idrec::record<transet_db, recordid_t, recordsize_t, ZRCOLA_RECORD_ALIGN> transet_rec;


    ///
    /// Translation sequence database
    ///
    class transeq_db {
    public:
#pragma pack(push)
#pragma pack(2)
        ///
        /// Translation sequence data
        ///
        struct transeq {
        public:
            transeqid_t seq;            ///< Translation sequence ID
            unsigned __int16 rank;      ///< Translation sequence rank

        protected:
            unsigned __int16 name_to;   ///< Translation sequence name end in \c data
            unsigned __int16 sets_to;   ///< Translation sequence sets end in \c data
            wchar_t data[];             ///< Translation sequence name and sets

        private:
            inline transeq(_In_ const transeq &other);
            inline transeq& operator=(_In_ const transeq &other);

        public:
            ///
            /// Constructs the translation sequence
            ///
            /// \param[in] seq       Translation sequence ID
            /// \param[in] rank      Translation sequence rank
            /// \param[in] name      Translation sequence source
            /// \param[in] name_len  Number of UTF-16 characters in \p src
            /// \param[in] sets       Translation sequence destination
            /// \param[in] sets_len   Number of UTF-16 characters in \p sets
            ///
            inline transeq(
                _In_opt_                         transeqid_t       seq      = 0,
                _In_opt_                         unsigned __int16  rank     = 0,
                _In_opt_z_count_(name_len) const wchar_t          *name     = NULL,
                _In_opt_                         size_t            name_len = 0,
                _In_opt_count_  (sets_len) const transetid_t      *sets      = NULL,
                _In_opt_                         size_t            sets_len  = 0)
            {
                this->seq  = seq;
                this->rank = rank;
                this->name_to = static_cast<unsigned __int16>(name_len);
                if (name && name_len) memcpy(this->data, name, sizeof(wchar_t)*name_len);
                this->sets_to = static_cast<unsigned __int16>(this->name_to + sets_len);
                if (sets && sets_len) memcpy(this->data + this->name_to, sets, sizeof(transetid_t)*sets_len);
            }

            inline const wchar_t*         name    () const { return data;           };
            inline       wchar_t*         name    ()       { return data;           };
            inline const wchar_t*         name_end() const { return data + name_to; };
            inline       wchar_t*         name_end()       { return data + name_to; };
            inline       unsigned __int16 name_len() const { return name_to;        };

            inline const transetid_t*     sets    () const { return reinterpret_cast<const transetid_t*>(data + name_to); };
            inline       transetid_t*     sets    ()       { return reinterpret_cast<      transetid_t*>(data + name_to); };
            inline const transetid_t*     sets_end() const { return reinterpret_cast<const transetid_t*>(data + sets_to); };
            inline       transetid_t*     sets_end()       { return reinterpret_cast<      transetid_t*>(data + sets_to); };
            inline       unsigned __int16 sets_len() const { return sets_to - name_to; };
        };
#pragma pack(pop)

        ///
        /// Translation sequence index
        ///
        class indexTranSeq : public index<unsigned __int16, unsigned __int32, transeq>
        {
        public:
            ///
            /// Constructs the index
            ///
            /// \param[in] h  Reference to vector holding the data
            ///
            indexTranSeq(_In_ std::vector<unsigned __int16> &h) : index<unsigned __int16, unsigned __int32, transeq>(h) {}

            ///
            /// Compares two translation sequences by ID (for searching)
            ///
            /// \param[in] a  Pointer to first element
            /// \param[in] b  Pointer to second element
            ///
            /// \returns
            /// - <0 when a <  b
            /// - =0 when a == b
            /// - >0 when a >  b
            ///
            virtual int compare(_In_ const transeq &a, _In_ const transeq &b) const
            {
                     if (a.seq < b.seq) return -1;
                else if (a.seq > b.seq) return  1;

                return 0;
            }
        } idxTranSeq;   ///< Translation sequence index

        ///
        /// Rank index
        ///
        class indexRank : public index<unsigned __int16, unsigned __int32, transeq>
        {
        public:
            ///
            /// Constructs the index
            ///
            /// \param[in] h  Reference to vector holding the data
            ///
            indexRank(_In_ std::vector<unsigned __int16> &h) : index<unsigned __int16, unsigned __int32, transeq>(h) {}

            ///
            /// Compares two translation sets by rank (for searching)
            ///
            /// \param[in] a  Pointer to first element
            /// \param[in] b  Pointer to second element
            ///
            /// \returns
            /// - <0 when a <  b
            /// - =0 when a == b
            /// - >0 when a >  b
            ///
            virtual int compare(_In_ const transeq &a, _In_ const transeq &b) const
            {
                     if (a.rank < b.rank) return -1;
                else if (a.rank > b.rank) return +1;

                return 0;
            }

            ///
            /// Compares two translation sets by rank (for sorting)
            ///
            /// \param[in] a  Pointer to first element
            /// \param[in] b  Pointer to second element
            ///
            /// \returns
            /// - <0 when a <  b
            /// - =0 when a == b
            /// - >0 when a >  b
            ///
            virtual int compare_sort(_In_ const transeq &a, _In_ const transeq &b) const
            {
                     if (a.rank < b.rank) return -1;
                else if (a.rank > b.rank) return +1;

                unsigned __int16
                    a_name_len = a.name_len(),
                    b_name_len = b.name_len();
                int r = _wcsncoll(a.name(), b.name(), std::min<unsigned __int16>(a_name_len, b_name_len));
                if (r != 0) return r;
                     if (a_name_len < b_name_len) return -1;
                else if (a_name_len > b_name_len) return +1;

                return 0;
            }
        } idxRank;  ///< Rank index

        std::vector<unsigned __int16> data;     ///< Translation sequence data

    public:
        ///
        /// Constructs the database
        ///
        inline transeq_db() : idxTranSeq(data), idxRank(data) {}

        ///
        /// Clears the database
        ///
        inline void clear()
        {
            idxTranSeq.clear();
            idxRank   .clear();
            data      .clear();
        }
    };


    typedef stdex::idrec::record<transeq_db, recordid_t, recordsize_t, ZRCOLA_RECORD_ALIGN> transeq_rec;
};


const ZRCola::recordid_t ZRCola::translation_rec::id = *(ZRCola::recordid_t*)"TRN";
const ZRCola::recordid_t ZRCola::transet_rec    ::id = *(ZRCola::recordid_t*)"TSE";
const ZRCola::recordid_t ZRCola::transeq_rec    ::id = *(ZRCola::recordid_t*)"TSQ";


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
    stream << db.idxSrc;

    // Write inverse translation index.
    if (stream.fail()) return stream;
    stream << db.idxDst;

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
    stream >> db.idxSrc;
    if (!stream.good()) return stream;

    // Read inverse translation index.
    stream >> db.idxDst;
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
/// Writes translation set database to a stream
///
/// \param[in] stream  Output stream
/// \param[in] db      Translation set database
///
/// \returns The stream \p stream
///
inline std::ostream& operator <<(_In_ std::ostream& stream, _In_ const ZRCola::transet_db &db)
{
    // Write translation set index.
    if (stream.fail()) return stream;
    stream << db.idxTranSet;

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
/// Reads translation set database from a stream
///
/// \param[in ] stream  Input stream
/// \param[out] db      Translation set database
///
/// \returns The stream \p stream
///
inline std::istream& operator >>(_In_ std::istream& stream, _Out_ ZRCola::transet_db &db)
{
    // Read translation set index.
    stream >> db.idxTranSet;
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
/// Writes translation sequence database to a stream
///
/// \param[in] stream  Output stream
/// \param[in] db      Translation sequence database
///
/// \returns The stream \p stream
///
inline std::ostream& operator <<(_In_ std::ostream& stream, _In_ const ZRCola::transeq_db &db)
{
    // Write translation sequence index.
    if (stream.fail()) return stream;
    stream << db.idxTranSeq;

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
    unsigned __int32 count = (unsigned __int32)data_count;
    stream.write((const char*)&count, sizeof(count));

    // Write data.
    if (stream.fail()) return stream;
    stream.write((const char*)db.data.data(), sizeof(unsigned __int16)*static_cast<std::streamsize>(count));

    return stream;
}


///
/// Reads translation sequence database from a stream
///
/// \param[in ] stream  Input stream
/// \param[out] db      Translation sequence database
///
/// \returns The stream \p stream
///
inline std::istream& operator >>(_In_ std::istream& stream, _Out_ ZRCola::transeq_db &db)
{
    // Read translation sequence index.
    stream >> db.idxTranSeq;
    if (!stream.good()) return stream;

    // Read rank index.
    stream >> db.idxRank;
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
