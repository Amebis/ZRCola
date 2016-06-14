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

using namespace std;
using namespace stdex;


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
    fstream dst((LPCTSTR)filenameOut, ios_base::out | ios_base::trunc | ios_base::binary);
    if (dst.fail()) {
        _ftprintf(stderr, wxT("%s: error ZCC0002: Error opening output file.\n"), filenameOut.fn_str());
        return 1;
    }

    bool has_errors = false;

    // Set of strings to translate.
    bool build_pot = parser.GetParamCount() > 2;
    set<wstring> pot;

    // Open file ID.
    streamoff dst_start = idrec::open<ZRCola::recordid_t, ZRCola::recordsize_t>(dst, ZRCOLA_DB_ID);

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
                for (; !ZRCola::DBSource::IsEOF(rs); rs->MoveNext()) {
                    // Read translation from the database.
                    if (src.GetTranslation(rs, trans)) {
                        // Add translation to index and data.
                        unsigned __int32 idx = db.data.size();
                        db.data.push_back(trans.chr);
                        wxASSERT_MSG((int)0xffff8000 <= trans.rank && trans.rank <= (int)0x00007fff, wxT("transformation rank out of bounds"));
                        db.data.push_back((unsigned __int16)trans.rank);
                        wstring::size_type n = trans.str.length();
                        wxASSERT_MSG(n <= 0xffff, wxT("transformation string too long"));
                        db.data.push_back((unsigned __int16)n);
                        for (wstring::size_type i = 0; i < n; i++)
                            db.data.push_back(trans.str[i]);
                        db.idxComp  .push_back(idx);
                        db.idxDecomp.push_back(idx);
                    } else
                        has_errors = true;
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
                for (; !ZRCola::DBSource::IsEOF(rs); rs->MoveNext()) {
                    // Read key sequence from the database.
                    if (src.GetKeySequence(rs, ks)) {
                        // Add key sequence to index and data.
                        unsigned __int32 idx = db.data.size();
                        db.data.push_back(ks.chr);
                        vector<ZRCola::DBSource::keyseq::keycode>::size_type n = ks.seq.size();
                        wxASSERT_MSG(n <= 0xffff, wxT("key sequence too long"));
                        db.data.push_back((unsigned __int16)n);
                        for (vector<ZRCola::DBSource::keyseq::keycode>::size_type i = 0; i < n; i++) {
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
                        wxString seq_str;
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
                for (; !ZRCola::DBSource::IsEOF(rs); rs->MoveNext()) {
                    // Read language from the database.
                    if (src.GetLanguage(rs, lang)) {
                        if (build_pot)
                            pot.insert(lang.name);

                        // Add language to index and data.
                        unsigned __int32 idx = db.data.size();
                        for (wstring::size_type i = 0; i < sizeof(ZRCola::langid_t)/sizeof(unsigned __int16); i++)
                            db.data.push_back(((const unsigned __int16*)lang.id.data)[i]);
                        wstring::size_type n = lang.name.length();
                        wxASSERT_MSG(n <= 0xffff, wxT("language name too long"));
                        db.data.push_back((unsigned __int16)n);
                        for (wstring::size_type i = 0; i < n; i++)
                            db.data.push_back(lang.name[i]);
                        db.idxLng.push_back(idx);
                    } else
                        has_errors = true;
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
                for (; !ZRCola::DBSource::IsEOF(rs); rs->MoveNext()) {
                    // Read language characters from the database.
                    if (src.GetLanguageCharacter(rs, lc)) {
                        // Add language characters to index and data.
                        unsigned __int32 idx = db.data.size();
                        db.data.push_back(lc.chr);
                        for (wstring::size_type i = 0; i < sizeof(ZRCola::langid_t)/sizeof(unsigned __int16); i++)
                            db.data.push_back(((const unsigned __int16*)lc.lang.data)[i]);
                        db.idxChr.push_back(idx);
#ifdef ZRCOLA_LANGCHAR_LANG_IDX
                        db.idxLng.push_back(idx);
#endif
                    } else
                        has_errors = true;
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
                for (; !ZRCola::DBSource::IsEOF(rs); rs->MoveNext()) {
                    // Read character group from the database.
                    if (src.GetCharacterGroup(rs, cg)) {
                        if (build_pot)
                            pot.insert(cg.name);

                        if (cg.chars.empty()) {
                            // Skip empty character groups.
                            continue;
                        }

                        // Add character group to index and data.
                        unsigned __int32 idx = db.data.size();
                        wxASSERT_MSG((int)0xffff8000 <= cg.id && cg.id <= (int)0x00007fff, wxT("character group ID out of bounds"));
                        db.data.push_back((unsigned __int16)cg.id);
                        wxASSERT_MSG((int)0xffff8000 <= cg.rank && cg.rank <= (int)0x00007fff, wxT("character group rank out of bounds"));
                        db.data.push_back((unsigned __int16)cg.rank);
                        wstring::size_type n_name = cg.name.length();
                        wxASSERT_MSG(n_name <= 0xffff, wxT("character group name too long"));
                        db.data.push_back((unsigned __int16)n_name);
                        wstring::size_type n_char = cg.chars.length();
                        wxASSERT_MSG(n_char <= 0xffff, wxT("too many character group characters"));
                        db.data.push_back((unsigned __int16)n_char);
                        for (wstring::size_type i = 0; i < n_name; i++)
                            db.data.push_back(cg.name[i]);
                        for (wstring::size_type i = 0; i < n_char; i++)
                            db.data.push_back(cg.chars[i]);
                        for (std::vector<unsigned __int16>::size_type i = 0, n = cg.show.size(); i < n; i++)
                            db.data.push_back(cg.show[i]);
                        db.idxRnk.push_back(idx);
                    } else
                        has_errors = true;
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

    set<ZRCola::chrcatid_t> categories_used;

    {
        // Get characters.
        ATL::CComPtr<ADORecordset> rs;
        if (src.SelectCharacters(rs)) {
            size_t count = src.GetRecordsetCount(rs);
            if (count < 0xffffffff) { // 4G check (-1 is reserved for error condition)
                ZRCola::DBSource::character_desc_idx idxChrDsc, idxChrDscSub;

                ZRCola::DBSource::character_bank chrs;

                // Phase 1: Parse characters and build indexes.
                for (; !ZRCola::DBSource::IsEOF(rs); rs->MoveNext()) {
                    // Read character from the database.
                    unique_ptr<ZRCola::DBSource::character> c(new ZRCola::DBSource::character);
                    if (src.GetCharacter(rs, *c)) {
                        const ZRCola::DBSource::character &chr = *c.get();
                        chrs[chr.chr].swap(c);
                    } else
                        has_errors = true;
                }

                // Phase 2: Build related character lists.
                chrs.build_related();

                ZRCola::character_db db;

                // Preallocate memory.
                db.idxChr.reserve(count);
                db.data  .reserve(count*4);

                // Phase 3: Parse characters and build index and data.
                for (size_t i = 0, i_end = chrs.size(); i < i_end; i++) {
                    const ZRCola::DBSource::character &chr = *(chrs[i].get());
                    if (&chr == NULL) continue;

                    // Add character to index and data.
                    unsigned __int32 idx = db.data.size();
                    db.data.push_back((unsigned __int16)chr.chr);
                    for (wstring::size_type i = 0; i < sizeof(ZRCola::chrcatid_t)/sizeof(unsigned __int16); i++)
                        db.data.push_back(((const unsigned __int16*)chr.cat.data)[i]);
                    wstring::size_type n_desc = chr.desc.length();
                    wxASSERT_MSG(n_desc <= 0xffff, wxT("character description too long"));
                    db.data.push_back((unsigned __int16)n_desc);
                    wstring::size_type n_rel = chr.rel.length();
                    wxASSERT_MSG(n_rel <= 0xffff, wxT("too many related characters"));
                    db.data.push_back((unsigned __int16)n_rel);
                    for (wstring::size_type i = 0; i < n_desc; i++)
                        db.data.push_back(chr.desc[i]);
                    for (wstring::size_type i = 0; i < n_rel; i++)
                        db.data.push_back(chr.rel[i]);
                    db.idxChr.push_back(idx);

                    // Add description (and keywords) to index.
                    idxChrDsc   .add_keywords(chr.terms, chr.chr, 0);
                    idxChrDscSub.add_keywords(chr.terms, chr.chr, 3);

                    // Mark category used.
                    categories_used.insert(chr.cat);
                }

                // Sort indices.
                db.idxChr.sort();

                // Save text indices.
                idxChrDsc   .save(db.idxDsc   );
                idxChrDscSub.save(db.idxDscSub);

                // Write characters to file.
                dst << ZRCola::character_rec(db);
            } else {
                _ftprintf(stderr, wxT("%s: error ZCC0017: Error getting character count from database or too many characters.\n"), (LPCTSTR)filenameIn.c_str());
                has_errors = true;
            }
        } else {
            _ftprintf(stderr, wxT("%s: error ZCC0016: Error getting characters from database. Please make sure the file is ZRCola.zrc compatible.\n"), (LPCTSTR)filenameIn.c_str());
            has_errors = true;
        }
    }

    {
        // Get character categories.
        ATL::CComPtr<ADORecordset> rs;
        if (src.SelectCharacterCategories(rs)) {
            size_t count = src.GetRecordsetCount(rs);
            if (count < 0xffffffff) { // 4G check (-1 is reserved for error condition)
                ZRCola::DBSource::chrcat cc;
                ZRCola::chrcat_db db;

                // Preallocate memory.
                db.idxChrCat.reserve(count);
                db.idxRnk   .reserve(count);
                db.data     .reserve(count*4);

                // Parse character categories and build index and data.
                for (; !ZRCola::DBSource::IsEOF(rs); rs->MoveNext()) {
                    // Read character category from the database.
                    if (src.GetCharacterCategory(rs, cc)) {
                        if (build_pot)
                            pot.insert(cc.name);

                        if (categories_used.find(cc.id) == categories_used.end()) {
                            // Skip empty character categories.
                            continue;
                        }

                        // Add character category to index and data.
                        unsigned __int32 idx = db.data.size();
                        for (wstring::size_type i = 0; i < sizeof(ZRCola::chrcatid_t)/sizeof(unsigned __int16); i++)
                            db.data.push_back(((const unsigned __int16*)cc.id.data)[i]);
                        wxASSERT_MSG((int)0xffff8000 <= cc.rank && cc.rank <= (int)0x00007fff, wxT("character category rank out of bounds"));
                        db.data.push_back((unsigned __int16)cc.rank);
                        wstring::size_type n_name = cc.name.length();
                        wxASSERT_MSG(n_name <= 0xffff, wxT("character category name too long"));
                        db.data.push_back((unsigned __int16)n_name);
                        for (wstring::size_type i = 0; i < n_name; i++)
                            db.data.push_back(cc.name[i]);
                        db.idxChrCat.push_back(idx);
                        db.idxRnk   .push_back(idx);
                    } else
                        has_errors = true;
                }

                // Sort indices.
                db.idxChrCat.sort();
                db.idxRnk   .sort();

                // Write character categories to file.
                dst << ZRCola::chrcat_rec(db);
            } else {
                _ftprintf(stderr, wxT("%s: error ZCC0019: Error getting character category count from database or too many character categories.\n"), (LPCTSTR)filenameIn.c_str());
                has_errors = true;
            }
        } else {
            _ftprintf(stderr, wxT("%s: error ZCC0018: Error getting character categories from database. Please make sure the file is ZRCola.zrc compatible.\n"), (LPCTSTR)filenameIn.c_str());
            has_errors = true;
        }
    }

    idrec::close<ZRCola::recordid_t, ZRCola::recordsize_t, ZRCOLA_RECORD_ALIGN>(dst, dst_start);

    if (dst.fail()) {
        _ftprintf(stderr, wxT("%s: error ZCC1000: Writing to output file failed.\n"), (LPCTSTR)filenameOut.c_str());
        has_errors = true;
    }

    if (!has_errors && build_pot) {
        const wxString& filenamePot = parser.GetParam(2);
        fstream dst((LPCTSTR)filenamePot, ios_base::out | ios_base::trunc);
        if (dst.good()) {
            dst << "msgid \"\"" << endl
                << "msgstr \"\"" << endl
                << "\"Project-Id-Version: ZRCola.zrcdb\\n\"" << endl
                << "\"Language: en\\n\"" << endl
                << "\"MIME-Version: 1.0\\n\"" << endl
                << "\"Content-Type: text/plain; charset=UTF-8\\n\"" << endl
                << "\"Content-Transfer-Encoding: 8bit\\n\"" << endl
                << "\"X-Generator: ZRColaCompile " << ZRCOLA_VERSION_STR << "\\n\"" << endl;

            wstring_convert<codecvt_utf8<wchar_t>> conv;
            for (set<wstring>::const_iterator i = pot.cbegin(); i != pot.cend(); ++i) {
                // Convert UTF-16 to UTF-8 and escape.
                string t(conv.to_bytes(*i)), u;
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
                dst << endl
                    << "msgid \"" << u << "\"" << endl
                    << "msgstr \"\"" << endl;
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
