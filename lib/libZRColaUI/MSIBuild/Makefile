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

!INCLUDE "..\..\..\MSI\include\MSIBuildCfg.mak"


######################################################################
# AppSearch

All :: "$(LANG).$(PLAT).$(CFG).AppSearch-1.idt"

"$(LANG).$(PLAT).$(CFG).AppSearch-1.idt" : "Makefile" "..\..\..\MSI\include\MSIBuildCfg.mak"
	-if exist $@ del /f /q $@
	move /y << $@ > NUL
Property	Signature_
s$(MSIBUILD_LENGTH_ID)	s$(MSIBUILD_LENGTH_ID)
AppSearch	Property	Signature_
OLDZRCOLADIR	cllibZRColaUI10u_vc100.dll.$(PLAT)
<<NOKEEP


######################################################################
# CompLocator

All :: "$(LANG).$(PLAT).$(CFG).CompLocator-1.idt"

"$(LANG).$(PLAT).$(CFG).CompLocator-1.idt" : "Makefile" "..\..\..\MSI\include\MSIBuildCfg.mak"
	-if exist $@ del /f /q $@
	move /y << $@ > NUL
Signature_	ComponentId	Type
s$(MSIBUILD_LENGTH_ID)	s38	I2
CompLocator	Signature_
!IF "$(PLAT)" == "Win32"
cllibZRColaUI10u_vc100.dll.Win32	{D9A5BF44-DDFE-4A22-89F4-14D291581829}	1
!ENDIF
!IF "$(PLAT)" == "x64"
cllibZRColaUI10u_vc100.dll.x64	{360E78E5-9560-4C52-B806-45EDC682BB17}	1
!ENDIF
<<NOKEEP


######################################################################
# Component

All :: "$(LANG).$(PLAT).$(CFG).Component-1.idt"

"$(LANG).$(PLAT).$(CFG).Component-1.idt" : "Makefile" "..\..\..\MSI\include\MSIBuildCfg.mak"
	-if exist $@ del /f /q $@
	move /y << $@ > NUL
Component	ComponentId	Directory_	Attributes	Condition	KeyPath
s$(MSIBUILD_LENGTH_ID)	S38	s$(MSIBUILD_LENGTH_ID)	i2	S255	S$(MSIBUILD_LENGTH_ID)
Component	Component
!IF "$(PLAT)" == "Win32"
complibZRColaUI.dll.Win32	{D9A5BF44-DDFE-4A22-89F4-14D291581829}	ZRCOLABINDIR	0		filelibZRColaUI.dll.Win32
!ENDIF
!IF "$(PLAT)" == "x64"
complibZRColaUI.dll.x64	{360E78E5-9560-4C52-B806-45EDC682BB17}	ZRCOLABINDIR	256		filelibZRColaUI.dll.x64
!ENDIF
<<NOKEEP


######################################################################
# File

All :: "$(LANG).$(PLAT).$(CFG).File-1.idt"

"$(LANG).$(PLAT).$(CFG).File-1.idt" : "Makefile" "..\..\..\MSI\include\MSIBuildCfg.mak"
	-if exist $@ del /f /q $@
	move /y << $@ > NUL
File	Component_	FileName	FileSize	Version	Language	Attributes	Sequence
s$(MSIBUILD_LENGTH_ID)	s$(MSIBUILD_LENGTH_ID)	l255	i4	S$(MSIBUILD_LENGTH_ID)	S20	I2	i2
File	File
!IF "$(PLAT)" == "Win32"
!IF "$(CFG)" == "Release"
filelibZRColaUI.dll.Win32	complibZRColaUI.dll.Win32	LIBZRC~2.DLL|libZRColaUI10u_vc100.dll	0		0	1536	1
!ENDIF
!IF "$(CFG)" == "Debug"
filelibZRColaUI.dll.Win32	complibZRColaUI.dll.Win32	LIBZRC~4.DLL|libZRColaUI10ud_vc100.dll	0		0	1536	1
!ENDIF
!ENDIF
!IF "$(PLAT)" == "x64"
!IF "$(CFG)" == "Release"
filelibZRColaUI.dll.x64	complibZRColaUI.dll.x64	LIBZRC~6.DLL|libZRColaUI10u_vc100_x64.dll	0		0	1536	1
!ENDIF
!IF "$(CFG)" == "Debug"
filelibZRColaUI.dll.x64	complibZRColaUI.dll.x64	LIBZRC~8.DLL|libZRColaUI10ud_vc100_x64.dll	0		0	1536	1
!ENDIF
!ENDIF
<<NOKEEP


######################################################################
# Build MSM module!
######################################################################

!INCLUDE "..\..\..\MSI\MSIBuild\MSM.mak"