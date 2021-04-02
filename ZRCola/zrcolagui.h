///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct 26 2018)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#pragma once

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/cshelp.h>
#include <wx/intl.h>
class wxZRColaCharGrid;

#include <wx/string.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/menu.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/aui/aui.h>
#include <wx/aui/auibar.h>
#include <wx/choice.h>
class wxZRColaCharacterCatalogPanel;
class wxZRColaComposerPanel;
#include <wx/statusbr.h>
#include <wx/frame.h>
#include <wx/textctrl.h>
#include <wx/sizer.h>
#include <wx/statbox.h>
#include <wx/panel.h>
#include <wx/splitter.h>
#include <wx/timer.h>
#include <wx/grid.h>
#include <wx/checkbox.h>
#include <wx/srchctrl.h>
#include <wx/hyperlink.h>
#include <wx/checklst.h>
#include <wx/stattext.h>
#include <wx/button.h>
#include <wx/dialog.h>
#include <wx/radiobut.h>
#include <wx/listbox.h>
#include <wx/listbook.h>
#include <wx/listctrl.h>
#include <wx/statbmp.h>

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
			wxID_CHARACTER_SELECTOR = 1000,
			wxID_SEND_DESTINATION,
			wxID_COPY_DESTINATION_AND_RETURN,
			wxID_SEND_SOURCE,
			wxID_COPY_SOURCE_AND_RETURN,
			wxID_SEND_ABORT,
			wxID_COMPOSITION,
			wxID_WARN_PUA,
			wxID_SETTINGS,
			wxID_TOOLBAR_EDIT,
			wxID_TOOLBAR_TRANSLATE,
			wxID_PANEL_CHRGRPS,
			wxID_HELP_INSTRUCTIONS,
			wxID_HELP_SHORTCUTS,
			wxID_HELP_REQCHAR,
			wxID_HELP_UPDATE
		};

		wxMenuBar* m_menubar;
		wxMenu* m_menuProgram;
		wxMenu* m_menuEdit;
		wxMenu* m_menuTranslationSeq;
		wxMenuItem* m_menuItemComposition;
		wxMenuItem* m_menuItemWarnPUA;
		wxMenu* m_menuView;
		wxMenu* m_menuHelp;
		wxAuiToolBar* m_toolbarEdit;
		wxAuiToolBarItem* m_toolEditCut;
		wxAuiToolBarItem* m_toolEditCopy;
		wxAuiToolBarItem* m_toolEditPaste;
		wxAuiToolBar* m_toolbarTranslate;
		wxAuiToolBarItem* m_toolCharSelect;
		wxAuiToolBarItem* m_toolSendDestination;
		wxAuiToolBarItem* m_toolSendSource;
		wxChoice* m_toolTranslationSeq;
		wxAuiToolBarItem* m_toolComposition;
		wxAuiToolBarItem* m_toolWarnPUA;
		wxZRColaCharacterCatalogPanel* m_panelChrCat;
		wxStatusBar* m_statusBar;

		// Virtual event handlers, overide them in your derived class
		virtual void OnClose( wxCloseEvent& event ) { event.Skip(); }
		virtual void OnIconize( wxIconizeEvent& event ) { event.Skip(); }
		virtual void OnIdle( wxIdleEvent& event ) { event.Skip(); }
		virtual void OnTranslationSeqChoice( wxCommandEvent& event ) { event.Skip(); }


	public:
		wxZRColaComposerPanel* m_panel;

		wxZRColaFrameBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("ZRCola"), const wxPoint& pos = wxDefaultPosition, long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL, const wxString& name = wxT("ZRCola") );
		wxAuiManager m_mgr;

		~wxZRColaFrameBase();

};

///////////////////////////////////////////////////////////////////////////////
/// Class wxZRColaComposerPanelBase
///////////////////////////////////////////////////////////////////////////////
class wxZRColaComposerPanelBase : public wxPanel
{
	private:

