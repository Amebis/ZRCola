/*
    Copyright 2015-2017 Amebis

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
using namespace winstd;


///
/// (destination character rank, (source character rank, source character)) data holder
///
typedef pair<int, ZRCola::DBSource::charseq> com_translation;


///
/// Functor to compare two translations
///
struct translation_set_less {
    inline bool operator()(_In_ const com_translation& a, _In_ const com_translation& b) const
    {
             if (a.first       < b.first      ) return true;
        else if (a.first       > b.first      ) return false;
        else if (a.second.rank < b.second.rank) return true;
        else if (a.second.rank > b.second.rank) return false;
        else if (a.second.str  < b.second.str ) return true;
        else                                    return false;
    }
};


typedef map<wstring, set<com_translation, translation_set_less> > translation_db;


static set<wstring> translate_inv(_In_ const translation_db &db, _In_z_ const wchar_t *str, _Inout_ set<translation_db::key_type> &path)
{
    set<wstring> res;

    if (*str) {
        // Inverse translate remainder first.
        auto rem = translate_inv(db, str + 1, path);
        if (rem.empty())
            return res;

        translation_db::key_type _str(1, *str);
        auto const t = db.find(_str);
        if (t != db.end()) {
            // Current characted inverse translated. Iterate all possible inverse translations and combine them with the remainder.
            auto p = path.insert(_str);
            if (!p.second) {
                // Path already contains this character: Cycle detected!
                return res;
            }
            for (auto d = t->second.cbegin(), d_end = t->second.cend(); d != d_end; ++d) {
                auto src = translate_inv(db, d->second.str.c_str(), path);
                if (!src.empty()) {
                    for (auto dd = src.cbegin(), dd_end = src.cend(); dd != dd_end; ++dd) {
                        for (auto r = rem.cbegin(), r_end = rem.cend(); r != r_end; ++r)
                            res.insert(*dd + *r);
                    }
                } else {
                    // Cycle detected. Do not continue inverse translation.
                    for (auto r = rem.cbegin(), r_end = rem.cend(); r != r_end; ++r)
                        res.insert(_str + *r);
                }
            }
            path.erase(p.first);
        } else {
            // Current character is non-inverse translatable. Combine it with the remainder(s).
            for (auto r = rem.cbegin(), r_end = rem.cend(); r != r_end; ++r)
                res.insert(_str + *r);
        }
    } else {
        // Empty string results in empty inverse translation.
        res.insert(L"");
    }

    return res;
}


///
/// Main function
///
int _tmain(int argc, _TCHAR *argv[])
{
    wxApp::CheckBuildOptions(WX_BUILD_OPTIONS_SIGNATURE, "program");

    // Initialize wxWidgets.
    wxInitializer initializer;
    if (!initializer) {
        _ftprintf(stderr, wxT("Failed to initialize the wxWidgets library, aborting.\n"));
        return -1;
    }

    // Initialize configuration.
    wxConfigBase *cfgPrev = wxConfigBase::Set(new wxConfig(wxT(PRODUCT_CFG_APPLICATION), wxT(PRODUCT_CFG_VENDOR)));
    if (cfgPrev) wxDELETE(cfgPrev);

    // Initialize locale.
    wxLocale locale;
    if (wxInitializeLocale(locale))
        wxVERIFY(locale.AddCatalog(wxT("ZRColaCompile")));

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

    ZRCola::translation_db db_trans;

    {
        // Get translations.
        com_obj<ADORecordset> rs;
        if (src.SelectTranslations(rs)) {
            size_t count = src.GetRecordsetCount(rs);
            if (count < 0xffffffff) { // 4G check (-1 is reserved for error condition)
                // Parse translations and build temporary database.
                translation_db db_temp1;
                for (; !ZRCola::DBSource::IsEOF(rs); rs->MoveNext()) {
                    // Read translation from the database.
                    ZRCola::DBSource::translation trans;
                    if (src.GetTranslation(rs, trans)) {
                        // Add translation to temporary database.
                        auto const t = db_temp1.find(trans.dst.str);
                        if (t != db_temp1.end())
                            t->second.insert(com_translation(trans.dst.rank, std::move(trans.src)));
                        else {
                            translation_db::mapped_type d;
                            d.insert(com_translation(trans.dst.rank, std::move(trans.src)));
                            db_temp1.insert(std::move(pair<translation_db::key_type, translation_db::mapped_type>(trans.dst.str, std::move(d))));
                        }
                    } else
                        has_errors = true;
                }

                // Inverse translate source sequences down to non-inverse translatable characters.
                translation_db db_temp2;
                for (auto t1 = db_temp1.cbegin(), t1_end = db_temp1.cend(); t1 != t1_end; ++t1) {
                    for (auto d1 = t1->second.cbegin(), d1_end = t1->second.cend(); d1 != d1_end; ++d1) {
                        set<translation_db::key_type> path;
                        path.insert(t1->first);
                        auto str = translate_inv(db_temp1, d1->second.str.c_str(), path);
                        assert(!str.empty());

                        // Add translation to temporary database.
                        auto const t2 = db_temp2.find(t1->first);
                        if (t2 != db_temp2.end()) {
                            for (auto s = str.cbegin(), s_end = str.cend(); s != s_end; ++s)
                                t2->second.insert(com_translation(d1->first, std::move(ZRCola::DBSource::charseq(d1->second.rank, s->c_str()))));
                        } else {
                            translation_db::mapped_type d2;
                            for (auto s = str.cbegin(), s_end = str.cend(); s != s_end; ++s)
                                d2.insert(com_translation(d1->first, std::move(ZRCola::DBSource::charseq(d1->second.rank, s->c_str()))));
                            db_temp2.insert(std::move(pair<translation_db::key_type, translation_db::mapped_type>(t1->first, std::move(d2))));
                        }
                    }
                }

                // Preallocate memory.
                db_trans.idxSrc.reserve(count);
                db_trans.idxDst.reserve(count);
                db_trans.data  .reserve(count*5);

                // Parse translations and build index and data.
                ZRCola::DBSource::translation trans;
                trans.set = 0;
                for (auto t = db_temp2.cbegin(), t_end = db_temp2.cend(); t != t_end; ++t) {
                    // Add translation to index and data.
                    trans.dst.str = std::move(t->first);
                    for (auto d = t->second.cbegin(), d_end = t->second.cend(); d != d_end; ++d) {
                        trans.dst.rank = d->first;
                        trans.src.rank = d->second.rank;
                        trans.src.str  = std::move(d->second.str);
                        db_trans << trans;
                    }
                }
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
        // Get translation sets.
        com_obj<ADORecordset> rs;
        if (src.SelectTranlationSets(rs)) {
            size_t count = src.GetRecordsetCount(rs);
            if (count < 0xffffffff) { // 4G check (-1 is reserved for error condition)
                ZRCola::DBSource::transet ts;
                ZRCola::transet_db db;

                // Preallocate memory.
                db.idxTranSet.reserve(count);
                db.data      .reserve(count*4);

                // Add (de)composing translation set to index and data.
                ts.set = 0;
                ts.src = L"ZRCola Decomposed";
                ts.dst = L"ZRCola Composed";
                db << ts;
                if (build_pot) {
                    pot.insert(ts.src);
                    pot.insert(ts.dst);
                }

                // Parse translation sets and build index and data.
                for (; !ZRCola::DBSource::IsEOF(rs); rs->MoveNext()) {
                    // Read translation set from the database.
                    if (src.GetTranslationSet(rs, ts)) {
                        if (build_pot) {
                            pot.insert(ts.src);
                            pot.insert(ts.dst);
                        }

                        // Add translation set to index and data.
                        db << ts;

                        // Get translations.
                        com_obj<ADORecordset> rs_tran;
                        if (src.SelectTranslations(ts.set, rs_tran)) {
                            size_t count = src.GetRecordsetCount(rs_tran);
                            if (count < 0xffffffff) { // 4G check (-1 is reserved for error condition)
                                // Parse translations and build temporary database.
                                ZRCola::DBSource::translation trans;
                                trans.set = ts.set;
                                for (; !ZRCola::DBSource::IsEOF(rs_tran); rs_tran->MoveNext()) {
                                    // Read translation from the database.
                                    if (src.GetTranslation(rs_tran, trans)) {
                                        // Add translation to index and data.
                                        db_trans << trans;
                                    } else
                                        has_errors = true;
                                }
                            } else {
                                _ftprintf(stderr, wxT("%s: error ZCC0004: Error getting translation count from database or too many translations.\n"), (LPCTSTR)filenameIn.c_str());
                                has_errors = true;
                            }
                        } else {
                            _ftprintf(stderr, wxT("%s: error ZCC0003: Error getting translations from database. Please make sure the file is ZRCola.zrc compatible.\n"), (LPCTSTR)filenameIn.c_str());
                            has_errors = true;
                        }
                    } else
                        has_errors = true;
                }

                // Sort indices.
                db.idxTranSet.sort();

                // Write translation sets to file.
                dst << ZRCola::transet_rec(db);
            } else {
                _ftprintf(stderr, wxT("%s: error ZCC0009: Error getting translation set count from database or too many translation sets.\n"), (LPCTSTR)filenameIn.c_str());
                has_errors = true;
            }
        } else {
            _ftprintf(stderr, wxT("%s: error ZCC0008: Error getting translation sets from database. Please make sure the file is ZRCola.zrc compatible.\n"), (LPCTSTR)filenameIn.c_str());
            has_errors = true;
        }
    }

    // Sort indices.
    db_trans.idxSrc.sort();
    db_trans.idxDst.sort();

    // Write translations to file.
    dst << ZRCola::translation_rec(db_trans);

    {
        // Get key sequences.
        com_obj<ADORecordset> rs;
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
                        db << ks;
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

                    if (ZRCola::keyseq_db::keyseq::CompareSequence(ks1.seq(), ks1.seq_len(), ks2.seq(), ks2.seq_len()) == 0) {
                        wxString seq_str;
                        ZRCola::keyseq_db::GetSequenceAsText(ks1.seq(), ks1.seq_len(), seq_str);
                        _ftprintf(stderr, wxT("%s: warning ZCC0007: Duplicate key sequence (%ls => %s or %s). The keyboard behaviour will be unpredictable.\n"),
                            (LPCTSTR)filenameIn.c_str(),
                            seq_str.c_str(),
                            ZRCola::GetUnicodeDump(ks1.chr(), ks1.chr_len()).c_str(),
                            ZRCola::GetUnicodeDump(ks2.chr(), ks2.chr_len()).c_str());
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
        com_obj<ADORecordset> rs;
        if (src.SelectLanguages(rs)) {
            size_t count = src.GetRecordsetCount(rs);
            if (count < 0xffffffff) { // 4G check (-1 is reserved for error condition)
                ZRCola::DBSource::language lang;
                ZRCola::language_db db;

                // Preallocate memory.
                db.idxLang.reserve(count);
                db.data   .reserve(count*4);

                // Parse languages and build index and data.
                for (; !ZRCola::DBSource::IsEOF(rs); rs->MoveNext()) {
                    // Read language from the database.
                    if (src.GetLanguage(rs, lang)) {
                        if (build_pot)
                            pot.insert(lang.name);

                        // Add language to index and data.
                        db << lang;
                    } else
                        has_errors = true;
                }

                // Sort indices.
                db.idxLang.sort();

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
        com_obj<ADORecordset> rs;
        if (src.SelectLanguageCharacters(rs)) {
            size_t count = src.GetRecordsetCount(rs);
            if (count < 0xffffffff) { // 4G check (-1 is reserved for error condition)
                ZRCola::DBSource::langchar lc;
                ZRCola::langchar_db db;

                // Preallocate memory.
                db.idxChr .reserve(count);
#ifdef ZRCOLA_LANGCHAR_LANG_IDX
                db.idxLang.reserve(count);
#endif
                db.data   .reserve(count*4);

                // Parse language characters and build index and data.
                for (; !ZRCola::DBSource::IsEOF(rs); rs->MoveNext()) {
                    // Read language characters from the database.
                    if (src.GetLanguageCharacter(rs, lc)) {
                        // Add language characters to index and data.
                        db << lc;
                    } else
                        has_errors = true;
                }

                // Sort indices.
                db.idxChr .sort();
#ifdef ZRCOLA_LANGCHAR_LANG_IDX
                db.idxLang.sort();
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
        com_obj<ADORecordset> rs;
        if (src.SelectCharacterGroups(rs)) {
            size_t count = src.GetRecordsetCount(rs);
            if (count < 0xffffffff) { // 4G check (-1 is reserved for error condition)
                ZRCola::DBSource::chrgrp cg;
                ZRCola::chrgrp_db db;

                // Preallocate memory.
                db.idxRank.reserve(count);
                db.data   .reserve(count*4);

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
                        db << cg;
                    } else
                        has_errors = true;
                }

                // Sort indices.
                db.idxRank.sort();

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
        com_obj<ADORecordset> rs;
        if (src.SelectCharacters(rs)) {
            size_t count = src.GetRecordsetCount(rs);
            if (count < 0xffffffff) { // 4G check (-1 is reserved for error condition)
                ZRCola::DBSource::character_desc_idx idxChrDsc, idxChrDscSub;

                ZRCola::DBSource::character_bank chrs;
                ZRCola::DBSource::character chr;

                // Phase 1: Parse characters and build indexes.
                for (; !ZRCola::DBSource::IsEOF(rs); rs->MoveNext()) {
                    // Read character from the database.
                    if (src.GetCharacter(rs, chr))
                        chrs[chr.first] = std::move(chr.second);
                    else
                        has_errors = true;
                }

                // Phase 2: Build related character lists.
                chrs.build_related();

                ZRCola::character_db db;

                // Preallocate memory.
                db.idxChr.reserve(count);
                db.data  .reserve(count*4);

                // Phase 3: Parse characters and build index and data.
                for (auto chr = chrs.cbegin(), chr_end = chrs.cend(); chr != chr_end; ++chr) {
                    // Add character to index and data.
                    db << *chr;

                    // Add description (and keywords) to index.
                    idxChrDsc   .add_keywords(chr->second.terms, chr->first, 0);
                    idxChrDscSub.add_keywords(chr->second.terms, chr->first, 3);

                    // Mark category used.
                    categories_used.insert(chr->second.cat);
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
        com_obj<ADORecordset> rs;
        if (src.SelectCharacterCategories(rs)) {
            size_t count = src.GetRecordsetCount(rs);
            if (count < 0xffffffff) { // 4G check (-1 is reserved for error condition)
                ZRCola::DBSource::chrcat cc;
                ZRCola::chrcat_db db;

                // Preallocate memory.
                db.idxChrCat.reserve(count);
                db.idxRank  .reserve(count);
                db.data     .reserve(count*4);

                // Parse character categories and build index and data.
                for (; !ZRCola::DBSource::IsEOF(rs); rs->MoveNext()) {
                    // Read character category from the database.
                    if (src.GetCharacterCategory(rs, cc)) {
                        if (build_pot)
                            pot.insert(cc.name);

                        if (categories_used.find(cc.cat) == categories_used.end()) {
                            // Skip empty character categories.
                            continue;
                        }

                        // Add character category to index and data.
                        db << cc;
                    } else
                        has_errors = true;
                }

                // Sort indices.
                db.idxChrCat.sort();
                db.idxRank  .sort();

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

    {
        // Get characters tags.
        com_obj<ADORecordset> rs;
        if (src.SelectCharacterTags(rs)) {
            size_t count = src.GetRecordsetCount(rs);
            if (count < 0xffffffff) { // 4G check (-1 is reserved for error condition)
                ZRCola::DBSource::chrtag ct;
                ZRCola::chrtag_db db;

                // Preallocate memory.
                db.idxChr.reserve(count);
                db.idxTag.reserve(count);
                db.data  .reserve(count*4);

                // Parse characters tags and build index and data.
                for (; !ZRCola::DBSource::IsEOF(rs); rs->MoveNext()) {
                    // Read characters tags from the database.
                    if (src.GetCharacterTag(rs, ct)) {
                        // Add characters tags to index and data.
                        db << ct;
                    } else
                        has_errors = true;
                }

                // Sort indices.
                db.idxChr.sort();
                db.idxTag.sort();

                // Write characters tags to file.
                dst << ZRCola::chrtag_rec(db);
            } else {
                _ftprintf(stderr, wxT("%s: error ZCC0021: Error getting characters tags count from database or too many character tags.\n"), (LPCTSTR)filenameIn.c_str());
                has_errors = true;
            }
        } else {
            _ftprintf(stderr, wxT("%s: error ZCC0020: Error getting characters tags from database. Please make sure the file is ZRCola.zrc compatible.\n"), (LPCTSTR)filenameIn.c_str());
            has_errors = true;
        }
    }

    {
        // Get tag names.
        com_obj<ADORecordset> rs;
        if (src.SelectTagNames(rs)) {
            size_t count = src.GetRecordsetCount(rs);
            if (count < 0xffffffff) { // 4G check (-1 is reserved for error condition)
                ZRCola::DBSource::tagname tn;
                ZRCola::tagname_db db;

                // Preallocate memory.
                db.idxName.reserve(count*3);
                db.idxTag .reserve(count*3);
                db.data   .reserve(count*3*4);

                // Parse tags and build index and data.
                for (; !ZRCola::DBSource::IsEOF(rs); rs->MoveNext()) {
                    // Read tag name from the database.
                    if (src.GetTagName(rs, tn)) {
                        // Add tag name to index and data.
                        db << tn;
                    } else
                        has_errors = true;
                }

                // Sort indices.
                db.idxName.sort();
                db.idxTag .sort();

                // Write tags to file.
                dst << ZRCola::tagname_rec(db);
            } else {
                _ftprintf(stderr, wxT("%s: error ZCC0023: Error getting tag name count from database or too many tags.\n"), (LPCTSTR)filenameIn.c_str());
                has_errors = true;
            }
        } else {
            _ftprintf(stderr, wxT("%s: error ZCC0022: Error getting tags from database. Please make sure the file is ZRCola.zrc compatible.\n"), (LPCTSTR)filenameIn.c_str());
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
                << "\"X-Generator: ZRColaCompile\\n\"" << endl;

            wstring_convert<codecvt_utf8<wchar_t>> conv;
            for (auto i = pot.cbegin(); i != pot.cend(); ++i) {
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
