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


static inline void Compose(
    _In_count_(compositionsCount) const ZRCola::composition* compositions,
    _In_                          const size_t compositionsCount,
    _In_z_count_(inputMax)        const wchar_t* input,
    _In_                          size_t inputMax,
    _Out_                         std::wstring &output,
    _Out_opt_                     std::vector<ZRCola::mapping>* map)
{
    assert(compositions || compositionsCount == 0);
    assert(input || inputMax == 0);

    // Trim inputMax to actual length.
    inputMax = wcsnlen(input, inputMax);

    // Clear the output string and preallocate at least inputMax chars.
    // Since composing is usually reducing the number of chars, memory reallocation is not expected later.
    output.clear();
    output.reserve(inputMax);
    if (map)
        map->clear();

    for (size_t i = 0; i < inputMax;) {
        // Start with the full search area at i-th character.
        for (size_t l = 0, r = compositionsCount, ii = i, j = 0;; ii++, j++) {
            if (ii < inputMax) {
                size_t l_prev = l;
                wchar_t c = input[ii];
                while (l < r) {
                    // Test the composition in the middle of the search area.
                    size_t m = (l + r) / 2;

                    // Get the j-th character of the composition.
                    // All compositions that get short on characters are lexically ordered before.
                    // Thus the j-th character is considered 0.
                    wchar_t s = j < wcslen(compositions[m].src) ? compositions[m].src[j] : 0;

                    // Do the bisection test.
                         if (c < s) r = m;
                    else if (s < c) l = m + 1;
                    else {
                        // Character found.

                        // Narrow the search area on the left to start at the first composition in the run.
                        for (size_t rr = m; l < rr;) {
                            size_t m = (l + rr) / 2;
                            wchar_t s = j < wcslen(compositions[m].src) ? compositions[m].src[j] : 0;
                            if (c <= s) rr = m; else l = m + 1;
                        }

                        // Narrow the search area on the right to end at the first composition not in the run.
                        for (size_t ll = m + 1; ll < r;) {
                            size_t m = (ll + r) / 2;
                            wchar_t s = j < wcslen(compositions[m].src) ? compositions[m].src[j] : 0;
                            if (s <= c) ll = m + 1; else r = m;
                        }

                        break;
                    }
                }

                if (l >= r) {
                    // The search area is empty.
                    if (j && wcslen(compositions[l_prev].src) == j) {
                        // The first composition of the previous run was a match.
                        output += compositions[l_prev].dst;
                        i = ii;
                        if (j > 1 && map) {
                            // Mapping changed.
                            map->push_back(ZRCola::mapping(output.length(), i));
                        }
                    } else {
                        // The exact match was not found.
                        output += input[i];
                        i++;
                    }
                    break;
                }
            } else {
                // End of input reached.

                if (l && wcslen(compositions[l - 1].src) == j) {
                    // The first composition of the previous run was a match.
                    output += compositions[l].dst;
                    i = ii;
                    if (j > 1 && map) {
                        // Mapping changed.
                        map->push_back(ZRCola::mapping(output.length(), i));
                    }
                } else {
                    output += input[i];
                    i++;
                }

                break;
            }
        }
    }
}


void ZRCOLA_API ZRCola::Compose(_In_z_count_(inputMax) const wchar_t* input, _In_ size_t inputMax, _Out_ std::wstring &output, _Out_opt_ std::vector<mapping>* map)
{
    ::Compose(compositions, compositionsCount, input, inputMax, output, map);
}
