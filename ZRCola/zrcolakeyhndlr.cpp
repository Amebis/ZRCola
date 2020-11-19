/*
    Copyright 2015-2020 Amebis

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
// wxZRColaKeyHandler
//////////////////////////////////////////////////////////////////////////

wxZRColaKeyHandler::wxZRColaKeyHandler() :
    m_is_insert(false),
    wxEvtHandler()
{
}


bool wxZRColaKeyHandler::ProcessEvent(wxEvent& event)
{
    if (event.GetEventType() == wxEVT_KEY_DOWN) {
        // The character event occured.
        wxKeyEvent &e = (wxKeyEvent&)event;
        if (e.GetKeyCode() == WXK_INSERT) {
            // Insert key has been pressed.
            m_is_insert = true;
            wxFrame *pFrame = wxDynamicCast(dynamic_cast<ZRColaApp*>(wxTheApp)->m_mainWnd, wxFrame);
            if (pFrame && pFrame->GetStatusBar())
                pFrame->SetStatusText(_("INS key is pressed. Type the Unicode code of desired character now (up to four hexadecimal digits: 0-9, A-F), then release INS."));
        } else if (m_is_insert) {
            wxChar chr = e.GetUnicodeKey();
            if (!chr) {
                int key = e.GetKeyCode();
                if (WXK_NUMPAD0 <= key && key <= WXK_NUMPAD9)
                    chr = '0' + (wxChar)(key - WXK_NUMPAD0);
            }
            wxFrame *pFrame = wxDynamicCast(dynamic_cast<ZRColaApp*>(wxTheApp)->m_mainWnd, wxFrame);
            if (('0' <= chr && chr <= '9' || 'A' <= chr && chr <= 'F') && m_insert_seq.size() < 4) {
                // A hex-digit pressed. Save it.
                m_insert_seq.push_back((char)chr);

                if (pFrame && pFrame->GetStatusBar())
                    pFrame->SetStatusText(wxString::Format(wxT("U+%s"), (const wxStringCharType*)wxString(m_insert_seq.data(), m_insert_seq.size())));

                event.StopPropagation();
                return true;
            } else {
                // Not a hex-digit.
                m_is_insert = false;
                m_insert_seq.clear();

                if (pFrame && pFrame->GetStatusBar())
                    pFrame->SetStatusText(wxEmptyString);
            }
        } else if ((e.GetUnicodeKey() || !e.HasAnyModifiers())
#if defined(__WXMSW__)
            && ::GetKeyState(VK_RMENU) >= 0
#endif
            )
        {
            auto app = dynamic_cast<ZRColaApp*>(wxTheApp);
            ZRCola::keyseq_db::indexKey::size_type start;
            bool found;
            wxFrame *pFrame = wxDynamicCast(app->m_mainWnd, wxFrame);

            {
                // Parse key event and save it at the end of the key sequence.
                ZRCola::keyseq_db::keyseq::key_t key;
                key.key = (wchar_t)e.GetRawKeyCode();
#if defined(__WXMSW__)
                // Translate from local keyboard to scan code.
                key.key = static_cast<wchar_t>(::MapVirtualKey(key.key, MAPVK_VK_TO_VSC) & 0xffff);

                // Translate from scan code to U.S. Keyboard.
                static const HKL s_hkl = ::LoadKeyboardLayout(_T("00000409"), 0);
                key.key = static_cast<wchar_t>(::MapVirtualKeyEx(key.key, MAPVK_VSC_TO_VK, s_hkl) & 0xffff);
#endif
                key.modifiers =
                    (e.ShiftDown()   ? ZRCola::keyseq_db::keyseq::SHIFT : 0) |
                    (e.ControlDown() ? ZRCola::keyseq_db::keyseq::CTRL  : 0) |
                    (e.AltDown()     ? ZRCola::keyseq_db::keyseq::ALT   : 0);
                m_seq.push_back(key);

                std::unique_ptr<ZRCola::keyseq_db::keyseq> ks((ZRCola::keyseq_db::keyseq*)new char[sizeof(ZRCola::keyseq_db::keyseq) + sizeof(ZRCola::keyseq_db::keyseq::key_t)*m_seq.size()]);
                ks->ZRCola::keyseq_db::keyseq::keyseq(m_seq.data(), m_seq.size());
                found = app->m_ks_db.idxKey.find(*ks, start);
            }

            if (found) {
                // The exact key sequence found.
                const ZRCola::keyseq_db::keyseq &ks = app->m_ks_db.idxKey[start];
                m_seq.clear();

                if (pFrame && pFrame->GetStatusBar())
                    pFrame->SetStatusText(wxEmptyString);

                wxObject *obj = event.GetEventObject();
                if (obj && obj->IsKindOf(wxCLASSINFO(wxTextCtrl))) {
                    // Push text to source control.
                    ((wxTextCtrl*)obj)->WriteText(wxString(ks.chr(), ks.chr_len()));

                    // Event is fully processed now.
                    event.StopPropagation();
                    return true;
                }
            } else if (start < app->m_ks_db.idxKey.size() &&
                ZRCola::keyseq_db::keyseq::CompareSequence(m_seq.data(), m_seq.size(), app->m_ks_db.idxKey[start].seq(), std::min<size_t>(app->m_ks_db.idxKey[start].seq_len(), m_seq.size())) == 0)
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
    } else if (event.GetEventType() == wxEVT_KEY_UP) {
        wxKeyEvent &e = (wxKeyEvent&)event;
        if (m_is_insert && e.GetKeyCode() == WXK_INSERT) {
            // Insert key has been depressed.
            wxFrame *pFrame = wxDynamicCast(dynamic_cast<ZRColaApp*>(wxTheApp)->m_mainWnd, wxFrame);
            if (pFrame && pFrame->GetStatusBar())
                pFrame->SetStatusText(wxEmptyString);

            auto count = m_insert_seq.size();
            if (count) {
                // Zero terminate sequence and parse the Unicode value.
                m_insert_seq.push_back(0);
                wchar_t chr = (wchar_t)strtoul(m_insert_seq.data(), NULL, 16);

                if (chr) {
                    wxObject *obj = event.GetEventObject();
                    if (obj && obj->IsKindOf(wxCLASSINFO(wxTextCtrl))) {
                        // Push text to source control.
                        ((wxTextCtrl*)obj)->WriteText(chr);
                    }
                }

                m_insert_seq.clear();
            }

            m_is_insert = false;

            event.StopPropagation();
            return true;
        }
    }

    return wxEvtHandler::ProcessEvent(event);
}
