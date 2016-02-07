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
        wxID_SEND_COMPOSED = wxID_HIGHEST,
        wxID_SEND_ABORT,
        wxID_DECOMPOSED,
        wxID_COMPOSED,
    };

public:
    wxZRColaFrame();
    bool Create();
    virtual bool Destroy();

protected:
    void OnDecomposedText(wxCommandEvent& event);
    void OnComposedText(wxCommandEvent& event);
    void OnSendComposedUpdate(wxUpdateUIEvent& event);
    void OnSendComposed(wxCommandEvent& event);
    void OnSendAbort(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    wxDECLARE_EVENT_TABLE();

protected:
    virtual WXLRESULT MSWWindowProc(WXUINT message, WXWPARAM wParam, WXLPARAM lParam);

protected:
    wxTextCtrl m_decomposed;    ///< ZRCola decomposed text window
    wxTextCtrl m_composed;      ///< ZRCola composed text window
    wxToolBar m_toolBar;        ///< The toolbar

    WXHWND m_hWndSource;        ///< handle of the active window, when the ZRCola hotkey was pressed
};
