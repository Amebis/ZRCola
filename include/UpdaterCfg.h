/*
    Copyright 2015-2020 Amebis

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

#include "version.h"

//
// Update server(s)
// Zero delimited list of update servers
//
#define UPDATER_HTTP_SERVER         "prenos.amebis.si\0"

//
// Update server port
//
#define UPDATER_HTTP_PORT           80

//
// Update server catalog location
//
#define UPDATER_HTTP_PATH           "/ZRCola/catalog-0001.xml"

//
// Hash for publishing packages and signing catalog
//
#define wxUpdaterHashGen            wxCryptoHashSHA256

//
// Hash for verifying catalog signature and packages
//
#define wxUpdaterHashChk            wxCryptoHashSHA256

//
// When changing the hash algorithm:
//
// 1. Bump UPDATER_HTTP_PATH and set wxUpdaterHashChk to new hash class.
// 2. Publish the updated product using the old catalog and old wxUpdaterHashGen hash.
// 3. Set wxUpdaterHashGen to new hash class, update Makefile to publish future updates to the new catalog.
//