	protected:
		enum
		{
			wxID_TIMER_SAVE = 1000
		};

		wxSplitterWindow* m_splitterSource;
		wxPanel* m_panelSourceEdit;
		wxPanel* m_panelSourceHex;
		wxTextCtrl* m_sourceHex;
		wxSplitterWindow* m_splitterDestination;
		wxPanel* m_panelDestinationEdit;
		wxPanel* m_panelDestinationHex;
		wxTextCtrl* m_destinationHex;
		wxTimer m_timerSave;

		// Virtual event handlers, overide them in your derived class
		virtual void OnSourcePaint( wxPaintEvent& event ) { event.Skip(); }
		virtual void OnSourceText( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnSourceHexPaint( wxPaintEvent& event ) { event.Skip(); }
		virtual void OnDestinationPaint( wxPaintEvent& event ) { event.Skip(); }
		virtual void OnDestinationText( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnDestinationHexPaint( wxPaintEvent& event ) { event.Skip(); }
		virtual void OnSaveTimer( wxTimerEvent& event ) { event.Skip(); }


	public:
		wxTextCtrl* m_source;
		wxTextCtrl* m_destination;

		wxZRColaComposerPanelBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxTAB_TRAVERSAL, const wxString& name = wxT("ZRColaComposerPanel") );
		~wxZRColaComposerPanelBase();

		void m_splitterSourceOnIdle( wxIdleEvent& )
		{
			m_splitterSource->SetSashPosition( FromDIP(-5) );
			m_splitterSource->Disconnect( wxEVT_IDLE, wxIdleEventHandler( wxZRColaComposerPanelBase::m_splitterSourceOnIdle ), NULL, this );
		}

		void m_splitterDestinationOnIdle( wxIdleEvent& )
		{
			m_splitterDestination->SetSashPosition( FromDIP(-5) );
			m_splitterDestination->Disconnect( wxEVT_IDLE, wxIdleEventHandler( wxZRColaComposerPanelBase::m_splitterDestinationOnIdle ), NULL, this );
		}

};

///////////////////////////////////////////////////////////////////////////////
/// Class wxZRColaCharacterCatalogPanelBase
///////////////////////////////////////////////////////////////////////////////
class wxZRColaCharacterCatalogPanelBase : public wxPanel
{
	private:

	protected:
		wxChoice* m_choice;
		wxZRColaCharGrid* m_grid;
		wxCheckBox* m_show_all;

		// Virtual event handlers, overide them in your derived class
		virtual void OnChoice( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnGridClick( wxGridEvent& event ) { event.Skip(); }
		virtual void OnGridKeyDown( wxKeyEvent& event ) { event.Skip(); }
		virtual void OnShowAll( wxCommandEvent& event ) { event.Skip(); }


	public:

		wxZRColaCharacterCatalogPanelBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxTAB_TRAVERSAL, const wxString& name = wxT("ZRColaCharacterCatalog") );
		~wxZRColaCharacterCatalogPanelBase();

};

///////////////////////////////////////////////////////////////////////////////
/// Class wxZRColaCharSelectBase
///////////////////////////////////////////////////////////////////////////////
class wxZRColaCharSelectBase : public wxDialog
{
	private:

	protected:
		wxSearchCtrl* m_search;
		wxHyperlinkCtrl* m_search_more;
		wxPanel* m_search_panel;
		wxCheckListBox* m_categories;
		wxHyperlinkCtrl* m_categoriesAll;
		wxHyperlinkCtrl* m_categoriesNone;
		wxHyperlinkCtrl* m_categoriesInvert;
		wxZRColaCharGrid* m_gridResults;
		wxZRColaCharGrid* m_gridRecent;
		wxStaticText* m_labelUnicode;
		wxTextCtrl* m_unicode;
		wxTextCtrl* m_shortcut;
		wxGrid* m_gridPreview;
		wxTextCtrl* m_description;
		wxTextCtrl* m_tags;
		wxTextCtrl* m_category;
		wxHyperlinkCtrl* m_navigateBack;
		wxHyperlinkCtrl* m_navigateForward;
		wxZRColaCharGrid* m_gridRelated;
		wxButton* m_buttonInsert;

