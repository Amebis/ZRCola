#
#    Copyright 1991-2019 Amebis
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

All ::

Clean ::
	-if exist "$(OUTPUT_DIR)\locale\$(LANG)\wxstd.mo"        del /f /q "$(OUTPUT_DIR)\locale\$(LANG)\wxstd.mo"
	-if exist "$(OUTPUT_DIR)\locale\$(LANG)\ZRCola-zrcdb.mo" del /f /q "$(OUTPUT_DIR)\locale\$(LANG)\ZRCola-zrcdb.mo"

!IFDEF HAS_VERSION

######################################################################
# 2nd Phase
# - The version is known, do the rest.
######################################################################

######################################################################
# Localization
######################################################################

Localization :: \
	"$(OUTPUT_DIR)\locale\$(LANG)" \
	"$(OUTPUT_DIR)\locale\$(LANG)\wxstd.mo" \
	"$(OUTPUT_DIR)\locale\$(LANG)\ZRCola-zrcdb.mo"


######################################################################
# Folder creation
######################################################################

"$(OUTPUT_DIR)\locale\$(LANG)" :
	if not exist $@ md $@

"$(OUTPUT_DIR)\locale\$(LANG)" : "$(OUTPUT_DIR)\locale"


######################################################################
# File copy
######################################################################

"$(PUBLISH_DIR)\ZRColaInstall$(LANG_SUFFIX).exe" : "$(OUTPUT_DIR)\Win32.Release\ZRColaInstall$(LANG_SUFFIX).exe"
	copy /y $** $@ > NUL


######################################################################
# Building
######################################################################

"$(OUTPUT_DIR)\locale\$(LANG)\wxstd.mo" : \
!IF EXISTS("$(WXWIN)\locale\$(LANG).po")
	"$(WXWIN)\locale\$(LANG).po"
!ELSEIF EXISTS("$(WXWIN)\locale\$(LANG_BASE).po")
	"$(WXWIN)\locale\$(LANG_BASE).po"
!ELSE
	"$(WXWIN)\locale\wxstd.pot"
!ENDIF
	msgfmt.exe --output-file=$@ --alignment=1 --endianness=little $**

"$(OUTPUT_DIR)\locale\$(LANG)\ZRCola-zrcdb.mo" : \
!IF EXISTS("$(OUTPUT_DIR)\locale\$(LANG)\ZRCola-zrcdb.po")
	"$(OUTPUT_DIR)\locale\$(LANG)\ZRCola-zrcdb.po"
!ELSE
	"$(OUTPUT_DIR)\locale\ZRCola-zrcdb.pot"
!ENDIF
	msgfmt.exe --output-file=$@ --alignment=1 --endianness=little $**

"$(OUTPUT_DIR)\Win32.Release\ZRColaInstall$(LANG_SUFFIX).exe" ::
	msbuild.exe $(MSBUILDFLAGS) "ZRColaUtils.sln" /t:Build /p:Configuration=Release /p:Platform=x86

!ENDIF


######################################################################
# Platform-configuration Specific
######################################################################

PLAT=Win32
PLAT_SUFFIX=32
PLAT_SLN=x86

CFG=Release
CFG_SUFFIX=
!INCLUDE "MakefileLangPlatCfg.mak"

CFG=Debug
CFG_SUFFIX=D
!INCLUDE "MakefileLangPlatCfg.mak"

PLAT=x64
PLAT_SUFFIX=64
PLAT_SLN=x64

CFG=Release
CFG_SUFFIX=
!INCLUDE "MakefileLangPlatCfg.mak"

CFG=Debug
CFG_SUFFIX=D
!INCLUDE "MakefileLangPlatCfg.mak"
