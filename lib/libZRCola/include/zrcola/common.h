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


///
/// Data records alignment
///
#define ZRCOLA_RECORD_ALIGN 1

///
/// Database IDs
///
#define ZRCOLA_DB_ID    (*(ZRCola::recordid_t*)"ZRC")


namespace ZRCola {
    typedef unsigned __int32 recordid_t;
    typedef unsigned __int32 recordsize_t;


    ///
    /// Composed-decomposed index transformation mapping
    ///
    class ZRCOLA_NOVTABLE ZRCOLA_API mapping {
    public:
        size_t cmp;     ///< Character index in composed string
        size_t decmp;   ///< Character index in decomposed string

        inline mapping() {};
        inline mapping(_In_ size_t c, _In_ size_t d) : cmp(c), decmp(d) {}
    };


    ///
    /// A vector for composed-decomposed index transformation mapping
    ///
    class ZRCOLA_API mapping_vector : public std::vector<mapping> {
    public:
        ///
        /// Transforms character index of decomposed to composed string
        ///
        /// \param[in] decmp  Character index in decomposed string
        ///
        /// \returns  Character index in composed string
        ///
        size_t to_composed(_In_ size_t decmp) const;

        ///
        /// Transforms destination index to source index
        ///
        /// \param[in] cmp  Character index in composed string
        ///
        /// \returns  Character index in decomposed string
        ///
        size_t to_decomposed(_In_ size_t cmp) const;
    };
};

#pragma warning(pop)
