/*
    SPDX-License-Identifier: GPL-3.0-or-later
    Copyright © 2015-2021 Amebis
*/

#include "pch.h"


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
    #pragma warning(suppress: 26812) // wxLanguage is unscoped.
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
        m_buttonUpdate->Enable(m_updater->CheckForUpdate() == wxUpdCheckThread::wxResult::UpdateAvailable);
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
