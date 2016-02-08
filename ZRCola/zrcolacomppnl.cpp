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

wxZRColaComposerPanel::wxZRColaComposerPanel(wxWindow* parent) : wxZRColaComposerPanelBase(parent)
{
}


wxZRColaComposerPanel::~wxZRColaComposerPanel()
{
}


void wxZRColaComposerPanel::OnDecomposedText(wxCommandEvent& event)
{
    // TODO: Do the real ZRCola composition here.
    m_composed->SetValue(m_decomposed->GetValue());

    event.Skip();
}


void wxZRColaComposerPanel::OnComposedText(wxCommandEvent& event)
{
    // TODO: Do the real ZRCola decomposition here.
    m_decomposed->SetValue(m_composed->GetValue());

    event.Skip();
}
