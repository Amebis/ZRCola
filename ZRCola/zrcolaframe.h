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

#include <wxex/appbar.h>
#include <wx/textctrl.h>
#include <wx/toolbar.h>


///
/// Global hotkey message identifiers
///
#define wxZRColaHKID_INVOKE 0


///
/// ZRCola main frame
///
class wxZRColaFrame : public wxAppBarFrame
{
protected:
    enum {
        wxID_SEND,
        wxID_COMPOSER,
    };

public:
    wxZRColaFrame();
    bool Create();
    virtual bool Destroy();

protected:
    void OnCompose(wxCommandEvent& event);
    void OnSendUpdate(wxUpdateUIEvent& event);
    void OnSend(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    wxDECLARE_EVENT_TABLE();

protected:
    virtual WXLRESULT MSWWindowProc(WXUINT message, WXWPARAM wParam, WXLPARAM lParam);

protected:
    wxTextCtrl m_preview;
    wxTextCtrl m_composer;
    wxToolBar m_toolBar;

    WXHWND m_hWndSource;    ///< handle of the active window, when the ZRCola hotkey was pressed
};
