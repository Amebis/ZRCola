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

///
/// Unknown language ID
///
#define ZRCOLA_LANG_VOID    "   "


namespace ZRCola {
    typedef unsigned __int32 recordid_t;
    typedef unsigned __int32 recordsize_t;


    ///
    /// Language ID type
    /// Three letter abbreviation, zero terminated
    ///
    typedef char langid_t[4];


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
                     if (r < 0) end = m;
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

    private:
        static int __cdecl compare_s(void *p, const void *a, const void *b)
        {
            const index<T, T_idx, T_data> *t = (const index<T, T_idx, T_data>*)p;
            return t->compare_sort((const T_data&)t->host[*(const T_idx*)a], (const T_data&)t->host[*(const T_idx*)b]);
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

#pragma warning(pop)
