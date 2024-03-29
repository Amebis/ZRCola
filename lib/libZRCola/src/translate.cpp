﻿/*
    SPDX-License-Identifier: GPL-3.0-or-later
    Copyright © 2015-2022 Amebis
*/

#include "pch.h"


void ZRCola::translation_db::Translate(_In_ transetid_t set, _In_z_count_(inputMax) const char_t* input, _In_ size_t inputMax, _Out_ string_t &output, _Out_opt_ std::vector<mapping>* map) const
{
    assert(input || inputMax == 0);

    // Trim inputMax to actual length.
    inputMax = inputMax != (size_t)-1 ? wcsnlen(input, inputMax) : wcslen(input);

    // Clear the output.
    output.clear();
    output.reserve(inputMax);
    if (map)
        map->clear();

    // Limit search to the given set first.
    indexSrc::size_type l_set, r_set;
    idxSrc.find(translation(set    ), l_set);
    idxSrc.find(translation(set + 1), r_set);

    for (size_t i = 0; i < inputMax;) {
        // Find the longest matching translation at i-th character.
        size_t l_match = (size_t)-1;
        for (size_t l = l_set, r = r_set, ii = i, j = 0; ii < inputMax && l < r; ii++, j++) {
            auto c = input[ii];
            while (l < r) {
                // Test the translation in the middle of the search area.
                size_t m = (l + r) / 2;

                // Get the j-th character of the translation.
                // All translations that get short on characters are lexically ordered before.
                // Thus the j-th character is considered 0.
                auto s = idxSrc[m].src_at(j);

                // Do the bisection test.
                     if (c < s) r = m;
                else if (s < c) l = m + 1;
                else {
                    // Character found.

                    // Narrow the search area on the left to start at the first translation in the run.
                    for (size_t r2 = m; l < r2;) {
                        size_t m2 = (l + r2) / 2;
                        if (c <= idxSrc[m2].src_at(j)) r2 = m2; else l = m2 + 1;
                    }

                    // Narrow the search area on the right to end at the first translation not in the run.
                    for (size_t l2 = m + 1; l2 < r;) {
                        size_t m2 = (l2 + r) / 2;
                        if (idxSrc[m2].src_at(j) <= c) l2 = m2 + 1; else r = m2;
                    }

                    if (j + 1 == idxSrc[l].src_len()) {
                        // The first translation of the run was a match (thus far). Save it.
                        l_match = l;
                    }

                    break;
                }
            }
        }

        if (l_match < r_set) {
            // The saved translation was an exact match.
            const translation &trans = idxSrc[l_match];
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


void ZRCola::translation_db::TranslateInv(_In_ transetid_t set, _In_z_count_(inputMax) const char_t* input, _In_ size_t inputMax, _In_opt_ const langchar_db *lc_db, _In_opt_ langid_t lang, _Out_ string_t &output, _Out_opt_ std::vector<mapping>* map) const
{
    assert(input || inputMax == 0);

    // Trim inputMax to actual length.
    inputMax = inputMax != (size_t)-1 ? wcsnlen(input, inputMax) : wcslen(input);

    // Clear the output.
    output.clear();
    output.reserve(inputMax);
    if (map)
        map->clear();

    // Limit search to the given set first.
    indexDst::size_type l_set, r_set;
    idxDst.find(translation(set    ), l_set);
    idxDst.find(translation(set + 1), r_set);

    for (size_t i = 0; i < inputMax;) {
        // Find the longest matching inverse translation at i-th character.
        size_t l_match = (size_t)-1;
        for (size_t l = l_set, r = r_set, ii = i, j = 0; ii < inputMax && l < r; ii++, j++) {
            auto c = input[ii];
            while (l < r) {
                // Test the inverse translation in the middle of the search area.
                size_t m = (l + r) / 2;

                // Get the j-th character of the inverse translation.
                // All inverse translations that get short on characters are lexically ordered before.
                // Thus the j-th character is considered 0.
                auto s = idxDst[m].dst_at(j);

                // Do the bisection test.
                     if (c < s) r = m;
                else if (s < c) l = m + 1;
                else {
                    // Character found.

                    // Narrow the search area on the left to start at the first inverse translation in the run.
                    for (size_t r2 = m; l < r2;) {
                        size_t m2 = (l + r2) / 2;
                        if (c <= idxDst[m2].dst_at(j)) r2 = m2; else l = m2 + 1;
                    }

                    // Narrow the search area on the right to end at the first inverse translation not in the run.
                    for (size_t l2 = m + 1; l2 < r;) {
                        size_t m2 = (l2 + r) / 2;
                        if (idxDst[m2].dst_at(j) <= c) l2 = m2 + 1; else r = m2;
                    }

                    if (j + 1 == idxDst[l].dst_len()) {
                        // The first inverse translation of the run was a match (thus far). Save it.
                        l_match = l;
                    }

                    break;
                }
            }
        }

        if (l_match < r_set) {
            // The saved inverse translation was an exact match.
            const translation &trans = idxDst[l_match];
            if (trans.src_len() && trans.src()[0] != u'#' && (!lc_db || !lc_db->IsLocalCharacter(trans.dst(), trans.dst_end(), lang))) {
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
