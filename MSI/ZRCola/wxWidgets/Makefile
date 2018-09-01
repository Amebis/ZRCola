#
#    Copyright 1991-2017 Amebis
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

!INCLUDE "..\..\..\include\MSIBuildCfg.mak"

!IF "$(PLAT)" == "x64"
WXWIDGETS_PLAT=_x64
!ELSE
WXWIDGETS_PLAT=
!ENDIF

!IF "$(CFG)" == "Debug"
WXWIDGETS_CFG=ud
!ELSE
WXWIDGETS_CFG=u
!ENDIF


######################################################################
# Component

All :: "$(LANG).$(PLAT).$(CFG).Component-1.idt"

"$(LANG).$(PLAT).$(CFG).Component-1.idt" : "Makefile" "..\..\..\include\MSIBuildCfg.mak"
	-if exist $@ del /f /q $@
	move /y << $@ > NUL
Component	ComponentId	Directory_	Attributes	Condition	KeyPath
s$(MSIBUILD_LENGTH_ID)	S38	s$(MSIBUILD_LENGTH_ID)	i2	S255	S$(MSIBUILD_LENGTH_ID)
Component	Component
!IF "$(PLAT)" == "Win32"
compwxbase.dll.Win32	{86B11E59-4F0A-45F7-A4F6-B4813A5F8A39}	ZRCOLABINDIR	0		filewxbase.dll.Win32
compwxbase_net.dll.Win32	{7E138C1C-20CF-4FB5-9504-7F3DD4DC5E54}	ZRCOLABINDIR	0		filewxbase_net.dll.Win32
compwxbase_xml.dll.Win32	{B8175665-7408-4D9A-9341-D2933E7D5BC6}	ZRCOLABINDIR	0		filewxbase_xml.dll.Win32
compwxmsw_core.dll.Win32	{EEE366E4-F151-4E43-9760-1E925906D292}	ZRCOLABINDIR	0		filewxwxmsw_core.dll.Win32
compwxmsw_adv.dll.Win32	{C18CD779-BC33-4AD2-8E73-25B6AEDB7A40}	ZRCOLABINDIR	0		filewxmsw_adv.dll.Win32
compwxmsw_aui.dll.Win32	{BB9D5874-A26B-4143-8329-59EDD6E09B1C}	ZRCOLABINDIR	0		filewxmsw_aui.dll.Win32
!ENDIF
!IF "$(PLAT)" == "x64"
compwxbase.dll.x64	{AD46C6EF-EF2E-4DC7-8BBB-255F9881C9C6}	ZRCOLABINDIR	256		filewxbase.dll.x64
compwxbase_net.dll.x64	{DE55D199-B90D-4F59-AB5A-08408D8276C2}	ZRCOLABINDIR	256		filewxbase_net.dll.x64
compwxbase_xml.dll.x64	{13B4FB42-8446-4B64-BBE8-130E87DEAB9A}	ZRCOLABINDIR	256		filewxbase_xml.dll.x64
compwxmsw_core.dll.x64	{9E2A25DD-0726-44C2-97CD-6AD74BAFE4DC}	ZRCOLABINDIR	256		filewxwxmsw_core.dll.x64
compwxmsw_adv.dll.x64	{C0B70F8C-9885-4188-84E0-2C57A67F32FB}	ZRCOLABINDIR	256		filewxmsw_adv.dll.x64
compwxmsw_aui.dll.x64	{ED92B13B-A964-43DC-BB8A-0E47C4AEE31C}	ZRCOLABINDIR	256		filewxmsw_aui.dll.x64
!ENDIF
<<NOKEEP


######################################################################
# File

All :: "$(LANG).$(PLAT).$(CFG).File-1.idt"

"$(LANG).$(PLAT).$(CFG).File-1.idt" : "Makefile" "..\..\..\include\MSIBuildCfg.mak"
	-if exist $@ del /f /q $@
	move /y << $@ > NUL
File	Component_	FileName	FileSize	Version	Language	Attributes	Sequence
s$(MSIBUILD_LENGTH_ID)	s$(MSIBUILD_LENGTH_ID)	l255	i4	S$(MSIBUILD_LENGTH_ID)	S20	I2	i2
File	File
filewxbase.dll.$(PLAT)	compwxbase.dll.$(PLAT)	WXBASE~1.DLL|wxbase30$(WXWIDGETS_CFG)_vc100$(WXWIDGETS_PLAT).dll	0		0	512	1
filewxbase_net.dll.$(PLAT)	compwxbase_net.dll.$(PLAT)	WXBASE~2.DLL|wxbase30$(WXWIDGETS_CFG)_net_vc100$(WXWIDGETS_PLAT).dll	0		0	512	1
filewxbase_xml.dll.$(PLAT)	compwxbase_xml.dll.$(PLAT)	WXBASE~3.DLL|wxbase30$(WXWIDGETS_CFG)_xml_vc100$(WXWIDGETS_PLAT).dll	0		0	512	1
filewxwxmsw_core.dll.$(PLAT)	compwxmsw_core.dll.$(PLAT)	WXMSW3~1.DLL|wxmsw30$(WXWIDGETS_CFG)_core_vc100$(WXWIDGETS_PLAT).dll	0		0	512	1
filewxmsw_adv.dll.$(PLAT)	compwxmsw_adv.dll.$(PLAT)	WXMSW3~2.DLL|wxmsw30$(WXWIDGETS_CFG)_adv_vc100$(WXWIDGETS_PLAT).dll	0		0	512	1
filewxmsw_aui.dll.$(PLAT)	compwxmsw_aui.dll.$(PLAT)	WXMSW3~3.DLL|wxmsw30$(WXWIDGETS_CFG)_aui_vc100$(WXWIDGETS_PLAT).dll	0		0	512	1
<<NOKEEP


######################################################################
# Build MSM module!
######################################################################

!INCLUDE "..\..\MSIBuild\MSM.mak"
