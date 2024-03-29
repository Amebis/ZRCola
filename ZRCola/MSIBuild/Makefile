#    SPDX-License-Identifier: GPL-3.0-or-later
#    Copyright © 1991-2022 Amebis

!INCLUDE "..\..\include\MSIBuildCfg.mak"

!IFNDEF MSIBUILD_PLAT_GUID
!ERROR Parameter MSIBUILD_PLAT_GUID is undefined.
!ENDIF

!IFNDEF MSIBUILD_LANGID
!ERROR Parameter MSIBUILD_LANGID is undefined.
!ENDIF

!IFNDEF MSIBUILD_LANG_GUID
!ERROR Parameter MSIBUILD_LANG_GUID is undefined.
!ENDIF

MSIBUILD_IS_LOCALIZEABLE=1
!IF "$(LANG)" != "en_US"
MSIBUILD_PO="..\locale\$(LANG).po"
!ENDIF


######################################################################
# AppSearch

All :: "$(LANG).$(PLAT).$(CFG).AppSearch-1.idt"

"$(LANG).$(PLAT).$(CFG).AppSearch-1.idt" : "Makefile" "..\..\include\MSIBuildCfg.mak"
	move /y << $@ > NUL
Property	Signature_
s$(MSIBUILD_LENGTH_ID)	s$(MSIBUILD_LENGTH_ID)
AppSearch	Property	Signature_
OLDZRCOLADIR	ZRCola.exe.$(PLAT)
<<NOKEEP


######################################################################
# Binary

All :: \
	"$(LANG).$(PLAT).$(CFG).Binary-1.idt" \
	"$(LANG).$(PLAT).$(CFG).Binary-1\tutorial1.bmp" \
	"$(LANG).$(PLAT).$(CFG).Binary-1\tutorial2.bmp" \
	"$(LANG).$(PLAT).$(CFG).Binary-1\tutorial3.bmp"

"$(LANG).$(PLAT).$(CFG).Binary-1.idt" : "Makefile" "..\..\include\MSIBuildCfg.mak" "$(LANG).$(PLAT).$(CFG).Binary-1"
	move /y << $@ > NUL
Name	Data
s$(MSIBUILD_LENGTH_ID)	v0
Binary	Name
tutorial1.bmp	tutorial1.bmp
tutorial2.bmp	tutorial2.bmp
tutorial3.bmp	tutorial3.bmp
<<NOKEEP

"$(LANG).$(PLAT).$(CFG).Binary-1\tutorial1.bmp" : "Binary\tutorial1.bmp"
	copy /y $** $@ > NUL

"$(LANG).$(PLAT).$(CFG).Binary-1\tutorial2.bmp" : "Binary\tutorial2.bmp"
	copy /y $** $@ > NUL

"$(LANG).$(PLAT).$(CFG).Binary-1\tutorial3.bmp" : "Binary\tutorial3.bmp"
	copy /y $** $@ > NUL


######################################################################
# CompLocator

All :: "$(LANG).$(PLAT).$(CFG).CompLocator-1.idt"

"$(LANG).$(PLAT).$(CFG).CompLocator-1.idt" : "Makefile" "..\..\include\MSIBuildCfg.mak"
	move /y << $@ > NUL
Signature_	ComponentId	Type
s$(MSIBUILD_LENGTH_ID)	s38	I2
CompLocator	Signature_
ZRCola.exe.$(PLAT)	{6DB513F9-BC90-4CE7-821$(MSIBUILD_PLAT_GUID)-AA50BF31F34E}	1
<<NOKEEP


######################################################################
# Component

All :: "$(LANG).$(PLAT).$(CFG).Component-1.idt"

"$(LANG).$(PLAT).$(CFG).Component-1.idt" : "Makefile" "..\..\include\MSIBuildCfg.mak"
	move /y << $@ > NUL
Component	ComponentId	Directory_	Attributes	Condition	KeyPath
s$(MSIBUILD_LENGTH_ID)	S38	s$(MSIBUILD_LENGTH_ID)	i2	S255	S$(MSIBUILD_LENGTH_ID)
Component	Component
ZRCola.exe.$(PLAT)	{6DB513F9-BC90-4CE7-821$(MSIBUILD_PLAT_GUID)-AA50BF31F34E}	ZRCOLABINDIR	$(MSIBUILD_COMPONENT_ATTRIB_FILE)		ZRCola.exe.$(PLAT)
!IF "$(LANG)" != "en_US"
ZRCola.mo.$(LANG)	{4F59C403-D6DE-4B3F-$(MSIBUILD_LANG_GUID)-1190DB941EB3}	ZRCOLALOC$(MSIBUILD_LANG_GUID)DIR	$(MSIBUILD_COMPONENT_ATTRIB_FILE)		ZRCola.mo.$(LANG)
!ENDIF
<<NOKEEP


