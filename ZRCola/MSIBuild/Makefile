#
#    Copyright 1991-2018 Amebis
#
#    This file is part of ZRCola.
#
#    ZRCola is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.
#
#    ZRCola is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with ZRCola. If not, see <http://www.gnu.org/licenses/>.
#

!INCLUDE "..\..\include\MSIBuildCfg.mak"

!IFNDEF MSIBUILD_LANGID
!ERROR Parameter MSIBUILD_LANGID is undefined.
!ENDIF

MSIBUILD_IS_LOCALIZEABLE=1


######################################################################
# AppSearch

All :: "$(LANG).$(PLAT).$(CFG).AppSearch-1.idt"

"$(LANG).$(PLAT).$(CFG).AppSearch-1.idt" : "Makefile" "..\..\include\MSIBuildCfg.mak"
	-if exist $@ del /f /q $@
	move /y << $@ > NUL
Property	Signature_
s$(MSIBUILD_LENGTH_ID)	s$(MSIBUILD_LENGTH_ID)
AppSearch	Property	Signature_
OLDZRCOLADIR	clZRCola.exe.$(PLAT)
<<NOKEEP


######################################################################
# Binary

All :: \
	"$(LANG).$(PLAT).$(CFG).Binary-1.idt" \
	"$(LANG).$(PLAT).$(CFG).Binary-1\tutorial1.bmp" \
	"$(LANG).$(PLAT).$(CFG).Binary-1\tutorial2.bmp" \
	"$(LANG).$(PLAT).$(CFG).Binary-1\tutorial3.bmp"

"$(LANG).$(PLAT).$(CFG).Binary-1.idt" : "Makefile" "..\..\include\MSIBuildCfg.mak" "$(LANG).$(PLAT).$(CFG).Binary-1"
	-if exist $@ del /f /q $@
	move /y << $@ > NUL
Name	Data
s$(MSIBUILD_LENGTH_ID)	v0
Binary	Name
bintutorial1.bmp	tutorial1.bmp
bintutorial2.bmp	tutorial2.bmp
bintutorial3.bmp	tutorial3.bmp
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
	-if exist $@ del /f /q $@
	move /y << $@ > NUL
Signature_	ComponentId	Type
s$(MSIBUILD_LENGTH_ID)	s38	I2
CompLocator	Signature_
!IF "$(PLAT)" == "Win32"
clZRCola.exe.Win32	{F30B6545-6203-4B7D-8575-85245A0F90E5}	1
!ENDIF
!IF "$(PLAT)" == "x64"
clZRCola.exe.x64	{D45B105D-2303-459A-AF4B-52AC3AAD5510}	1
!ENDIF
<<NOKEEP


######################################################################
# Component

All :: "$(LANG).$(PLAT).$(CFG).Component-1.idt"

"$(LANG).$(PLAT).$(CFG).Component-1.idt" : "Makefile" "..\..\include\MSIBuildCfg.mak"
	-if exist $@ del /f /q $@
	move /y << $@ > NUL
Component	ComponentId	Directory_	Attributes	Condition	KeyPath
s$(MSIBUILD_LENGTH_ID)	S38	s$(MSIBUILD_LENGTH_ID)	i2	S255	S$(MSIBUILD_LENGTH_ID)
Component	Component
!IF "$(PLAT)" == "Win32"
compZRCola.exe.Win32	{F30B6545-6203-4B7D-8575-85245A0F90E5}	ZRCOLABINDIR	0		fileZRCola.exe.Win32
!ENDIF
!IF "$(PLAT)" == "x64"
compZRCola.exe.x64	{D45B105D-2303-459A-AF4B-52AC3AAD5510}	ZRCOLABINDIR	256		fileZRCola.exe.x64
!ENDIF
!IF "$(LANG)" == "ru_RU"
compZRCola.mo.ru_RU	{DEA5D11C-2B0E-4B85-ACE3-D76334866F50}	ZRCOLALOCRURUDIR	$(MSIBUILD_COMPONENT_ATTRIB_FILE)		fileZRCola.mo.ru_RU
!ENDIF
!IF "$(LANG)" == "sl_SI"
compZRCola.mo.sl_SI	{7FF95C71-D8DE-4D2B-A26D-FEB5A1F54D63}	ZRCOLALOCSLSIDIR	$(MSIBUILD_COMPONENT_ATTRIB_FILE)		fileZRCola.mo.sl_SI
!ENDIF
<<NOKEEP


######################################################################
# Control

All :: "$(LANG).$(PLAT).$(CFG).Control-2.idt"

