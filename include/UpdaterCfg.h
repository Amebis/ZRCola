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