######################################################################
# Control

All :: "$(LANG).$(PLAT).$(CFG).Control-2.idt"

"en_US.$(PLAT).$(CFG).Control-2.idtx" : "Makefile" "..\..\include\MSIBuildCfg.mak"
	move /y << $@ > NUL
Dialog_	Control	Type	X	Y	Width	Height	Attributes	Property	Text	Control_Next	Help
s$(MSIBUILD_LENGTH_ID)	s50	s20	i2	i2	i2	i2	I4	S50	L0	S50	L$(MSIBUILD_LENGTH_HELP)
Control	Dialog_	Control
Tutorial1	BannerBitmap	Bitmap	0	0	400	45	1		UIHeader.bmp	Title	
Tutorial1	Title	Text	15	15	320	15	196611		[DlgTitleFont][SimpleProductName] Tutorial : Starting	Screenshot	
Tutorial1	Screenshot	Bitmap	154	60	92	54	5		tutorial1.bmp	Text1	
Tutorial1	Text1	Text	15	119	370	24	196611		When configured, ZRCola starts at system startup and runs in the background. If needed, it can be brought up by pressing the combination Win+F5.	Text2	
Tutorial1	Text2	Text	15	148	370	36	196611		IMPORTANT: Upon pressing the combination Win+F5 the program memorises the location of the cursor in the text. If ZRCola is brought to the foreground without using the aforementioned combination of keys, the program won't know where to insert the special characters!	Continue	
Tutorial1	Continue	PushButton	325	265	60	20	3		&Continue	BannerBitmap	
Tutorial2	BannerBitmap	Bitmap	0	0	400	45	1		UIHeader.bmp	Title	
Tutorial2	Title	Text	15	15	320	15	196611		[DlgTitleFont][SimpleProductName] Tutorial : Composition	Screenshot	
Tutorial2	Screenshot	Bitmap	81	60	238	137	5		tutorial2.bmp	Text1	
Tutorial2	Text1	Text	15	202	370	36	196611		The string of basic and substitute characters is entered into the upper input window of the program. In the window below the resulted special characters are displayed. To insert them in the text, the F5 key is used.	Continue	
Tutorial2	Continue	PushButton	325	265	60	20	3		&Continue	BannerBitmap	
Tutorial3	BannerBitmap	Bitmap	0	0	400	45	1		UIHeader.bmp	Title	
Tutorial3	Title	Text	15	15	320	15	196611		[DlgTitleFont][SimpleProductName] Tutorial : Entering Characters	Screenshot	
Tutorial3	Screenshot	Bitmap	147	60	106	112	5		tutorial3.bmp	Text1	
Tutorial3	Text1	Text	15	177	370	24	196611		Characters can be entered using keyboard shortcuts, the character catalogue (F4) or the character finding tool (F8).	Continue	
Tutorial3	Continue	PushButton	325	265	60	20	3		&Continue	BannerBitmap	
<<NOKEEP

"$(LANG).$(PLAT).$(CFG).Control-2.idt" : "en_US.$(PLAT).$(CFG).Control-2.idtx" $(MSIBUILD_PO)
	cscript.exe "..\..\MSI\MSIBuild\MSI.wsf" //Job:IDTTranslate //Nologo $@ $** /CP:$(MSIBUILD_CODEPAGE)


######################################################################
# ControlEvent

All :: "$(LANG).$(PLAT).$(CFG).ControlEvent-1.idt"

"$(LANG).$(PLAT).$(CFG).ControlEvent-1.idt" : "Makefile" "..\..\include\MSIBuildCfg.mak"
	move /y << $@ > NUL
Dialog_	Control_	Event	Argument	Condition	Ordering
s$(MSIBUILD_LENGTH_ID)	s50	s50	s255	S255	I2
ControlEvent	Dialog_	Control_	Event	Argument	Condition
Tutorial1	Continue	EndDialog	Return	1	1000
Tutorial2	Continue	EndDialog	Return	1	1000
Tutorial3	Continue	EndDialog	Return	1	1000
<<NOKEEP


