/*
    SPDX-License-Identifier: GPL-3.0-or-later
    Copyright © 2015-2022 Amebis
*/

#include "pch.h"


const ZRCola::chrcatid_t ZRCola::chrcatid_t::blank = {};


#ifndef _WIN32

_Use_decl_annotations_
size_t ZRCola::wcslen(const char_t *str)
{
    for (size_t i = 0; ; ++i)
        if (!str[i])
            return i;
}

_Use_decl_annotations_
size_t ZRCola::wcsnlen(const char_t *str, size_t count)
{
    for (size_t i = 0; ; ++i)
        if (i >= count || !str[i])
            return i;
}

#endif


_Use_decl_annotations_
bool ZRCola::character_db::Search(const char_t *str, const std::set<chrcatid_t> &cats, std::map<string_t, charrank_t> &hits, std::map<string_t, charrank_t> &hits_sub, bool (__cdecl *fn_abort)(void *cookie), void *cookie) const
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
        string_t term;
        if (*str == u'"') {
            const auto *str_end = ++str;
            for (;;) {
                if (*str_end == 0) {
                    term.assign(str, str_end);
                    break;
                } else if (*str_end == u'"') {
                    term.assign(str, str_end);
                    str_end++;
                    break;
                } else
                    str_end++;
            }
            str = str_end;
        } else {
            const auto *str_end = str + 1;
            for (; *str_end && !iswspace(*str_end); str_end++);
            term.assign(str, str_end);
            str = str_end;
        }

        if (!term.empty()) {
            if (fn_abort && fn_abort(cookie)) return false;

            // Find the term.
            std::transform(term.begin(), term.end(), term.begin(), std::towlower);

            if (fn_abort && fn_abort(cookie)) return false;

            const char_t *val;
            size_t val_len;

            if (idxDsc.find(term.c_str(), term.size(), &val, &val_len)) {
                // The term was found.
                for (size_t i = 0, j = 0; i < val_len; i += j + 1) {
                    if (fn_abort && fn_abort(cookie)) return false;
                    j = wcsnlen(val + i, val_len - i);
                    if (cats.find(GetCharCat(val + i, j)) != cats.end()) {
                        string_t c(val + i, j);
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
                        string_t c(val + i, j);
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
