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

All ::

Clean ::
	msbuild.exe $(MSBUILDFLAGS) "ZRCola.sln"          /t:Clean /p:Configuration=$(CFG) /p:Platform=$(PLAT_SLN)
!IF "$(PLAT)" == "Win32"
	msbuild.exe $(MSBUILDFLAGS) "ZRColaUtils.sln"     /t:Clean /p:Configuration=$(CFG) /p:Platform=$(PLAT_SLN)
!ENDIF
	msbuild.exe $(MSBUILDFLAGS) "MSI\MSICA\MSICA.sln" /t:Clean /p:Configuration=$(CFG) /p:Platform=$(PLAT_SLN)
	msbuild.exe $(MSBUILDFLAGS) "Updater\Updater.sln" /t:Clean /p:Configuration=$(CFG) /p:Platform=$(PLAT_SLN)
	-if exist "$(OUTPUT_DIR)\ZRCola$(PLAT_SUFFIX)$(CFG_SUFFIX).ddf" del /f /q "$(OUTPUT_DIR)\ZRCola$(PLAT_SUFFIX)$(CFG_SUFFIX).ddf"
	-if exist "$(OUTPUT_DIR)\ZRCola$(PLAT_SUFFIX)$(CFG_SUFFIX).cab" del /f /q "$(OUTPUT_DIR)\ZRCola$(PLAT_SUFFIX)$(CFG_SUFFIX).cab"
	-if exist "$(OUTPUT_DIR)\ZRCola$(PLAT_SUFFIX)$(CFG_SUFFIX).inf" del /f /q "$(OUTPUT_DIR)\ZRCola$(PLAT_SUFFIX)$(CFG_SUFFIX).inf"
	-if exist "$(OUTPUT_DIR)\ZRCola$(PLAT_SUFFIX)$(CFG_SUFFIX).rpt" del /f /q "$(OUTPUT_DIR)\ZRCola$(PLAT_SUFFIX)$(CFG_SUFFIX).rpt"


!IFDEF HAS_VERSION

######################################################################
# 2nd Phase
# - The version is known, do the rest.
######################################################################

######################################################################
# Building
######################################################################

"$(OUTPUT_DIR)\$(PLAT).$(CFG)\ZRCola.exe" :: Localization

"$(OUTPUT_DIR)\$(PLAT).$(CFG)\ZRCola.exe" ::
	msbuild.exe $(MSBUILDFLAGS) "ZRCola.sln" /t:Build /p:Configuration=$(CFG) /p:Platform=$(PLAT_SLN)

"$(OUTPUT_DIR)\ZRCola$(PLAT_SUFFIX)$(CFG_SUFFIX).ddf" : \
#	"$(OUTPUT_DIR)\ZRCola.de_DE.$(PLAT_SUFFIX)$(CFG_SUFFIX).2.msi" \
	"$(OUTPUT_DIR)\ZRCola.en_US.$(PLAT_SUFFIX)$(CFG_SUFFIX).2.msi" \
	"$(OUTPUT_DIR)\ZRCola.ru_RU.$(PLAT_SUFFIX)$(CFG_SUFFIX).2.msi" \
	"$(OUTPUT_DIR)\ZRCola.sl_SI.$(PLAT_SUFFIX)$(CFG_SUFFIX).2.msi"
	-if exist $@ del /f /q $@
	-if exist "$(@:"=).tmp" del /f /q "$(@:"=).tmp"
	cscript.exe "MSI\MSIBuild\MSI.wsf" //Job:MakeDDF //Nologo "$(@:"=).tmp" $** /O:"$(OUTPUT_DIR)\ZRCola$(PLAT_SUFFIX)$(CFG_SUFFIX)" /C:LZX
	move /y "$(@:"=).tmp" $@ > NUL

"$(OUTPUT_DIR)\ZRCola$(PLAT_SUFFIX)$(CFG_SUFFIX).cab" \
"$(OUTPUT_DIR)\ZRCola$(PLAT_SUFFIX)$(CFG_SUFFIX).inf" \
"$(OUTPUT_DIR)\ZRCola$(PLAT_SUFFIX)$(CFG_SUFFIX).rpt" : "$(OUTPUT_DIR)\ZRCola$(PLAT_SUFFIX)$(CFG_SUFFIX).ddf"
	makecab.exe /F $**

!ENDIF