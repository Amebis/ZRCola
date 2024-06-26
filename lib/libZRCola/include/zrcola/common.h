﻿/*
    SPDX-License-Identifier: GPL-3.0-or-later
    Copyright © 2015-2022 Amebis
*/

#pragma once

#ifdef _WIN32
#define _WINSOCKAPI_    // Prevent inclusion of winsock.h in windows.h.
#include <Windows.h>
#endif
#include <stdex/compat.hpp>
#include <stdex/mapping.hpp>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <wchar.h>
#include <istream>
#include <memory>
#include <ostream>
#include <utility>
#include <vector>


#pragma warning(push)
#pragma warning(disable: 4251)
#pragma warning(disable: 4512)


///
/// Data records alignment
///
#define ZRCOLA_RECORD_ALIGN 1

///
/// Database IDs
///
#define ZRCOLA_DB_ID        0x43525a // "ZRC"

#ifdef __GNUC__
#ifdef __i386__
#define __cdecl __attribute__((__cdecl__))
#else
#define __cdecl
#endif
#endif

namespace ZRCola {
    typedef uint32_t recordid_t;
    typedef uint32_t recordsize_t;


    ///
    /// ZRCola database character type
    ///
#ifdef _WIN32
    typedef wchar_t char_t;
#else
    typedef char16_t char_t;
#endif


    ///
    /// ZRCola database string type
    ///
#ifdef _WIN32
    typedef std::wstring string_t;
#else
    typedef std::u16string string_t;
#endif


#pragma pack(push)
#pragma pack(2)
    ///
    /// Key-value index pair for mappings
    ///
    template <class T>
    struct mappair_t
    {
        T idx_key;  ///< Index of key
        T idx_val;  ///< Index of value
    };
#pragma pack(pop)


#pragma pack(push)
#pragma pack(2)
    ///
    /// Language ID type
    /// Three letter abbreviation, zero terminated
    ///
    struct langid_t {
        char data[4];

        inline langid_t& operator=(_In_ const langid_t &src)
        {
            data[0] = src.data[0];
            data[1] = src.data[1];
            data[2] = src.data[2];
            data[3] = src.data[3];
            return *this;
        }

        inline langid_t& operator=(_In_z_ const char *src)
        {
            data[3] = (
            data[2] = (
            data[1] = (
            data[0] = src[0]    ) != 0 ?
                      src[1] : 0) != 0 ?
                      src[2] : 0) != 0 ?
                      src[3] : 0;
            return *this;
        }

        ///
        /// Blank language ID
        ///
        static const langid_t blank;
    };
#pragma pack(pop)


    ///
    /// Compares two language IDs
    ///
    /// \param[in] a  First language ID
    /// \param[in] b  Second language ID
    ///
    /// \returns
    /// - true when \p a == \p b
    /// - false otherwise
    ///
    inline bool operator==(const langid_t &a, const langid_t & b)
    {
        return
                                a.data[0] == b.data[0] &&
            (a.data[0] == 0 || (a.data[1] == b.data[1] &&
            (a.data[1] == 0 || (a.data[2] == b.data[2] &&
            (a.data[2] == 0 ||  a.data[3] == b.data[3])))));
    }


    ///
    /// Compares two language IDs
    ///
    /// \param[in] a  First language ID
    /// \param[in] b  Second language ID
    ///
    /// \returns
    /// - true when \p a != \p b
    /// - false otherwise
    ///
    inline bool operator!=(const langid_t &a, const langid_t & b)
    {
        return !operator==(a, b);
    }


    ///
    /// Compares two language IDs
    ///
    /// \param[in] a  First language ID
    /// \param[in] b  Second language ID
    ///
    /// \returns
    /// - true when \p a < \p b
    /// - false otherwise
    ///
    inline bool operator<(const langid_t& a, const langid_t& b)
    {
             if (a.data[0] < b.data[0]) return true;
        else if (a.data[0] > b.data[0]) return false;
        else if (a.data[1] < b.data[1]) return true;
        else if (a.data[1] > b.data[1]) return false;
        else if (a.data[2] < b.data[2]) return true;
        else if (a.data[2] > b.data[2]) return false;
        else if (a.data[3] < b.data[3]) return true;
        else                            return false;
    }


