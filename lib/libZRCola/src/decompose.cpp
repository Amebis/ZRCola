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


void ZRCOLA_API ZRCola::Decompose(_In_z_count_(inputMax) const wchar_t *input, _In_ size_t inputMax, _Out_ std::wstring &output, _Out_opt_ std::vector<mapping>* map)
{
    assert(input || inputMax == 0);

    // Trim inputMax to actual length.
    inputMax = inputMax != (size_t)-1 ? wcsnlen(input, inputMax) : wcslen(input);

    // Clear the output string and preallocate at least 2*inputMax chars.
    // Since decomposition expands the string, let's keep our fingers crossed to avoid reallocation later.
    output.clear();
    output.reserve(inputMax * 2);
    if (map)
        map->clear();

    for (size_t i = 0; i < inputMax;) {
        // Find whether the character can be decomposed.
        wchar_t c = input[i];
        for (size_t l = 0, r = decompositionsCount;; ) {
            if (l < r) {
                size_t m = (l + r) / 2;
                     if (c < decompositions[m].src) r = m;
                else if (decompositions[m].src < c) l = m + 1;
                else {
                    // Character found.
                    output += decompositions[m].dst;
                    i++;
                    if (map) {
                        // Mapping changed.
                        map->push_back(mapping(i, output.length()));
                    }
                    break;
                }
            } else {
                // Character not found.
                output += c;
                i++;
                break;
            }
        }
    }
}
