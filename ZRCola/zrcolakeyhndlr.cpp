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
    wxString sPath(wxPathOnly(wxTheApp->argv[0]));
    sPath << wxT("\\..\\data\\ZRCola.zrcdb"); // TODO: Make database file configurable

    std::fstream dat((LPCTSTR)sPath, std::ios_base::in | std::ios_base::binary);
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
    if (event.GetEventType() == wxEVT_CHAR) {
        // The character event occured.
        ZRCola::keyseq_db::indexKey::size_type start, end;
        bool found;

        {
            wxKeyEvent &e = (wxKeyEvent&)event;
            ZRCola::keyseq_db::keyseq *ks = (ZRCola::keyseq_db::keyseq*)new char[sizeof(ZRCola::keyseq_db::keyseq) + sizeof(ZRCola::keyseq_db::keyseq::key_t)*1];
            ks->chr = 0;
            ks->seq_len = 1;
            ks->seq[0].key = wxToupper(e.m_uniChar);
            ks->seq[0].modifiers =
                (e.ShiftDown()   ? ZRCola::keyseq_db::keyseq::SHIFT : 0) |
                (e.ControlDown() ? ZRCola::keyseq_db::keyseq::CTRL  : 0) |
                (e.AltDown()     ? ZRCola::keyseq_db::keyseq::ALT   : 0);
            found = m_ks_db.idxKey.find((const unsigned __int16&)*ks, start, end);
            delete ks;
        }

        if (found) {
            const ZRCola::keyseq_db::keyseq &ks = (const ZRCola::keyseq_db::keyseq&)m_ks_db.data[m_ks_db.idxKey[start]];

            wxObject *obj = event.GetEventObject();
            if (obj && obj->IsKindOf(wxCLASSINFO(wxTextCtrl))) {
                // Push text to source control.
                ((wxTextCtrl*)obj)->WriteText(ks.chr);

                // Event is fully processed now.
                event.StopPropagation();
                return true;
            }
        }
    }

    return wxEvtHandler::ProcessEvent(event);
}
