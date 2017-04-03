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

#include "stdafx.h"


//////////////////////////////////////////////////////////////////////////
// wxZRColaSettings
//////////////////////////////////////////////////////////////////////////

wxZRColaSettings::wxZRColaSettings(wxWindow* parent) :
    m_lang_auto(true),
    m_lang(ZRCola::langid_t::blank),
    m_transeq(1, 0), // By default use translation set 0 (ZRCola Decomposed >> ZRCola Composed)
    wxZRColaSettingsBase(parent)
{
    auto app = dynamic_cast<ZRColaApp*>(wxTheApp);
    m_languages->Clear();
    for (size_t i = 0, n = app->m_lang_db.idxLang.size(); i < n; i++) {
        const auto &lang = app->m_lang_db.idxLang[i];
        wxString
            label(lang.name(), lang.name_len()),
            label_tran(wxGetTranslation(label, wxT("ZRCola-zrcdb")));
        m_languages->Insert(label_tran, i);
    }

    m_transAvailable->Clear();
    for (size_t i = 0, n = app->m_ts_db.idxTranSet.size(); i < n; i++) {
        const ZRCola::transet_db::transet &ts = app->m_ts_db.idxTranSet[i];
        wxString
            src(ts.src(), ts.src_len()),
            dst(ts.dst(), ts.dst_len()),
            src_tran(wxGetTranslation(src, wxT("ZRCola-zrcdb"))),
            dst_tran(wxGetTranslation(dst, wxT("ZRCola-zrcdb"))),
            label_tran(wxString::Format(wxT("%s \x00bb %s"), src_tran.c_str(), dst_tran.c_str()));
        m_transAvailable->Append(label_tran, (void*)ts.set);
    }
}


void wxZRColaSettings::OnInitDialog(wxInitDialogEvent& event)
{
    event.Skip();

    // Set state of auto-start according to Startup folder shortcut presence.
#if defined(__WXMSW__)
    wxString linkName(wxExpandEnvVars("%APPDATA%\\Microsoft\\Windows\\Start Menu\\Programs\\Startup\\ZRCola.lnk"));
    m_autoStart->SetValue(wxFileExists(linkName));
#else
    m_autoStart->SetValue(false);
    m_panelAutoStart->Enable(false);
#endif

    m_languages->Enable(!m_lang_auto);
    (m_lang_auto ? m_langAuto : m_langManual)->SetValue(true);

    auto app = dynamic_cast<ZRColaApp*>(wxTheApp);
    ZRCola::language_db::indexLang::size_type start;
    m_languages->Select(app->m_lang_db.idxLang.find(ZRCola::language_db::language(m_lang), start) ? start : -1);

    m_transActive->Clear();
    for (auto s = m_transeq.cbegin(), s_end = m_transeq.cend(); s != s_end; ++s) {
        ZRCola::transet_db::indexTranSet::size_type start;
        if (app->m_ts_db.idxTranSet.find(ZRCola::transet_db::transet(*s), start)) {
            const ZRCola::transet_db::transet &ts = app->m_ts_db.idxTranSet[start];
            wxString
                src(ts.src(), ts.src_len()),
                dst(ts.dst(), ts.dst_len()),
                src_tran(wxGetTranslation(src, wxT("ZRCola-zrcdb"))),
                dst_tran(wxGetTranslation(dst, wxT("ZRCola-zrcdb"))),
                label_tran(wxString::Format(wxT("%s \x00bb %s"), src_tran.c_str(), dst_tran.c_str()));
            m_transActive->Append(label_tran, (void*)ts.set);
        } else {
            wxString label_tran(wxString::Format(_("Unknown transformation (%u)"), (unsigned int)*s));
            m_transActive->Append(label_tran, (void*)*s);
        }
    }
}


void wxZRColaSettings::OnLanguageUpdate(wxUpdateUIEvent& event)
{
    m_languages->Enable(m_langManual->GetValue());

    wxZRColaSettingsBase::OnLanguageUpdate(event);
}


