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
/// \param[in] db      Translation database
///
/// \returns The stream \p stream
///
inline std::ostream& operator <<(std::ostream& stream, const ZRCola::translation_db &db)
{
    assert(db.idxComp.size() == db.idxDecomp.size());

    unsigned __int32 count;

    // Write index count.
    ZRCola::translation_db::indexComp::size_type trans_count = db.idxComp.size();
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
    stream.write((const char*)db.idxComp.data(), sizeof(unsigned __int32)*count);

    // Write decomposition index.
    if (stream.fail()) return stream;
    stream.write((const char*)db.idxDecomp.data(), sizeof(unsigned __int32)*count);

    // Write data count.
    std::vector<unsigned __int16>::size_type data_count = db.data.size();
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
    stream.write((const char*)db.data.data(), sizeof(unsigned __int16)*count);

    return stream;
}


///
/// Writes key sequence database to a stream
///
/// \param[in] stream  Output stream
/// \param[in] db      Key sequence database
///
/// \returns The stream \p stream
///
inline std::ostream& operator <<(std::ostream& stream, const ZRCola::keyseq_db &db)
{
    assert(db.idxChr.size() == db.idxKey.size());

    unsigned __int32 count;

    // Write index count.
    ZRCola::keyseq_db::indexChr::size_type ks_count = db.idxChr.size();
#if defined(_WIN64) || defined(__x86_64__) || defined(__ppc64__)
    // 4G check
    if (ks_count > 0xffffffff) {
        stream.setstate(std::ios_base::failbit);
        return stream;
    }
#endif
    if (stream.fail()) return stream;
    count = (unsigned __int32)ks_count;
    stream.write((const char*)&count, sizeof(count));

    // Write character index.
    if (stream.fail()) return stream;
    stream.write((const char*)db.idxChr.data(), sizeof(unsigned __int32)*count);

    // Write key index.
    if (stream.fail()) return stream;
    stream.write((const char*)db.idxKey.data(), sizeof(unsigned __int32)*count);

    // Write data count.
    std::vector<unsigned __int16>::size_type data_count = db.data.size();
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
    stream.write((const char*)db.data.data(), sizeof(unsigned __int16)*count);

    return stream;
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

    wxConfig config(wxT(ZRCOLA_CFG_APPLICATION), wxT(ZRCOLA_CFG_VENDOR));

    // Set desired locale.
    wxLocale locale;
    wxLanguage language = (wxLanguage)config.Read(wxT("Language"), wxLANGUAGE_DEFAULT);
    if (wxLocale::IsAvailable(language)) {
        wxString sPath;
        if (config.Read(wxT("LocalizationRepositoryPath"), &sPath))
            locale.AddCatalogLookupPathPrefix(sPath);
        wxVERIFY(locale.Init(language));
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
        _ftprintf(stderr, wxT("Error initializing COM.\n"));
        return -1;
    }

    ZRCola::DBSource src;
    const wxString& filenameIn = parser.GetParam(0);
    if (!src.Open(filenameIn)) {
        _ftprintf(stderr, wxT("%s: error ZCC0001: Error opening input file.\n"), filenameIn.fn_str());
        return 1;
    }

    const wxString& filenameOut = parser.GetParam(1);
    std::fstream dst((LPCTSTR)filenameOut, std::ios_base::out | std::ios_base::trunc | std::ios_base::binary);
    if (dst.fail()) {
        _ftprintf(stderr, wxT("%s: error ZCC0002: Error opening output file.\n"), filenameOut.fn_str());
        return 1;
    }

    bool has_errors = false;

    // Open file ID.
    std::streamoff dst_start = stdex::idrec::open<ZRCola::recordid_t, ZRCola::recordsize_t>(dst, ZRCOLA_DB_ID);

    {
        // Get translations.
        ATL::CComPtr<ADORecordset> rs;
        if (src.SelectTranslations(rs)) {
            size_t count = src.GetRecordsetCount(rs);
            if (count < 0xffffffff) { // 4G check (-1 is reserved for error condition)
                ZRCola::DBSource::translation trans;
                ZRCola::translation_db db;

                // Preallocate memory.
                db.idxComp  .reserve(count);
                db.idxDecomp.reserve(count);
                db.data     .reserve(count*4);

                // Parse translations and build index and data.
                while (!ZRCola::DBSource::IsEOF(rs)) {
                    // Read translation from the database.
                    if (src.GetTranslation(rs, trans)) {
                        // Add translation to index and data.
                        unsigned __int32 idx = db.data.size();
                        db.data.push_back(trans.chr);
                        std::wstring::size_type n = trans.str.length();
                        wxASSERT_MSG(n <= 0xffff, wxT("transformation string too long"));
                        db.data.push_back((unsigned __int16)n);
                        for (std::wstring::size_type i = 0; i < n; i++)
                            db.data.push_back(trans.str[i]);
                        db.idxComp  .push_back(idx);
                        db.idxDecomp.push_back(idx);
                    } else
                        has_errors = true;

                    wxVERIFY(SUCCEEDED(rs->MoveNext()));
                }

                // Sort indices.
                db.idxComp  .sort();
                db.idxDecomp.sort();

                // Write translations to file.
                dst << ZRCola::translation_rec(db);
            } else {
                _ftprintf(stderr, wxT("%s: error ZCC0004: Error getting translation count from database or too many translations.\n"), (LPCTSTR)filenameIn.c_str());
                has_errors = true;
            }
        } else {
            _ftprintf(stderr, wxT("%s: error ZCC0003: Error getting translations from database. Please make sure the file is ZRCola.zrc compatible.\n"), (LPCTSTR)filenameIn.c_str());
            has_errors = true;
        }
    }


    {
        // Get key sequences.
        ATL::CComPtr<ADORecordset> rs;
        if (src.SelectKeySequences(rs)) {
            size_t count = src.GetRecordsetCount(rs);
            if (count < 0xffffffff) { // 4G check (-1 is reserved for error condition)
                ZRCola::DBSource::keyseq ks;
                ZRCola::keyseq_db db;

                // Preallocate memory.
                db.idxChr.reserve(count);
                db.idxKey.reserve(count);
                db.data  .reserve(count*4);

                // Parse key sequences and build index and data.
                while (!ZRCola::DBSource::IsEOF(rs)) {
                    // Read key sequence from the database.
                    if (src.GetKeySequence(rs, ks)) {
                        // Add key sequence to index and data.
                        unsigned __int32 idx = db.data.size();
                        db.data.push_back(ks.chr);
                        std::vector<ZRCola::DBSource::keyseq::keycode>::size_type n = ks.seq.size();
                        wxASSERT_MSG(n <= 0xffff, wxT("key sequence too long"));
                        db.data.push_back((unsigned __int16)n);
                        for (std::vector<ZRCola::DBSource::keyseq::keycode>::size_type i = 0; i < n; i++) {
                            const ZRCola::DBSource::keyseq::keycode &kc = ks.seq[i];
                            db.data.push_back(kc.key);
                            db.data.push_back(
                                (kc.shift ? ZRCola::keyseq_db::keyseq::SHIFT : 0) |
                                (kc.ctrl  ? ZRCola::keyseq_db::keyseq::CTRL  : 0) |
                                (kc.alt   ? ZRCola::keyseq_db::keyseq::ALT   : 0));
                        }
                        db.idxChr.push_back(idx);
                        db.idxKey.push_back(idx);
                    } else
                        has_errors = true;

                    wxVERIFY(SUCCEEDED(rs->MoveNext()));
                }

                // Sort indices.
                db.idxChr.sort();
                db.idxKey.sort();

                // Check key sequences.
                for (ZRCola::keyseq_db::indexKey::size_type i = 1, n = db.idxKey.size(); i < n; i++) {
                    const ZRCola::keyseq_db::keyseq
                        &ks1 = db.idxKey[i - 1],
                        &ks2 = db.idxKey[i    ];

                    if (ZRCola::keyseq_db::keyseq::CompareSequence(ks1.seq, ks1.seq_len, ks2.seq, ks2.seq_len) == 0) {
                        std::wstring seq_str;
                        ZRCola::keyseq_db::GetSequenceAsText(ks1.seq, ks1.seq_len, seq_str);
                        _ftprintf(stderr, wxT("%s: warning ZCC0007: Duplicate key sequence (%ls => %04X or %04X). The keyboard behaviour will be unpredictable.\n"), (LPCTSTR)filenameIn.c_str(), seq_str.c_str(), ks1.chr, ks2.chr);
                    }
                }

                // Write translations to file.
                dst << ZRCola::keyseq_rec(db);
            } else {
                _ftprintf(stderr, wxT("%s: error ZCC0006: Error getting key sequence count from database or too many key sequences.\n"), (LPCTSTR)filenameIn.c_str());
                has_errors = true;
            }
        } else {
            _ftprintf(stderr, wxT("%s: error ZCC0005: Error getting key sequences from database. Please make sure the file is ZRCola.zrc compatible.\n"), (LPCTSTR)filenameIn.c_str());
            has_errors = true;
        }
    }

    stdex::idrec::close<ZRCola::recordid_t, ZRCola::recordsize_t, ZRCOLA_RECORD_ALIGN>(dst, dst_start);

    if (dst.fail()) {
        _ftprintf(stderr, wxT("%s: error ZCC1000: Writing to output file failed.\n"), (LPCTSTR)filenameOut.c_str());
        has_errors = true;
    }

    if (has_errors) {
        dst.close();
        wxRemoveFile(filenameOut);
        return 1;
    } else
        return 0;
}
