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

#pragma once

#include "zrcolagui.h"
#include "zrcolacomppnl.h"


///
/// Global hotkey message identifiers
///
#define wxZRColaHKID_INVOKE_COMPOSE     0
#define wxZRColaHKID_INVOKE_DECOMPOSE   1


///
/// ZRCola main frame
///
class wxZRColaFrame : public wxZRColaFrameBase
{
protected:
    enum {
        wxID_SEND = wxID_HIGHEST,
        wxID_SEND_COMPOSED,
        wxID_SEND_DECOMPOSED,
        wxID_SEND_ABORT,
    };

public:
    wxZRColaFrame();
    virtual ~wxZRColaFrame();

protected:
    void OnSendUpdate(wxUpdateUIEvent& event);
    void OnSend(wxCommandEvent& event);
    void OnSendComposed(wxCommandEvent& event);
    void OnSendDecomposed(wxCommandEvent& event);
    void OnSendAbort(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    wxDECLARE_EVENT_TABLE();

protected:
    virtual WXLRESULT MSWWindowProc(WXUINT message, WXWPARAM wParam, WXLPARAM lParam);

protected:
    WXHWND m_hWndSource;            ///< handle of the active window, when the ZRCola hotkey was pressed
    int m_hotkey;                   ///< hotkey ID that was pressed
    wxZRColaComposerPanel *m_panel; ///< composer panel
};