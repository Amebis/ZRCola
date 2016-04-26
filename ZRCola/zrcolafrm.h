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


///
/// Forward declarations
///
class wxZRColaFrame;


#pragma once

#include "zrcolagui.h"
#include "zrcolachrslct.h"
#include <zrcola/language.h>
#include <wx/persist/toplevel.h>


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
public:
    enum
    {
        wxID_DECOMP_LANGUAGE_START = 6000,
        wxID_DECOMP_LANGUAGE_END   = 6099,
    };

    wxZRColaFrame();
    virtual ~wxZRColaFrame();

protected:
    void OnForwardEventUpdate(wxUpdateUIEvent& event);
    void OnForwardEvent(wxCommandEvent& event);
    void OnInsertCharacter(wxCommandEvent& event);
    void OnSendUpdate(wxUpdateUIEvent& event);
    void OnSendComposed(wxCommandEvent& event);
    void OnSendDecomposed(wxCommandEvent& event);
    void OnSendAbort(wxCommandEvent& event);
    void OnDecomposedLanguageUpdate(wxUpdateUIEvent& event);
    void OnDecomposedLanguage(wxCommandEvent& event);
    virtual void OnDecompLanguageChoice(wxCommandEvent& event);
    void OnExit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    wxDECLARE_EVENT_TABLE();

    friend class wxPersistentZRColaFrame;
    friend class wxZRColaComposerPanel;

private:
    void DoSend(const wxString& str);

protected:
    virtual WXLRESULT MSWWindowProc(WXUINT message, WXWPARAM wParam, WXLPARAM lParam);

protected:
    ZRCola::langid_t m_lang;            ///< Language for decomposing
    WXHWND m_hWndSource;                ///< handle of the active window, when the ZRCola hotkey was pressed
    wxZRColaCharSelect *m_chrSelect;    ///< Character selection dialog
};


///
/// Supports saving/restoring wxZRColaFrame GUI state
///
class wxPersistentZRColaFrame : public wxPersistentTLW
{
public:
    wxPersistentZRColaFrame(wxZRColaFrame *wnd);

    virtual void Save() const;
    virtual bool Restore();
};


inline wxPersistentObject *wxCreatePersistentObject(wxZRColaFrame *wnd)
{
    return new wxPersistentZRColaFrame(wnd);
}
