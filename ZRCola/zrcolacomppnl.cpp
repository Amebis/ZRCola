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
// wxZRColaComposerPanel
//////////////////////////////////////////////////////////////////////////

wxZRColaComposerPanel::wxZRColaComposerPanel(wxWindow* parent) :
    m_progress(false),
    wxZRColaComposerPanelBase(parent)
{
}


wxZRColaComposerPanel::~wxZRColaComposerPanel()
{
}


void wxZRColaComposerPanel::OnDecomposedText(wxCommandEvent& event)
{
    if (m_progress) {
        // We are being updated by wxZRColaComposerPanel::OnComposedText()
        event.Skip();
    } else {
        std::wstring composed;
        ZRCola::Compose(m_decomposed->GetValue(), (size_t)-1, composed);

        // Update composed text.
        m_progress = true;
        m_composed->SetValue(composed);
        event.Skip();
        m_progress = false;
    }
}


void wxZRColaComposerPanel::OnComposedText(wxCommandEvent& event)
{
    if (m_progress) {
        // We are being updated by wxZRColaComposerPanel::OnDecomposedText()
        event.Skip();
    } else {
        std::wstring decomposed;
        ZRCola::Decompose(m_composed->GetValue(), (size_t)-1, decomposed);

        // Update decomposed text.
        m_progress = true;
        m_decomposed->SetValue(decomposed);
        event.Skip();
        m_progress = false;
    }
}
