/*
    SPDX-License-Identifier: GPL-3.0-or-later
    Copyright © 2015-2022 Amebis
*/

#include "pch.h"


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