    ///
    /// Compares two language IDs
    ///
    /// \param[in] a  First language ID
    /// \param[in] b  Second language ID
    ///
    /// \returns
    /// - true when \p a > \p b
    /// - false otherwise
    ///
    inline bool operator>(const langid_t& a, const langid_t& b)
    {
        return operator<(b, a);
    }


    ///
    /// Compares two language IDs
    ///
    /// \param[in] a  First language ID
    /// \param[in] b  Second language ID
    ///
    /// \returns
    /// - true when \p a <= \p b
    /// - false otherwise
    ///
    inline bool operator<=(const langid_t &a, const langid_t & b)
    {
        return !operator>(a, b);
    }


    ///
    /// Compares two language IDs
    ///
    /// \param[in] a  First language ID
    /// \param[in] b  Second language ID
    ///
    /// \returns
    /// - true when \p a >= \p b
    /// - false otherwise
    ///
    inline bool operator>=(const langid_t &a, const langid_t & b)
    {
        return !operator<(a, b);
    }


#ifdef _WIN32
    ///
    /// Converts language from Windows to ZRCola notation.
    ///
    /// \param[in]     lang_win  Windows language ID
    /// \param[in,out] lang      ZRCola language ID
    ///
    void LangConvert(_In_ LANGID lang_win, _Inout_ langid_t &lang);
#endif


    ///
    /// Memory index
    ///
    template <class T_data, class T_idx = uint32_t, class T_el = T_data>
    class index : public std::vector<T_idx>
    {
        typedef std::vector<T_idx> base_t;

    public:
        typedef size_t size_type;

    protected:
        std::vector<T_data> &host;  ///< Reference to host data

    public:
        ///
        /// Constructs the index
        ///
        /// \param[in] h  Reference to vector holding the data
        ///
        index(_In_ std::vector<T_data> &h) : host(h) {}


        ///
        /// Returns data at given position according to the index
        ///
        /// \param[in] pos  Position
        ///
        /// \returns Data reference
        ///
        inline const T_el& at(size_type pos) const
        {
            return *reinterpret_cast<const T_el*>(&host[base_t::at(pos)]);
        }


        ///
        /// Returns data at given position according to the index
        ///
        /// \param[in] pos  Position
        ///
        /// \returns Data reference
        ///
        inline T_el& at(size_type pos)
        {
            return *reinterpret_cast<T_el*>(&host[base_t::at(pos)]);
        }


        ///
        /// Returns data at given position according to the index
        ///
        /// \param[in] pos  Position
        ///
        /// \returns Data reference
        ///
        inline const T_el& operator[](size_type pos) const
        {
            return *reinterpret_cast<const T_el*>(&host[base_t::operator[](pos)]);
        }


        ///
        /// Returns data at given position according to the index
        ///
        /// \param[in] pos  Position
        ///
        /// \returns Data reference
        ///
        inline T_el& operator[](size_type pos)
        {
            return *reinterpret_cast<T_el*>(&host[base_t::operator[](pos)]);
        }


        ///
        /// Sorts index
        ///
        inline void sort()
        {
            qsort_s(base_t::data(), base_t::size(), sizeof(T_idx), compare_s, this);
        }


        ///
        /// Compares two elements (for searching)
        ///
        /// \param[in] a  Pointer to first element
        /// \param[in] b  Pointer to second element
        ///
        /// \returns
        /// - <0 when a <  b
        /// - =0 when a == b
        /// - >0 when a >  b
        ///
        virtual int compare(_In_ const T_el &a, _In_ const T_el &b) const = 0;


        ///
        /// Compares two elements (for sorting)
        ///
        /// \param[in] a  Pointer to first element
        /// \param[in] b  Pointer to second element
        ///
        /// \returns
        /// - <0 when a <  b
        /// - =0 when a == b
        /// - >0 when a >  b
        ///
        virtual int compare_sort(_In_ const T_el &a, _In_ const T_el &b) const
        {
            // Revert to `compare()` by default.
            return compare(a, b);
        }


