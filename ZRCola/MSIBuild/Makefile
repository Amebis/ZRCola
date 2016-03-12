#
#    Copyright 1991-2016 Amebis
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

!INCLUDE "..\..\MSI\include\MSIBuildCfg.mak"

MSIBUILD_IS_LOCALIZEABLE=1


######################################################################
# AppSearch

All :: "$(LANG).$(PLAT).$(CFG).AppSearch-1.idt"

"$(LANG).$(PLAT).$(CFG).AppSearch-1.idt" : "Makefile" "..\..\MSI\include\MSIBuildCfg.mak"
	-if exist $@ del /f /q $@
	move /y << $@ > NUL
Property	Signature_
s$(MSIBUILD_LENGTH_ID)	s$(MSIBUILD_LENGTH_ID)
AppSearch	Property	Signature_
OLDZRCOLADIR	clZRCola.exe.$(PLAT)
<<NOKEEP


######################################################################
# CompLocator

All :: "$(LANG).$(PLAT).$(CFG).CompLocator-1.idt"

"$(LANG).$(PLAT).$(CFG).CompLocator-1.idt" : "Makefile" "..\..\MSI\include\MSIBuildCfg.mak"
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

"$(LANG).$(PLAT).$(CFG).Component-1.idt" : "Makefile" "..\..\MSI\include\MSIBuildCfg.mak"
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
<<NOKEEP


######################################################################
# Feature

All :: "$(LANG).$(PLAT).$(CFG).Feature-2.idt"

"En.$(PLAT).$(CFG).Feature-2.idtx" : "Makefile" "..\..\MSI\include\MSIBuildCfg.mak"
	-if exist $@ del /f /q $@
	move /y << $@ > NUL
Feature	Feature_Parent	Title	Description	Display	Level	Directory_	Attributes
s$(MSIBUILD_LENGTH_ID)	S$(MSIBUILD_LENGTH_ID)	L64	L255	I2	i2	S$(MSIBUILD_LENGTH_ID)	i2
1252	Feature	Feature
featZRCola		ZRCola	Input system for linguistic use	1	1	ZRCOLADIR	8
<<NOKEEP

"De.$(PLAT).$(CFG).Feature-2.idt" : "En.$(PLAT).$(CFG).Feature-2.idtx" "..\locale\de_DE.po"
	rcxgettext.exe idtp $@ $**

"En.$(PLAT).$(CFG).Feature-2.idt" : "En.$(PLAT).$(CFG).Feature-2.idtx"
	copy /y $** $@ > NUL

"It.$(PLAT).$(CFG).Feature-2.idt" : "En.$(PLAT).$(CFG).Feature-2.idtx" "..\locale\it_IT.po"
	rcxgettext.exe idtp $@ $**

"Sl.$(PLAT).$(CFG).Feature-2.idt" : "En.$(PLAT).$(CFG).Feature-2.idtx" "..\locale\sl.po"
	rcxgettext.exe idtp $@ $**


######################################################################
# FeatureComponents

All :: "$(LANG).$(PLAT).$(CFG).FeatureComponents-1.idt"

"$(LANG).$(PLAT).$(CFG).FeatureComponents-1.idt" : "Makefile" "..\..\MSI\include\MSIBuildCfg.mak"
	-if exist $@ del /f /q $@
	move /y << $@ > NUL
Feature_	Component_
s$(MSIBUILD_LENGTH_ID)	s$(MSIBUILD_LENGTH_ID)
FeatureComponents	Feature_	Component_
featZRCola	complibZRCola.dll.$(PLAT)
featZRCola	complibZRColaUI.dll.$(PLAT)
featZRCola	compZRCola.exe.$(PLAT)
<<NOKEEP


######################################################################
# File

All :: "$(LANG).$(PLAT).$(CFG).File-1.idt"

"$(LANG).$(PLAT).$(CFG).File-1.idt" : "Makefile" "..\..\MSI\include\MSIBuildCfg.mak"
	-if exist $@ del /f /q $@
	move /y << $@ > NUL
File	Component_	FileName	FileSize	Version	Language	Attributes	Sequence
s$(MSIBUILD_LENGTH_ID)	s$(MSIBUILD_LENGTH_ID)	l255	i4	S$(MSIBUILD_LENGTH_ID)	S20	I2	i2
File	File
fileZRCola.exe.$(PLAT)	compZRCola.exe.$(PLAT)	ZRCola.exe	0		0	1536	1
<<NOKEEP


######################################################################
# Icon

All :: \
	"$(LANG).$(PLAT).$(CFG).Icon-1.idt" \
	"$(LANG).$(PLAT).$(CFG).Icon-1\ZRCola.ico"

"$(LANG).$(PLAT).$(CFG).Icon-1.idt" : "Makefile" "..\..\MSI\include\MSIBuildCfg.mak" "$(LANG).$(PLAT).$(CFG).Icon-1"
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

"$(LANG).$(PLAT).$(CFG).RemoveFile-1.idt" : "Makefile" "..\..\MSI\include\MSIBuildCfg.mak"
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

"En.$(PLAT).$(CFG).Shortcut-2.idtx" : "Makefile" "..\..\MSI\include\MSIBuildCfg.mak"
	-if exist $@ del /f /q $@
	move /y << $@ > NUL
Shortcut	Directory_	Name	Component_	Target	Arguments	Description	Hotkey	Icon_	IconIndex	ShowCmd	WkDir
s$(MSIBUILD_LENGTH_ID)	s$(MSIBUILD_LENGTH_ID)	l128	s$(MSIBUILD_LENGTH_ID)	s$(MSIBUILD_LENGTH_ID)	S255	L255	I2	S$(MSIBUILD_LENGTH_ID)	I2	I2	S$(MSIBUILD_LENGTH_ID)
1252	Shortcut	Shortcut
shctZRCola	ZRColaShortcutDir	ZRCola	compZRCola.exe.$(PLAT)	featZRCola		Input system for linguistic use		iconZRCola.ico			ZRCOLABINDIR
<<NOKEEP

"De.$(PLAT).$(CFG).Shortcut-2.idt" : "En.$(PLAT).$(CFG).Shortcut-2.idtx" "..\locale\de_DE.po"
	rcxgettext.exe idtp $@ $**

"En.$(PLAT).$(CFG).Shortcut-2.idt" : "En.$(PLAT).$(CFG).Shortcut-2.idtx"
	copy /y $** $@ > NUL

"It.$(PLAT).$(CFG).Shortcut-2.idt" : "En.$(PLAT).$(CFG).Shortcut-2.idtx" "..\locale\it_IT.po"
	rcxgettext.exe idtp $@ $**

"Sl.$(PLAT).$(CFG).Shortcut-2.idt" : "En.$(PLAT).$(CFG).Shortcut-2.idtx" "..\locale\sl.po"
	rcxgettext.exe idtp $@ $**


######################################################################
# Build MSM module!
######################################################################

!INCLUDE "..\..\MSI\MSIBuild\MSM.mak"
