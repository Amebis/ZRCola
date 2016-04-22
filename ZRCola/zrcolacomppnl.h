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

///
/// Forward declarations
///
class wxZRColaComposerPanel;


#pragma once

#include "zrcolagui.h"
#include "zrcolakeyhndlr.h"
#include <utility>


///
/// ZRCola composer panel
///
class wxZRColaComposerPanel : public wxZRColaComposerPanelBase
{
public:
    wxZRColaComposerPanel(wxWindow* parent);
    virtual ~wxZRColaComposerPanel();

    friend class wxZRColaFrame; // Allow main frame direct access to our members.

protected:
    virtual void OnDecomposedPaint(wxPaintEvent& event);
    virtual void OnDecomposedText(wxCommandEvent& event);
    virtual void OnComposedPaint(wxPaintEvent& event);
    virtual void OnComposedText(wxCommandEvent& event);

protected:
    bool m_progress;                    ///< Boolean flag to avoid recursive updates of composed and decomposed text controls
    ZRCola::mapping_vector m_mapping1;  ///< Character index mapping vector between decomposed and normalized text
    ZRCola::mapping_vector m_mapping2;  ///< Character index mapping vector between normalized and composed text
    std::pair<long, long>
        m_selDecomposed,                ///< Character index of selected text in decomposed text control
        m_selComposed;                  ///< Character index of selected text in composed text control
    wxZRColaKeyHandler m_keyhandler;    ///< Key handler for decomposed window
};