        ///
        /// Search for the element in the index
        /// The elements matching \p el are located on the interval [\p start, \p end) in the index.
        ///
        /// \param[in]  el     Element we are looking for (needle)
        /// \param[out] start  Index of the first matching element found
        /// \param[out] end    Index of the first non-matching element found
        ///
        /// \returns
        /// - \c true if found
        /// - \c false otherwise
        ///
        bool find(_In_ const T_el &el, _Out_ size_type &start, _Out_ size_type &end) const
        {
            // Start with the full search area.
            for (start = 0, end = base_t::size(); start < end; ) {
                auto m = (start + end) / 2;
                int r = compare(el, at(m));
                     if (r < 0) end   = m;
                else if (r > 0) start = m + 1;
                else {
                    // Narrow the search area on the left to start at the first element in the run.
                    for (auto end2 = m; start < end2;) {
                        auto m2 = (start + end2) / 2;
                        if (compare(el, at(m2)) <= 0) end2 = m2; else start = m2 + 1;
                    }

                    // Narrow the search area on the right to end at the first element not in the run.
                    for (auto start2 = m + 1; start2 < end;) {
                        auto m2 = (start2 + end) / 2;
                        if (0 <= compare(el, at(m2))) start2 = m2 + 1; else end = m2;
                    }

                    return true;
                }
            }

            return false;
        }

        ///
        /// Search for the first element in the index
        ///
        /// \param[in]  el     Element we are looking for (needle)
        /// \param[out] start  Index of the first matching element found
        ///
        /// \returns
        /// - \c true if found
        /// - \c false otherwise
        ///
        bool find(_In_ const T_el &el, _Out_ size_type &start) const
        {
            // Start with the full search area.
            size_t end;
            for (start = 0, end = base_t::size(); start < end; ) {
                auto m = (start + end) / 2;
                int r = compare(el, at(m));
                     if (r < 0) end   = m;
                else if (r > 0) start = m + 1;
                else {
                    // Narrow the search area on the left to start at the first element in the run.
                    for (auto end2 = m; start < end2;) {
                        m = (start + end2) / 2;
                        if (compare(el, at(m)) <= 0) end2 = m; else start = m + 1;
                    }

                    return true;
                }
            }

            return false;
        }


        ///
        /// Writes index to a stream
        ///
        /// \param[in] stream  Output stream
        /// \param[in] idx     Index
        ///
        /// \returns The stream \p stream
        ///
        friend std::ostream& operator <<(_In_ std::ostream& stream, _In_ const index& idx)
        {
            // Write index count.
            auto idx_count = idx.size();
#if defined(_WIN64) || defined(__x86_64__) || defined(__ppc64__)
            // 4G check
            if (idx_count > 0xffffffff) {
                stream.setstate(std::ios_base::failbit);
                return stream;
            }
#endif
            if (stream.fail()) return stream;
            uint32_t count = (uint32_t)idx_count;
            stream.write((const char*)&count, sizeof(count));

            // Write index data.
            if (stream.fail()) return stream;
            stream.write((const char*)idx.data(), sizeof(T_idx) * static_cast<std::streamsize>(count));

            return stream;
        }


        ///
        /// Reads index from a stream
        ///
        /// \param[in]  stream  Input stream
        /// \param[out] idx     Index
        ///
        /// \returns The stream \p stream
        ///
        friend std::istream& operator >>(_In_ std::istream& stream, _Out_ index& idx)
        {
            uint32_t count;

            // Read index count.
            stream.read((char*)&count, sizeof(count));
            if (!stream.good()) {
                idx.clear();
                return stream;
            }

            if (count) {
                // Read index data.
                idx.resize(count);
                stream.read((char*)idx.data(), sizeof(T_idx) * static_cast<std::streamsize>(count));
            }
            else
                idx.clear();

            return stream;
        }

