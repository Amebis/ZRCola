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
    m_lang(ZRCola::langid_t_blank),
    wxZRColaSettingsBase(parent)
{
    ZRColaApp *app = ((ZRColaApp*)wxTheApp);
    m_languages->Clear();
    for (size_t i = 0, n = app->m_lang_db.idxLng.size(); i < n; i++) {
        const ZRCola::language_db::language &lang = app->m_lang_db.idxLng[i];
        wxString
            label(lang.name, lang.name_len),
            label_tran(wxGetTranslation(label, wxT("ZRCola-zrcdb")));
        m_languages->Insert(label_tran, i);
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

    ZRColaApp *app = ((ZRColaApp*)wxTheApp);
    char l[sizeof(ZRCola::language_db::language)] = {};
    ((ZRCola::language_db::language*)l)->id = m_lang;
    ZRCola::language_db::indexLang::size_type start;
    m_languages->Select(app->m_lang_db.idxLng.find(*(ZRCola::language_db::language*)l, start) ? start : -1);
}


void wxZRColaSettings::OnLangAuto(wxCommandEvent& event)
{
    m_languages->Enable(!event.IsChecked());
}


void wxZRColaSettings::OnLangManual(wxCommandEvent& event)
{
    m_languages->Enable(event.IsChecked());
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

        ZRColaApp *app = ((ZRColaApp*)wxTheApp);
        const ZRCola::language_db::language &lang = app->m_lang_db.idxLng[m_languages->GetSelection()];

        if (m_lang != lang.id) {
            m_lang = lang.id;

            // Notify composed text something changed and should re-decompose.
            wxCommandEvent event2(wxEVT_COMMAND_TEXT_UPDATED);
            app->m_mainWnd->m_panel->m_composed->ProcessWindowEvent(event2);
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

wxPersistentZRColaSettings::wxPersistentZRColaSettings(wxZRColaSettings *wnd) : wxPersistentDialog(wnd)
{
}


void wxPersistentZRColaSettings::Save() const
{
    wxPersistentDialog::Save();

    const wxZRColaSettings * const wnd = static_cast<const wxZRColaSettings*>(GetWindow());

    SaveValue(wxT("langAuto"), wnd->m_lang_auto);
    SaveValue(wxT("lang"    ), wxString::FromAscii(wnd->m_lang.data, _countof(wnd->m_lang.data)));
}


bool wxPersistentZRColaSettings::Restore()
{
    wxZRColaSettings * const wnd = static_cast<wxZRColaSettings*>(GetWindow());

    ZRColaApp *app = ((ZRColaApp*)wxTheApp);
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
    } else if (!app->m_lang_db.idxLng.empty()) {
        const ZRCola::language_db::language &lang = app->m_lang_db.idxLng[0];
        wnd->m_lang = lang.id;
    } else
        wnd->m_lang = ZRCola::langid_t_blank;

    return wxPersistentDialog::Restore();
}
