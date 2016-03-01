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
/// Writes translation database to a stream
///
/// \param[in] stream  Output stream
/// \param[in] t_db    Translation database
///
/// \returns The stream \p stream
///
inline std::ostream& operator <<(std::ostream& stream, const ZRCola::translation_db &t_db)
{
    assert(t_db.idxComp.size() == t_db.idxDecomp.size());

    unsigned __int32 count;

    // Write index count.
    std::vector<ZRCola::translation_db::index>::size_type trans_count = t_db.idxComp.size();
#if defined(_WIN64) || defined(__x86_64__) || defined(__ppc64__)
    // 4G check
    if (trans_count > 0xffffffff) {
        stream.setstate(std::ios_base::failbit);
        return stream;
    }
#endif
    if (stream.fail()) return stream;
    count = (unsigned __int32)trans_count;
    stream.write((const char*)&count, sizeof(count));

    // Write composition index.
    if (stream.fail()) return stream;
    stream.write((const char*)t_db.idxComp.data(), sizeof(ZRCola::translation_db::index)*count);

    // Write decomposition index.
    if (stream.fail()) return stream;
    stream.write((const char*)t_db.idxDecomp.data(), sizeof(ZRCola::translation_db::index)*count);

    // Write data count.
    std::vector<wchar_t>::size_type data_count = t_db.data.size();
#if defined(_WIN64) || defined(__x86_64__) || defined(__ppc64__)
    // 4G check
    if (data_count > 0xffffffff) {
        stream.setstate(std::ios_base::failbit);
        return stream;
    }
#endif
    if (stream.fail()) return stream;
    count = (unsigned __int32)data_count;
    stream.write((const char*)&count, sizeof(count));

    // Write data.
    if (stream.fail()) return stream;
    stream.write((const char*)t_db.data.data(), sizeof(wchar_t)*count);

    return stream;
}


///
/// Binary compares two Unicode strings
///
/// \param[in] str_a    First string
/// \param[in] count_a  Number of characters in string \p str_a
/// \param[in] str_b    Second string
/// \param[in] count_b  Number of characters in string \p str_b
///
/// \returns
/// - <0 when str_a <  str_b
/// - =0 when str_a == str_b
/// - >0 when str_a >  str_b
///
/// \note
/// The function does not treat \\0 characters as terminators for performance reasons.
/// Therefore \p count_a and \p count_b must represent exact string lengths.
///
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


///
/// Function to use in \c qsort_s for composition index sorting
///
/// \param[in] data  Pointer to translation data
/// \param[in] a     Pointer to first composition index element
/// \param[in] b     Pointer to second composition index element
///
/// \returns
/// - <0 when a <  b
/// - =0 when a == b
/// - >0 when a >  b
///
static int __cdecl CompareCompositionIndex(void *data, const void *a, const void *b)
{
    const wchar_t
        *chr_a = (const wchar_t*)data + ((const ZRCola::translation_db::index*)a)->start,
        *chr_b = (const wchar_t*)data + ((const ZRCola::translation_db::index*)b)->start;
    const wchar_t
        *str_a = chr_a + 1,
        *str_b = chr_b + 1;
    size_t
        count_a = (const wchar_t*)data + ((const ZRCola::translation_db::index*)a)->end - str_a,
        count_b = (const wchar_t*)data + ((const ZRCola::translation_db::index*)b)->end - str_b;

    int r = CompareBinary(str_a, count_a, str_b, count_b);
    if (r != 0) return r;

         if (*chr_a < *chr_b) return -1;
    else if (*chr_a > *chr_b) return +1;

    return 0;
}


///
/// Function to use in \c qsort_s for decomposition index sorting
///
/// \param[in] data  Pointer to translation data
/// \param[in] a     Pointer to first decomposition index element
/// \param[in] b     Pointer to second decomposition index element
///
/// \returns
/// - <0 when a <  b
/// - =0 when a == b
/// - >0 when a >  b
///
static int __cdecl CompareDecompositionIndex(void *data, const void *a, const void *b)
{
    const wchar_t
        *chr_a = (const wchar_t*)data + ((const ZRCola::translation_db::index*)a)->start,
        *chr_b = (const wchar_t*)data + ((const ZRCola::translation_db::index*)b)->start;

         if (*chr_a < *chr_b) return -1;
    else if (*chr_a > *chr_b) return +1;

    const wchar_t
        *str_a = chr_a + 1,
        *str_b = chr_b + 1;
    size_t
        count_a = (const wchar_t*)data + ((const ZRCola::translation_db::index*)a)->end - str_a,
        count_b = (const wchar_t*)data + ((const ZRCola::translation_db::index*)b)->end - str_b;

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
                ZRCola::DBSource::translation trans;
                ZRCola::translation_db t_db;

                // Preallocate memory.
                t_db.idxComp  .reserve(trans_count);
                t_db.idxDecomp.reserve(trans_count);
                t_db.data     .reserve(trans_count*4);

                // Parse translations and build index and data.
                while (!ZRCola::DBSource::IsEOF(rs)) {
                    // Read translation from the database.
                    if (src.GetTranslation(rs, trans)) {
                        // Add translation to index and data.
                        ZRCola::translation_db::index ti;
                        ti.start = t_db.data.size();
                        t_db.data.push_back(trans.chr);
                        for (std::wstring::size_type i = 0, n = trans.str.length(); i < n; i++)
                            t_db.data.push_back(trans.str[i]);
                        ti.end = t_db.data.size();
                        t_db.idxComp  .push_back(ti);
                        t_db.idxDecomp.push_back(ti);
                    } else
                        has_errors = true;

                    wxVERIFY(SUCCEEDED(rs->MoveNext()));
                }

                // Sort indices.
                qsort_s(t_db.idxComp  .data(), trans_count, sizeof(ZRCola::translation_db::index), CompareCompositionIndex  , t_db.data.data());
                qsort_s(t_db.idxDecomp.data(), trans_count, sizeof(ZRCola::translation_db::index), CompareDecompositionIndex, t_db.data.data());

                // Write translations to file.
                dst << ZRCola::translation_rec(t_db);
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