    private:
        static int __cdecl compare_s(void *p, const void *a, const void *b)
        {
            const index<T_data, T_idx, T_el> *_this = reinterpret_cast<const index<T_data, T_idx, T_el>*>(p);
            const T_data *data = _this->host.data();
            return _this->compare_sort(
                *reinterpret_cast<const T_el*>(data + *reinterpret_cast<const T_idx*>(a)),
                *reinterpret_cast<const T_el*>(data + *reinterpret_cast<const T_idx*>(b)));
        }
    };


    ///
    /// Memory text index
    ///
    template <class T_key, class T_val, class T_idx = uint32_t>
    class textindex : public std::vector< mappair_t<T_idx> >
    {
        typedef std::vector< mappair_t<T_idx> > base_t;

    public:
        typedef size_t size_type;
        std::vector<T_key> keys;    ///< Key data
        std::vector<T_val> values;  ///< Index values

    public:
        ///
        /// Constructs the index
        ///
        textindex() {}


        ///
        /// Clears the index
        ///
        inline void clear()
        {
            std::vector< mappair_t<T_idx> >::clear();
            keys  .clear();
            values.clear();
        }


        ///
        /// Finds data for given key
        ///
        /// \param[in ] key      Pointer to key
        /// \param[in ] key_len  Count of \p key elements
        /// \param[out] val      Pointer to receive pointer to key's values
        /// \param[out] val_len  Pointer to receive count of \p val elements
        ///
        /// \returns
        /// - \c true if found
        /// - \c false otherwise
        ///
        _Success_(return) bool find(_In_count_(key_len) const T_key *key, _In_ size_t key_len, _Out_ const T_val **val, _Out_ size_t *val_len) const
        {
            for (size_type start = 0, end = base_t::size(); start < end; ) {
                auto m = (start + end) / 2;
                int r = compare(key, key_len, m);
                     if (r < 0) end   = m;
                else if (r > 0) start = m + 1;
                else {
                    // Get values at position m.
                    start    =                       base_t::at(m    ).idx_val;
                    *val_len = (m < base_t::size() ? base_t::at(m + 1).idx_val : values.size()) - start;
                    *val     = &values.at(start);
                    return true;
                }
            }

            return false;
        }


        ///
        /// Writes text index to a stream
        ///
        /// \param[in] stream  Output stream
        /// \param[in] idx     Text index
        ///
        /// \returns The stream \p stream
        ///
        friend std::ostream& operator <<(_In_ std::ostream& stream, _In_ const textindex& idx)
        {
            uint32_t count;

            // Write index count.
            auto idx_count = idx.size();
#if defined(_WIN64) || defined(__x86_64__) || defined(__ppc64__)
            // 4G check
            if (idx_count > 0xffffffff) {
                stream.setstate(std::ios_base::failbit);
                return stream;
            }
#endif
            if (stream.fail()) return stream;
            count = (uint32_t)idx_count;
            stream.write((const char*)&count, sizeof(count));

            // Write index data.
            if (stream.fail()) return stream;
            auto idx_data = idx.data();
            stream.write((const char*)idx_data, sizeof(*idx_data) * static_cast<std::streamsize>(count));

            // Write key count.
            auto key_count = idx.keys.size();
#if defined(_WIN64) || defined(__x86_64__) || defined(__ppc64__)
            // 4G check
            if (idx_count > 0xffffffff) {
                stream.setstate(std::ios_base::failbit);
                return stream;
            }
#endif
            if (stream.fail()) return stream;
            count = (uint32_t)key_count;
            stream.write((const char*)&count, sizeof(count));

            // Write key data.
            if (stream.fail()) return stream;
            auto idx_keys_data = idx.keys.data();
            stream.write((const char*)idx_keys_data, sizeof(*idx_keys_data) * static_cast<std::streamsize>(count));

            // Write value count.
            auto value_count = idx.values.size();
#if defined(_WIN64) || defined(__x86_64__) || defined(__ppc64__)
            // 4G check
            if (idx_count > 0xffffffff) {
                stream.setstate(std::ios_base::failbit);
                return stream;
            }
#endif
            if (stream.fail()) return stream;
            count = (uint32_t)value_count;
            stream.write((const char*)&count, sizeof(count));

            // Write value data.
            if (stream.fail()) return stream;
            auto idx_values_data = idx.values.data();
            stream.write((const char*)idx_values_data, sizeof(*idx_values_data) * static_cast<std::streamsize>(count));

            return stream;
        }


