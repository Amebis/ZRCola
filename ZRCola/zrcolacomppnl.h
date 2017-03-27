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
class wxZRColaComposerPanel;


#pragma once

#include "zrcolagui.h"
#include "zrcolakeyhndlr.h"
#include <wx/persist/window.h>
#include <wx/timer.h>
#include <utility>


///
/// ZRCola composer panel
///
class wxZRColaComposerPanel : public wxZRColaComposerPanelBase
{
public:
    wxZRColaComposerPanel(wxWindow* parent);
    virtual ~wxZRColaComposerPanel();

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

    static wxString GetStateFileName();
    static size_t GetValue(wxTextCtrl *wnd, wxString &text);
    static void SetHexValue(wxTextCtrl *wnd, std::pair<long, long> &range, ZRCola::mapping_vector &mapping, const wchar_t *src, size_t len, long from, long to);

protected:
    bool m_sourceChanged;                           ///< Boolean flag to mark source text "dirty" to trigger translation
    bool m_destinationChanged;                      ///< Boolean flag to mark destination text "dirty" to trigger inverse translation
    ZRCola::mapping_vector m_mapping1;              ///< Character index mapping vector between source and normalized text
    ZRCola::mapping_vector m_mapping2;              ///< Character index mapping vector between normalized and destination text
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
};


inline wxPersistentObject *wxCreatePersistentObject(wxZRColaComposerPanel *wnd)
{
    return new wxPersistentZRColaComposerPanel(wnd);
}