######################################################################
# Dialog

All :: "$(LANG).$(PLAT).$(CFG).Dialog-1.idt"

"$(LANG).$(PLAT).$(CFG).Dialog-1.idt" : "Makefile" "..\..\include\MSIBuildCfg.mak"
	move /y << $@ > NUL
Dialog	HCentering	VCentering	Width	Height	Attributes	Title	Control_First	Control_Default	Control_Cancel
s$(MSIBUILD_LENGTH_ID)	i2	i2	i2	i2	I4	L128	s50	S50	S50
Dialog	Dialog
Tutorial1	50	50	400	300	3	[Setup] [SimpleProductName] [DisplayVersion]	Title	Continue	Continue
Tutorial2	50	50	400	300	3	[Setup] [SimpleProductName] [DisplayVersion]	Title	Continue	Continue
Tutorial3	50	50	400	300	3	[Setup] [SimpleProductName] [DisplayVersion]	Title	Continue	Continue
<<NOKEEP


######################################################################
# InstallUISequence

All :: "$(LANG).$(PLAT).$(CFG).InstallUISequence-1.idt"

"$(LANG).$(PLAT).$(CFG).InstallUISequence-1.idt" : "Makefile" "..\..\include\MSIBuildCfg.mak"
	move /y << $@ > NUL
Action	Condition	Sequence
s$(MSIBUILD_LENGTH_ID)	S255	I2
InstallUISequence	Action
Tutorial1	NOT Installed AND &featZRCola>=3	1400
Tutorial2	NOT Installed AND &featZRCola>=3	1401
Tutorial3	NOT Installed AND &featZRCola>=3	1402
<<NOKEEP


######################################################################
# Feature

All :: "$(LANG).$(PLAT).$(CFG).Feature-2.idt"

"en_US.$(PLAT).$(CFG).Feature-2.idtx" : "Makefile" "..\..\include\MSIBuildCfg.mak"
	move /y << $@ > NUL
Feature	Feature_Parent	Title	Description	Display	Level	Directory_	Attributes
s$(MSIBUILD_LENGTH_ID)	S$(MSIBUILD_LENGTH_ID)	L64	L255	I2	i2	S$(MSIBUILD_LENGTH_ID)	i2
Feature	Feature
featZRCola		ZRCola	Input system for linguistic use	1	1	ZRCOLADIR	8
<<NOKEEP

!IF "$(LANG)" == "en_US"
"en_US.$(PLAT).$(CFG).Feature-2.idt" : "en_US.$(PLAT).$(CFG).Feature-2.idtx"
	copy /y $** $@ > NUL
!ELSE
"$(LANG).$(PLAT).$(CFG).Feature-2.idt" : "en_US.$(PLAT).$(CFG).Feature-2.idtx" $(MSIBUILD_PO)
	cscript.exe "..\..\MSI\MSIBuild\MSI.wsf" //Job:IDTTranslate //Nologo $@ $** /CP:$(MSIBUILD_CODEPAGE)
!ENDIF


######################################################################
# FeatureComponents

All :: "$(LANG).$(PLAT).$(CFG).FeatureComponents-1.idt"

"$(LANG).$(PLAT).$(CFG).FeatureComponents-1.idt" : "Makefile" "..\..\include\MSIBuildCfg.mak"
	move /y << $@ > NUL
Feature_	Component_
s$(MSIBUILD_LENGTH_ID)	s$(MSIBUILD_LENGTH_ID)
FeatureComponents	Feature_	Component_
featZRCola	ZRCola.exe.$(PLAT)
featZRCola	LocalizationRepositoryPath
featZRCola	Language
featZRCola	DatabasePath
featZRCola	ZRCola.zrcdb
featZRCola	ZRCola_Re.otf
!IF "$(LANG)" != "en_US"
featZRCola	ZRCola.mo.$(LANG)
featZRCola	ZRCola.zrcdb.mo.$(LANG)
featZRCola	libZRColaUI.mo.$(LANG)
featZRCola	wxExtend.mo.$(LANG)
featZRCola	Updater.mo.$(LANG)
featZRCola	wxstd.mo.$(LANG)
!ENDIF
<<NOKEEP


######################################################################
# File

All :: "$(LANG).$(PLAT).$(CFG).File-1.idt"

