#    SPDX-License-Identifier: GPL-3.0-or-later
#    Copyright © 1991-2021 Amebis

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
# Publishing
######################################################################

"$(OUTPUT_DIR)\PDB.zip" : $(OUTPUT_DIR)\$(PLAT).Release\*.pdb


######################################################################
# Configuration Specific
######################################################################

CFG=Release
CFG_SUFFIX=
!INCLUDE "MakefilePlatCfg.mak"

CFG=Debug
CFG_SUFFIX=D
!INCLUDE "MakefilePlatCfg.mak"
