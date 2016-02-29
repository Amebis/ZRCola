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


static inline int CompareBinary(const wchar_t *str_a, size_t count_a, const wchar_t *str_b, size_t count_b)
{
    for (size_t i = 0; ; i++) {
             if (i >= count_a && i >= count_b) break;
        else if (i >= count_a && i <  count_b) return -1;
        else if (i <  count_a && i >= count_b) return +1;
        else if (str_a[i] < str_b[i]) return -1;
        else if (str_a[i] > str_b[i]) return +1;
    }

    return 0;
}


static int __cdecl CompareCompositionIndex(void *data, const void *a, const void *b)
{
    const wchar_t
        *chr_a = (const wchar_t*)data + ((const ZRCola::translation_index*)a)->start,
        *chr_b = (const wchar_t*)data + ((const ZRCola::translation_index*)b)->start;
    const wchar_t
        *str_a = chr_a + 1,
        *str_b = chr_b + 1;
    size_t
        count_a = (const wchar_t*)data + ((const ZRCola::translation_index*)a)->end - str_a,
        count_b = (const wchar_t*)data + ((const ZRCola::translation_index*)b)->end - str_b;

    int r = CompareBinary(str_a, count_a, str_b, count_b);
    if (r != 0) return r;

         if (*chr_a < *chr_b) return -1;
    else if (*chr_a > *chr_b) return +1;

    return 0;
}


static int __cdecl CompareDecompositionIndex(void *data, const void *a, const void *b)
{
    const wchar_t
        *chr_a = (const wchar_t*)data + ((const ZRCola::translation_index*)a)->start,
        *chr_b = (const wchar_t*)data + ((const ZRCola::translation_index*)b)->start;

         if (*chr_a < *chr_b) return -1;
    else if (*chr_a > *chr_b) return +1;

    const wchar_t
        *str_a = chr_a + 1,
        *str_b = chr_b + 1;
    size_t
        count_a = (const wchar_t*)data + ((const ZRCola::translation_index*)a)->end - str_a,
        count_b = (const wchar_t*)data + ((const ZRCola::translation_index*)b)->end - str_b;

    return CompareBinary(str_a, count_a, str_b, count_b);
}


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

    bool has_errors = false;

    // Open file ID.
    std::streamoff dst_start = stdex::idrec::open<ZRCola::recordid_t, ZRCola::recordsize_t>(dst, ZRCOLA_DB_ID);

    {
        // Get translations.
        ATL::CComPtr<ADORecordset> rs;
        if (src.SelectTranslations(rs)) {
            size_t trans_count = src.GetRecordsetCount(rs);
            if (trans_count < 0xffffffff) { // 4G check (-1 is reserved for error condition)
                // Allocate memory.
                std::vector<ZRCola::translation_index> comp_index;
                comp_index.reserve(trans_count);
                std::vector<ZRCola::translation_index> decomp_index;
                decomp_index.reserve(trans_count);
                std::vector<wchar_t> comp_data;
                comp_data.reserve(trans_count*4);
                ZRCola::DBSource::translation trans;

                // Parse translations and build index and data.
                while (!ZRCola::DBSource::IsEOF(rs)) {
                    // Read translation from the database.
                    if (src.GetTranslation(rs, trans)) {
                        // Add translation to index and data.
                        ZRCola::translation_index ti;
                        ti.start = comp_data.size();
                        comp_data.push_back(trans.chr);
                        for (std::wstring::size_type i = 0, n = trans.str.length(); i < n; i++)
                            comp_data.push_back(trans.str[i]);
                        ti.end = comp_data.size();
                        comp_index.push_back(ti);
                        decomp_index.push_back(ti);
                    } else
                        has_errors = true;

                    wxVERIFY(SUCCEEDED(rs->MoveNext()));
                }

                // Sort indices.
                qsort_s(  comp_index.data(), trans_count, sizeof(ZRCola::translation_index), CompareCompositionIndex  , comp_data.data());
                qsort_s(decomp_index.data(), trans_count, sizeof(ZRCola::translation_index), CompareDecompositionIndex, comp_data.data());

                // Write translations to file.
                std::streamoff start = stdex::idrec::open<ZRCola::recordid_t, ZRCola::recordsize_t>(dst, ZRCOLA_DB_COMPOSITIONS_ID);
                {
                    unsigned int _count = trans_count;
                    dst.write((const char*)&_count, sizeof(_count));
                    dst.write((const char*)  comp_index.data(), sizeof(__int32)*_count);
                    dst.write((const char*)decomp_index.data(), sizeof(__int32)*_count);
                }
                {
                    std::vector<wchar_t>::size_type count = comp_data.size();
                    if (count <= 0xffffffff) { // 4G check
                        unsigned int _count = (unsigned int)count;
                        dst.write((const char*)&_count, sizeof(_count));
                        dst.write((const char*)comp_data.data(), sizeof(wchar_t)*_count);
                    } else {
                        _ftprintf(stderr, wxT("%s: error ZCC0005: Translation data exceeds 4G.\n"), (LPCTSTR)filenameIn.c_str());
                        has_errors = true;
                    }
                }
                stdex::idrec::close<ZRCola::recordid_t, ZRCola::recordsize_t, ZRCOLA_RECORD_ALIGN>(dst, start);
            } else {
                _ftprintf(stderr, wxT("%s: error ZCC0004: Error getting translation count from database or too many translations.\n"), (LPCTSTR)filenameIn.c_str());
                has_errors = true;
            }
        } else {
            _ftprintf(stderr, wxT("%s: error ZCC0003: Error getting translations from database. Please make sure the file is ZRCola.zrc compatible.\n"), (LPCTSTR)filenameIn.c_str());
            has_errors = true;
        }
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
