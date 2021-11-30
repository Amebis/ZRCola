/*
    SPDX-License-Identifier: GPL-3.0-or-later
    Copyright © 2015-2021 Amebis
*/

#include "pch.h"


bool ZRCola::chrtag_db::Search(_In_ const std::map<tagid_t, unsigned __int16> &tags, _In_ const character_db &ch_db, _In_ const std::set<chrcatid_t> &cats, _Inout_ std::map<std::wstring, charrank_t> &hits, _In_opt_ bool (__cdecl *fn_abort)(void *cookie), _In_opt_ void *cookie) const
{
    for (auto tag = tags.cbegin(), tag_end = tags.cend(); tag != tag_end; ++tag) {
        if (fn_abort && fn_abort(cookie)) return false;

        // Search for tagged characters.
        size_t start, end;
        if (idxTag.find(chrtag(NULL, 0, tag->first), start, end)) {
            for (size_t i = start; i < end; i++) {
                if (fn_abort && fn_abort(cookie)) return false;
                const chrtag &ct = idxTag[i];
                unsigned __int16 len = ct.chr_len();
                if (cats.find(ch_db.GetCharCat(ct.chr(), len)) != cats.end()) {
                    std::wstring chr(ct.chr(), len);
                    auto idx = hits.find(chr);
                    if (idx == hits.end()) {
                        // New character.
                        hits.insert(std::make_pair(std::move(chr), tag->second));
                    } else {
                        // Increase count for existing character.
                        idx->second += tag->second;
                    }
                }
            }
        }
    }

    return true;
}


bool ZRCola::tagname_db::Search(_In_z_ const wchar_t *str, _In_ LCID locale, _Inout_ std::map<tagid_t, unsigned __int16> &hits, _In_opt_ bool (__cdecl *fn_abort)(void *cookie), _In_opt_ void *cookie) const
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

        // Get name.
        std::wstring name;
        if (*str == L'"') {
            const wchar_t *str_end = ++str;
            for (;;) {
                if (*str_end == 0) {
                    name.assign(str, str_end);
                    break;
                } else if (*str_end == L'"') {
                    name.assign(str, str_end);
                    str_end++;
                    break;
                } else
                    str_end++;
            }
            str = str_end;
        } else {
            const wchar_t *str_end = str + 1;
            for (; *str_end && !iswspace(*str_end); str_end++);
            name.assign(str, str_end);
            str = str_end;
        }

        if (!name.empty()) {
            if (fn_abort && fn_abort(cookie)) return false;

            // Find the name.
            std::unique_ptr<tagname> tn(reinterpret_cast<tagname*>(new char[sizeof(tagname) + sizeof(wchar_t)*name.length()]));
            tn->tagname::tagname(0, locale, name.data(), name.length());
            size_t start, end;
            if (idxName.find(*tn, start, end)) {
                // The name was found.
                for (size_t i = start; i < end; i++) {
                    if (fn_abort && fn_abort(cookie)) return false;
                    const tagname &val = idxName[i];
                    auto idx = hits.find(val.tag);
                    if (idx == hits.end()) {
                        // New tag.
                        hits.insert(std::make_pair(val.tag, (unsigned __int16)1));
                    } else {
                        // Increase count for existing tag.
                        idx->second++;
                    }
                }
            }
        }
    }

    return true;
}
