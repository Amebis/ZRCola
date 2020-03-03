#
#    Copyright 1991-2020 Amebis
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

Clean ::
	msbuild.exe $(MSBUILDFLAGS) "ZRCola.sln"          /t:Clean /p:Configuration=$(CFG) /p:Platform=$(PLAT)
!IF "$(PLAT)" == "Win32"
	msbuild.exe $(MSBUILDFLAGS) "ZRColaUtils.sln"     /t:Clean /p:Configuration=$(CFG) /p:Platform=$(PLAT)
!ENDIF
	msbuild.exe $(MSBUILDFLAGS) "MSI\MSICA\MSICA.sln" /t:Clean /p:Configuration=$(CFG) /p:Platform=$(PLAT)
	msbuild.exe $(MSBUILDFLAGS) "Updater\Updater.sln" /t:Clean /p:Configuration=$(CFG) /p:Platform=$(PLAT)
	-if exist "$(OUTPUT_DIR)\$(PLAT).$(CFG).ddf" del /f /q "$(OUTPUT_DIR)\$(PLAT).$(CFG).ddf"
	-if exist "$(OUTPUT_DIR)\$(PLAT).$(CFG).cab" del /f /q "$(OUTPUT_DIR)\$(PLAT).$(CFG).cab"
	-if exist "$(OUTPUT_DIR)\$(PLAT).$(CFG).inf" del /f /q "$(OUTPUT_DIR)\$(PLAT).$(CFG).inf"
	-if exist "$(OUTPUT_DIR)\$(PLAT).$(CFG).rpt" del /f /q "$(OUTPUT_DIR)\$(PLAT).$(CFG).rpt"


!IFDEF HAS_VERSION

######################################################################
# 2nd Phase
# - The version is known, do the rest.
######################################################################

######################################################################
# Building
######################################################################

"$(OUTPUT_DIR)\$(PLAT).$(CFG).ddf" : \
#	"$(OUTPUT_DIR)\de_DE.$(PLAT).$(CFG).2.msi" \
	"$(OUTPUT_DIR)\en_US.$(PLAT).$(CFG).2.msi" \
	"$(OUTPUT_DIR)\ru_RU.$(PLAT).$(CFG).2.msi" \
	"$(OUTPUT_DIR)\sl_SI.$(PLAT).$(CFG).2.msi"
	-if exist "$(@:"=).tmp" del /f /q "$(@:"=).tmp"
	cscript.exe "MSI\MSIBuild\MSI.wsf" //Job:MakeDDF //Nologo "$(@:"=).tmp" $** /O:"$(OUTPUT_DIR)\$(PLAT).$(CFG)" /C:LZX
	move /y "$(@:"=).tmp" $@ > NUL

"$(OUTPUT_DIR)\$(PLAT).$(CFG).cab" \
"$(OUTPUT_DIR)\$(PLAT).$(CFG).inf" \
"$(OUTPUT_DIR)\$(PLAT).$(CFG).rpt" : "$(OUTPUT_DIR)\$(PLAT).$(CFG).ddf"
	makecab.exe /F $**

!ENDIF