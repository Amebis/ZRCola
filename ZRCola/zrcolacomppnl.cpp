/*
    SPDX-License-Identifier: GPL-3.0-or-later
    Copyright © 2015-2021 Amebis
*/

#include "pch.h"


//////////////////////////////////////////////////////////////////////////
// wxZRColaComposerPanel
//////////////////////////////////////////////////////////////////////////

wxZRColaComposerPanel::wxZRColaComposerPanel(wxWindow* parent) :
    m_sourceChanged(false),
    m_destinationChanged(false),
    m_sourceRestyled(false),
    m_destinationRestyled(false),
    m_styleNormal(*wxBLACK, *wxWHITE, wxFont(20, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxT("ZRCola"))),
    m_stylePUA(*wxBLUE, *wxWHITE, wxFont(20, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxT("ZRCola"))),
    m_selSource(0, 0),
    m_selDestination(0, 0),
    wxZRColaComposerPanelBase(parent)
{
    // RichEdit control has no inner margins by default.
    m_source->SetMargins(FromDIP(wxPoint(5, 2)));
    m_destination->SetMargins(FromDIP(wxPoint(5, 2)));

    m_source->PushEventHandler(&m_keyhandler);
}


wxZRColaComposerPanel::~wxZRColaComposerPanel()
{
    m_source->PopEventHandler();

    // This is a controlled exit. Purge saved state.
    wxString fileName(GetStateFileName());
    if (wxFileExists(fileName))
        wxRemoveFile(fileName);
}


void wxZRColaComposerPanel::RestoreFromStateFile()
{
    // Restore the previously saved state (if exists).
    wxString fileName(GetStateFileName());
    if (wxFileExists(fileName)) {
        wxFFile file(fileName, wxT("rb"));
        if (file.IsOpened()) {
            // Load source text.
            unsigned __int64 n;
            file.Read(&n, sizeof(n));
            if (!file.Error()) {
                wxString source;
                file.Read(wxStringBuffer(source, (size_t)n), sizeof(wchar_t)*(size_t)n);
                if (!file.Error()) {
                    // Load destination text.
                    file.Read(&n, sizeof(n));
                    if (!file.Error()) {
                        wxString destination;
                        file.Read(wxStringBuffer(destination, (size_t)n), sizeof(wchar_t)*(size_t)n);
                        if (!file.Error()) {
                            // Restore state.
                            m_source->SetValue(source);
                            m_source->GetSelection(&m_selSource.first, &m_selSource.second);
                            SetHexValue(m_sourceHex, m_selSourceHex, m_mappingSourceHex, source.GetData(), source.Length(), m_selSource.first, m_selSource.second);
                            m_sourceChanged = false;

                            m_destination->SetValue(destination);
                            m_destination->GetSelection(&m_selDestination.first, &m_selDestination.second);
                            SetHexValue(m_destinationHex, m_selDestinationHex, m_mappingDestinationHex, destination.GetData(), destination.Length(), m_selDestination.first, m_selDestination.second);
                            m_destinationChanged = false;
                        }
                    }
                }
            }
        }
    }
}


