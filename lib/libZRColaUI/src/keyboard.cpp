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


void ZRCola::keyseq_db::GetSequenceAsText(_In_count_(seq_len) const ZRCola::keyseq_db::keyseq::key_t *seq, _In_ size_t seq_len, _Out_ std::wstring& str)
{
    assert(seq || !seq_len);

    str.clear();
    for (size_t i = 0; i < seq_len; i++) {
        if (i) str += L", ";
        if (seq[i].modifiers & keyseq::CTRL ) str += L"Ctrl+";
        if (seq[i].modifiers & keyseq::ALT  ) str += L"Alt+";
        if (seq[i].modifiers & keyseq::SHIFT) str += L"Shift+";
        str += seq[i].key;
    }
}
