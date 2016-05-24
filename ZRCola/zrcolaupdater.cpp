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
// wxZRColaUpdater
//////////////////////////////////////////////////////////////////////////

wxZRColaUpdater::wxZRColaUpdater(wxWindow* parent) :
    m_logOrig(NULL),
    m_updater(NULL),
    wxZRColaUpdaterBase(parent)
{
    // Setup logging.
    m_logOrig = wxLog::SetActiveTarget(new wxLogTextCtrl(m_log));

    // Connect events.
    Connect(wxID_ANY, wxEVT_UPDATER_CHECK_COMPLETE, wxThreadEventHandler(wxZRColaUpdater::OnCheckComplete), NULL, this);

    // Launch Updater thread.
    ZRColaApp *app = (ZRColaApp*)wxTheApp;
    m_updater = new wxUpdCheckThread(app->m_locale.GetCanonicalName(), this);
    if (m_updater->Run() != wxTHREAD_NO_ERROR) {
        wxFAIL_MSG(wxT("Can't create the thread!"));
        delete m_updater;
        m_updater = NULL;
    }
}


wxZRColaUpdater::~wxZRColaUpdater()
{
    if (m_updater) {
        // Terminate the Updater thread.
        m_updater->Delete();
    }

    // Disconnect events.
    Disconnect(wxID_ANY, wxEVT_UPDATER_CHECK_COMPLETE, wxThreadEventHandler(wxZRColaUpdater::OnCheckComplete), NULL, this);

    if (m_logOrig) {
        // Return logging to previous state.
        delete wxLog::SetActiveTarget(m_logOrig);
    }
}


void wxZRColaUpdater::OnCheckComplete(wxThreadEvent& event)
{
    event.Skip();

    m_buttonUpdate->Enable(event.GetInt() == wxUpdCheckThread::wxUpdUpdateAvailable);
}


void wxZRColaUpdater::OnUpdate(wxCommandEvent& event)
{
    event.Skip();

    if (m_updater)
        m_updater->LaunchUpdate();
}