void wxZRColaSettings::OnTransformationUpdate(wxUpdateUIEvent& event)
{
    m_transActivate->Enable(m_transAvailable->GetSelection() != wxNOT_FOUND);

    int idx = m_transActive->GetSelection();
    m_transDeactivate->Enable(idx != wxNOT_FOUND);
    m_transActiveUp  ->Enable(idx != wxNOT_FOUND && (unsigned int)idx > 0);
    m_transActiveDown->Enable(idx != wxNOT_FOUND && (unsigned int)idx < m_transActive->GetCount() - 1);

    wxZRColaSettingsBase::OnTransformationUpdate(event);
}


void wxZRColaSettings::OnTransActivate(wxCommandEvent& event)
{
    int idx = m_transAvailable->GetSelection();
    wxASSERT_MSG(idx != wxNOT_FOUND, wxT("no available transformation selected"));
    m_transActive->Append(
        m_transAvailable->GetString(idx),
        m_transAvailable->GetClientData(idx));

    wxZRColaSettingsBase::OnTransActivate(event);
}


void wxZRColaSettings::OnTransDeactivate(wxCommandEvent& event)
{
    int idx = m_transActive->GetSelection();
    wxASSERT_MSG(idx != wxNOT_FOUND, wxT("no active transformation selected"));
    m_transActive->Delete(idx);

    wxZRColaSettingsBase::OnTransDeactivate(event);
}


void wxZRColaSettings::OnTransActiveUp(wxCommandEvent& event)
{
    int idx = m_transActive->GetSelection();
    wxASSERT_MSG(idx != wxNOT_FOUND && (unsigned int)idx > 0, wxT("no or first active transformation selected"));
    wxString label = m_transActive->GetString(idx);
    void *data = m_transActive->GetClientData(idx);
    m_transActive->Delete(idx);
    m_transActive->Insert(label, idx - 1, data);
    m_transActive->Select(idx - 1);

    wxZRColaSettingsBase::OnTransActiveUp(event);
}


void wxZRColaSettings::OnTransActiveDown(wxCommandEvent& event)
{
    int idx = m_transActive->GetSelection();
    wxASSERT_MSG(idx != wxNOT_FOUND && (unsigned int)idx < m_transActive->GetCount() - 1, wxT("no or last active transformation selected"));
    wxString label = m_transActive->GetString(idx);
    void *data = m_transActive->GetClientData(idx);
    m_transActive->Delete(idx);
    m_transActive->Insert(label, idx + 1, data);
    m_transActive->Select(idx + 1);

    wxZRColaSettingsBase::OnTransActiveDown(event);
}


void wxZRColaSettings::OnApplyButtonClick(wxCommandEvent& event)
{
    event.Skip();

#if defined(__WXMSW__)
    wxString linkName(wxExpandEnvVars("%APPDATA%\\Microsoft\\Windows\\Start Menu\\Programs\\Startup\\ZRCola.lnk"));
    if (m_autoStart->IsChecked()) {
        // Create the shortcut.
        IShellLink *sl;
        HRESULT hr = ::CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (LPVOID*)&sl);
        if (SUCCEEDED(hr)) {
            // Setup ZRCola shortcut.
            sl->SetPath(wxTheApp->argv[0]);
            sl->SetDescription(_("Start ZRCola automatically on logon"));
            sl->SetShowCmd(SW_SHOWMINNOACTIVE);

            // Query IShellLink for the IPersistFile interface, used for saving the
            // shortcut in persistent storage.
            IPersistFile *pf;
            hr = sl->QueryInterface(IID_IPersistFile, (LPVOID*)&pf);
            if (SUCCEEDED(hr)) {
                // Save the link by calling IPersistFile::Save.
                hr = pf->Save(linkName, TRUE);
                pf->Release();
            }

            sl->Release();
        }
    } else if (wxFileExists(linkName)) {
        // The shortcut already exists. Remove it.
        wxRemoveFile(linkName);
    }
