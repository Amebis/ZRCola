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


bool ZRCola::chrtag_db::Search(_In_ const std::map<tagid_t, unsigned __int16> &tags, _Inout_ std::map<wchar_t, charrank_t> &hits, _In_opt_ bool (__cdecl *fn_abort)(void *cookie), _In_opt_ void *cookie) const
{
    for (auto tag = tags.cbegin(), tag_end = tags.cend(); tag != tag_end; ++tag) {
        if (fn_abort && fn_abort(cookie)) return false;

        // Search for tagged characters.
        chrtag el = { 0, tag->first };
        size_t start, end;
        if (idxTag.find(el, start, end)) {
            for (size_t i = start; i < end; i++) {
                if (fn_abort && fn_abort(cookie)) return false;
                const chrtag &ct = idxTag[i];
                auto idx = hits.find(ct.chr);
                if (idx == hits.end()) {
                    // New character.
                    hits.insert(std::make_pair(ct.chr, tag->second));
                } else {
                    // Increase count for existing character.
                    idx->second += tag->second;
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
            tn->locale = locale;
            memcpy(tn->name, name.data(), sizeof(wchar_t)*(tn->name_len = (unsigned __int16)name.length()));
            size_t start, end;
            if (idxName.find(*tn, start, end)) {
                // The name was found.
                for (size_t i = start; i < end; i++) {
                    if (fn_abort && fn_abort(cookie)) return false;
                    const tagname &name = idxName[i];
                    auto idx = hits.find(name.tag);
                    if (idx == hits.end()) {
                        // New tag.
                        hits.insert(std::make_pair(name.tag, 1));
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
