/*
    Copyright 2015-2016 Amebis

    This file is part of ZRCola.

    ZRCola is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    ZRCola is distributed in the hope that it will be useful, but
    WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with ZRCola. If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

//
// Product version as a single DWORD
// Note: Used for version comparison within C/C++ code.
//
#define ZRCOLA_VERSION          0x01ff0c00

//
// Product version by components
// Note: Resource Compiler has limited preprocessing capability,
// thus we need to specify major, minor and other version components
// separately.
//
#define ZRCOLA_VERSION_MAJ      1
#define ZRCOLA_VERSION_MIN      255
#define ZRCOLA_VERSION_REV      12
#define ZRCOLA_VERSION_BUILD    0

//
// Human readable product version and build year for UI
//
#define ZRCOLA_VERSION_STR      "2.0-beta4"
#define ZRCOLA_BUILD_YEAR_STR   "2016"

//
// Numerical version presentation for ProductVersion propery in
// MSI packages (syntax: N.N[.N[.N]])
//
#define ZRCOLA_VERSION_INST     "1.255.12"

//
// The product code for ProductCode property in MSI packages
// Replace with new on every version change, regardless how minor it is.
//
#define ZRCOLA_VERSION_GUID     "{EAC8769F-1036-4565-828E-06B3220F1EF7}"

//
// The product vendor and application name for configuration keeping.
//
#define ZRCOLA_CFG_VENDOR       "Amebis"
#define ZRCOLA_CFG_APPLICATION  "ZRCola"
