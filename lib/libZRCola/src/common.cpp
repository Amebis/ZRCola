/*
    SPDX-License-Identifier: GPL-3.0-or-later
    Copyright © 2015-2022 Amebis
*/

#include "pch.h"


const ZRCola::langid_t ZRCola::langid_t::blank = {};


_Use_decl_annotations_
int ZRCola::CompareString(const wchar_t* str_a, size_t count_a, const wchar_t* str_b, size_t count_b)
{
    for (size_t i = 0; ; i++) {
        if (i >= count_a && i >= count_b) return  0;
        else if (i >= count_a && i < count_b) return -1;
        else if (i < count_a && i >= count_b) return +1;
        else if (str_a[i] < str_b[i]) return -1;
        else if (str_a[i] > str_b[i]) return +1;
    }
}


_Use_decl_annotations_
inline std::string ZRCola::GetUnicodeDumpA(const wchar_t* str, size_t count, const char* sep)
{
    std::string out;
    size_t sep_len = strlen(sep);
    size_t dump_len_max = sep_len + 8 + 1;
    char* dump;
    std::unique_ptr<char[]> dump_obj(dump = new char[dump_len_max]);
    if (count && str[0]) {
        size_t i = 0;
        static const char error[] = "????";
        int n = snprintf(dump, dump_len_max, "%04X", str[i++]);
        if (n >= 0)
            out.insert(out.end(), dump, dump + n);
        else
            out.insert(out.end(), error, error + std::size(error) - 1);
        while (i < count && str[i]) {
            n = snprintf(dump, dump_len_max, "%s%04X", sep, str[i++]);
            if (n >= 0)
                out.insert(out.end(), dump, dump + n);
            else {
                out.insert(out.end(), sep, sep + sep_len);
                out.insert(out.end(), error, error + std::size(error) - 1);
            }
        }
    }

    return out;
}


_Use_decl_annotations_
std::wstring ZRCola::GetUnicodeDumpW(const wchar_t* str, size_t count, const wchar_t* sep)
{
    std::wstring out;
    size_t sep_len = wcslen(sep);
    size_t dump_len_max = sep_len + 8 + 1;
    wchar_t* dump;
    std::unique_ptr<wchar_t[]> dump_obj(dump = new wchar_t[dump_len_max]);
    if (count && str[0]) {
        size_t i = 0;
        static const wchar_t error[] = L"????";
        int n = swprintf(dump, dump_len_max, L"%04X", str[i++]);
        if (n >= 0)
            out.insert(out.end(), dump, dump + n);
        else
            out.insert(out.end(), error, error + std::size(error) - 1);
        while (i < count && str[i]) {
            n = swprintf(dump, dump_len_max, L"%s%04X", sep, str[i++]);
            if (n >= 0)
                out.insert(out.end(), dump, dump + n);
            else {
                out.insert(out.end(), sep, sep + sep_len);
                out.insert(out.end(), error, error + std::size(error) - 1);
            }
        }
    }

    return out;
}