        ///
        /// Reads text index from a stream
        ///
        /// \param[in]  stream  Input stream
        /// \param[out] idx     Text index
        ///
        /// \returns The stream \p stream
        ///
        friend std::istream& operator >>(_In_ std::istream& stream, _Out_ textindex& idx)
        {
            uint32_t count;

            // Read text index count.
            stream.read((char*)&count, sizeof(count));
            if (!stream.good()) {
                idx.clear();
                return stream;
            }

            if (count) {
                // Read text index.
                idx.resize(count);
                auto p = idx.data();
                stream.read((char*)p, sizeof(*p) * static_cast<std::streamsize>(count));
                if (!stream.good()) return stream;
            }
            else
                idx.clear();

            // Read keys count.
            stream.read((char*)&count, sizeof(count));
            if (!stream.good()) return stream;

            if (count) {
                // Read keys.
                idx.keys.resize(count);
                auto p = idx.keys.data();
                stream.read((char*)p, sizeof(*p) * static_cast<std::streamsize>(count));
                if (!stream.good()) return stream;
            }
            else
                idx.keys.clear();

            // Read value count.
            stream.read((char*)&count, sizeof(count));
            if (!stream.good()) return stream;

            if (count) {
                // Read values.
                idx.values.resize(count);
                auto p = idx.values.data();
                stream.read((char*)p, sizeof(*p) * static_cast<std::streamsize>(count));
            }
            else
                idx.values.clear();

            return stream;
        }

    protected:
        inline int compare(_In_count_(key_len) const T_key *key, _In_ size_t key_len, size_type pos) const
        {
            // Get key at position pos.
            auto pos_next = pos + 1;
            size_t
                start    =                              base_t::at(pos     ).idx_key,
                key2_len = (pos_next < base_t::size() ? base_t::at(pos_next).idx_key : keys.size()) - start;
            auto key2 = &keys.at(start);

            // Compare keys.
            int r = memcmp(key, key2, sizeof(T_key)*std::min<size_t>(key_len, key2_len));
                 if (r       != 0       ) return  r;
            else if (key_len <  key2_len) return -1;
            else if (key_len >  key2_len) return  1;

            return 0;
        }
    };

    ///
    /// Source-destination index transformation mapping
    ///
    using mapping = stdex::mapping<size_t>;

    ///
    /// A vector for destination-source index transformation mapping
    ///
    using mapping_vector = std::vector<mapping>;

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
    int CompareString(_In_ const char_t* str_a, _In_ size_t count_a, _In_ const char_t* str_b, _In_ size_t count_b);

    ///
    /// Generates and returns Unicode representation of the string using hexadecimal codes.
    ///
    /// \param[in] str    Unicode string
    /// \param[in] count  Number of characters in string \p str
    /// \param[in] sep    Separator
    ///
    std::string GetUnicodeDumpA(_In_z_count_(count) const char_t* str, _In_ size_t count, _In_z_ const char* sep = "+");

    ///
    /// Generates and returns Unicode representation of the string using hexadecimal codes.
    ///
    /// \param[in] str    Unicode string
    /// \param[in] count  Number of characters in string \p str
    /// \param[in] sep    Separator
    ///
    std::wstring GetUnicodeDumpW(_In_z_count_(count) const char_t* str, _In_ size_t count, _In_z_ const wchar_t* sep = L"+");

#ifdef _UNICODE
#define GetUnicodeDump GetUnicodeDumpW
#else
#define GetUnicodeDump GetUnicodeDumpA
#endif
};

#pragma warning(pop)
