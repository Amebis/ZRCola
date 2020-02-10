/*
    Copyright 2015-2019 Amebis

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

#include "pch.h"


//////////////////////////////////////////////////////////////////////////
// wxZRColaCharRequest
//////////////////////////////////////////////////////////////////////////

wxZRColaCharRequest::wxZRColaCharRequest(wxWindow* parent) : wxZRColaCharRequestBase(parent)
{
    m_character->PushEventHandler(new wxZRColaKeyHandler());
    m_context->PushEventHandler(new wxZRColaKeyHandler());
}


wxZRColaCharRequest::~wxZRColaCharRequest()
{
    m_context->PopEventHandler(true);
    m_character->PopEventHandler(true);
}


void wxZRColaCharRequest::OnOKButtonClick(wxCommandEvent& event)
{
    event.Skip();

    const wxString chr = m_character->GetValue();
    const wxString ctx = m_context->GetValue();
    wxString body;

    body += _("Character") + wxT(": ");
    for (size_t i = 0, i_end = chr.Length(); i < i_end; i++) {
        if (chr[i] < 0x7f)
            body += chr[i];
        else
            body += wxString::Format(wxT("(U+%04X)"), chr[i]);
    }
    body += wxT('\n');
    if (!ctx.IsEmpty()) {
        body += _("Context") + wxT(": ");
        body += ctx;
        body += wxT('\n');
    }

    wxLaunchDefaultBrowser(wxString::Format(wxT("mailto:janos.jezovnik@zrc-sazu.si&subject=%s&body=%s"),
        wxURLEncode(wxString((wxString(wxT("[ZRCola] ")) + _("Request a New Character")).ToUTF8())),
        wxURLEncode(body.ToUTF8())
        ));
}


//////////////////////////////////////////////////////////////////////////
// wxPersistentZRColaCharRequest
//////////////////////////////////////////////////////////////////////////

const int wxPersistentZRColaCharRequest::s_guiLevel = 1;


wxPersistentZRColaCharRequest::wxPersistentZRColaCharRequest(wxZRColaCharRequest *wnd) : wxPersistentTLWEx(wnd)
{
}


void wxPersistentZRColaCharRequest::Save() const
{
    SaveValue(wxT("guiLevel"), s_guiLevel);
    wxPersistentTLWEx::Save();

    auto wnd = static_cast<const wxZRColaCharRequest*>(GetWindow()); // dynamic_cast is not reliable as we are typically called late in the wxTopLevelWindowMSW destructor.

    SaveValue(wxT("character"), wnd->m_character->GetValue());
    SaveValue(wxT("context"), wnd->m_context->GetValue());
}


bool wxPersistentZRColaCharRequest::Restore()
{
    auto wnd = dynamic_cast<wxZRColaCharRequest*>(GetWindow());

    wxString str;
    if (RestoreValue(wxT("character"), &str))
        wnd->m_character->SetValue(str);
    if (RestoreValue(wxT("context"), &str))
        wnd->m_context->SetValue(str);

    int guiLevel;
    return RestoreValue(wxT("guiLevel"), &guiLevel) && guiLevel == s_guiLevel ? wxPersistentTLWEx::Restore() : true;
}
