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
#include <wx/textctrl.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/sizer.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/// Class wxZRColaDialogBase
///////////////////////////////////////////////////////////////////////////////
class wxZRColaDialogBase : public wxDialog 
{
	private:
	
	protected:
		enum
		{
			wxID_DECOMPOSED = 1000,
			wxID_COMPOSED
		};
		
		wxTextCtrl* m_decomposed;
		wxTextCtrl* m_composed;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnClose( wxCloseEvent& event ) { event.Skip(); }
		virtual void OnDecomposedText( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnComposedText( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		wxZRColaDialogBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("ZRCola"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 500,200 ), long style = wxCAPTION|wxCLOSE_BOX|wxMAXIMIZE_BOX|wxMINIMIZE_BOX|wxRESIZE_BORDER|wxSYSTEM_MENU ); 
		~wxZRColaDialogBase();
	
};

#endif //__ZRCOLAGUI_H__
