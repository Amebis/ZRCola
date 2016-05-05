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
    enum
    {
        wxID_TIMER = 1,
    };

    wxZRColaComposerPanel(wxWindow* parent);
    virtual ~wxZRColaComposerPanel();

    friend class wxPersistentZRColaComposerPanel;   // Allow saving/restoring window state.

protected:
    virtual void OnDecomposedPaint(wxPaintEvent& event);
    virtual void OnDecomposedHexPaint(wxPaintEvent& event);
    virtual void OnDecomposedText(wxCommandEvent& event);
    virtual void OnComposedPaint(wxPaintEvent& event);
    virtual void OnComposedHexPaint(wxPaintEvent& event);
    virtual void OnComposedText(wxCommandEvent& event);
    virtual void OnTimerTimeout(wxTimerEvent& event);
    DECLARE_EVENT_TABLE()

    static wxString GetStateFileName();
    static void GetHex(wxString &hex, ZRCola::mapping_vector &mapping, const wchar_t *src, size_t len);

protected:
    bool m_progress;                                ///< Boolean flag to avoid recursive updates of composed and decomposed text controls
    ZRCola::mapping_vector m_mapping1;              ///< Character index mapping vector between decomposed and normalized text
    ZRCola::mapping_vector m_mapping2;              ///< Character index mapping vector between normalized and composed text
    std::pair<long, long>
        m_selDecomposed,                            ///< Character index of selected text in decomposed text control
        m_selDecomposedHex,                         ///< Character index of selected text in decomposed HEX dump text control
        m_selComposed,                              ///< Character index of selected text in composed text control
        m_selComposedHex;                           ///< Character index of selected text in composed HEX dump text control
    wxZRColaKeyHandler m_keyhandler;                ///< Key handler for decomposed window
    wxTimer *m_timer;                               ///< Timer to trigger the state save
    ZRCola::mapping_vector m_mappingDecomposedHex;  ///< Character index mapping vector between decomposed text and its HEX dump
    ZRCola::mapping_vector m_mappingComposedHex;    ///< Character index mapping vector between composed text and its HEX dump
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
