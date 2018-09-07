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

///
/// Forward declarations
///
class wxZRColaKeyHandler;


#pragma once

#include <zrcolaui/keyboard.h>

#include <wx/event.h>
#include <vector>


///
/// ZRCola keyboard event handler
///
class wxZRColaKeyHandler : public wxEvtHandler
{
public:
    wxZRColaKeyHandler();

    virtual bool ProcessEvent(wxEvent& event);

protected:
    std::vector<ZRCola::keyseq_db::keyseq::key_t> m_seq;    ///< Key sequence
    bool m_is_insert;                                       ///< Is Insert key pressed?
    std::vector<char> m_insert_seq;                         ///< The Insert char sequence
};
