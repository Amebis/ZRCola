/*
    Copyright 2015-2020 Amebis

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

#include "pch.h"


#ifdef _WIN32
void ZRCola::LangConvert(_In_ LANGID lang_win, _Inout_ ZRCola::langid_t &lang)
{
    switch (PRIMARYLANGID(lang_win)) {
        case LANG_BELARUSIAN : lang = "bel"; break;
        case LANG_CZECH      : lang = "cze"; break;
        case LANG_DANISH     : lang = "dan"; break;
        case LANG_GERMAN     : lang = "deu"; break;
        case LANG_ENGLISH    : lang = "eng"; break;
        case LANG_ESTONIAN   : lang = "est"; break;
        case LANG_FRENCH     : lang = "fra"; break;
        case LANG_IRISH      : lang = "gle"; break;
        case LANG_HUNGARIAN  : lang = "hun"; break;
        case LANG_LATVIAN    : lang = "lav"; break;
        case LANG_LITHUANIAN : lang = "lit"; break;
        case LANG_MACEDONIAN : lang = "mkd"; break;
        case LANG_MALTESE    : lang = "mlt"; break;
        case LANG_NORWEGIAN  : lang = "nor"; break;
        case LANG_POLISH     : lang = "pol"; break;
        case LANG_PORTUGUESE : lang = "por"; break;
        case LANG_ROMANIAN   : lang = "rum"; break;
        case LANG_RUSSIAN    : lang = "rus"; break;
        case LANG_SLOVAK     : lang = "slk"; break;
        case LANG_SLOVENIAN  : lang = "slv"; break;
        case LANG_SPANISH    : lang = "spa"; break;
        case LANG_ALBANIAN   : lang = "sqi"; break;
        case LANG_SWEDISH    : lang = "swe"; break;
        case LANG_TURKISH    : lang = "tur"; break;
        case LANG_UKRAINIAN  : lang = "ukr"; break;

        case LANG_CROATIAN   : // LANG_BOSNIAN, and LANG_SERBIAN
            switch (SUBLANGID(lang_win)) {
                case SUBLANG_BOSNIAN_BOSNIA_HERZEGOVINA_LATIN    : lang = "bos"; break;
                case SUBLANG_BOSNIAN_BOSNIA_HERZEGOVINA_CYRILLIC : lang = "boz"; break;
                case SUBLANG_CROATIAN_CROATIA                    :
                case SUBLANG_CROATIAN_BOSNIA_HERZEGOVINA_LATIN   : lang = "hrv"; break;
                case SUBLANG_SERBIAN_LATIN                       :
                case SUBLANG_SERBIAN_BOSNIA_HERZEGOVINA_LATIN    :
                case SUBLANG_SERBIAN_MONTENEGRO_LATIN            :
                case SUBLANG_SERBIAN_SERBIA_LATIN                : lang = "srp"; break;
                case SUBLANG_SERBIAN_CYRILLIC                    :
                case SUBLANG_SERBIAN_BOSNIA_HERZEGOVINA_CYRILLIC :
                case SUBLANG_SERBIAN_MONTENEGRO_CYRILLIC         :
                case SUBLANG_SERBIAN_SERBIA_CYRILLIC             : lang = "srz"; break;
            }
            break;
    }
}
#endif


bool ZRCola::langchar_db::IsLocalCharacter(_In_ const wchar_t *chr, _In_ const wchar_t *chr_end, _In_ ZRCola::langid_t lang) const
{
    size_t n = chr_end - chr;
    assert(n <= 0xffff);
    std::unique_ptr<langchar> lc((langchar*)new char[sizeof(langchar) + sizeof(wchar_t)*n]);
    lc->langchar::langchar(lang, chr, n);
    indexChr::size_type start;
    return idxChr.find(*lc, start);
}
