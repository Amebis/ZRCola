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


#ifdef _WIN32
void ZRCola::LangConvert(_In_ LANGID lang_win, _Inout_ ZRCola::langid_t &lang)
{
    switch (PRIMARYLANGID(lang_win)) {
        case LANG_BELARUSIAN : memcpy(lang, "bel", sizeof(lang)); break;
        case LANG_CZECH      : memcpy(lang, "cze", sizeof(lang)); break;
        case LANG_DANISH     : memcpy(lang, "dan", sizeof(lang)); break;
        case LANG_GERMAN     : memcpy(lang, "deu", sizeof(lang)); break;
        case LANG_ENGLISH    : memcpy(lang, "eng", sizeof(lang)); break;
        case LANG_ESTONIAN   : memcpy(lang, "est", sizeof(lang)); break;
        case LANG_FRENCH     : memcpy(lang, "fra", sizeof(lang)); break;
        case LANG_IRISH      : memcpy(lang, "gle", sizeof(lang)); break;
        case LANG_HUNGARIAN  : memcpy(lang, "hun", sizeof(lang)); break;
        case LANG_LATVIAN    : memcpy(lang, "lav", sizeof(lang)); break;
        case LANG_LITHUANIAN : memcpy(lang, "lit", sizeof(lang)); break;
        case LANG_MACEDONIAN : memcpy(lang, "mkd", sizeof(lang)); break;
        case LANG_MALTESE    : memcpy(lang, "mlt", sizeof(lang)); break;
        case LANG_NORWEGIAN  : memcpy(lang, "nor", sizeof(lang)); break;
        case LANG_POLISH     : memcpy(lang, "pol", sizeof(lang)); break;
        case LANG_PORTUGUESE : memcpy(lang, "por", sizeof(lang)); break;
        case LANG_ROMANIAN   : memcpy(lang, "rum", sizeof(lang)); break;
        case LANG_RUSSIAN    : memcpy(lang, "rus", sizeof(lang)); break;
        case LANG_SLOVAK     : memcpy(lang, "slk", sizeof(lang)); break;
        case LANG_SLOVENIAN  : memcpy(lang, "slv", sizeof(lang)); break;
        case LANG_SPANISH    : memcpy(lang, "spa", sizeof(lang)); break;
        case LANG_ALBANIAN   : memcpy(lang, "sqi", sizeof(lang)); break;
        case LANG_SWEDISH    : memcpy(lang, "swe", sizeof(lang)); break;
        case LANG_TURKISH    : memcpy(lang, "tur", sizeof(lang)); break;
        case LANG_UKRAINIAN  : memcpy(lang, "ukr", sizeof(lang)); break;

        case LANG_CROATIAN   : // LANG_BOSNIAN, and LANG_SERBIAN
            switch (SUBLANGID(lang_win)) {
                case SUBLANG_BOSNIAN_BOSNIA_HERZEGOVINA_LATIN    : memcpy(lang, "bos", sizeof(lang)); break;
                case SUBLANG_BOSNIAN_BOSNIA_HERZEGOVINA_CYRILLIC : memcpy(lang, "boz", sizeof(lang)); break;
                case SUBLANG_CROATIAN_CROATIA                    :
                case SUBLANG_CROATIAN_BOSNIA_HERZEGOVINA_LATIN   : memcpy(lang, "hrv", sizeof(lang)); break;
                case SUBLANG_SERBIAN_LATIN                       :
                case SUBLANG_SERBIAN_BOSNIA_HERZEGOVINA_LATIN    :
                case SUBLANG_SERBIAN_MONTENEGRO_LATIN            :
                case SUBLANG_SERBIAN_SERBIA_LATIN                : memcpy(lang, "srp", sizeof(lang)); break;
                case SUBLANG_SERBIAN_CYRILLIC                    :
                case SUBLANG_SERBIAN_BOSNIA_HERZEGOVINA_CYRILLIC :
                case SUBLANG_SERBIAN_MONTENEGRO_CYRILLIC         :
                case SUBLANG_SERBIAN_SERBIA_CYRILLIC             : memcpy(lang, "srz", sizeof(lang)); break;
            }
            break;
    }
}
#endif


bool ZRCola::langchar_db::IsLocalCharacter(_In_ wchar_t chr, _In_ ZRCola::langid_t lang) const
{
    for (size_t l = 0, r = idxChr.size(); l < r; ) {
        // Test the character in the middle of the search area.
        size_t m = (l + r) / 2;
        const langchar &lc = idxChr[m];

        // Do the bisection test on character.
             if (chr    < lc.chr) r = m;
        else if (lc.chr < chr   ) l = m + 1;
        else {
            // Do the bisection test on language.
            int res = memcmp(lang, lc.lang, sizeof(langid_t));
                 if (res < 0) r = m;
            else if (res > 0) l = m + 1;
            else {
                // Match found.
                return true;
            }
        }
    }

    return false;
}
