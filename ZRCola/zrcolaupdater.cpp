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
    m_logLevelOrig(wxLOG_Warning),
    m_logOrig(NULL),
    m_updater(NULL),
    m_finished(false),
    wxZRColaUpdaterBase(parent)
{
    // Setup logging.
    m_logOrig = wxLog::SetActiveTarget(new wxLogTextCtrl(m_log));
    m_logLevelOrig = wxLog::GetLogLevel();
    wxLog::SetLogLevel(wxLOG_Info);

    //// Connect events.
    //Connect(wxID_ANY, wxEVT_UPDATER_CHECK_COMPLETE, wxThreadEventHandler(wxZRColaUpdater::OnCheckComplete), NULL, this);

    // Prepare Updater.
    auto app = dynamic_cast<ZRColaApp*>(wxTheApp);
    m_updater = new wxUpdCheckThread(app->m_lang_ui == wxLANGUAGE_DEFAULT ? wxT("en_US") : wxLocale::GetLanguageCanonicalName(app->m_lang_ui), this);
    //if (m_updater->Run() != wxTHREAD_NO_ERROR) {
    //    wxFAIL_MSG(wxT("Can't create the thread!"));
    //    delete m_updater;
    //    m_updater = NULL;
    //}
}


wxZRColaUpdater::~wxZRColaUpdater()
{
    if (m_updater) {
        // Terminate the Updater thread.
        //m_updater->Delete();
        delete m_updater;
    }

    //// Disconnect events.
    //Disconnect(wxID_ANY, wxEVT_UPDATER_CHECK_COMPLETE, wxThreadEventHandler(wxZRColaUpdater::OnCheckComplete), NULL, this);

    wxLog::SetLogLevel(m_logLevelOrig);
    if (m_logOrig) {
        // Return logging to previous state.
        delete wxLog::SetActiveTarget(m_logOrig);
    }
}


//void wxZRColaUpdater::OnCheckComplete(wxThreadEvent& event)
//{
//    event.Skip();
//
//    m_buttonUpdate->Enable(event.GetInt() == wxUpdCheckThread::wxUpdUpdateAvailable);
//}


void wxZRColaUpdater::OnIdle(wxIdleEvent& event)
{
    if (!m_finished) {
        m_buttonUpdate->Enable(m_updater->CheckForUpdate() == wxUpdCheckThread::wxUpdUpdateAvailable);
        m_finished = true;
    }
}


void wxZRColaUpdater::OnUpdate(wxCommandEvent& event)
{
    event.Skip();

    if (m_updater) {
        if (wxMessageBox(_("This program will now close and the upgrade will be launched.\nAre you sure?"), _("Product Update"), wxICON_QUESTION | wxYES_NO | wxNO_DEFAULT, this) == wxYES) {
            m_updater->LaunchUpdate();

            Close(true);
            dynamic_cast<ZRColaApp*>(wxTheApp)->m_mainWnd->Close();
        }
    }
}


void wxZRColaUpdater::OnClose(wxCommandEvent& event)
{
    if (m_updater)
        m_updater->Abort();

    event.Skip();
}