"en_US.$(PLAT).$(CFG).Control-2.idtx" : "Makefile" "..\..\include\MSIBuildCfg.mak"
	-if exist $@ del /f /q $@
	move /y << $@ > NUL
Dialog_	Control	Type	X	Y	Width	Height	Attributes	Property	Text	Control_Next	Help
s$(MSIBUILD_LENGTH_ID)	s50	s20	i2	i2	i2	i2	I4	S50	L0	S50	L$(MSIBUILD_LENGTH_HELP)
1252	Control	Dialog_	Control
dlgTutorial1	ctlBannerBitmap	Bitmap	0	0	400	45	1		binUIHeader.bmp	ctlTitle	
dlgTutorial1	ctlTitle	Text	15	15	320	15	196611		[DlgTitleFont][SimpleProductName] Tutorial : Starting	ctlScreenshot	
dlgTutorial1	ctlScreenshot	Bitmap	154	60	92	54	5		bintutorial1.bmp	ctlText1	
dlgTutorial1	ctlText1	Text	15	119	370	24	196611		When configured, ZRCola starts at system startup and runs in the background. If needed, it can be brought up by pressing the combination Win+F5.	ctlText2	
dlgTutorial1	ctlText2	Text	15	148	370	36	196611		IMPORTANT: Upon pressing the combination Win+F5 the program memorises the location of the cursor in the text. If ZRCola is brought to the foreground without using the aforementioned combination of keys, the program won't know where to insert the special characters!	ctlContinue	
dlgTutorial1	ctlContinue	PushButton	325	265	60	20	3		&Continue	ctlBannerBitmap	
dlgTutorial2	ctlBannerBitmap	Bitmap	0	0	400	45	1		binUIHeader.bmp	ctlTitle	
dlgTutorial2	ctlTitle	Text	15	15	320	15	196611		[DlgTitleFont][SimpleProductName] Tutorial : Composition	ctlScreenshot	
dlgTutorial2	ctlScreenshot	Bitmap	81	60	238	137	5		bintutorial2.bmp	ctlText1	
dlgTutorial2	ctlText1	Text	15	202	370	36	196611		The string of basic and substitute characters is entered into the upper input window of the program. In the window below the resulted special characters are displayed. To insert them in the text, the F5 key is used.	ctlContinue	
dlgTutorial2	ctlContinue	PushButton	325	265	60	20	3		&Continue	ctlBannerBitmap	
dlgTutorial3	ctlBannerBitmap	Bitmap	0	0	400	45	1		binUIHeader.bmp	ctlTitle	
dlgTutorial3	ctlTitle	Text	15	15	320	15	196611		[DlgTitleFont][SimpleProductName] Tutorial : Entering Characters	ctlScreenshot	
dlgTutorial3	ctlScreenshot	Bitmap	147	60	106	112	5		bintutorial3.bmp	ctlText1	
dlgTutorial3	ctlText1	Text	15	177	370	24	196611		Characters can be entered using keyboard shortcuts, the character catalogue (F4) or the character finding tool (F8).	ctlContinue	
dlgTutorial3	ctlContinue	PushButton	325	265	60	20	3		&Continue	ctlBannerBitmap	
<<NOKEEP

!IF "$(LANG)" == "en_US"
"en_US.$(PLAT).$(CFG).Control-2.idt" : "en_US.$(PLAT).$(CFG).Control-2.idtx"
	copy /y $** $@ > NUL
!ELSE
"$(LANG).$(PLAT).$(CFG).Control-2.idt" : "en_US.$(PLAT).$(CFG).Control-2.idtx" "..\locale\$(LANG).po"
	cscript.exe "..\..\MSI\MSIBuild\MSI.wsf" //Job:IDTTranslate //Nologo $@ $** /CP:$(MSIBUILD_CODEPAGE)
!ENDIF


######################################################################
# ControlEvent

All :: "$(LANG).$(PLAT).$(CFG).ControlEvent-1.idt"

"$(LANG).$(PLAT).$(CFG).ControlEvent-1.idt" : "Makefile" "..\..\include\MSIBuildCfg.mak"
	-if exist $@ del /f /q $@
	move /y << $@ > NUL
Dialog_	Control_	Event	Argument	Condition	Ordering
s$(MSIBUILD_LENGTH_ID)	s50	s50	s255	S255	I2
ControlEvent	Dialog_	Control_	Event	Argument	Condition
dlgTutorial1	ctlContinue	EndDialog	Return	1	1000
dlgTutorial2	ctlContinue	EndDialog	Return	1	1000
dlgTutorial3	ctlContinue	EndDialog	Return	1	1000
<<NOKEEP


