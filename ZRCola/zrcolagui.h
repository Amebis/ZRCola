///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Jun 17 2015)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __ZRCOLAGUI_H__
#define __ZRCOLAGUI_H__

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/cshelp.h>
#include <wx/intl.h>
#include <wx/string.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/menu.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/stattext.h>
#include <wx/choice.h>
#include <wx/toolbar.h>
#include "zrcolacomppnl.h"
#include <wx/sizer.h>
#include <wx/statusbr.h>
#include <wx/frame.h>
#include <wx/textctrl.h>
#include <wx/statbox.h>
#include <wx/panel.h>
#include <wx/splitter.h>

///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/// Class wxZRColaFrameBase
///////////////////////////////////////////////////////////////////////////////
class wxZRColaFrameBase : public wxFrame 
{
	private:
	
	protected:
		enum
		{
			wxID_AUTOSTART = 1000,
			wxID_SEND_COMPOSED,
			wxID_SEND_DECOMPOSED,
			wxID_SEND_ABORT,
			wxID_DECOMP_LANG_AUTO
		};
		
		wxMenuBar* m_menubar;
		wxMenu* m_menuProgram;
		wxMenu* m_menuEdit;
		wxMenu* m_menuDecompLanguage;
		wxMenu* m_menuHelp;
		wxToolBar* m_toolbar;
		wxToolBarToolBase* m_toolEditCut; 
		wxToolBarToolBase* m_toolEditCopy; 
		wxToolBarToolBase* m_toolEditPaste; 
		wxToolBarToolBase* m_toolSendComposed; 
		wxToolBarToolBase* m_toolSendDecomposed; 
		wxStaticText* m_toolDecompLanguageLbl;
		wxChoice* m_toolDecompLanguage;
		wxZRColaComposerPanel* m_panel;
		wxStatusBar* m_statusBar;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnDecompLanguageChoice( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		wxZRColaFrameBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("ZRCola"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 600,400 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL, const wxString& name = wxT("ZRCola") );
		
		~wxZRColaFrameBase();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class wxZRColaComposerPanelBase
///////////////////////////////////////////////////////////////////////////////
class wxZRColaComposerPanelBase : public wxPanel 
{
	private:
	
	protected:
		wxSplitterWindow* m_splitterDecomposed;
		wxPanel* m_panelDecomposedEdit;
		wxTextCtrl* m_decomposed;
		wxPanel* m_panelDecomposedHex;
		wxTextCtrl* m_decomposedHex;
		wxSplitterWindow* m_splitterComposed;
		wxPanel* m_panelComposedEdit;
		wxTextCtrl* m_composed;
		wxPanel* m_panelComposedHex;
		wxTextCtrl* m_composedHex;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnDecomposedPaint( wxPaintEvent& event ) { event.Skip(); }
		virtual void OnDecomposedText( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnDecomposedHexPaint( wxPaintEvent& event ) { event.Skip(); }
		virtual void OnComposedPaint( wxPaintEvent& event ) { event.Skip(); }
		virtual void OnComposedText( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnComposedHexPaint( wxPaintEvent& event ) { event.Skip(); }
		
	
	public:
		
		wxZRColaComposerPanelBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxTAB_TRAVERSAL, const wxString& name = wxT("ZRColaComposerPanel") ); 
		~wxZRColaComposerPanelBase();
		
		void m_splitterDecomposedOnIdle( wxIdleEvent& )
		{
			m_splitterDecomposed->SetSashPosition( -5 );
			m_splitterDecomposed->Disconnect( wxEVT_IDLE, wxIdleEventHandler( wxZRColaComposerPanelBase::m_splitterDecomposedOnIdle ), NULL, this );
		}
		
		void m_splitterComposedOnIdle( wxIdleEvent& )
		{
			m_splitterComposed->SetSashPosition( -5 );
			m_splitterComposed->Disconnect( wxEVT_IDLE, wxIdleEventHandler( wxZRColaComposerPanelBase::m_splitterComposedOnIdle ), NULL, this );
		}
	
};

#endif //__ZRCOLAGUI_H__
