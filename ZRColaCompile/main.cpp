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

#include "stdafx.h"


///
/// Main function
///
int _tmain(int argc, _TCHAR *argv[])
{
    wxApp::CheckBuildOptions(WX_BUILD_OPTIONS_SIGNATURE, "program");

    // Inizialize wxWidgets.
    wxInitializer initializer;
    if (!initializer) {
        _ftprintf(stderr, wxT("Failed to initialize the wxWidgets library, aborting.\n"));
        return -1;
    }

    // Set desired locale.
    // TODO: Check user language setting and select the language accordingly.
    wxLocale locale;
    if (wxLocale::IsAvailable(wxLANGUAGE_SLOVENIAN)) {
        wxString sPath(wxPathOnly(argv[0]));
        sPath << wxT("\\..\\locale");
        locale.AddCatalogLookupPathPrefix(sPath);
        wxVERIFY(locale.Init(wxLANGUAGE_SLOVENIAN));
        wxVERIFY(locale.AddCatalog(wxT("ZRColaCompile")));
    }

    // Parse command line.
    static const wxCmdLineEntryDesc cmdLineDesc[] =
    {
        { wxCMD_LINE_SWITCH, "h"  , "help", _("Show this help message"), wxCMD_LINE_VAL_NONE  , wxCMD_LINE_OPTION_HELP      },
        { wxCMD_LINE_PARAM ,  NULL, NULL  , _("input file")            , wxCMD_LINE_VAL_STRING, wxCMD_LINE_OPTION_MANDATORY },
        { wxCMD_LINE_PARAM ,  NULL, NULL  , _("output file")           , wxCMD_LINE_VAL_STRING, wxCMD_LINE_OPTION_MANDATORY },

        { wxCMD_LINE_NONE }
    };
    wxCmdLineParser parser(cmdLineDesc, argc, argv);
    switch (parser.Parse()) {
    case -1:
        // Help was given, terminating.
        return 0;

    case 0:
        // everything is ok; proceed
        break;

    default:
        wxLogMessage(wxT("Syntax error detected, aborting."));
        return -1;
    }

    // Initialize COM (CoInitialize).
    wxCoInitializer initializerOLE(COINIT_MULTITHREADED | COINIT_SPEED_OVER_MEMORY);
    if (!initializerOLE) {
        _ftprintf(stderr, _("Error initializing COM.\n"));
        return -1;
    }

    ZRCola::DBSource src;
    const wxString& filenameIn = parser.GetParam(0);
    if (!src.Open(filenameIn)) {
        _ftprintf(stderr, _("Error opening %s input file.\n"), filenameIn.fn_str());
        return 1;
    }

    const wxString& filenameOut = parser.GetParam(1);
    std::fstream dst((LPCTSTR)filenameOut, std::ios_base::out | std::ios_base::trunc | std::ios_base::binary);
    if (dst.fail()) {
        _ftprintf(stderr, _("Error opening %s output file.\n"), filenameOut.fn_str());
        return 1;
    }

    ATL::CComPtr<ADORecordset> rs_comp;
    wxCHECK(src.SelectCompositions(rs_comp), 1);

    bool has_errors = false;

    // Open file ID.
    std::streamoff dst_start = stdex::idrec::open<ZRCola::recordid_t, ZRCola::recordsize_t>(dst, ZRCOLA_DB_ID);

    // Get number of compositions.
    size_t comp_count = src.GetRecordsetCount(rs_comp);
    if (comp_count < 0xffffffff) { // 4G check (-1 is reserved for error condition)
        // Allocate memory.
        std::vector<ZRCola::composition_index> comp_index;
        comp_index.reserve(comp_count);
        std::vector<wchar_t> comp_data;
        comp_data.reserve(comp_count*4);
        ZRCola::DBSource::composition comp;

        // Parse compositions and build index and data.
        while (!ZRCola::DBSource::IsEOF(rs_comp)) {
            // Read composition from the database.
            if (src.GetComposition(rs_comp, comp)) {
                // Add composition to index and data.
                ZRCola::composition_index ci;
                ci.src = (unsigned int)comp_data.size();
                for (std::wstring::size_type i = 0, n = comp.src.length(); i < n; i++)
                    comp_data.push_back(comp.src[i]);
                ci.dst = (unsigned int)comp_data.size();
                comp_data.push_back(comp.dst);
                comp_index.push_back(ci);
            } else
                has_errors = true;

            wxVERIFY(SUCCEEDED(rs_comp->MoveNext()));
        }

        // Write compositions to file.
        std::streamoff start = stdex::idrec::open<ZRCola::recordid_t, ZRCola::recordsize_t>(dst, ZRCOLA_DB_COMPOSITIONS_ID);
        {
            unsigned int _count = comp_count;
            dst.write((const char*)&_count, sizeof(_count));
            dst.write((const char*)comp_index.data(), sizeof(ZRCola::composition_index)*_count);
        }
        {
            std::vector<wchar_t>::size_type count = comp_data.size();
            if (count <= 0xffffffff) { // 4G check
                unsigned int _count = (unsigned int)count;
                dst.write((const char*)&_count, sizeof(_count));
                dst.write((const char*)comp_data.data(), sizeof(wchar_t)*_count);
            } else {
                _ftprintf(stderr, wxT("%s: error ZCC0004: Composition data exceeds 4G. Please make sure the file is ZRCola.zrc compatible.\n"), (LPCTSTR)filenameIn.c_str());
                has_errors = true;
            }
        }
        stdex::idrec::close<ZRCola::recordid_t, ZRCola::recordsize_t, ZRCOLA_RECORD_ALIGN>(dst, start);
    } else {
        _ftprintf(stderr, wxT("%s: error ZCC0003: Error getting composition count from database or too many compositions. Please make sure the file is ZRCola.zrc compatible.\n"), (LPCTSTR)filenameIn.c_str());
        has_errors = true;
    }

    stdex::idrec::close<ZRCola::recordid_t, ZRCola::recordsize_t, ZRCOLA_RECORD_ALIGN>(dst, dst_start);

    if (dst.fail()) {
        _ftprintf(stderr, wxT("Writing to %s output file failed.\n"), (LPCTSTR)filenameOut.c_str());
        has_errors = true;
    }

    if (has_errors) {
        dst.close();
        wxRemoveFile(filenameOut);
        return 1;
    } else
        return 0;
}