"$(LANG).$(PLAT).$(CFG).File-1.idt" : "Makefile" "..\..\include\MSIBuildCfg.mak"
	move /y << $@ > NUL
File	Component_	FileName	FileSize	Version	Language	Attributes	Sequence
s$(MSIBUILD_LENGTH_ID)	s$(MSIBUILD_LENGTH_ID)	l255	i4	S$(MSIBUILD_LENGTH_ID)	S20	I2	i2
File	File
ZRCola.exe.$(PLAT)	ZRCola.exe.$(PLAT)	ZRCola.exe	0		0	1536	1
!IF "$(LANG)" != "en_US"
ZRCola.mo.$(LANG)	ZRCola.mo.$(LANG)	ZRCola.mo	0		$(MSIBUILD_LANGID)	0	1
!ENDIF
<<NOKEEP


######################################################################
# Icon

All :: \
	"$(LANG).$(PLAT).$(CFG).Icon-1.idt" \
	"$(LANG).$(PLAT).$(CFG).Icon-1\ZRCola.ico"

"$(LANG).$(PLAT).$(CFG).Icon-1.idt" : "Makefile" "..\..\include\MSIBuildCfg.mak" "$(LANG).$(PLAT).$(CFG).Icon-1"
	move /y << $@ > NUL
Name	Data
s$(MSIBUILD_LENGTH_ID)	v0
Icon	Name
ZRCola.ico	ZRCola.ico
<<NOKEEP

"$(LANG).$(PLAT).$(CFG).Icon-1\ZRCola.ico" : "..\res\zrcola.ico"
	copy /y $** $@ > NUL


######################################################################
# RemoveFile

All :: "$(LANG).$(PLAT).$(CFG).RemoveFile-1.idt"

"$(LANG).$(PLAT).$(CFG).RemoveFile-1.idt" : "Makefile" "..\..\include\MSIBuildCfg.mak"
	move /y << $@ > NUL
FileKey	Component_	FileName	DirProperty	InstallMode
s$(MSIBUILD_LENGTH_ID)	s$(MSIBUILD_LENGTH_ID)	L255	s$(MSIBUILD_LENGTH_ID)	i2
RemoveFile	FileKey
ZRCola.exe.$(PLAT)	ZRCola.exe.$(PLAT)		ZRColaShortcutDir	2
<<NOKEEP


######################################################################
# Shortcut

All :: "$(LANG).$(PLAT).$(CFG).Shortcut-2.idt"

"en_US.$(PLAT).$(CFG).Shortcut-2.idtx" : "Makefile" "..\..\include\MSIBuildCfg.mak"
	move /y << $@ > NUL
Shortcut	Directory_	Name	Component_	Target	Arguments	Description	Hotkey	Icon_	IconIndex	ShowCmd	WkDir	DisplayResourceDLL	DisplayResourceId	DescriptionResourceDLL	DescriptionResourceId
s$(MSIBUILD_LENGTH_ID)	s$(MSIBUILD_LENGTH_ID)	l128	s$(MSIBUILD_LENGTH_ID)	s$(MSIBUILD_LENGTH_ID)	S255	L255	I2	S$(MSIBUILD_LENGTH_ID)	I2	I2	S$(MSIBUILD_LENGTH_ID)	S255	I2	S255	I2
Shortcut	Shortcut
ZRCola.exe.$(PLAT)	ZRColaShortcutDir	ZRCola	ZRCola.exe.$(PLAT)	featZRCola		Input system for linguistic use		ZRCola.ico			ZRCOLABINDIR				
<<NOKEEP

!IF "$(LANG)" == "en_US"
"en_US.$(PLAT).$(CFG).Shortcut-2.idt" : "en_US.$(PLAT).$(CFG).Shortcut-2.idtx"
	copy /y $** $@ > NUL
!ELSE
"$(LANG).$(PLAT).$(CFG).Shortcut-2.idt" : "en_US.$(PLAT).$(CFG).Shortcut-2.idtx" $(MSIBUILD_PO)
	cscript.exe "..\..\MSI\MSIBuild\MSI.wsf" //Job:IDTTranslate //Nologo $@ $** /CP:$(MSIBUILD_CODEPAGE)
!ENDIF


######################################################################
# Build MSM module!
######################################################################

!INCLUDE "..\..\MSI\MSIBuild\MSM.mak"
