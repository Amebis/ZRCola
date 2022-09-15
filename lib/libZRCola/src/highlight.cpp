/*
    SPDX-License-Identifier: GPL-3.0-or-later
    Copyright © 2021-2022 Amebis
*/

#include "pch.h"

_Use_decl_annotations_
void ZRCola::highlight_db::Highlight(const char16_t* input, size_t inputMax, std::function<void (hlghtsetid_t set, size_t start, size_t end)> callback) const
{
    size_t start = 0;
    hlghtsetid_t set = ZRCOLA_HLGHTSETID_DEFAULT;

    for (size_t i = 0; i < inputMax;) {
        // Find the longest matching highlight at i-th character.
        size_t l_match = (size_t)-1;
        for (size_t l = 0, r = idxChr.size(), ii = i, j = 0; ii < inputMax && l < r; ii++, j++) {
            char16_t c = input[ii];
            while (l < r) {
                // Test the highlight in the middle of the search area.
                size_t m = (l + r) / 2;

                // Get the j-th character of the highlight.
                // All highlights that get short on characters are lexically ordered before.
                // Thus the j-th character is considered 0.
                char16_t s = idxChr[m].chr_at(j);

                // Do the bisection test.
                     if (c < s) r = m;
                else if (s < c) l = m + 1;
                else {
                    // Character found.

                    // Narrow the search area on the left to start at the first highlight in the run.
                    for (size_t r2 = m; l < r2;) {
                        size_t m2 = (l + r2) / 2;
                        if (c <= idxChr[m2].chr_at(j)) r2 = m2; else l = m2 + 1;
                    }

                    // Narrow the search area on the right to end at the first highlight not in the run.
                    for (size_t l2 = m + 1; l2 < r;) {
                        size_t m2 = (l2 + r) / 2;
                        if (idxChr[m2].chr_at(j) <= c) l2 = m2 + 1; else r = m2;
                    }

                    if (j + 1 == idxChr[l].chr_len()) {
                        // The first highlight of the run was a match (thus far). Save it.
                        l_match = l;
                    }

                    break;
                }
            }
        }

        if (l_match < idxChr.size()) {
            // The saved highlight was an exact match.
            const highlight &hghl = idxChr[l_match];
            if (set != hghl.set) {
                callback(set, start, i);
                start = i;
                set   = hghl.set;
            }
            i += hghl.chr_len();
        } else {
            // The match was not found.
            if (set != ZRCOLA_HLGHTSETID_DEFAULT) {
                callback(set, start, i);
                start = i;
                set   = ZRCOLA_HLGHTSETID_DEFAULT;
            }
            i++;
        }
    }

    callback(set, start, inputMax);
}