void wxZRColaComposerPanel::SynchronizePanels()
{
    if (m_sourceChanged) {
        m_timerSave.Stop();

        auto app = dynamic_cast<ZRColaApp*>(wxTheApp);

        wxString src = m_source->GetValue();
        size_t len = src.Length();
        std::wstring dst(src.data(), len), dst2;
        ZRCola::mapping_vector map;

        m_mapping.clear();

        if (app->m_mainWnd->m_composition) {
            // ZRCola decompose first, then re-compose.
            app->m_t_db.TranslateInv(ZRCOLA_TRANSETID_DEFAULT, dst.data(), dst.size(), dst2, &map);
            m_mapping.push_back(std::move(map));

            app->m_t_db.Translate(ZRCOLA_TRANSETID_DEFAULT, dst2.data(), dst2.size(), dst, &map);
            m_mapping.push_back(std::move(map));
        }

        // Other translations
        const ZRCola::transetid_t *sets_begin, *sets_end;
        GetTranslationSeq(sets_begin, sets_end);
        for (auto s = sets_begin; s != sets_end; ++s) {
            app->m_t_db.Translate(*s, dst.data(), dst.size(), dst2, &map);
            m_mapping.push_back(std::move(map));
            dst = std::move(dst2);
        }

        m_source->GetSelection(&m_selSource.first, &m_selSource.second);

        // Update source HEX dump.
        SetHexValue(m_sourceHex, m_selSourceHex, m_mappingSourceHex, src.data(), len, m_selSource.first, m_selSource.second);

        // Update destination text, and its HEX dump.
        m_destination->SetValue(dst);
        m_destination->SetSelection(
            m_selDestination.first  = (long)MapToDestination(m_selSource.first ),
            m_selDestination.second = (long)MapToDestination(m_selSource.second));
        SetHexValue(m_destinationHex, m_selDestinationHex, m_mappingDestinationHex, dst.data(), dst.length(), m_selDestination.first, m_selDestination.second);

        // Schedule state save after 3s.
        m_timerSave.Start(3000, true);
    } else if (m_destinationChanged) {
        m_timerSave.Stop();

        auto app = dynamic_cast<ZRColaApp*>(wxTheApp);

        wxString src = m_destination->GetValue();
        size_t len = src.Length();
        std::wstring dst(src.data(), len), dst2;
        ZRCola::mapping_vector map;

        m_mapping.clear();

        // Other translations
        const ZRCola::transetid_t *sets_begin, *sets_end;
        GetTranslationSeq(sets_begin, sets_end);
        for (auto s = sets_end; (s--) != sets_begin;) {
            app->m_t_db.TranslateInv(*s, dst.data(), dst.size(), dst2, &map);
            dst = std::move(dst2);

            map.invert();
            m_mapping.push_back(std::move(map));
        }

        if (app->m_mainWnd->m_composition) {
            // ZRCola decompose.
            app->m_t_db.TranslateInv(ZRCOLA_TRANSETID_DEFAULT, dst.data(), dst.size(), &app->m_lc_db, app->m_mainWnd->m_settings->m_lang, dst2, &map);
            dst = std::move(dst2);

            map.invert();
            m_mapping.push_back(std::move(map));
        }

        m_destination->GetSelection(&m_selDestination.first, &m_selDestination.second);

        // Update destination HEX dump.
        SetHexValue(m_destinationHex, m_selDestinationHex, m_mappingDestinationHex, src.data(), len, m_selDestination.first, m_selDestination.second);

        // Update source text, and its HEX dump.
        m_source->SetValue(dst);
        m_source->SetSelection(
            m_selSource.first  = (long)MapToSource(m_selDestination.first ),
            m_selSource.second = (long)MapToSource(m_selDestination.second));
        SetHexValue(m_sourceHex, m_selSourceHex, m_mappingSourceHex, dst.data(), dst.length(), m_selSource.first, m_selSource.second);

        // Schedule state save after 3s.
        m_timerSave.Start(3000, true);
    }

    m_sourceChanged      = false;
    m_destinationChanged = false;
}


void wxZRColaComposerPanel::OnSourcePaint(wxPaintEvent& event)
{
    event.Skip();

    if (m_sourceRestyled)
        return;

    long from, to;
    m_source->GetSelection(&from, &to);

    if (m_selSource.first != from || m_selSource.second != to) {
        // Save new selection first, to avoid loop.
        m_selSource.first  = from;
        m_selSource.second = to;

        m_sourceHex->SetSelection(
            m_selSourceHex.first  = (long)m_mappingSourceHex.to_dst(from),
            m_selSourceHex.second = (long)m_mappingSourceHex.to_dst(to  ));

        m_destination->SetSelection(
            m_selDestination.first  = (long)MapToDestination(from),
            m_selDestination.second = (long)MapToDestination(to  ));

        m_destinationHex->SetSelection(
            m_selDestinationHex.first  = (long)m_mappingDestinationHex.to_dst(m_selDestination.first ),
            m_selDestinationHex.second = (long)m_mappingDestinationHex.to_dst(m_selDestination.second));
    }
}


void wxZRColaComposerPanel::OnSourceHexPaint(wxPaintEvent& event)
{
    event.Skip();

    long from, to;
    m_sourceHex->GetSelection(&from, &to);

    if (m_selSourceHex.first != from || m_selSourceHex.second != to) {
        // Save new selection first, to avoid loop.
        m_selSourceHex.first  = from;
        m_selSourceHex.second = to;

        m_source->SetSelection(
            m_selSource.first  = (long)m_mappingSourceHex.to_src(from),
            m_selSource.second = (long)m_mappingSourceHex.to_src(to  ));

        m_destination->SetSelection(
            m_selDestination.first  = (long)MapToDestination(m_selSource.first ),
            m_selDestination.second = (long)MapToDestination(m_selSource.second));

        m_destinationHex->SetSelection(
            m_selDestinationHex.first  = (long)m_mappingDestinationHex.to_dst(m_selDestination.first ),
            m_selDestinationHex.second = (long)m_mappingDestinationHex.to_dst(m_selDestination.second));
    }
}


