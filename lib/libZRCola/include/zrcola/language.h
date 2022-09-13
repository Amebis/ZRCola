/*
    SPDX-License-Identifier: GPL-3.0-or-later
    Copyright © 2015-2022 Amebis
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
    class langchar_db {
    public:
#pragma pack(push)
#pragma pack(2)
        ///
        /// Character data
        ///
        struct langchar {
        public:
            langid_t lang;      ///< Language ID

        protected:
            uint16_t chr_to;    ///< Character end in \c data
            wchar_t data[];     ///< Character

        private:
            inline langchar(_In_ const langchar &other);
            inline langchar& operator=(_In_ const langchar &other);

        public:
            ///
            /// Constructs the language character
            ///
            /// \param[in] lang     Character language
            /// \param[in] chr      Character
            /// \param[in] chr_len  Number of UTF-16 characters in \p chr
            ///
            inline langchar(
                _In_opt_                        langid_t  lang    = langid_t::blank,
                _In_opt_z_count_(chr_len) const wchar_t  *chr     = NULL,
                _In_opt_                        size_t    chr_len = 0)
            {
                this->lang = lang;
                this->chr_to = static_cast<uint16_t>(chr_len);
                if (chr && chr_len) memcpy(this->data, chr, sizeof(wchar_t)*chr_len);
            }

            inline const wchar_t* chr    () const { return data;          };
            inline       wchar_t* chr    ()       { return data;          };
            inline const wchar_t* chr_end() const { return data + chr_to; };
            inline       wchar_t* chr_end()       { return data + chr_to; };
            inline       uint16_t chr_len() const { return chr_to;        };
        };
#pragma pack(pop)

        ///
        /// Character index
        ///
        class indexChr : public index<uint16_t, uint32_t, langchar>
        {
        public:
            ///
            /// Constructs the index
            ///
            /// \param[in] h  Reference to vector holding the data
            ///
            indexChr(_In_ std::vector<uint16_t> &h) : index<uint16_t, uint32_t, langchar>(h) {}

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
                int r = ZRCola::CompareString(a.chr(), a.chr_len(), b.chr(), b.chr_len());
                if (r != 0) return r;

                     if (a.lang < b.lang) return -1;
                else if (a.lang > b.lang) return  1;

                return 0;
            }
        } idxChr;      ///< Character index


#ifdef ZRCOLA_LANGCHAR_LANG_IDX
        ///
        /// Language Index
        ///
        class indexLang : public index<uint16_t, uint32_t, langchar>
        {
        public:
            ///
            /// Constructs the index
            ///
            /// \param[in] h  Reference to vector holding the data
            ///
            indexLang(_In_ std::vector<uint16_t> &h) : index<uint16_t, uint32_t, langchar>(h) {}

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
                     if (a.lang < b.lang) return -1;
                else if (a.lang > b.lang) return  1;

                int r = ZRCola::CompareString(a.chr, a.chr_len(), b.chr(), b.chr_len());
                if (r != 0) return r;

                return 0;
            }
        } idxLang;  ///< Language index
#endif

        std::vector<uint16_t> data; ///< Character data

    public:
        ///
        /// Constructs the database
        ///
        inline langchar_db() : idxChr(data)
#ifdef ZRCOLA_LANGCHAR_LANG_IDX
            , idxLang(data)
#endif
        {}

        ///
        /// Clears the database
        ///
        inline void clear()
        {
            idxChr .clear();
#ifdef ZRCOLA_LANGCHAR_LANG_IDX
            idxLang.clear();
#endif
            data   .clear();
        }

        ///
        /// Tests presence of character in the given language
        ///
        /// \param[in]  chr      Pointer to character
        /// \param[in]  chr_end  Pointer to character end
        /// \param[in]  lang     Language
        ///
        /// \returns
        /// - \c true when character is used in language
        /// - \c false otherwise
        bool IsLocalCharacter(_In_ const wchar_t *chr, _In_ const wchar_t *chr_end, _In_ langid_t lang) const;
    };


    typedef stdex::idrec::record<langchar_db, recordid_t, 0x432d4c /*"L-C"*/, recordsize_t, ZRCOLA_RECORD_ALIGN> langchar_rec;


    ///
    /// Language database
    ///
    class language_db {
    public:
#pragma pack(push)
#pragma pack(2)
        ///
        /// Language data
        ///
        struct language {
        public:
            langid_t lang;      ///< Language ID

        protected:
            uint16_t name_to;   ///< Language name end in \c data
            wchar_t data[];     ///< Language name

        private:
            inline language(_In_ const language &other);
            inline language& operator=(_In_ const language &other);

        public:
            ///
            /// Constructs the language
            ///
            /// \param[in] lang      Language ID
            /// \param[in] name      Language name
            /// \param[in] name_len  Number of UTF-16 characters in \p name
            ///
            inline language(
                _In_opt_                         langid_t  lang     = langid_t::blank,
                _In_opt_z_count_(name_len) const wchar_t  *name     = NULL,
                _In_opt_                         size_t    name_len = 0)
            {
                this->lang = lang;
                this->name_to = static_cast<uint16_t>(name_len);
                if (name && name_len) memcpy(this->data, name, sizeof(wchar_t)*name_len);
            }

            inline const wchar_t* name    () const { return data;           };
            inline       wchar_t* name    ()       { return data;           };
            inline const wchar_t* name_end() const { return data + name_to; };
            inline       wchar_t* name_end()       { return data + name_to; };
            inline       uint16_t name_len() const { return name_to;        };
        };
#pragma pack(pop)

        ///
        /// Language index
        ///
        class indexLang : public index<uint16_t, uint32_t, language>
        {
        public:
            ///
            /// Constructs the index
            ///
            /// \param[in] h  Reference to vector holding the data
            ///
            indexLang(_In_ std::vector<uint16_t> &h) : index<uint16_t, uint32_t, language>(h) {}

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
                     if (a.lang < b.lang) return -1;
                else if (a.lang > b.lang) return  1;

                return 0;
            }
        } idxLang;  ///< Language index

        std::vector<uint16_t> data; ///< Language data

    public:
        ///
        /// Constructs the database
        ///
        inline language_db() : idxLang(data) {}

        ///
        /// Clears the database
        ///
        inline void clear()
        {
            idxLang.clear();
            data   .clear();
        }
    };


    typedef stdex::idrec::record<language_db, recordid_t, 0x474e4c /*"LNG"*/, recordsize_t, ZRCOLA_RECORD_ALIGN> language_rec;
};


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
    stream << db.idxLang;
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
    uint32_t count = (uint32_t)data_count;
    stream.write((const char*)&count, sizeof(count));

    // Write data.
    if (stream.fail()) return stream;
    stream.write((const char*)db.data.data(), sizeof(uint16_t)*static_cast<std::streamsize>(count));

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
    stream >> db.idxLang;
    if (!stream.good()) return stream;
#endif

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
    stream << db.idxLang;

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
    stream >> db.idxLang;
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