#endif

    if (m_langAuto->GetValue()) {
        m_lang_auto = true;
#if defined(__WXMSW__)
        // Set update keyboard language.
        HKL hkl = ::GetKeyboardLayout(0);
        ZRCola::LangConvert(LOWORD(hkl), m_lang);
#endif
    } else {
        m_lang_auto = false;

        auto app = dynamic_cast<ZRColaApp*>(wxTheApp);
        const auto &lang = app->m_lang_db.idxLang[m_languages->GetSelection()];

        if (m_lang != lang.lang) {
            m_lang = lang.lang;

            // Notify destination text something changed and should re-inverse transform.
            wxCommandEvent event2(wxEVT_COMMAND_TEXT_UPDATED);
            app->m_mainWnd->m_panel->m_destination->ProcessWindowEvent(event2);
        }
    }

    m_transeq.clear();
    for (unsigned int i = 0, n = m_transActive->GetCount(); i < n; i++)
        m_transeq.push_back((ZRCola::transetid_t)m_transActive->GetClientData(i));
}



void wxZRColaSettings::OnOKButtonClick(wxCommandEvent& event)
{
    wxZRColaSettings::OnApplyButtonClick(event);
}


//////////////////////////////////////////////////////////////////////////
// wxPersistentZRColaSettings
//////////////////////////////////////////////////////////////////////////

wxPersistentZRColaSettings::wxPersistentZRColaSettings(wxZRColaSettings *wnd) : wxPersistentDialog(wnd)
{
}


void wxPersistentZRColaSettings::Save() const
{
    wxPersistentDialog::Save();

    auto wnd = static_cast<const wxZRColaSettings*>(GetWindow()); // dynamic_cast is not reliable as we are typically called late in the wxTopLevelWindowMSW destructor.

    SaveValue(wxT("langAuto"), wnd->m_lang_auto);
    SaveValue(wxT("lang"    ), wxString::FromAscii(wnd->m_lang.data, _countof(wnd->m_lang.data)));

    wxString str;
    bool first = true;
    for (auto s = wnd->m_transeq.cbegin(), s_end = wnd->m_transeq.cend(); s != s_end; ++s, first = false)
        str += wxString::Format(first ? wxT("%u") : wxT("|%u"), *s);
    SaveValue(wxT("transeq"), str);
}


bool wxPersistentZRColaSettings::Restore()
{
    auto wnd = dynamic_cast<wxZRColaSettings*>(GetWindow());
    auto app = dynamic_cast<ZRColaApp*>(wxTheApp);
    wxString lang;

    // Restore automatic language detection setting first.
    RestoreValue(wxT("langAuto"), &(wnd->m_lang_auto));
    if (wnd->m_lang_auto) {
#if defined(__WXMSW__)
        // Set keyboard language.
        HKL hkl = ::GetKeyboardLayout(0);
        ZRCola::LangConvert(LOWORD(hkl), wnd->m_lang);
#endif
    } else if (RestoreValue(wxT("lang"), &lang) && lang.Length() == 3) {
        // The language was read from configuration.
        wnd->m_lang = lang.c_str();
    } else if (!app->m_lang_db.idxLang.empty()) {
        const auto &lang = app->m_lang_db.idxLang[0];
        wnd->m_lang = lang.lang;
    } else
        wnd->m_lang = ZRCola::langid_t::blank;

    wxString str;
    if (RestoreValue(wxT("transeq"), &str)) {
        std::vector<ZRCola::transetid_t> transeq;
        for (wxStringTokenizer tok(str, wxT("|")); tok.HasMoreTokens(); ) {
            unsigned long val = _tcstoul(tok.GetNextToken().c_str(), NULL, 10);
            for (size_t i = 0, n = app->m_ts_db.idxTranSet.size(); i < n; i++) {
                const ZRCola::transet_db::transet &cg = app->m_ts_db.idxTranSet[i];
                if (cg.set == val) {
                    transeq.push_back(val);
                    break;
                }
            }
        }
        if (!transeq.empty())
            wnd->m_transeq = std::move(transeq);
    }

    return wxPersistentDialog::Restore();
}
