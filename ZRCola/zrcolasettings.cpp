/*
    SPDX-License-Identifier: GPL-3.0-or-later
    Copyright © 2015-2021 Amebis
*/

#include "pch.h"


//////////////////////////////////////////////////////////////////////////
// wxZRColaSettings
//////////////////////////////////////////////////////////////////////////

wxZRColaSettings::wxZRColaSettings(wxWindow* parent) :
    m_lang_auto(true),
    m_lang(ZRCola::langid_t::blank),
    wxZRColaSettingsBase(parent)
{
    auto app = dynamic_cast<ZRColaApp*>(wxTheApp);
    m_languages->Clear();
    for (size_t i = 0, n = app->m_lang_db.idxLang.size(); i < n; i++) {
        const auto &lang = app->m_lang_db.idxLang[i];
        wxString
            label(lang.name(), lang.name_len()),
            label_tran(wxGetTranslation(label, wxT("ZRCola-zrcdb")));
        m_languages->Insert(label_tran, (unsigned int)i);
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
    m_languages->Select(app->m_lang_db.idxLang.find(ZRCola::language_db::language(m_lang), start) ? (int)start : -1);
}


void wxZRColaSettings::OnLanguageUpdate(wxUpdateUIEvent& event)
{
    m_languages->Enable(m_langManual->GetValue());

    wxZRColaSettingsBase::OnLanguageUpdate(event);
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

            // Notify destination text something changed and should re-inverse translate.
            wxCommandEvent event2(wxEVT_COMMAND_TEXT_UPDATED);
            app->m_mainWnd->m_panel->m_destination->ProcessWindowEvent(event2);
        }
    }
}


void wxZRColaSettings::OnOKButtonClick(wxCommandEvent& event)
{
    wxZRColaSettings::OnApplyButtonClick(event);
}


//////////////////////////////////////////////////////////////////////////
// wxPersistentZRColaSettings
//////////////////////////////////////////////////////////////////////////

const int wxPersistentZRColaSettings::s_guiLevel = 1;


wxPersistentZRColaSettings::wxPersistentZRColaSettings(wxZRColaSettings *wnd) : wxPersistentTLWEx(wnd)
{
}


void wxPersistentZRColaSettings::Save() const
{
    SaveValue(wxT("guiLevel"), s_guiLevel);
    wxPersistentTLWEx::Save();

    auto wnd = static_cast<const wxZRColaSettings*>(GetWindow()); // dynamic_cast is not reliable as we are typically called late in the wxTopLevelWindowMSW destructor.

    SaveValue(wxT("langAuto"), wnd->m_lang_auto);
    SaveValue(wxT("lang"    ), wxString::FromAscii(wnd->m_lang.data, _countof(wnd->m_lang.data)));
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
        wnd->m_lang = app->m_lang_db.idxLang[0].lang;
    } else
        wnd->m_lang = ZRCola::langid_t::blank;

    int guiLevel;
    return RestoreValue(wxT("guiLevel"), &guiLevel) && guiLevel == s_guiLevel ? wxPersistentTLWEx::Restore() : true;
}
