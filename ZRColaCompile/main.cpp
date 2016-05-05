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
/// Writes language database to a stream
///
/// \param[in] stream  Output stream
/// \param[in] db      Language database
///
/// \returns The stream \p stream
///
inline std::ostream& operator <<(std::ostream& stream, const ZRCola::language_db &db)
{
    unsigned __int32 count;

    // Write index count.
    ZRCola::language_db::indexLang::size_type lang_count = db.idxLng.size();
#if defined(_WIN64) || defined(__x86_64__) || defined(__ppc64__)
    // 4G check
    if (lang_count > 0xffffffff) {
        stream.setstate(std::ios_base::failbit);
        return stream;
    }
#endif
    if (stream.fail()) return stream;
    count = (unsigned __int32)lang_count;
    stream.write((const char*)&count, sizeof(count));

    // Write language index.
    if (stream.fail()) return stream;
    stream.write((const char*)db.idxLng.data(), sizeof(unsigned __int32)*count);


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
/// Writes language character database to a stream
///
/// \param[in] stream  Output stream
/// \param[in] db      Language character database
///
/// \returns The stream \p stream
///
inline std::ostream& operator <<(std::ostream& stream, const ZRCola::langchar_db &db)
{
#ifdef ZRCOLA_LANGCHAR_LANG_IDX
    assert(db.idxChr.size() == db.idxLng.size());
#endif

    unsigned __int32 count;

    // Write index count.
    ZRCola::langchar_db::indexChar::size_type lc_count = db.idxChr.size();
#if defined(_WIN64) || defined(__x86_64__) || defined(__ppc64__)
    // 4G check
    if (lc_count > 0xffffffff) {
        stream.setstate(std::ios_base::failbit);
        return stream;
    }
#endif
    if (stream.fail()) return stream;
    count = (unsigned __int32)lc_count;
    stream.write((const char*)&count, sizeof(count));

    // Write character index.
    if (stream.fail()) return stream;
    stream.write((const char*)db.idxChr.data(), sizeof(unsigned __int32)*count);

#ifdef ZRCOLA_LANGCHAR_LANG_IDX
    // Write language index.
    if (stream.fail()) return stream;
    stream.write((const char*)db.idxLng.data(), sizeof(unsigned __int32)*count);
#endif

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
/// Writes character group database to a stream
///
/// \param[in] stream  Output stream
/// \param[in] db      Character group database
///
/// \returns The stream \p stream
///
inline std::ostream& operator <<(std::ostream& stream, const ZRCola::chrgrp_db &db)
{
    unsigned __int32 count;

    // Write index count.
    ZRCola::keyseq_db::indexChr::size_type ks_count = db.idxRnk.size();
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

    // Write rank index.
    if (stream.fail()) return stream;
    stream.write((const char*)db.idxRnk.data(), sizeof(unsigned __int32)*count);

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
        { wxCMD_LINE_PARAM ,  NULL, NULL  , _("<input file>"          ), wxCMD_LINE_VAL_STRING, wxCMD_LINE_OPTION_MANDATORY },
        { wxCMD_LINE_PARAM ,  NULL, NULL  , _("<output file>"         ), wxCMD_LINE_VAL_STRING, wxCMD_LINE_OPTION_MANDATORY },
        { wxCMD_LINE_PARAM ,  NULL, NULL  , _("<output POT catalog>"  ), wxCMD_LINE_VAL_STRING, wxCMD_LINE_OPTION           },

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

    // Set of strings to translate.
    bool build_pot = parser.GetParamCount() > 2;
    std::set<std::wstring> pot;

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
                        wxASSERT_MSG((int)0xffff8000 <= trans.rank && trans.rank <= (int)0x00007fff, wxT("transformation rank out of bounds"));
                        db.data.push_back((unsigned __int16)trans.rank);
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

    {
        // Get languages.
        ATL::CComPtr<ADORecordset> rs;
        if (src.SelectLanguages(rs)) {
            size_t count = src.GetRecordsetCount(rs);
            if (count < 0xffffffff) { // 4G check (-1 is reserved for error condition)
                ZRCola::DBSource::language lang;
                ZRCola::language_db db;

                // Preallocate memory.
                db.idxLng.reserve(count);
                db.data  .reserve(count*4);

                // Parse languages and build index and data.
                while (!ZRCola::DBSource::IsEOF(rs)) {
                    // Read language from the database.
                    if (src.GetLanguage(rs, lang)) {
                        // Add language to index and data.
                        unsigned __int32 idx = db.data.size();
                        for (std::wstring::size_type i = 0; i < sizeof(ZRCola::langid_t)/sizeof(unsigned __int16); i++)
                            db.data.push_back(((const unsigned __int16*)lang.id)[i]);
                        std::wstring::size_type n = lang.name.length();
                        wxASSERT_MSG(n <= 0xffff, wxT("language name too long"));
                        db.data.push_back((unsigned __int16)n);
                        for (std::wstring::size_type i = 0; i < n; i++)
                            db.data.push_back(lang.name[i]);
                        db.idxLng.push_back(idx);
                        if (build_pot)
                            pot.insert(lang.name);
                    } else
                        has_errors = true;

                    wxVERIFY(SUCCEEDED(rs->MoveNext()));
                }

                // Sort indices.
                db.idxLng.sort();

                // Write languages to file.
                dst << ZRCola::language_rec(db);
            } else {
                _ftprintf(stderr, wxT("%s: error ZCC0009: Error getting language count from database or too many languages.\n"), (LPCTSTR)filenameIn.c_str());
                has_errors = true;
            }
        } else {
            _ftprintf(stderr, wxT("%s: error ZCC0008: Error getting languages from database. Please make sure the file is ZRCola.zrc compatible.\n"), (LPCTSTR)filenameIn.c_str());
            has_errors = true;
        }
    }

    {
        // Get language characters.
        ATL::CComPtr<ADORecordset> rs;
        if (src.SelectLanguageCharacters(rs)) {
            size_t count = src.GetRecordsetCount(rs);
            if (count < 0xffffffff) { // 4G check (-1 is reserved for error condition)
                ZRCola::DBSource::langchar lc;
                ZRCola::langchar_db db;

                // Preallocate memory.
                db.idxChr.reserve(count);
#ifdef ZRCOLA_LANGCHAR_LANG_IDX
                db.idxLng.reserve(count);
#endif
                db.data  .reserve(count*4);

                // Parse language characters and build index and data.
                while (!ZRCola::DBSource::IsEOF(rs)) {
                    // Read language characters from the database.
                    if (src.GetLanguageCharacter(rs, lc)) {
                        // Add language characters to index and data.
                        unsigned __int32 idx = db.data.size();
                        db.data.push_back(lc.chr);
                        for (std::wstring::size_type i = 0; i < sizeof(ZRCola::langid_t)/sizeof(unsigned __int16); i++)
                            db.data.push_back(((const unsigned __int16*)lc.lang)[i]);
                        db.idxChr.push_back(idx);
#ifdef ZRCOLA_LANGCHAR_LANG_IDX
                        db.idxLng.push_back(idx);
#endif
                    } else
                        has_errors = true;

                    wxVERIFY(SUCCEEDED(rs->MoveNext()));
                }

                // Sort indices.
                db.idxChr .sort();
#ifdef ZRCOLA_LANGCHAR_LANG_IDX
                db.idxLng.sort();
#endif

                // Write language characters to file.
                dst << ZRCola::langchar_rec(db);
            } else {
                _ftprintf(stderr, wxT("%s: error ZCC0011: Error getting language characters count from database or too many langchars.\n"), (LPCTSTR)filenameIn.c_str());
                has_errors = true;
            }
        } else {
            _ftprintf(stderr, wxT("%s: error ZCC0010: Error getting language characters from database. Please make sure the file is ZRCola.zrc compatible.\n"), (LPCTSTR)filenameIn.c_str());
            has_errors = true;
        }
    }

    {
        // Get character groups.
        ATL::CComPtr<ADORecordset> rs;
        if (src.SelectCharacterGroups(rs)) {
            size_t count = src.GetRecordsetCount(rs);
            if (count < 0xffffffff) { // 4G check (-1 is reserved for error condition)
                ZRCola::DBSource::chrgrp cg;
                ZRCola::chrgrp_db db;

                // Preallocate memory.
                db.idxRnk.reserve(count);
                db.data  .reserve(count*4);

                // Parse character groups and build index and data.
                while (!ZRCola::DBSource::IsEOF(rs)) {
                    // Read character group from the database.
                    if (src.GetCharacterGroup(rs, cg)) {
                        // Add character group to index and data.
                        unsigned __int32 idx = db.data.size();
                        wxASSERT_MSG((int)0xffff8000 <= cg.id && cg.id <= (int)0x00007fff, wxT("character group ID out of bounds"));
                        db.data.push_back((unsigned __int16)cg.id);
                        wxASSERT_MSG((int)0xffff8000 <= cg.rank && cg.rank <= (int)0x00007fff, wxT("character group rank out of bounds"));
                        db.data.push_back((unsigned __int16)cg.rank);
                        std::wstring::size_type n_name = cg.name.length();
                        wxASSERT_MSG(n_name <= 0xffff, wxT("character group name too long"));
                        db.data.push_back((unsigned __int16)n_name);
                        std::wstring::size_type n_char = cg.chars.length();
                        wxASSERT_MSG(n_char <= 0xffff, wxT("too many character group characters"));
                        db.data.push_back((unsigned __int16)n_char);
                        for (std::wstring::size_type i = 0; i < n_name; i++)
                            db.data.push_back(cg.name[i]);
                        for (std::wstring::size_type i = 0; i < n_char; i++)
                            db.data.push_back(cg.chars[i]);
                        db.idxRnk.push_back(idx);
                        if (build_pot)
                            pot.insert(cg.name);
                    } else
                        has_errors = true;

                    wxVERIFY(SUCCEEDED(rs->MoveNext()));
                }

                // Sort indices.
                db.idxRnk.sort();

                // Write character groups to file.
                dst << ZRCola::chrgrp_rec(db);
            } else {
                _ftprintf(stderr, wxT("%s: error ZCC0015: Error getting character group count from database or too many character groups.\n"), (LPCTSTR)filenameIn.c_str());
                has_errors = true;
            }
        } else {
            _ftprintf(stderr, wxT("%s: error ZCC0014: Error getting character groups from database. Please make sure the file is ZRCola.zrc compatible.\n"), (LPCTSTR)filenameIn.c_str());
            has_errors = true;
        }
    }

    stdex::idrec::close<ZRCola::recordid_t, ZRCola::recordsize_t, ZRCOLA_RECORD_ALIGN>(dst, dst_start);

    if (dst.fail()) {
        _ftprintf(stderr, wxT("%s: error ZCC1000: Writing to output file failed.\n"), (LPCTSTR)filenameOut.c_str());
        has_errors = true;
    }

    if (!has_errors && build_pot) {
        const wxString& filenamePot = parser.GetParam(2);
        std::fstream dst((LPCTSTR)filenamePot, std::ios_base::out | std::ios_base::trunc);
        if (dst.good()) {
            dst << "msgid \"\"" << std::endl
                << "msgstr \"\"" << std::endl
                << "\"Project-Id-Version: ZRCola.zrcdb\\n\"" << std::endl
                << "\"Language: en\\n\"" << std::endl
                << "\"MIME-Version: 1.0\\n\"" << std::endl
                << "\"Content-Type: text/plain; charset=UTF-8\\n\"" << std::endl
                << "\"Content-Transfer-Encoding: 8bit\\n\"" << std::endl
                << "\"X-Generator: ZRColaCompile " << ZRCOLA_VERSION_STR << "\\n\"" << std::endl;

            std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
            for (std::set<std::wstring>::const_iterator i = pot.cbegin(); i != pot.cend(); ++i) {
                // Convert UTF-16 to UTF-8 and escape.
                std::string t(conv.to_bytes(*i)), u;
                for (size_t i = 0, n = t.size(); i < n; i++) {
                    char c = t[i];
                    switch (c) {
                    case '\'': u += "\\\'"; break;
                    case '\"': u += "\\\""; break;
                    case '\n': u += "\\\n"; break;
                    case '\t': u += "\\\t"; break;
                    default  : u += c;
                    }
                }
                dst << std::endl
                    << "msgid \"" << u << "\"" << std::endl
                    << "msgstr \"\"" << std::endl;
            }

            if (dst.fail()) {
                _ftprintf(stderr, wxT("%s: error ZCC0013: Writing to POT catalog failed.\n"), (LPCTSTR)filenameOut.c_str());
                has_errors = true;
            }

            dst.close();
        } else {
            _ftprintf(stderr, wxT("%s: error ZCC0012: Error opening POT catalog.\n"), filenameOut.fn_str());
            has_errors = true;
        }
    }

    if (has_errors) {
        dst.close();
        wxRemoveFile(filenameOut);
        return 1;
    } else
        return 0;
}
