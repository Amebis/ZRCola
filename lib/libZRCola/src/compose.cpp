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


void ZRCOLA_API ZRCola::Compose(_In_z_count_(inputMax) const wchar_t *input, _In_ size_t inputMax, _Out_ std::wstring &output, _Out_opt_ std::vector<mapping>* map)
{
    assert(input || inputMax == 0);

    // Trim inputMax to actual length.
    inputMax = wcsnlen(input, inputMax);

    // Clear the output string and preallocate at least inputMax chars.
    // Since composing is usually reducing the number of chars, memory reallocation is not expected later.
    output.clear();
    output.reserve(inputMax);


}