void wxZRColaComposerPanel::OnSourceText(wxCommandEvent& event)
{
    event.Skip();

    if (m_sourceRestyled)
        return;

    // Set the flag the source text changed to trigger idle-time translation.
    m_sourceChanged = true;

    m_sourceRestyled = true;
    m_source->SetStyle(0, GetWindowTextLength(m_source->GetHWND()), m_styleNormal);
    m_sourceRestyled = false;
}


void wxZRColaComposerPanel::OnDestinationPaint(wxPaintEvent& event)
{
    event.Skip();

    if (m_destinationRestyled)
        return;

    long from, to;
    m_destination->GetSelection(&from, &to);

    if (m_selDestination.first != from || m_selDestination.second != to) {
        // Save new selection first, to avoid loop.
        m_selDestination.first  = from;
        m_selDestination.second = to;

        m_destinationHex->SetSelection(
            m_selDestinationHex.first  = (long)m_mappingDestinationHex.to_dst(from),
            m_selDestinationHex.second = (long)m_mappingDestinationHex.to_dst(to  ));

        m_source->SetSelection(
            m_selSource.first  = (long)MapToSource(from),
            m_selSource.second = (long)MapToSource(to  ));

        m_sourceHex->SetSelection(
            m_selSourceHex.first  = (long)m_mappingSourceHex.to_dst(m_selSource.first ),
            m_selSourceHex.second = (long)m_mappingSourceHex.to_dst(m_selSource.second));
    }
}


void wxZRColaComposerPanel::OnDestinationHexPaint(wxPaintEvent& event)
{
    event.Skip();

    long from, to;
    m_destinationHex->GetSelection(&from, &to);

    if (m_selDestinationHex.first != from || m_selDestinationHex.second != to) {
        // Save new selection first, to avoid loop.
        m_selDestinationHex.first  = from;
        m_selDestinationHex.second = to;

        m_destination->SetSelection(
            m_selDestination.first  = (long)m_mappingDestinationHex.to_src(from),
            m_selDestination.second = (long)m_mappingDestinationHex.to_src(to  ));

        m_source->SetSelection(
            m_selSource.first  = (long)MapToSource(m_selDestination.first ),
            m_selSource.second = (long)MapToSource(m_selDestination.second));

        m_sourceHex->SetSelection(
            m_selSourceHex.first  = (long)m_mappingSourceHex.to_dst(m_selSource.first ),
            m_selSourceHex.second = (long)m_mappingSourceHex.to_dst(m_selSource.second));
    }
}


void wxZRColaComposerPanel::OnDestinationText(wxCommandEvent& event)
{
    event.Skip();

    if (m_destinationRestyled)
        return;

    // Set the flag the destination text changed to trigger idle-time inverse translation.
    m_destinationChanged = true;

    auto app = dynamic_cast<ZRColaApp*>(wxTheApp);
    m_destinationRestyled = true;
    if (app->m_mainWnd->m_warnPUA) {
        wxString src = m_destination->GetValue();
        size_t len = src.Length();
        for (size_t i = 0, j; i < len;) {
            bool pua_i = ZRCola::ispua(src[i]);
            for (j = i + 1; j < len && pua_i == ZRCola::ispua(src[j]); j++);
            m_destination->SetStyle((long)i, (long)j, pua_i ? m_stylePUA : m_styleNormal);
            i = j;
        }
    } else
        m_destination->SetStyle(0, GetWindowTextLength(m_destination->GetHWND()), m_styleNormal);
    m_destinationRestyled = false;
}


void wxZRColaComposerPanel::OnSaveTimer(wxTimerEvent& event)
{
    wxString fileName(GetStateFileName());
    wxFFile file(fileName, wxT("wb"));
    if (file.IsOpened()) {
        {
            // Save source text.
            wxString text = m_source->GetValue();
            size_t len = text.Length();
            file.Write(&len, sizeof(len));
            file.Write((const wchar_t*)text, sizeof(wchar_t)*len);
        }

        {
            // Save destination text.
            wxString text = m_destination->GetValue();
            size_t len = text.Length();
            file.Write(&len, sizeof(len));
            file.Write((const wchar_t*)text, sizeof(wchar_t)*len);
        }
    }

    event.Skip();
}


