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
# Working/output folder for building MSI file
# (relative from MSIBuild folder)

MSIBUILD_OUTPUT_DIR=..\..\output


######################################################################
# Path to MSIBuild folder
# (relative from ZRCola folder)

MSIBUILD_ROOT=..\MSIBuild


######################################################################
# GUID used to determine MSI upgrade logic

MSIBUILD_UPGRADE_GUID={1D2E5E0E-0B83-4D50-AC6$(MSIBUILD_PLAT_GUID)-1121A71FF09B}
MSIBUILD_REMOVE_BEFORE_INSTALL_CONDITION=OLDPRODUCTFOUND1


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

MSIBUILD_MODULES=\
	"$(MSIBUILD_ROOT)\Core\$(LANG).$(PLAT).$(CFG).msm" \
	"$(MSIBUILD_ROOT)\Version\$(LANG).$(PLAT).$(CFG).msm" \
	"$(MSIBUILD_ROOT)\UI\$(LANG).$(PLAT).$(CFG).msm" \
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
