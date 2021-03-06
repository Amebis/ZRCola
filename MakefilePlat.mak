#
#    Copyright © 1991-2021 Amebis
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

######################################################################
# Setup
######################################################################

SetupCompile ::
	msbuild.exe $(MSBUILDFLAGS) "ZRCola.sln"          /t:Build /p:Configuration=Release /p:Platform=$(PLAT)
	msbuild.exe $(MSBUILDFLAGS) "MSI\MSICA\MSICA.sln" /t:Build /p:Configuration=Release /p:Platform=$(PLAT)
	msbuild.exe $(MSBUILDFLAGS) "Updater\Updater.sln" /t:Build /p:Configuration=Release /p:Platform=$(PLAT)

SetupDebugCompile ::
	msbuild.exe $(MSBUILDFLAGS) "ZRCola.sln"          /t:Build /p:Configuration=Debug   /p:Platform=$(PLAT)
	msbuild.exe $(MSBUILDFLAGS) "MSI\MSICA\MSICA.sln" /t:Build /p:Configuration=Debug   /p:Platform=$(PLAT)
	msbuild.exe $(MSBUILDFLAGS) "Updater\Updater.sln" /t:Build /p:Configuration=Debug   /p:Platform=$(PLAT)


######################################################################
# Configuration Specific
######################################################################

CFG=Release
CFG_SUFFIX=
!INCLUDE "MakefilePlatCfg.mak"

CFG=Debug
CFG_SUFFIX=D
!INCLUDE "MakefilePlatCfg.mak"