wxString wxZRColaComposerPanel::GetStateFileName()
{
    wxString path;

    path = wxFileName::GetTempDir();
    if (!wxEndsWithPathSeparator(path))
        path += wxFILE_SEP_PATH;

    if (!wxDirExists(path))
        wxMkdir(path);

    wxString fileName(path);
    fileName += wxT("ZRColaComposerPanel-state.tmp");

    return fileName;
}


void wxZRColaComposerPanel::SetHexValue(wxTextCtrl *wnd, std::pair<long, long> &range, ZRCola::mapping_vector &mapping, const wchar_t *src, size_t len, long from, long to)
{
    wxString hex;
    bool first = true;

    mapping.clear();
    for (size_t i = 0; i < len && src[i]; i++) {
        wchar_t c = src[i];
        if (c == L'\n') {
            hex += L"\r\n";
            first = true;
        } else {
            hex += wxString::Format(first ? wxT("%04X") : wxT(" %04X"), src[i]);
            mapping.push_back(ZRCola::mapping(i + 1, hex.Length()));
            first = false;
        }
    }

    wnd->SetValue(hex);
    wnd->SetSelection(
        range.first  = (long)mapping.to_dst(from),
        range.second = (long)mapping.to_dst(to  ));
}


//////////////////////////////////////////////////////////////////////////
// wxPersistentZRColaComposerPanel
//////////////////////////////////////////////////////////////////////////

const int wxPersistentZRColaComposerPanel::s_guiLevel = 1;


wxPersistentZRColaComposerPanel::wxPersistentZRColaComposerPanel(wxZRColaComposerPanel *wnd) : wxPersistentWindow<wxZRColaComposerPanel>(wnd)
{
}


wxString wxPersistentZRColaComposerPanel::GetKind() const
{
    return wxT(wxPERSIST_TLW_KIND);
}


void wxPersistentZRColaComposerPanel::Save() const
{
    auto wnd = static_cast<wxZRColaComposerPanel*>(GetWindow()); // dynamic_cast is not reliable as we are typically called late in the wxTopLevelWindowMSW destructor.

    SaveValue(wxT("guiLevel"       ), s_guiLevel);
    SaveValue(wxT("dpiX"           ), wxClientDC(wnd).GetPPI().x);
    SaveValue(wxT("splitDecomposed"), wnd->m_splitterSource     ->GetSashPosition());
    SaveValue(wxT("splitComposed"  ), wnd->m_splitterDestination->GetSashPosition());
}


bool wxPersistentZRColaComposerPanel::Restore()
{
    auto wnd = dynamic_cast<wxZRColaComposerPanel*>(GetWindow());

    int guiLevel;
    if (!RestoreValue(wxT("guiLevel"), &guiLevel) || guiLevel != s_guiLevel)
        return true;

    int dpiHorz = wxClientDC(wnd).GetPPI().x;
    int dpiHorzVal;
    int sashVal;

    if (!RestoreValue(wxT("dpiX"), &dpiHorzVal))
        dpiHorzVal = 96;

    if (RestoreValue(wxT("splitDecomposed"), &sashVal)) {
        // wxFormBuilder sets initial splitter stash in idle event handler after GUI settles. Overriding our loaded value. Disconnect it's idle event handler.
        wnd->m_splitterSource->Disconnect( wxEVT_IDLE, wxIdleEventHandler( wxZRColaComposerPanelBase::m_splitterSourceOnIdle ), NULL, wnd );
        wnd->m_splitterSource->SetSashPosition(wxMulDivInt32(sashVal, dpiHorz, dpiHorzVal));
    }

    if (RestoreValue(wxT("splitComposed"), &sashVal)) {
        // wxFormBuilder sets initial splitter stash in idle event handler after GUI settles. Overriding our loaded value. Disconnect it's idle event handler.
        wnd->m_splitterDestination->Disconnect( wxEVT_IDLE, wxIdleEventHandler( wxZRColaComposerPanelBase::m_splitterDestinationOnIdle ), NULL, wnd );
        wnd->m_splitterDestination->SetSashPosition(wxMulDivInt32(sashVal, dpiHorz, dpiHorzVal));
    }

    return true;
}