######################################################################
# Dialog

All :: "$(LANG).$(PLAT).$(CFG).Dialog-1.idt"

"$(LANG).$(PLAT).$(CFG).Dialog-1.idt" : "Makefile" "..\..\include\MSIBuildCfg.mak"
	-if exist $@ del /f /q $@
	move /y << $@ > NUL
Dialog	HCentering	VCentering	Width	Height	Attributes	Title	Control_First	Control_Default	Control_Cancel
s$(MSIBUILD_LENGTH_ID)	i2	i2	i2	i2	I4	L128	s50	S50	S50
Dialog	Dialog
dlgTutorial1	50	50	400	300	3	[Setup] [SimpleProductName] [DisplayVersion]	ctlTitle	ctlContinue	ctlContinue
dlgTutorial2	50	50	400	300	3	[Setup] [SimpleProductName] [DisplayVersion]	ctlTitle	ctlContinue	ctlContinue
dlgTutorial3	50	50	400	300	3	[Setup] [SimpleProductName] [DisplayVersion]	ctlTitle	ctlContinue	ctlContinue
<<NOKEEP


######################################################################
# InstallUISequence

All :: "$(LANG).$(PLAT).$(CFG).InstallUISequence-1.idt"

"$(LANG).$(PLAT).$(CFG).InstallUISequence-1.idt" : "Makefile" "..\..\include\MSIBuildCfg.mak"
	-if exist $@ del /f /q $@
	move /y << $@ > NUL
Action	Condition	Sequence
s$(MSIBUILD_LENGTH_ID)	S255	I2
InstallUISequence	Action
dlgTutorial1	NOT Installed AND &featZRCola>=3	1400
dlgTutorial2	NOT Installed AND &featZRCola>=3	1401
dlgTutorial3	NOT Installed AND &featZRCola>=3	1402
<<NOKEEP


######################################################################
# Feature

All :: "$(LANG).$(PLAT).$(CFG).Feature-2.idt"

"en_US.$(PLAT).$(CFG).Feature-2.idtx" : "Makefile" "..\..\include\MSIBuildCfg.mak"
	-if exist $@ del /f /q $@
	move /y << $@ > NUL
Feature	Feature_Parent	Title	Description	Display	Level	Directory_	Attributes
s$(MSIBUILD_LENGTH_ID)	S$(MSIBUILD_LENGTH_ID)	L64	L255	I2	i2	S$(MSIBUILD_LENGTH_ID)	i2
1252	Feature	Feature
featZRCola		ZRCola	Input system for linguistic use	1	1	ZRCOLADIR	8
<<NOKEEP

!IF "$(LANG)" == "en_US"
"en_US.$(PLAT).$(CFG).Feature-2.idt" : "en_US.$(PLAT).$(CFG).Feature-2.idtx"
	copy /y $** $@ > NUL
!ELSE
"$(LANG).$(PLAT).$(CFG).Feature-2.idt" : "en_US.$(PLAT).$(CFG).Feature-2.idtx" "..\locale\$(LANG).po"
	cscript.exe "..\..\MSI\MSIBuild\MSI.wsf" //Job:IDTTranslate //Nologo $@ $** /CP:$(MSIBUILD_CODEPAGE)
!ENDIF


######################################################################
# FeatureComponents

All :: "$(LANG).$(PLAT).$(CFG).FeatureComponents-1.idt"

"$(LANG).$(PLAT).$(CFG).FeatureComponents-1.idt" : "Makefile" "..\..\include\MSIBuildCfg.mak"
	-if exist $@ del /f /q $@
	move /y << $@ > NUL
Feature_	Component_
s$(MSIBUILD_LENGTH_ID)	s$(MSIBUILD_LENGTH_ID)
FeatureComponents	Feature_	Component_
featZRCola	compZRCola.exe.$(PLAT)
featZRCola	compLocalizationRepositoryPath
featZRCola	compLanguage
featZRCola	compDatabasePath
featZRCola	compZRCola.zrcdb
featZRCola	compZRCola_Re.otf
!IF "$(LANG)" != "en_US"
featZRCola	compZRCola.mo.$(LANG)
featZRCola	compZRCola.zrcdb.mo.$(LANG)
featZRCola	complibZRColaUI.mo.$(LANG)
featZRCola	compwxExtend.mo.$(LANG)
featZRCola	compUpdater.mo.$(LANG)
featZRCola	compwxstd.mo.$(LANG)
!ENDIF
<<NOKEEP


