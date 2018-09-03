/*
    Copyright 2015-2017 Amebis

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

#include "stdafx.h"


//////////////////////////////////////////////////////////////////////////
// wxZRColaCharacterCatalogPanel
//////////////////////////////////////////////////////////////////////////

BEGIN_EVENT_TABLE(wxZRColaCharacterCatalogPanel, wxZRColaCharacterCatalogPanelBase)
    EVT_MENU(wxZRColaCharacterCatalogPanel::wxID_FOCUS_DECOMPOSED, wxZRColaCharacterCatalogPanel::OnFocusDecomposed)
END_EVENT_TABLE()


wxZRColaCharacterCatalogPanel::wxZRColaCharacterCatalogPanel(wxWindow* parent) : wxZRColaCharacterCatalogPanelBase(parent)
{
    std::fstream dat((LPCTSTR)dynamic_cast<ZRColaApp*>(wxTheApp)->GetDatabaseFilePath(), std::ios_base::in | std::ios_base::binary);
    if (dat.good()) {
        if (stdex::idrec::find<ZRCola::recordid_t, ZRCola::recordsize_t, ZRCOLA_RECORD_ALIGN>(dat, ZRCOLA_DB_ID, sizeof(ZRCola::recordid_t))) {
            ZRCola::recordsize_t size;
            dat.read((char*)&size, sizeof(ZRCola::recordsize_t));
            if (dat.good()) {
                ZRCola::chrgrp_rec rec(m_cg_db);
                if (rec.find(dat, size)) {
                    dat >> rec;
                    if (!dat.good()) {
                        wxFAIL_MSG(wxT("Error reading character group data from ZRCola.zrcdb."));
                        m_cg_db.idxRnk.clear();
                        m_cg_db.data  .clear();
                    }
                } else
                    wxFAIL_MSG(wxT("ZRCola.zrcdb has no character group data."));
            }
        } else
            wxFAIL_MSG(wxT("ZRCola.zrcdb is not a valid ZRCola database."));
    }

    if (!m_cg_db.idxRnk.empty()) {
        // Populate character group list.
        for (size_t i = 0, n = m_cg_db.idxRnk.size(); i < n; i++) {
            const ZRCola::chrgrp_db::chrgrp &cg = m_cg_db.idxRnk[i];
            wxString
                label(cg.data, cg.name_len),
                label_tran2(wxGetTranslation(label, wxT("ZRCola-zrcdb")));
            m_choice->Insert(label_tran2, i);
        }
        m_cg_id = m_cg_db.idxRnk[0].id;
        m_choice->Select(0);

        Update();
    }

    // Register frame specific hotkey(s).
    {
        wxAcceleratorEntry entries[1];
        entries[0].Set(wxACCEL_NORMAL, WXK_ESCAPE, wxID_FOCUS_DECOMPOSED);
        SetAcceleratorTable(wxAcceleratorTable(_countof(entries), entries));
    }
}


wxZRColaCharacterCatalogPanel::~wxZRColaCharacterCatalogPanel()
{
}


void wxZRColaCharacterCatalogPanel::OnChoice(wxCommandEvent& event)
{
    const ZRCola::chrgrp_db::chrgrp &cg = m_cg_db.idxRnk[event.GetSelection()];

    if (m_cg_id != cg.id) {
        m_cg_id = cg.id;
        Update();
    }

    event.Skip();
}


void wxZRColaCharacterCatalogPanel::OnGridClick(wxGridEvent& event)
{
    auto app = dynamic_cast<ZRColaApp*>(wxTheApp);
    if (app->m_mainWnd) {
        app->m_mainWnd->m_panel->m_decomposed->WriteText(m_grid->GetCellValue(event.GetRow(), event.GetCol()));
        app->m_mainWnd->m_panel->m_decomposed->SetFocus();
    }

    event.Skip();
}


void wxZRColaCharacterCatalogPanel::OnGridKeyDown(wxKeyEvent& event)
{
    switch (event.GetKeyCode()) {
    case WXK_RETURN:
    case WXK_NUMPAD_ENTER:
        auto app = dynamic_cast<ZRColaApp*>(wxTheApp);
        if (app->m_mainWnd) {
            app->m_mainWnd->m_panel->m_decomposed->WriteText(m_grid->GetCellValue(m_grid->GetGridCursorRow(), m_grid->GetGridCursorCol()));
            app->m_mainWnd->m_panel->m_decomposed->SetFocus();

            event.StopPropagation();
            return;
        }
    }

    event.Skip();
}


void wxZRColaCharacterCatalogPanel::OnShowAll(wxCommandEvent& event)
{
    UNREFERENCED_PARAMETER(event);

    Update();

    event.Skip();
}


void wxZRColaCharacterCatalogPanel::OnFocusDecomposed(wxCommandEvent& event)
{
    auto app = dynamic_cast<ZRColaApp*>(wxTheApp);
    if (app->m_mainWnd) {
        app->m_mainWnd->m_panel->m_decomposed->SetFocus();

        event.StopPropagation();
        return;
    }

    event.Skip();
}


void wxZRColaCharacterCatalogPanel::Update()
{
    const ZRCola::chrgrp_db::chrgrp &cg = m_cg_db.idxRnk[m_choice->GetSelection()];

    if (m_show_all->GetValue()) {
        m_grid->SetCharacters(
            wxString(cg.get_chars(), cg.char_len),
            wxArrayShort((const short*)cg.get_char_shown(), (const short*)cg.get_char_shown() + (cg.char_len + 15)/16));
    } else {
        // Select frequently used characters only.
        const wchar_t *src = cg.get_chars();
        const unsigned __int16 *shown = cg.get_char_shown();
        wxString chars;
        for (unsigned __int16 i = 0, j = 0; i < cg.char_len; j++) {
            for (unsigned __int16 k = 0, mask = shown[j]; k < 16 && i < cg.char_len; k++, mask >>= 1, i++) {
                if (mask & 1)
                    chars += src[i];
            }
        }
        m_grid->SetCharacters(chars);
    }

    // As size of the grid might have changed, relayout the panel.
    Layout();
}


//////////////////////////////////////////////////////////////////////////
// wxPersistentZRColaCharacterCatalogPanel
//////////////////////////////////////////////////////////////////////////

wxPersistentZRColaCharacterCatalogPanel::wxPersistentZRColaCharacterCatalogPanel(wxZRColaCharacterCatalogPanel *wnd) : wxPersistentWindow<wxZRColaCharacterCatalogPanel>(wnd)
{
}


wxString wxPersistentZRColaCharacterCatalogPanel::GetKind() const
{
    return wxT(wxPERSIST_TLW_KIND);
}


void wxPersistentZRColaCharacterCatalogPanel::Save() const
{
    auto wnd = static_cast<const wxZRColaCharacterCatalogPanel*>(GetWindow()); // dynamic_cast is not reliable as we are typically called late in the wxTopLevelWindowMSW destructor.

    SaveValue(wxT("charGroup"), wnd->m_cg_id);
    SaveValue(wxT("showAll"  ), wnd->m_show_all->GetValue());
}


bool wxPersistentZRColaCharacterCatalogPanel::Restore()
{
    auto wnd = dynamic_cast<wxZRColaCharacterCatalogPanel*>(GetWindow());
    bool update = false;

    // Restore selected character group.
    int cg_id;
    if (RestoreValue(wxT("charGroup"), &cg_id)) {
        for (size_t i = 0, n = wnd->m_cg_db.idxRnk.size(); i < n; i++) {
            const ZRCola::chrgrp_db::chrgrp &cg = wnd->m_cg_db.idxRnk[i];
            if (cg.id == cg_id) {
                if (wnd->m_cg_id != cg.id) {
                    wnd->m_cg_id = cg.id;
                    wnd->m_choice->Select(i);
                    update = true;
                }

                break;
            }
        }
    }

    bool show_all;
    if (RestoreValue(wxT("showAll"), &show_all)) {
        if (wnd->m_show_all->GetValue() != show_all) {
            wnd->m_show_all->SetValue(show_all);
            update = true;
        }
    }

    if (update)
        wnd->Update();

    return true;
}
