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

# Note: When changing WXWIDGETS_VER or WXWIDGETS_COMPILER, the DLL filenames will change.
# Then, you MUST change the component GUIDs as well.
WXWIDGETS_VER=311
WXWIDGETS_COMPILER=_vc100

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
compwxbase.dll.Win32	{4C7F5A16-5B74-47D5-A405-57C7BD96F090}	ZRCOLABINDIR	0		filewxbase.dll.Win32
compwxbase_net.dll.Win32	{00534942-B21F-4401-BFE4-C7677F4BE3E5}	ZRCOLABINDIR	0		filewxbase_net.dll.Win32
compwxbase_xml.dll.Win32	{71E30748-DD22-4D8B-B26F-60C5150D9AAE}	ZRCOLABINDIR	0		filewxbase_xml.dll.Win32
compwxmsw_core.dll.Win32	{FB8D46B9-86DC-4590-8F0A-8294F6A45E6A}	ZRCOLABINDIR	0		filewxwxmsw_core.dll.Win32
compwxmsw_adv.dll.Win32	{7B7DB31D-1760-493A-BBE9-35237A6EF9E0}	ZRCOLABINDIR	0		filewxmsw_adv.dll.Win32
compwxmsw_aui.dll.Win32	{49CF9864-63B6-4F43-8EC7-7294B966B419}	ZRCOLABINDIR	0		filewxmsw_aui.dll.Win32
!ENDIF
!IF "$(PLAT)" == "x64"
compwxbase.dll.x64	{905D8859-BFD2-410A-81DB-93231088C066}	ZRCOLABINDIR	256		filewxbase.dll.x64
compwxbase_net.dll.x64	{8A0A49C8-DAEB-4983-8E8D-9DE30AE1D0B6}	ZRCOLABINDIR	256		filewxbase_net.dll.x64
compwxbase_xml.dll.x64	{24A5F181-EFE7-4542-8AAB-1D21DB74E954}	ZRCOLABINDIR	256		filewxbase_xml.dll.x64
compwxmsw_core.dll.x64	{F5B3F6E6-4FFB-4A79-A794-202D6F93F716}	ZRCOLABINDIR	256		filewxwxmsw_core.dll.x64
compwxmsw_adv.dll.x64	{D7722FB9-59BB-498F-93C3-78451471A023}	ZRCOLABINDIR	256		filewxmsw_adv.dll.x64
compwxmsw_aui.dll.x64	{FB703979-0E3D-458E-82DD-DAC33329FA9D}	ZRCOLABINDIR	256		filewxmsw_aui.dll.x64
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
filewxbase.dll.$(PLAT)	compwxbase.dll.$(PLAT)	WXBASE~1.DLL|wxbase$(WXWIDGETS_VER)$(WXWIDGETS_CFG)$(WXWIDGETS_COMPILER)$(WXWIDGETS_PLAT).dll	0		0	512	1
filewxbase_net.dll.$(PLAT)	compwxbase_net.dll.$(PLAT)	WXBASE~2.DLL|wxbase$(WXWIDGETS_VER)$(WXWIDGETS_CFG)_net$(WXWIDGETS_COMPILER)$(WXWIDGETS_PLAT).dll	0		0	512	1
filewxbase_xml.dll.$(PLAT)	compwxbase_xml.dll.$(PLAT)	WXBASE~3.DLL|wxbase$(WXWIDGETS_VER)$(WXWIDGETS_CFG)_xml$(WXWIDGETS_COMPILER)$(WXWIDGETS_PLAT).dll	0		0	512	1
filewxwxmsw_core.dll.$(PLAT)	compwxmsw_core.dll.$(PLAT)	WXMSW3~1.DLL|wxmsw$(WXWIDGETS_VER)$(WXWIDGETS_CFG)_core$(WXWIDGETS_COMPILER)$(WXWIDGETS_PLAT).dll	0		0	512	1
filewxmsw_adv.dll.$(PLAT)	compwxmsw_adv.dll.$(PLAT)	WXMSW3~2.DLL|wxmsw$(WXWIDGETS_VER)$(WXWIDGETS_CFG)_adv$(WXWIDGETS_COMPILER)$(WXWIDGETS_PLAT).dll	0		0	512	1
filewxmsw_aui.dll.$(PLAT)	compwxmsw_aui.dll.$(PLAT)	WXMSW3~3.DLL|wxmsw$(WXWIDGETS_VER)$(WXWIDGETS_CFG)_aui$(WXWIDGETS_COMPILER)$(WXWIDGETS_PLAT).dll	0		0	512	1
<<NOKEEP


######################################################################
# Build MSM module!
######################################################################

!INCLUDE "..\..\MSIBuild\MSM.mak"