		// Virtual event handlers, overide them in your derived class
		virtual void OnIdle( wxIdleEvent& event ) { event.Skip(); }
		virtual void OnSearchText( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnSearchMore( wxHyperlinkEvent& event ) { event.Skip(); }
		virtual void OnCategoriesToggle( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnCategoriesAll( wxHyperlinkEvent& event ) { event.Skip(); }
		virtual void OnCategoriesNone( wxHyperlinkEvent& event ) { event.Skip(); }
		virtual void OnCategoriesInvert( wxHyperlinkEvent& event ) { event.Skip(); }
		virtual void OnResultCellDClick( wxGridEvent& event ) { event.Skip(); }
		virtual void OnResultSelectCell( wxGridEvent& event ) { event.Skip(); }
		virtual void OnResultsKeyDown( wxKeyEvent& event ) { event.Skip(); }
		virtual void OnRecentCellDClick( wxGridEvent& event ) { event.Skip(); }
		virtual void OnRecentSelectCell( wxGridEvent& event ) { event.Skip(); }
		virtual void OnRecentKeyDown( wxKeyEvent& event ) { event.Skip(); }
		virtual void OnUnicodeText( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnPreviewKeyDown( wxKeyEvent& event ) { event.Skip(); }
		virtual void OnNavigateBack( wxHyperlinkEvent& event ) { event.Skip(); }
		virtual void OnNavigateForward( wxHyperlinkEvent& event ) { event.Skip(); }
		virtual void OnRelatedSelectCell( wxGridEvent& event ) { event.Skip(); }
		virtual void OnInsert( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnInsertUpdateUI( wxUpdateUIEvent& event ) { event.Skip(); }


	public:

		wxZRColaCharSelectBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Character Search"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE, const wxString& name = wxT("ZRColaCharSelect") );
		~wxZRColaCharSelectBase();

};

///////////////////////////////////////////////////////////////////////////////
/// Class wxZRColaSettingsBase
///////////////////////////////////////////////////////////////////////////////
class wxZRColaSettingsBase : public wxDialog
{
	private:

	protected:
		wxListbook* m_listbook;
		wxPanel* m_panelLanguage;
		wxStaticText* m_langLabel;
		wxRadioButton* m_langAuto;
		wxRadioButton* m_langManual;
		wxListBox* m_languages;
		wxPanel* m_panelAutoStart;
		wxStaticText* m_autoStartLabel;
		wxCheckBox* m_autoStart;
		wxStdDialogButtonSizer* m_sdbSizerButtons;
		wxButton* m_sdbSizerButtonsOK;
		wxButton* m_sdbSizerButtonsApply;
		wxButton* m_sdbSizerButtonsCancel;

		// Virtual event handlers, overide them in your derived class
		virtual void OnInitDialog( wxInitDialogEvent& event ) { event.Skip(); }
		virtual void OnLanguageUpdate( wxUpdateUIEvent& event ) { event.Skip(); }
		virtual void OnApplyButtonClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnOKButtonClick( wxCommandEvent& event ) { event.Skip(); }


	public:

		wxZRColaSettingsBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Settings"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxDEFAULT_DIALOG_STYLE, const wxString& name = wxT("ZRColaSettings") );
		~wxZRColaSettingsBase();

};

///////////////////////////////////////////////////////////////////////////////
/// Class wxZRColaAboutBase
///////////////////////////////////////////////////////////////////////////////
class wxZRColaAboutBase : public wxDialog
{
	private:

	protected:
		wxStaticBitmap* m_bitmapIcon;
		wxStaticText* m_staticTextZRCola;
		wxStaticText* m_staticTextVersion;
		wxStaticText* m_staticTextColophone;
		wxStaticText* m_staticTextCopyright;
		wxHyperlinkCtrl* m_hyperlink;
		wxStaticText* m_staticTextDeclaration;
		wxTextCtrl* m_declaration;
		wxStdDialogButtonSizer* m_sdbSizerButtons;
		wxButton* m_sdbSizerButtonsOK;

	public:

		wxZRColaAboutBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("About ZRCola"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE, const wxString& name = wxT("ZRColaAbout") );
		~wxZRColaAboutBase();

};

///////////////////////////////////////////////////////////////////////////////
/// Class wxZRColaUpdaterBase
///////////////////////////////////////////////////////////////////////////////
class wxZRColaUpdaterBase : public wxDialog
{
	private:

	protected:
		wxTextCtrl* m_log;
		wxButton* m_buttonUpdate;
		wxButton* m_buttonClose;

		// Virtual event handlers, overide them in your derived class
		virtual void OnIdle( wxIdleEvent& event ) { event.Skip(); }
		virtual void OnUpdate( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnClose( wxCommandEvent& event ) { event.Skip(); }


	public:

		wxZRColaUpdaterBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Product Update"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE, const wxString& name = wxT("ZRColaUpdater") );
		~wxZRColaUpdaterBase();

};

///////////////////////////////////////////////////////////////////////////////
/// Class wxZRColaCharRequestBase
///////////////////////////////////////////////////////////////////////////////
class wxZRColaCharRequestBase : public wxDialog
{
	private:

	protected:
		wxStaticText* m_characterLbl;
		wxTextCtrl* m_character;
		wxStaticText* m_characterNote;
		wxStaticText* m_contextLbl;
		wxTextCtrl* m_context;
		wxStaticText* m_note;
		wxStdDialogButtonSizer* m_sdbSizerButtons;
		wxButton* m_sdbSizerButtonsOK;
		wxButton* m_sdbSizerButtonsCancel;

		// Virtual event handlers, overide them in your derived class
		virtual void OnOKButtonClick( wxCommandEvent& event ) { event.Skip(); }


	public:

		wxZRColaCharRequestBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Request a New Character"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE, const wxString& name = wxT("ZRColaCharRequest") );
		~wxZRColaCharRequestBase();

};

///////////////////////////////////////////////////////////////////////////////
/// Class wxZRColaTranslationSeqBase
///////////////////////////////////////////////////////////////////////////////
class wxZRColaTranslationSeqBase : public wxDialog
{
	private:

	protected:
		wxStaticText* m_transLbl;
		wxStaticText* m_availableLbl;
		wxListBox* m_available;
		wxButton* m_add;
		wxButton* m_remove;
		wxStaticText* m_selectedLbl;
		wxListBox* m_selected;
		wxButton* m_selectedUp;
		wxButton* m_selectedDown;
		wxStdDialogButtonSizer* m_sdbSizerButtons;
		wxButton* m_sdbSizerButtonsOK;
		wxButton* m_sdbSizerButtonsCancel;

		// Virtual event handlers, overide them in your derived class
		virtual void OnInitDialog( wxInitDialogEvent& event ) { event.Skip(); }
		virtual void OnUpdate( wxUpdateUIEvent& event ) { event.Skip(); }
		virtual void OnAvailableChar( wxKeyEvent& event ) { event.Skip(); }
		virtual void OnAvailableDClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnAdd( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnRemove( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnSelectedChar( wxKeyEvent& event ) { event.Skip(); }
		virtual void OnSelectedDClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnSelectedUp( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnSelectedDown( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnOKButtonClick( wxCommandEvent& event ) { event.Skip(); }


	public:

		wxZRColaTranslationSeqBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Custom Translation Sequence"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE, const wxString& name = wxT("ZRColaTranslationSeq") );
		~wxZRColaTranslationSeqBase();

};

