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
#include <zrcola/language.h>
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
        wxID_DECOMP_LANGUAGE_START  = 6000,
        wxID_DECOMP_LANGUAGE_END    = 6099,
        wxID_FOCUS_CHARACTER_CATALOG,
    };

    wxZRColaFrame();
    virtual ~wxZRColaFrame();

    friend class wxPersistentZRColaFrame;
    friend class wxZRColaComposerPanel;

protected:
    void OnAutostartUpdate(wxUpdateUIEvent& event);
    void OnAutostart(wxCommandEvent& event);
    void OnExit(wxCommandEvent& event);
    void OnForwardEventUpdate(wxUpdateUIEvent& event);
    void OnForwardEvent(wxCommandEvent& event);
    void OnSendUpdate(wxUpdateUIEvent& event);
    void OnSendComposed(wxCommandEvent& event);
    void OnSendDecomposed(wxCommandEvent& event);
    void OnSendAbort(wxCommandEvent& event);
    void OnDecomposedLanguageAutoUpdate(wxUpdateUIEvent& event);
    void OnDecomposedLanguageAuto(wxCommandEvent& event);
    void OnDecomposedLanguageUpdate(wxUpdateUIEvent& event);
    void OnDecomposedLanguage(wxCommandEvent& event);
    virtual void OnDecompLanguageChoice(wxCommandEvent& event);
    void OnToolbarEditUpdate(wxUpdateUIEvent& event);
    void OnToolbarEdit(wxCommandEvent& event);
    void OnToolbarComposeUpdate(wxUpdateUIEvent& event);
    void OnToolbarCompose(wxCommandEvent& event);
    void OnPanelCharacterCatalogUpdate(wxUpdateUIEvent& event);
    void OnPanelCharacterCatalog(wxCommandEvent& event);
    void OnPanelCharacterCatalogFocus(wxCommandEvent& event);
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
    void UpdateDecomposedLanguage();

protected:
#ifdef __WXMSW__
    virtual WXLRESULT MSWWindowProc(WXUINT message, WXWPARAM wParam, WXLPARAM lParam);
#endif

protected:
    bool m_lang_auto;               ///< Automatic language selection according to keyboard layout
    ZRCola::langid_t m_lang;        ///< Language for decomposing
    WXHWND m_hWndSource;            ///< handle of the active window, when the ZRCola hotkey was pressed
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
