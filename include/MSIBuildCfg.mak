#    SPDX-License-Identifier: GPL-3.0-or-later
#    Copyright © 1991-2022 Amebis

######################################################################
# MSI General Package Information

MSIBUILD_PRODUCT_NAME=ZRCola


######################################################################
# Language specific settings

!IF "$(LANG)" == "de_DE"
MSIBUILD_LANGID=1031
MSIBUILD_LANG_GUID=0407
MSIBUILD_CODEPAGE=1252
!ELSEIF "$(LANG)" == "ru_RU"
MSIBUILD_LANGID=1049
MSIBUILD_LANG_GUID=0419
MSIBUILD_CODEPAGE=1251
!ELSEIF "$(LANG)" == "sl_SI"
MSIBUILD_LANGID=1060
MSIBUILD_LANG_GUID=0424
MSIBUILD_CODEPAGE=1250
!ELSE
LANG=en_US
MSIBUILD_LANGID=1033
MSIBUILD_LANG_GUID=0409
MSIBUILD_CODEPAGE=1252
!ENDIF


######################################################################
# Platform specific settings

!IF "$(PLAT)" == "x64"
MSIBUILD_PLAT_GUID=1
!ELSEIF "$(PLAT)" == "ARM64"
MSIBUILD_PLAT_GUID=2
!ELSE
MSIBUILD_PLAT_GUID=0
!ENDIF


######################################################################
# Path to version file
# (relative from MSIBuild\Version folder)

MSIBUILD_VERSION_FILE=..\..\..\include\version.h


######################################################################
# GUID used to determine MSI upgrade logic

MSIBUILD_UPGRADE_GUID={1D2E5E0E-0B83-4D50-AC6$(MSIBUILD_PLAT_GUID)-1121A71FF09B}
MSIBUILD_REMOVE_BEFORE_INSTALL_CONDITION=OLDPRODUCTS1


######################################################################
# Minimum MSI version required to install this package

!IF "$(PLAT)" == "ARM64"
MSIBUILD_MSI_VERSION_MIN=500
!ELSE
MSIBUILD_MSI_VERSION_MIN=200
!ENDIF


######################################################################
# Length of ID and help fields in MSI tables (in characters)

MSIBUILD_LENGTH_ID=128
MSIBUILD_LENGTH_HELP=256


######################################################################
# Should MSIBuild compress files into CAB itself?

#MSIBUILD_COMPRESS=1


######################################################################
# Trigger per-user setup on logon (Active Setup)

MSIBUILD_HAS_ACTIVE_SETUP=1


######################################################################
# Prevent installation of 32-bit MSI on 64-bit Windows

MSIBUILD_HAS_X64=1
MSIBUILD_NO_WOW64=1


######################################################################
# Component and registry settings (platform dependant)

!IF "$(PLAT)" == "x64" || "$(PLAT)" == "ARM64"
MSIBUILD_COMPONENT_ATTRIB_FILE=256
MSIBUILD_COMPONENT_ATTRIB_REGISTRY=260
MSIBUILD_REG32_RELOCATION=\Wow6432Node
!ELSE
MSIBUILD_COMPONENT_ATTRIB_FILE=0
MSIBUILD_COMPONENT_ATTRIB_REGISTRY=4
MSIBUILD_REG32_RELOCATION=
!ENDIF


######################################################################
# List of modules to compile and include in link
# (relative from MSI\ZRCola folder)

MSIBUILD_MODULES=\
	"..\MSIBuild\Core\$(LANG).$(PLAT).$(CFG).msm" \
	"..\MSIBuild\Version\$(LANG).$(PLAT).$(CFG).msm" \
	"..\MSIBuild\UI\$(LANG).$(PLAT).$(CFG).msm" \
	"..\MSICA\lib\MSICALib\MSIBuild\$(LANG).$(PLAT).$(CFG).msm" \
	"..\MSICA\MSICA\MSIBuild\$(LANG).$(PLAT).$(CFG).msm" \
	"..\..\Updater\Updater\MSIBuild\$(LANG).$(PLAT).$(CFG).msm" \
	"..\..\Updater\UpdCheck\MSIBuild\$(LANG).$(PLAT).$(CFG).msm" \
	"..\..\lib\libZRColaUI\MSIBuild\$(LANG).$(PLAT).$(CFG).msm" \
	"..\..\lib\wxExtend\MSIBuild\$(LANG).$(PLAT).$(CFG).msm" \
	"..\..\ZRCola\MSIBuild\$(LANG).$(PLAT).$(CFG).msm" \
	"Main\$(LANG).$(PLAT).$(CFG).msm"


######################################################################
# wxExtend Module

WXEXTEND_STATIC=1
WXEXTEND_BIN_DIR=ZRCOLABINDIR
WXEXTEND_LOC_DIR=ZRCOLALOCDIR


######################################################################
# Updater Module

UPDATER_FEATURE=
UPDATER_BIN_DIR=ZRCOLABINDIR
UPDATER_LOC_DIR=ZRCOLALOCDIR
UPDATER_VERSION=ZRCola
