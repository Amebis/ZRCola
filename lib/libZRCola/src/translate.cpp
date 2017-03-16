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


void ZRCola::translation_db::Compose(_In_z_count_(inputMax) const wchar_t* input, _In_ size_t inputMax, _Out_ std::wstring &output, _Out_opt_ std::vector<mapping>* map) const
{
    assert(input || inputMax == 0);

    // Trim inputMax to actual length.
    inputMax = inputMax != (size_t)-1 ? wcsnlen(input, inputMax) : wcslen(input);

    // Clear the output.
    output.clear();
    output.reserve(inputMax);
    if (map)
        map->clear();

    auto compositionsCount = idxComp.size();

    for (size_t i = 0; i < inputMax;) {
        // Find the longest matching composition at i-th character.
        size_t l_match = (size_t)-1;
        for (size_t l = 0, r = compositionsCount, ii = i, j = 0; ii < inputMax && l < r; ii++, j++) {
            wchar_t c = input[ii];
            while (l < r) {
                // Test the composition in the middle of the search area.
                size_t m = (l + r) / 2;

                // Get the j-th character of the composition.
                // All compositions that get short on characters are lexically ordered before.
                // Thus the j-th character is considered 0.
                const translation &trans = idxComp[m];
                wchar_t s = trans.dec_at(j);

                // Do the bisection test.
                     if (c < s) r = m;
                else if (s < c) l = m + 1;
                else {
                    // Character found.

                    // Narrow the search area on the left to start at the first composition in the run.
                    for (size_t rr = m; l < rr;) {
                        size_t m = (l + rr) / 2;
                        const translation &trans = idxComp[m];
                        wchar_t s = trans.dec_at(j);
                        if (c <= s) rr = m; else l = m + 1;
                    }

                    // Narrow the search area on the right to end at the first composition not in the run.
                    for (size_t ll = m + 1; ll < r;) {
                        size_t m = (ll + r) / 2;
                        const translation &trans = idxComp[m];
                        wchar_t s = trans.dec_at(j);
                        if (s <= c) ll = m + 1; else r = m;
                    }

                    const translation &trans = idxComp[l];
                    if (j + 1 == trans.dec_len()) {
                        // The first composition of the run was a match (thus far). Save it.
                        l_match = l;
                    }

                    break;
                }
            }
        }

        if (l_match < compositionsCount) {
            // The saved composition was an exact match.
            const translation &trans = idxComp[l_match];
            output.append(trans.com(), trans.com_end());
            i += trans.dec_len();
            if (trans.dec_len() != trans.com_len() && map) {
                // Mapping changed.
                map->push_back(ZRCola::mapping(i, output.length()));
            }
        } else {
            // The match was not found.
            output += input[i];
            i++;
        }
    }
}


void ZRCola::translation_db::Decompose(_In_z_count_(inputMax) const wchar_t* input, _In_ size_t inputMax, _In_ const langchar_db *lc_db, _In_ langid_t lang, _Out_ std::wstring &output, _Out_opt_ std::vector<mapping>* map) const
{
    assert(input || inputMax == 0);

    // Trim inputMax to actual length.
    inputMax = inputMax != (size_t)-1 ? wcsnlen(input, inputMax) : wcslen(input);

    // Clear the output.
    output.clear();
    output.reserve(inputMax);
    if (map)
        map->clear();

    auto decompositionsCount = idxDecomp.size();

    for (size_t i = 0; i < inputMax;) {
        // Find the longest matching decomposition at i-th character.
        size_t l_match = (size_t)-1;
        for (size_t l = 0, r = decompositionsCount, ii = i, j = 0; ii < inputMax && l < r; ii++, j++) {
            wchar_t c = input[ii];
            while (l < r) {
                // Test the decomposition in the middle of the search area.
                size_t m = (l + r) / 2;

                // Get the j-th character of the decomposition.
                // All decompositions that get short on characters are lexically ordered before.
                // Thus the j-th character is considered 0.
                const translation &trans = idxDecomp[m];
                wchar_t s = trans.com_at(j);

                // Do the bisection test.
                     if (c < s) r = m;
                else if (s < c) l = m + 1;
                else {
                    // Character found.

                    // Narrow the search area on the left to start at the first decomposition in the run.
                    for (size_t rr = m; l < rr;) {
                        size_t m = (l + rr) / 2;
                        const translation &trans = idxDecomp[m];
                        wchar_t s = trans.com_at(j);
                        if (c <= s) rr = m; else l = m + 1;
                    }

                    // Narrow the search area on the right to end at the first decomposition not in the run.
                    for (size_t ll = m + 1; ll < r;) {
                        size_t m = (ll + r) / 2;
                        const translation &trans = idxDecomp[m];
                        wchar_t s = trans.com_at(j);
                        if (s <= c) ll = m + 1; else r = m;
                    }

                    const translation &trans = idxDecomp[l];
                    if (j + 1 == trans.com_len()) {
                        // The first decomposition of the run was a match (thus far). Save it.
                        l_match = l;
                    }

                    break;
                }
            }
        }

        if (l_match < decompositionsCount) {
            // The saved decomposition was an exact match.
            const translation &trans = idxDecomp[l_match];
            if (trans.dec_len() && trans.dec()[0] != L'#' && (!lc_db || !lc_db->IsLocalCharacter(trans.com(), trans.com_end(), lang))) {
                // Append decomposed sequence.
                output.append(trans.dec(), trans.dec_end());
                i += trans.com_len();
                if (trans.com_len() != trans.dec_len() && map) {
                    // Mapping changed.
                    map->push_back(ZRCola::mapping(i, output.length()));
                }
            } else {
                // Character is inhibited to decompose.
                output.append(trans.com(), trans.com_end());
                i += trans.com_len();
            }
        } else {
            // The match was not found.
            output += input[i];
            i++;
        }
    }
}
