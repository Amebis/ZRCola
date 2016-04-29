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

#include "stdafx.h"


size_t ZRCola::mapping_vector::to_src(_In_ size_t dst) const
{
    if (empty()) {
        // One-to-one mapping.
        return dst;
    }

    for (size_type l = 0, r = size();;) {
        if (l < r) {
            size_type m = (l + r) / 2;
            const mapping &el = (*this)[m];

                 if (   dst < el.dst) r = m;
            else if (el.dst <    dst) l = m + 1;
            else {
                // An exact match found.
                return el.src;
            }
        } else if (l) {
            // We found a map interval.
            const mapping &el = (*this)[l - 1];
            return el.src + (dst - el.dst);
        } else {
            // The destination character index is left of the first transformation.
            const mapping &el = (*this)[0];
            return std::min<size_t>(dst, el.src);
        }
    }
}


size_t ZRCola::mapping_vector::to_dst(_In_ size_t src) const
{
    if (empty()) {
        // One-to-one mapping.
        return src;
    }

    for (size_type l = 0, r = size();;) {
        if (l < r) {
            size_type m = (l + r) / 2;
            const mapping &el = (*this)[m];

                 if (   src < el.src) r = m;
            else if (el.src <    src) l = m + 1;
            else {
                // An exact match found.
                return el.dst;
            }
        } else if (l) {
            // We found a map interval.
            const mapping &el = (*this)[l - 1];
            return el.dst + (src - el.src);
        } else {
            // The source character index is left of the first transformation.
            const mapping &el = (*this)[0];
            return std::min<size_t>(src, el.dst);
        }
    }
}
