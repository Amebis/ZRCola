/*
    SPDX-License-Identifier: GPL-3.0-or-later
    Copyright © 2015-2021 Amebis
*/

///
/// Forward declarations
///
class wxZRColaComposerPanel;


#pragma once

#include <wxex/common.h>
#pragma warning(push)
#pragma warning(disable: WXWIDGETS_CODE_ANALYSIS_WARNINGS)
#include "zrcolagui.h"
#pragma warning(pop)
#include "zrcolakeyhndlr.h"
#pragma warning(push)
#pragma warning(disable: WXWIDGETS_CODE_ANALYSIS_WARNINGS)
#include <wx/persist/window.h>
#include <wx/timer.h>
#pragma warning(pop)
#include <utility>
#include <vector>


///
/// ZRCola composer panel
///
class wxZRColaComposerPanel : public wxZRColaComposerPanelBase
{
public:
    wxZRColaComposerPanel(wxWindow* parent);
    virtual ~wxZRColaComposerPanel();

    void RestoreFromStateFile();
    void SynchronizePanels();

    friend class wxPersistentZRColaComposerPanel;   // Allow saving/restoring window state.

protected:
    virtual void OnSourcePaint(wxPaintEvent& event);
    virtual void OnSourceHexPaint(wxPaintEvent& event);
    virtual void OnSourceText(wxCommandEvent& event);
    virtual void OnDestinationPaint(wxPaintEvent& event);
    virtual void OnDestinationHexPaint(wxPaintEvent& event);
    virtual void OnDestinationText(wxCommandEvent& event);
    virtual void OnSaveTimer(wxTimerEvent& event);
    inline size_t MapToDestination(_In_ size_t src) const;
    inline size_t MapToSource(_In_ size_t dst) const;
    inline void GetTranslationSeq(_Out_ const ZRCola::transetid_t *&sets_begin, _Out_ const ZRCola::transetid_t *&sets_end) const;

    static wxString GetStateFileName();
    static void SetHexValue(wxTextCtrl *wnd, std::pair<long, long> &range, ZRCola::mapping_vector &mapping, const wchar_t *src, size_t len, long from, long to);

protected:
    bool m_sourceChanged;                           ///< Boolean flag to mark source text "dirty" to trigger translation
    bool m_destinationChanged;                      ///< Boolean flag to mark destination text "dirty" to trigger inverse translation
    volatile bool
        m_sourceRestyled,                           ///< Boolean flag to mark source text is being restyled
        m_destinationRestyled;                      ///< Boolean flag to mark destination text is being restyled
    wxTextAttr
        m_styleNormal,                              ///< Normal text style
        m_stylePUA;                                 ///< PUA character text style
    std::vector<ZRCola::mapping_vector> m_mapping;  ///< Character index mapping vector between source and normalized text
    std::pair<long, long>
        m_selSource,                                ///< Character index of selected text in source text control
        m_selSourceHex,                             ///< Character index of selected text in source HEX dump text control
        m_selDestination,                           ///< Character index of selected text in destination text control
        m_selDestinationHex;                        ///< Character index of selected text in destination HEX dump text control
    wxZRColaKeyHandler m_keyhandler;                ///< Key handler for source window
    ZRCola::mapping_vector m_mappingSourceHex;      ///< Character index mapping vector between source text and its HEX dump
    ZRCola::mapping_vector m_mappingDestinationHex; ///< Character index mapping vector between destination text and its HEX dump
};


///
/// Supports saving/restoring wxZRColaComposerPanel state
///
class wxPersistentZRColaComposerPanel : public wxPersistentWindow<wxZRColaComposerPanel>
{
public:
    wxPersistentZRColaComposerPanel(wxZRColaComposerPanel *wnd);

    virtual wxString GetKind() const;
    virtual void Save() const;
    virtual bool Restore();

protected:
    static const int s_guiLevel;    ///< Persisted placements/sizing are considered incompatible between different GUI levels
};


inline size_t wxZRColaComposerPanel::MapToDestination(_In_ size_t src) const
{
    for (auto m = m_mapping.cbegin(), m_end = m_mapping.cend(); m != m_end; ++m)
        src = m->to_dst(src);

    return src;
}


inline size_t wxZRColaComposerPanel::MapToSource(_In_ size_t dst) const
{
    for (auto m = m_mapping.crbegin(), m_end = m_mapping.crend(); m != m_end; ++m)
        dst = m->to_src(dst);

    return dst;
}


inline void wxZRColaComposerPanel::GetTranslationSeq(_Out_ const ZRCola::transetid_t *&sets_begin, _Out_ const ZRCola::transetid_t *&sets_end) const
{
    auto app = dynamic_cast<ZRColaApp*>(wxTheApp);

    if (app->m_mainWnd->m_transeq_id == ZRCOLA_TRANSEQID_DEFAULT) {
        sets_begin = NULL;
        sets_end   = NULL;
    } else if (app->m_mainWnd->m_transeq_id == ZRCOLA_TRANSEQID_CUSTOM) {
        sets_begin = app->m_mainWnd->m_transeq->m_transeq.data();
        sets_end   = sets_begin + app->m_mainWnd->m_transeq->m_transeq.size();
    } else {
        size_t start;
        if (app->m_tsq_db.idxTranSeq.find(ZRCola::transeq_db::transeq(app->m_mainWnd->m_transeq_id), start)) {
            const auto &ts = app->m_tsq_db.idxTranSeq[start];
            sets_begin = ts.sets();
            sets_end   = ts.sets_end();
        } else {
            sets_begin = NULL;
            sets_end   = NULL;
        }
    }
}


inline wxPersistentObject *wxCreatePersistentObject(wxZRColaComposerPanel *wnd)
{
    return new wxPersistentZRColaComposerPanel(wnd);
}
