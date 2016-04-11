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
