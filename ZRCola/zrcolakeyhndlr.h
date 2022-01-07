/*
    SPDX-License-Identifier: GPL-3.0-or-later
    Copyright © 2015-2022 Amebis
*/

///
/// Forward declarations
///
class wxZRColaKeyHandler;


#pragma once

#include <zrcolaui/keyboard.h>

#include <wxex/common.h>
#pragma warning(push)
#pragma warning(disable: WXWIDGETS_CODE_ANALYSIS_WARNINGS)
#include <wx/event.h>
#pragma warning(pop)
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
