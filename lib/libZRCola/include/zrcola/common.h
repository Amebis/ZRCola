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

#include <istream>
#include <ostream>
#include <utility>
#include <vector>
#ifdef _WIN32
#include <Windows.h>
#endif


///
/// Public function calling convention
///
#ifdef LIBZRCOLA
#define ZRCOLA_API      __declspec(dllexport)
#else
#define ZRCOLA_API      __declspec(dllimport)
#endif
#define ZRCOLA_NOVTABLE __declspec(novtable)
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
#define ZRCOLA_DB_ID        (*(ZRCola::recordid_t*)"ZRC")


namespace ZRCola {
    typedef unsigned __int32 recordid_t;
    typedef unsigned __int32 recordsize_t;


    ///
    /// Key-value index pair for mappings
    ///
#pragma pack(push)
#pragma pack(2)
    template <class T>
    struct mappair_t
    {
        T idx_key;  ///< Index of key
        T idx_val;  ///< Index of value
    };
#pragma pack(pop)


    ///
    /// Language ID type
    /// Three letter abbreviation, zero terminated
    ///
    struct langid_t {
        char data[4];

        inline langid_t& operator=(const langid_t &src)
        {
            data[0] = src.data[0];
            data[1] = src.data[1];
            data[2] = src.data[2];
            data[3] = src.data[3];
            return *this;
        }

        inline langid_t& operator=(const char *src)
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
    };


    ///
    /// Blank language ID
    ///
    const langid_t langid_t_blank = {};


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
    void ZRCOLA_API LangConvert(_In_ LANGID lang_win, _Inout_ langid_t &lang);
#endif


    ///
    /// Memory index
    ///
    template <class T, class T_idx = unsigned __int32, class T_data = T>
    class index : public std::vector<T_idx>
    {
    protected:
        std::vector<T> &host;  ///< Reference to host data

    public:
        ///
        /// Constructs the index
        ///
        /// \param[in] h  Reference to vector holding the data
        ///
        index(_In_ std::vector<T> &h) : host(h) {}


        ///
        /// Returns data at given position according to the index
        ///
        /// \param[in] pos  Position
        ///
        /// \returns Data reference
        ///
        inline const T_data& at(size_type pos) const
        {
            return (const T_data&)host.at(std::vector<T_idx>::at(pos));
        }


        ///
        /// Returns data at given position according to the index
        ///
        /// \param[in] pos  Position
        ///
        /// \returns Data reference
        ///
        inline T_data& at(size_type pos)
        {
            return (T_data&)host.at(std::vector<T_idx>::at(pos));
        }


        ///
        /// Returns data at given position according to the index
        ///
        /// \param[in] pos  Position
        ///
        /// \returns Data reference
        ///
        inline const T_data& operator[](size_type pos) const
        {
            return (const T_data&)host[std::vector<T_idx>::at(pos)];
        }


        ///
        /// Returns data at given position according to the index
        ///
        /// \param[in] pos  Position
        ///
        /// \returns Data reference
        ///
        inline T_data& operator[](size_type pos)
        {
            return (T_data&)host[std::vector<T_idx>::at(pos)];
        }


