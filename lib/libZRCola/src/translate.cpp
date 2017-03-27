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


void ZRCola::translation_db::Translate(_In_z_count_(inputMax) const wchar_t* input, _In_ size_t inputMax, _Out_ std::wstring &output, _Out_opt_ std::vector<mapping>* map) const
{
    assert(input || inputMax == 0);

    // Trim inputMax to actual length.
    inputMax = inputMax != (size_t)-1 ? wcsnlen(input, inputMax) : wcslen(input);

    // Clear the output.
    output.clear();
    output.reserve(inputMax);
    if (map)
        map->clear();

    auto count = idxTrans.size();

    for (size_t i = 0; i < inputMax;) {
        // Find the longest matching translation at i-th character.
        size_t l_match = (size_t)-1;
        for (size_t l = 0, r = count, ii = i, j = 0; ii < inputMax && l < r; ii++, j++) {
            wchar_t c = input[ii];
            while (l < r) {
                // Test the translation in the middle of the search area.
                size_t m = (l + r) / 2;

                // Get the j-th character of the translation.
                // All translations that get short on characters are lexically ordered before.
                // Thus the j-th character is considered 0.
                const translation &trans = idxTrans[m];
                wchar_t s = trans.src_at(j);

                // Do the bisection test.
                     if (c < s) r = m;
                else if (s < c) l = m + 1;
                else {
                    // Character found.

                    // Narrow the search area on the left to start at the first translation in the run.
                    for (size_t rr = m; l < rr;) {
                        size_t m = (l + rr) / 2;
                        const translation &trans = idxTrans[m];
                        wchar_t s = trans.src_at(j);
                        if (c <= s) rr = m; else l = m + 1;
                    }

                    // Narrow the search area on the right to end at the first translation not in the run.
                    for (size_t ll = m + 1; ll < r;) {
                        size_t m = (ll + r) / 2;
                        const translation &trans = idxTrans[m];
                        wchar_t s = trans.src_at(j);
                        if (s <= c) ll = m + 1; else r = m;
                    }

                    const translation &trans = idxTrans[l];
                    if (j + 1 == trans.src_len()) {
                        // The first translation of the run was a match (thus far). Save it.
                        l_match = l;
                    }

                    break;
                }
            }
        }

        if (l_match < count) {
            // The saved translation was an exact match.
            const translation &trans = idxTrans[l_match];
            output.append(trans.dst(), trans.dst_end());
            i += trans.src_len();
            if (trans.src_len() != trans.dst_len() && map) {
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


void ZRCola::translation_db::TranslateInv(_In_z_count_(inputMax) const wchar_t* input, _In_ size_t inputMax, _In_ const langchar_db *lc_db, _In_ langid_t lang, _Out_ std::wstring &output, _Out_opt_ std::vector<mapping>* map) const
{
    assert(input || inputMax == 0);

    // Trim inputMax to actual length.
    inputMax = inputMax != (size_t)-1 ? wcsnlen(input, inputMax) : wcslen(input);

    // Clear the output.
    output.clear();
    output.reserve(inputMax);
    if (map)
        map->clear();

    auto count = idxTransInv.size();

    for (size_t i = 0; i < inputMax;) {
        // Find the longest matching inverse translation at i-th character.
        size_t l_match = (size_t)-1;
        for (size_t l = 0, r = count, ii = i, j = 0; ii < inputMax && l < r; ii++, j++) {
            wchar_t c = input[ii];
            while (l < r) {
                // Test the inverse translation in the middle of the search area.
                size_t m = (l + r) / 2;

                // Get the j-th character of the inverse translation.
                // All inverse translations that get short on characters are lexically ordered before.
                // Thus the j-th character is considered 0.
                const translation &trans = idxTransInv[m];
                wchar_t s = trans.dst_at(j);

                // Do the bisection test.
                     if (c < s) r = m;
                else if (s < c) l = m + 1;
                else {
                    // Character found.

                    // Narrow the search area on the left to start at the first inverse translation in the run.
                    for (size_t rr = m; l < rr;) {
                        size_t m = (l + rr) / 2;
                        const translation &trans = idxTransInv[m];
                        wchar_t s = trans.dst_at(j);
                        if (c <= s) rr = m; else l = m + 1;
                    }

                    // Narrow the search area on the right to end at the first inverse translation not in the run.
                    for (size_t ll = m + 1; ll < r;) {
                        size_t m = (ll + r) / 2;
                        const translation &trans = idxTransInv[m];
                        wchar_t s = trans.dst_at(j);
                        if (s <= c) ll = m + 1; else r = m;
                    }

                    const translation &trans = idxTransInv[l];
                    if (j + 1 == trans.dst_len()) {
                        // The first inverse translation of the run was a match (thus far). Save it.
                        l_match = l;
                    }

                    break;
                }
            }
        }

        if (l_match < count) {
            // The saved inverse translation was an exact match.
            const translation &trans = idxTransInv[l_match];
            if (trans.src_len() && trans.src()[0] != L'#' && (!lc_db || !lc_db->IsLocalCharacter(trans.dst(), trans.dst_end(), lang))) {
                // Append source sequence.
                output.append(trans.src(), trans.src_end());
                i += trans.dst_len();
                if (trans.dst_len() != trans.src_len() && map) {
                    // Mapping changed.
                    map->push_back(ZRCola::mapping(i, output.length()));
                }
            } else {
                // Character is inhibited to inverse translate.
                output.append(trans.dst(), trans.dst_end());
                i += trans.dst_len();
            }
        } else {
            // The match was not found.
            output += input[i];
            i++;
        }
    }
}
