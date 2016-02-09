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


size_t ZRCola::mapping_vector::to_composed(_In_ size_t decmp) const
{
    for (size_type l = 0, r = size();;) {
        if (l < r) {
            size_type m = (l + r) / 2;
            const mapping &el = (*this)[m];

                 if (decmp < el.decmp) r = m;
            else if (el.decmp < decmp) l = m + 1;
            else {
                // An exact match found.
                return el.cmp;
            }
        } else if (l) {
            // We found a map interval.
            const mapping &el = (*this)[l - 1];
            return el.cmp + (decmp - el.decmp);
        } else {
            // The decomposed character index is far left.
            return decmp;
        }
    }
}


size_t ZRCola::mapping_vector::to_decomposed(_In_ size_t cmp) const
{
    for (size_type l = 0, r = size();;) {
        if (l < r) {
            size_type m = (l + r) / 2;
            const mapping &el = (*this)[m];

                 if (cmp < el.cmp) r = m;
            else if (el.cmp < cmp) l = m + 1;
            else {
                // An exact match found.
                return el.decmp;
            }
        } else if (l) {
            // We found a map interval.
            const mapping &el = (*this)[l - 1];
            return el.decmp + (cmp - el.cmp);
        } else {
            // The composed character index is far left.
            return cmp;
        }
    }
}
