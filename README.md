# ZRCola

A Microsoft Windows application for composing texts using a wide range of Slavic (and general) letters from or beyond Unicode.

## Project Website
http://zrcola.zrc-sazu.si/en/

## Binaries for Download
https://prenos.amebis.si/ZRCola/

## Building

### Building Environment Requirements
- Microsoft Windows Vista or later
- Microsoft Visual Studio 2017
- _sed.exe_ and _grep.exe_
- Command line utilities from Microsoft Windows SDK Bin folder: distributed with Visual Studio, add Bin folder to path manually.
- Additional command line utilities from project's bin folder: add bin folder to path. The source code is provided on request.

### wxWidgets
ZRCola is using wxWidgets static libraries. You have to compile static libraries yourself.

#### Compiling wxWidgets x86 static libraries
1. Start _x86 Native Tools Command Prompt for VS 2017_
2. Change working folder to `build\msw`
3. Run: `nmake /f makefile.vc /ls RUNTIME_LIBS=static SHARED=0 COMPILER_VERSION=141`
4. Run: `nmake /f makefile.vc /ls RUNTIME_LIBS=static SHARED=0 COMPILER_VERSION=141 BUILD=release`

#### Compiling wxWidgets x64 static libraries
1. Start _x64 Native Tools Command Prompt for VS 2017_
2. Change working folder to `build\msw`
3. Run: `nmake /f makefile.vc /ls RUNTIME_LIBS=static SHARED=0 COMPILER_VERSION=141 TARGET_CPU=X64`
4. Run: `nmake /f makefile.vc /ls RUNTIME_LIBS=static SHARED=0 COMPILER_VERSION=141 TARGET_CPU=X64 BUILD=release`

#### Specifying wxWidgets path
ZRCola compilation references wxWidgets libraries using `WXWIN` environment variable. Please set it to wxWidgets folder (i.e. `C:\SDK\wxWidgets`).

### Digital Signing of Build Outputs
In order to have the build process digitally sign output files, one should provide the following:

1. A signing certificate installed in the current user's certificate store.
2. The following variables in the environment:
  - `ManifestCertificateThumbprint` - set the value to certificate's SHA1 thumbprint (hexadecimal, without spaces, i.e. `bc0d8da45f9eeefcbe4e334e1fc262804df88d7e`).
  - `ManifestTimestampUrl`          - set the value to URL used to perform timestamp signature (i.e. `http://timestamp.verisign.com/scripts/timstamp.dll`). In order to perform timestamp signing successfully, the computer running the build should be online and able to access this URL.

Please note that only Release builds are configured for timestamp signing. Debug configurations do not attempt to timestamp sign the resulting DLL and EXE files in order to speed up the building process and enable offline building.

### Building
Use Microsoft NMAKE to build the project. The resulting files can be found in output subfolder. Open _x64 Native Tools Command Prompt for VS 2017_ for building.

- `nmake Clean`	Delete all intermediate and output files.
- `nmake Setup`	Build a release version of project and release MSI setup files.
- `nmake SetupDebug`	Build a debug version of project and debug MSI setup files.
- `nmake Register`	Build a debug version of project, install fonts (reboot required), and Start Menu shortcuts. For development purposes only!
- `nmake Unregister`	Remove Start Menu shortcuts, and fonts. For development purposes only!

The `/ls` flag can be appended to the commands above to reduce NMAKE's verbosity. You can combine multiple targets (i.e. nmake Unregister Clean). Please, see NMAKE reference for further reading.

## Translating ZRCola
Instructions how to translate ZRCola to your language can be found [here](LOCALIZATION.md).

## Contact Information
Please contact the following addressee for further information and help:
- ZRC SAZU, ZRCola@zrc-sazu.si 
- Amebis, info@amebis.si
