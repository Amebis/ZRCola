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

#ifdef LIBZRCOLA
#define ZRCOLA_API  __declspec(dllexport)
#else
#define ZRCOLA_API  __declspec(dllimport)
#endif

namespace ZRCola {
    ///
    /// Source-destination index mapping
    ///
    class mapping {
    public:
        size_t src; ///< Source index
        size_t dst; ///< Destination index

        inline mapping() {};
        inline mapping(size_t s, size_t d) : src(s), dst(d) {}
    };
};
