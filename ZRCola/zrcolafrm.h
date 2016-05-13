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
#include "zrcolasettings.h"
#include <zrcola/language.h>
#include <wx/taskbar.h>
#include <wx/persist/toplevel.h>
#if defined(__WXMSW__)
#include <msctf.h>
#endif


///
/// Global hotkey message identifiers
///
#define wxZRColaHKID_INVOKE_COMPOSE     0
#define wxZRColaHKID_INVOKE_DECOMPOSE   1


///
/// ZRCola main frame
///
class wxZRColaFrame :
    public wxZRColaFrameBase
#if defined(__WXMSW__)
    , protected ITfLanguageProfileNotifySink
#endif
{
public:
    enum
    {
        wxID_FOCUS_CHARACTER_CATALOG = 2000,
    };

    wxZRColaFrame();
    virtual ~wxZRColaFrame();

    friend class wxPersistentZRColaFrame;
    friend class wxZRColaComposerPanel;

protected:
    void OnExit(wxCommandEvent& event);
    void OnForwardEventUpdate(wxUpdateUIEvent& event);
    void OnForwardEvent(wxCommandEvent& event);
    void OnInsertCharacter(wxCommandEvent& event);
    void OnSendUpdate(wxUpdateUIEvent& event);
    void OnSendComposed(wxCommandEvent& event);
    void OnSendDecomposed(wxCommandEvent& event);
    void OnSendAbort(wxCommandEvent& event);
    void OnSettings(wxCommandEvent& event);
    virtual void OnIdle(wxIdleEvent& event);
    void OnTaskbarIconClick(wxTaskBarIconEvent& event);
    virtual void OnIconize(wxIconizeEvent& event);
    void OnToolbarEditUpdate(wxUpdateUIEvent& event);
    void OnToolbarEdit(wxCommandEvent& event);
    void OnToolbarComposeUpdate(wxUpdateUIEvent& event);
    void OnToolbarCompose(wxCommandEvent& event);
    void OnPanelCharacterCatalogUpdate(wxUpdateUIEvent& event);
    void OnPanelCharacterCatalog(wxCommandEvent& event);
    void OnPanelCharacterCatalogFocus(wxCommandEvent& event);
    void OnHelpReqChar(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    wxDECLARE_EVENT_TABLE();

protected:
#if defined(__WXMSW__)
    ITfSource *m_tfSource;  ///< Text Services install sink helper
    DWORD m_dwCookie;       ///< Text Services installed sink cookie

    // ITfLanguageProfileNotifySink implementation
    virtual HRESULT STDMETHODCALLTYPE OnLanguageChange(LANGID langid, __RPC__out BOOL *pfAccept);
    virtual HRESULT STDMETHODCALLTYPE OnLanguageChanged();

    // IUnknown implementation
    virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, __RPC__deref_out void __RPC_FAR *__RPC_FAR *ppvObject);
    virtual ULONG STDMETHODCALLTYPE AddRef();
    virtual ULONG STDMETHODCALLTYPE Release();
    ULONG m_ulRefCount;     ///< COM object reference count
#endif

private:
    void DoSend(const wxString& str);

protected:
#ifdef __WXMSW__
    virtual WXLRESULT MSWWindowProc(WXUINT message, WXWPARAM wParam, WXLPARAM lParam);
#endif

protected:
    WXHWND m_hWndSource;                ///< Handle of the active window, when the ZRCola hotkey was pressed
    wxTaskBarIcon *m_taskBarIcon;       ///< Taskbar icon
    wxZRColaCharSelect *m_chrSelect;    ///< Character selection dialog
    wxZRColaSettings *m_settings;       ///< Configuration dialog
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
