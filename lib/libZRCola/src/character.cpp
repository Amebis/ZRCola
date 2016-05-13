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


void ZRCola::character_db::search_by_desc(_In_z_ const wchar_t *str, _Inout_ std::map<wchar_t, unsigned long> &hits, _Inout_ std::map<wchar_t, unsigned long> &hits_sub) const
{
    assert(str);

    while (*str) {
        // Skip white space.
        for (;;) {
            if (*str == 0)
                return;
            else if (!iswspace(*str))
                break;
            else
                str++;
        }

        // Get term.
        std::wstring term;
        if (*str == L'"') {
            const wchar_t *str_end = ++str;
            for (;;) {
                if (*str_end == 0) {
                    term.assign(str, str_end);
                    break;
                } else if (*str_end == L'"') {
                    term.assign(str, str_end);
                    str_end++;
                    break;
                } else
                    str_end++;
            }
            str = str_end;
        } else {
            const wchar_t *str_end = str + 1;
            for (; *str_end && !iswspace(*str_end); str_end++);
            term.assign(str, str_end);
            str = str_end;
        }

        if (!term.empty()) {
            // Find the term.
            std::transform(term.begin(), term.end(), term.begin(), std::towlower);

            const wchar_t *data;
            size_t len;

            if (idxDsc.find(term.c_str(), term.size(), &data, &len)) {
                // The term was found.
                for (size_t i = 0; i < len; i++) {
                    std::map<wchar_t, unsigned long>::iterator idx = hits.find(data[i]);
                    if (idx == hits.end()) {
                        // New character.
                        hits.insert(std::make_pair(data[i], 1));
                    } else {
                        // Increment existing character.
                        idx->second++;
                    }
                }
            }

            if (idxDscSub.find(term.c_str(), term.size(), &data, &len)) {
                // The term was found in the sub-term index.
                for (size_t i = 0; i < len; i++) {
                    std::map<wchar_t, unsigned long>::iterator idx = hits_sub.find(data[i]);
                    if (idx == hits_sub.end()) {
                        // New character.
                        hits_sub.insert(std::make_pair(data[i], 1));
                    } else {
                        // Increment existing character.
                        idx->second++;
                    }
                }
            }
        }
    }
}