        ///
        /// Sorts index
        ///
        inline void sort()
        {
            qsort_s(data(), size(), sizeof(T_idx), compare_s, this);
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
        virtual int compare(_In_ const T_data &a, _In_ const T_data &b) const = 0;


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
        virtual int compare_sort(_In_ const T_data &a, _In_ const T_data &b) const
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
        bool find(_In_ const T_data &el, _Out_ size_type &start, _Out_ size_type &end) const
        {
            // Start with the full search area.
            for (start = 0, end = size(); start < end; ) {
                size_type m = (start + end) / 2;
                int r = compare(el, at(m));
                     if (r < 0) end   = m;
                else if (r > 0) start = m + 1;
                else {
                    // Narrow the search area on the left to start at the first element in the run.
                    for (size_type end2 = m; start < end2;) {
                        size_type m = (start + end2) / 2;
                        int r = compare(el, at(m));
                        if (r <= 0) end2 = m; else start = m + 1;
                    }

                    // Narrow the search area on the right to end at the first element not in the run.
                    for (size_type start2 = m + 1; start2 < end;) {
                        size_type m = (start2 + end) / 2;
                        int r = compare(el, at(m));
                        if (0 <= r) start2 = m + 1; else end = m;
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
        bool find(_In_ const T_data &el, _Out_ size_type &start) const
        {
            // Start with the full search area.
            size_t end;
            for (start = 0, end = size(); start < end; ) {
                size_type m = (start + end) / 2;
                int r = compare(el, at(m));
                     if (r < 0) end   = m;
                else if (r > 0) start = m + 1;
                else {
                    // Narrow the search area on the left to start at the first element in the run.
                    for (size_type end2 = m; start < end2;) {
                        size_type m = (start + end2) / 2;
                        int r = compare(el, at(m));
                        if (r <= 0) end2 = m; else start = m + 1;
                    }

                    return true;
                }
            }

            return false;
        }

    private:
        static int __cdecl compare_s(void *p, const void *a, const void *b)
        {
            const index<T, T_idx, T_data> *t = (const index<T, T_idx, T_data>*)p;
            return t->compare_sort((const T_data&)t->host[*(const T_idx*)a], (const T_data&)t->host[*(const T_idx*)b]);
        }
    };


    ///
    /// Memory text index
    ///
    template <class T_key, class T_val, class T_idx = unsigned __int32>
    class textindex : public std::vector< mappair_t<T_idx> >
    {
    public:
        typedef std::vector< mappair_t<T_idx> > base_t;
        std::vector<T_key> keys;    ///< Key data
        std::vector<T_val> values;  ///< Index values

    public:
        ///
        /// Constructs the index
        ///
        textindex() {}


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
        bool find(_In_count_(key_len) const T_key *key, _In_ size_t key_len, _Out_ const T_val **val, _Out_ size_t *val_len) const
        {
            for (size_type start = 0, end = size(); start < end; ) {
                size_type m = (start + end) / 2;
                int r = compare(key, key_len, m);
                     if (r < 0) end   = m;
                else if (r > 0) start = m + 1;
                else {
                    // Get values at position m.
                    size_t start =               base_t::at(m    ).idx_val;
                    *val_len     = (m < size() ? base_t::at(m + 1).idx_val : values.size()) - start;
                    *val         = &values.at(start);
                    return true;
                }
            }

            return false;
        }

    protected:
        inline int compare(_In_count_(key_len) const T_key *key, _In_ size_t key_len, size_type pos) const
        {
            // Get key at position pos.
            size_type pos_next = pos + 1;
            size_t
                start    =                      base_t::at(pos     ).idx_key,
                key2_len = (pos_next < size() ? base_t::at(pos_next).idx_key : keys.size()) - start;
            std::vector<T_key>::const_pointer key2 = &keys.at(start);

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
    class ZRCOLA_NOVTABLE ZRCOLA_API mapping {
    public:
        size_t src;     ///< Character index in source string
        size_t dst;     ///< Character index in destination string

        inline mapping() {};
        inline mapping(_In_ size_t s, _In_ size_t d) : src(s), dst(d) {}

        ///
        /// Reverses source and destination indexes
        ///
        inline void invert() { size_t tmp = src; src = dst; dst = tmp; }
    };


    ///
    /// A vector for composed-decomposed index transformation mapping
    ///
    class ZRCOLA_API mapping_vector : public std::vector<mapping> {
    public:
        ///
        /// Transforms character index of destination to source
        ///
        /// \param[in] decmp  Character index in destination string
        ///
        /// \returns  Character index in source string
        ///
        size_t to_src(_In_ size_t dst) const;

        ///
        /// Transforms source index to destination index
        ///
        /// \param[in] cmp  Character index in source string
        ///
        /// \returns  Character index in destination string
        ///
        size_t to_dst(_In_ size_t src) const;

        ///
        /// Reverses source and destination indexes
        ///
        inline void invert()
        {
            for (iterator i = begin(), iEnd = end(); i != iEnd; ++i)
                i->invert();
        }
    };
};


///
/// Writes index to a stream
///
/// \param[in] stream  Output stream
/// \param[in] idx     Index
///
/// \returns The stream \p stream
///
template <class T, class T_idx, class T_data>
inline std::ostream& operator <<(_In_ std::ostream& stream, _In_ const ZRCola::index<T, T_idx, T_data> &idx)
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
    unsigned __int32 count = (unsigned __int32)idx_count;
    stream.write((const char*)&count, sizeof(count));

    // Write index data.
    if (stream.fail()) return stream;
    stream.write((const char*)idx.data(), sizeof(T_idx)*count);

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
template <class T, class T_idx, class T_data>
inline std::istream& operator >>(_In_ std::istream& stream, _Out_ ZRCola::index<T, T_idx, T_data> &idx)
{
    unsigned __int32 count;

    // Read index count.
    stream.read((char*)&count, sizeof(count));
    if (!stream.good()) return stream;

    if (count) {
        // Read index data.
        idx.resize(count);
        stream.read((char*)idx.data(), sizeof(T_idx)*count);
    } else
        idx.clear();

    return stream;
}


///
/// Writes text index to a stream
///
/// \param[in] stream  Output stream
/// \param[in] idx     Text index
///
/// \returns The stream \p stream
///
template <class T_key, class T_val, class T_idx>
inline std::ostream& operator <<(_In_ std::ostream& stream, _In_ const ZRCola::textindex<T_key, T_val, T_idx> &idx)
{
    unsigned __int32 count;

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
    count = (unsigned __int32)idx_count;
    stream.write((const char*)&count, sizeof(count));

    // Write index data.
    if (stream.fail()) return stream;
    stream.write((const char*)idx.data(), sizeof(ZRCola::textindex<T_key, T_val, T_idx>::value_type)*count);

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
    count = (unsigned __int32)key_count;
    stream.write((const char*)&count, sizeof(count));

    // Write key data.
    if (stream.fail()) return stream;
    stream.write((const char*)idx.keys.data(), sizeof(std::vector<T_key>::value_type)*count);

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
    count = (unsigned __int32)value_count;
    stream.write((const char*)&count, sizeof(count));

    // Write value data.
    if (stream.fail()) return stream;
    stream.write((const char*)idx.values.data(), sizeof(std::vector<T_val>::value_type)*count);

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
template <class T_key, class T_val, class T_idx>
inline std::istream& operator >>(_In_ std::istream& stream, _Out_ ZRCola::textindex<T_key, T_val, T_idx> &idx)
{
    unsigned __int32 count;

    // Read text index count.
    stream.read((char*)&count, sizeof(count));
    if (!stream.good()) return stream;

    if (count) {
        // Read text index.
        idx.resize(count);
        stream.read((char*)idx.data(), sizeof(ZRCola::textindex<T_key, T_val, T_idx>::value_type)*count);
        if (!stream.good()) return stream;
    } else
        idx.clear();

    // Read keys count.
    stream.read((char*)&count, sizeof(count));
    if (!stream.good()) return stream;

    if (count) {
        // Read keys.
        idx.keys.resize(count);
        stream.read((char*)idx.keys.data(), sizeof(std::vector<T_key>::value_type)*count);
        if (!stream.good()) return stream;
    } else
        idx.keys.clear();

    // Read value count.
    stream.read((char*)&count, sizeof(count));
    if (!stream.good()) return stream;

    if (count) {
        // Read values.
        idx.values.resize(count);
        stream.read((char*)idx.values.data(), sizeof(std::vector<T_val>::value_type)*count);
    } else
        idx.values.clear();

    return stream;
}

#pragma warning(pop)