######################################################################
# File

All :: "$(LANG).$(PLAT).$(CFG).File-1.idt"

"$(LANG).$(PLAT).$(CFG).File-1.idt" : "Makefile" "..\..\include\MSIBuildCfg.mak"
	-if exist $@ del /f /q $@
	move /y << $@ > NUL
File	Component_	FileName	FileSize	Version	Language	Attributes	Sequence
s$(MSIBUILD_LENGTH_ID)	s$(MSIBUILD_LENGTH_ID)	l255	i4	S$(MSIBUILD_LENGTH_ID)	S20	I2	i2
File	File
fileZRCola.exe.$(PLAT)	compZRCola.exe.$(PLAT)	ZRCola.exe	0		0	1536	1
!IF "$(LANG)" != "en_US"
fileZRCola.mo.$(LANG)	compZRCola.mo.$(LANG)	ZRCola.mo	0		$(MSIBUILD_LANGID)	0	1
!ENDIF
<<NOKEEP


######################################################################
# Icon

All :: \
	"$(LANG).$(PLAT).$(CFG).Icon-1.idt" \
	"$(LANG).$(PLAT).$(CFG).Icon-1\ZRCola.ico"

"$(LANG).$(PLAT).$(CFG).Icon-1.idt" : "Makefile" "..\..\include\MSIBuildCfg.mak" "$(LANG).$(PLAT).$(CFG).Icon-1"
	-if exist $@ del /f /q $@
	move /y << $@ > NUL
Name	Data
s$(MSIBUILD_LENGTH_ID)	v0
Icon	Name
iconZRCola.ico	ZRCola.ico
<<NOKEEP

"$(LANG).$(PLAT).$(CFG).Icon-1\ZRCola.ico" : "..\res\zrcola.ico"
	copy /y $** $@ > NUL


######################################################################
# RemoveFile

All :: "$(LANG).$(PLAT).$(CFG).RemoveFile-1.idt"

"$(LANG).$(PLAT).$(CFG).RemoveFile-1.idt" : "Makefile" "..\..\include\MSIBuildCfg.mak"
	-if exist $@ del /f /q $@
	move /y << $@ > NUL
FileKey	Component_	FileName	DirProperty	InstallMode
s$(MSIBUILD_LENGTH_ID)	s$(MSIBUILD_LENGTH_ID)	L255	s$(MSIBUILD_LENGTH_ID)	i2
RemoveFile	FileKey
rmfiZRCola	compZRCola.exe.$(PLAT)		ZRColaShortcutDir	2
<<NOKEEP


######################################################################
# Shortcut

All :: "$(LANG).$(PLAT).$(CFG).Shortcut-2.idt"

"en_US.$(PLAT).$(CFG).Shortcut-2.idtx" : "Makefile" "..\..\include\MSIBuildCfg.mak"
	-if exist $@ del /f /q $@
	move /y << $@ > NUL
Shortcut	Directory_	Name	Component_	Target	Arguments	Description	Hotkey	Icon_	IconIndex	ShowCmd	WkDir
s$(MSIBUILD_LENGTH_ID)	s$(MSIBUILD_LENGTH_ID)	l128	s$(MSIBUILD_LENGTH_ID)	s$(MSIBUILD_LENGTH_ID)	S255	L255	I2	S$(MSIBUILD_LENGTH_ID)	I2	I2	S$(MSIBUILD_LENGTH_ID)
1252	Shortcut	Shortcut
shctZRCola	ZRColaShortcutDir	ZRCola	compZRCola.exe.$(PLAT)	featZRCola		Input system for linguistic use		iconZRCola.ico			ZRCOLABINDIR
<<NOKEEP

!IF "$(LANG)" == "en_US"
"en_US.$(PLAT).$(CFG).Shortcut-2.idt" : "en_US.$(PLAT).$(CFG).Shortcut-2.idtx"
	copy /y $** $@ > NUL
!ELSE
"$(LANG).$(PLAT).$(CFG).Shortcut-2.idt" : "en_US.$(PLAT).$(CFG).Shortcut-2.idtx" "..\locale\$(LANG).po"
	cscript.exe "..\..\MSI\MSIBuild\MSI.wsf" //Job:IDTTranslate //Nologo $@ $** /CP:$(MSIBUILD_CODEPAGE)
!ENDIF


######################################################################
# Build MSM module!
######################################################################

!INCLUDE "..\..\MSI\MSIBuild\MSM.mak"
