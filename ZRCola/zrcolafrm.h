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
class wxZRColaFrame;
class wxPersistentZRColaFrame;


#pragma once

#include "zrcolagui.h"
#include "zrcolachrslct.h"
#include "zrcolachrreq.h"
#include "zrcolasettings.h"
#include "zrcolatranseq.h"
#include <zrcola/language.h>
#include <zrcola/translate.h>
#include <wxex/persist/toplevel.h>
#include <wx/taskbar.h>
#if defined(__WXMSW__)
#include <msctf.h>
#endif


///
/// Global hotkey message identifiers
///
#define wxZRColaHKID_INVOKE_TRANSLATE       0
#define wxZRColaHKID_INVOKE_TRANSLATE_INV   1


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
    void OnSendDestination(wxCommandEvent& event);
    void OnCopyDestinationAndReturn(wxCommandEvent& event);
    void OnSendSource(wxCommandEvent& event);
    void OnCopySourceAndReturn(wxCommandEvent& event);
    void OnSendAbort(wxCommandEvent& event);
    void OnSettings(wxCommandEvent& event);
    virtual void OnIdle(wxIdleEvent& event);
    void OnTaskbarIconClick(wxTaskBarIconEvent& event);
    virtual void OnIconize(wxIconizeEvent& event);
    void OnToolbarEditUpdate(wxUpdateUIEvent& event);
    void OnToolbarEdit(wxCommandEvent& event);
    void OnToolbarTranslateUpdate(wxUpdateUIEvent& event);
    void OnToolbarTranslate(wxCommandEvent& event);
    virtual void OnTranslationSeqChoice(wxCommandEvent& event);
    void OnPanelCharacterCatalogUpdate(wxUpdateUIEvent& event);
    void OnPanelCharacterCatalog(wxCommandEvent& event);
    void OnPanelCharacterCatalogFocus(wxCommandEvent& event);
    void OnHelpInstructions(wxCommandEvent& event);
    void OnHelpShortcuts(wxCommandEvent& event);
    void OnHelpReqChar(wxCommandEvent& event);
    void OnHelpUpdate(wxCommandEvent& event);
    void OnHelpAbout(wxCommandEvent& event);
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
    void DoCopyAndReturn(const wxString& str);

protected:
#ifdef __WXMSW__
    virtual WXLRESULT MSWWindowProc(WXUINT message, WXWPARAM wParam, WXLPARAM lParam);
#endif

protected:
    WXHWND m_hWndSource;                ///< Handle of the active window, when the ZRCola hotkey was pressed
    wxTaskBarIcon *m_taskBarIcon;       ///< Taskbar icon
    wxZRColaCharSelect *m_chrSelect;    ///< Character selection dialog
    wxZRColaCharRequest *m_chrReq;      ///< Request a New Character dialog
    wxZRColaSettings *m_settings;       ///< Configuration dialog
    ZRCola::transeqid_t m_transeq_id;   ///< Translation sequence ID
    wxZRColaTranslationSeq *m_transeq;  ///< Custom translation sequence dialog
};


///
/// Supports saving/restoring wxZRColaFrame GUI state
///
class wxPersistentZRColaFrame : public wxPersistentTLWEx
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
