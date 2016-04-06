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
// wxZRColaKeyHandler
//////////////////////////////////////////////////////////////////////////

wxZRColaKeyHandler::wxZRColaKeyHandler() : wxEvtHandler()
{
    std::fstream dat((LPCTSTR)((ZRColaApp*)wxTheApp)->GetDatabasePath(), std::ios_base::in | std::ios_base::binary);
    if (dat.good()) {
        if (stdex::idrec::find<ZRCola::recordid_t, ZRCola::recordsize_t, ZRCOLA_RECORD_ALIGN>(dat, ZRCOLA_DB_ID, sizeof(ZRCola::recordid_t))) {
            ZRCola::recordsize_t size;
            dat.read((char*)&size, sizeof(ZRCola::recordsize_t));
            if (dat.good()) {
                ZRCola::keyseq_rec rec(m_ks_db);
                if (rec.find(dat, size)) {
                    dat >> rec;
                    if (!dat.good()) {
                        wxFAIL_MSG(wxT("Error reading translation data from ZRCola.zrcdb."));
                        m_ks_db.idxChr.clear();
                        m_ks_db.idxKey.clear();
                        m_ks_db.data  .clear();
                    }
                } else
                    wxFAIL_MSG(wxT("ZRCola.zrcdb has no translation data."));
            }
        } else
            wxFAIL_MSG(wxT("ZRCola.zrcdb is not a valid ZRCola database."));
    }
}


bool wxZRColaKeyHandler::ProcessEvent(wxEvent& event)
{
    if (event.GetEventType() == wxEVT_KEY_DOWN) {
        // The character event occured.
        ZRCola::keyseq_db::indexKey::size_type start, end;
        bool found;
        wxFrame *pFrame = wxDynamicCast(wxTheApp->GetTopWindow(), wxFrame);

        {
            // Parse key event and save it at the end of the key sequence.
            wxKeyEvent &e = (wxKeyEvent&)event;
            ZRCola::keyseq_db::keyseq::key_t key;
            key.key = e.GetKeyCode(); //wxToupper(e.m_uniChar);
            key.modifiers =
                (e.ShiftDown()   ? ZRCola::keyseq_db::keyseq::SHIFT : 0) |
                (e.ControlDown() ? ZRCola::keyseq_db::keyseq::CTRL  : 0) |
                (e.AltDown()     ? ZRCola::keyseq_db::keyseq::ALT   : 0);
            m_seq.push_back(key);

            std::vector<ZRCola::keyseq_db::keyseq::key_t>::size_type n = m_seq.size();
            ZRCola::keyseq_db::keyseq *ks = (ZRCola::keyseq_db::keyseq*)new char[sizeof(ZRCola::keyseq_db::keyseq) + sizeof(ZRCola::keyseq_db::keyseq::key_t)*n];
            ks->chr = 0;
            ks->seq_len = n;
            memcpy(ks->seq, m_seq.data(), sizeof(ZRCola::keyseq_db::keyseq::key_t)*n);
            found = m_ks_db.idxKey.find(*ks, start, end);
            delete ks;
        }

        if (found) {
            // The exact key sequence found.
            const ZRCola::keyseq_db::keyseq &ks = m_ks_db.idxKey[start];
            m_seq.clear();

            if (pFrame && pFrame->GetStatusBar())
                pFrame->SetStatusText(wxEmptyString);

            wxObject *obj = event.GetEventObject();
            if (obj && obj->IsKindOf(wxCLASSINFO(wxTextCtrl))) {
                // Push text to source control.
                ((wxTextCtrl*)obj)->WriteText(ks.chr);

                // Event is fully processed now.
                event.StopPropagation();
                return true;
            }
        } else if (start < m_ks_db.idxKey.size() &&
            ZRCola::keyseq_db::keyseq::CompareSequence(m_seq.data(), m_seq.size(), m_ks_db.idxKey[start].seq, std::min<unsigned __int16>(m_ks_db.idxKey[start].seq_len, m_seq.size())) == 0)
        {
            // The sequence is a partial match. Continue watching.
            if (pFrame && pFrame->GetStatusBar())
                pFrame->SetStatusText(ZRCola::keyseq_db::GetSequenceAsText(m_seq.data(), m_seq.size()));

            event.StopPropagation();
            return true;
        } else {
            // The key sequence has no future chance to match. Start all over again.
            m_seq.clear();

            if (pFrame && pFrame->GetStatusBar())
                pFrame->SetStatusText(wxEmptyString);
        }
    }

    return wxEvtHandler::ProcessEvent(event);
}
