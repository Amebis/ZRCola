/*
    Copyright 2015-2018 Amebis

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


const ZRCola::chrcatid_t ZRCola::chrcatid_t::blank = {};


bool ZRCola::character_db::Search(_In_z_ const wchar_t *str, _In_ const std::set<chrcatid_t> &cats, _Inout_ std::map<std::wstring, charrank_t> &hits, _Inout_ std::map<std::wstring, charrank_t> &hits_sub, _In_opt_ bool (__cdecl *fn_abort)(void *cookie), _In_opt_ void *cookie) const
{
    assert(str);

    while (*str) {
        if (fn_abort && fn_abort(cookie)) return false;

        // Skip white space.
        for (;;) {
            if (*str == 0)
                return true;
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
            if (fn_abort && fn_abort(cookie)) return false;

            // Find the term.
            std::transform(term.begin(), term.end(), term.begin(), std::towlower);

            if (fn_abort && fn_abort(cookie)) return false;

            const wchar_t *val;
            size_t val_len;

            if (idxDsc.find(term.c_str(), term.size(), &val, &val_len)) {
                // The term was found.
                for (size_t i = 0, j = 0; i < val_len; i += j + 1) {
                    if (fn_abort && fn_abort(cookie)) return false;
                    j = wcsnlen(val + i, val_len - i);
                    if (cats.find(GetCharCat(val + i, j)) != cats.end()) {
                        std::wstring c(val + i, j);
                        auto idx = hits.find(c);
                        if (idx == hits.end()) {
                            // New character.
                            hits.insert(std::make_pair(std::move(c), 1.0/val_len));
                        } else {
                            // Increase rating of existing character.
                            idx->second += 1.0/val_len;
                        }
                    }
                }
            }

            if (idxDscSub.find(term.c_str(), term.size(), &val, &val_len)) {
                // The term was found in the sub-term index.
                for (size_t i = 0, j = 0; i < val_len; i += j + 1) {
                    if (fn_abort && fn_abort(cookie)) return false;
                    j = wcsnlen(val + i, val_len - i);
                    if (cats.find(GetCharCat(val + i, j)) != cats.end()) {
                        std::wstring c(val + i, j);
                        auto idx = hits_sub.find(c);
                        if (idx == hits_sub.end()) {
                            // New character.
                            hits_sub.insert(std::make_pair(c, 1.0/val_len));
                        } else {
                            // Increase rating of existing character.
                            idx->second += 1.0/val_len;
                        }
                    }
                }
            }
        }
    }

    return true;
}
