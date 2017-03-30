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

#pragma once

#include <zrcola/character.h>
#include <zrcola/common.h>
#include <zrcola/language.h>
#include <zrcola/tag.h>
#include <zrcola/translate.h>

#include <zrcolaui/chargroup.h>
#include <zrcolaui/keyboard.h>

#include <WinStd/COM.h>
#include <WinStd/Win.h>

#include <wx/debug.h>

#include <adoint.h>
#include <list>
#include <map>
#include <memory>
#include <set>
#include <string>
#include <vector>


namespace ZRCola {
    ///
    /// Source database
    ///
    class DBSource
    {
    public:
        ///
        /// Character sequence
        ///
        class charseq {
        public:
            int rank;         ///< Sequence rank
            std::wstring str; ///< Sequence string

            inline charseq()
            {
            }

            inline charseq(_In_ int _rank, _In_z_ const wchar_t *_str) :
                rank(_rank),
                str (_str)
            {
            }
        };


        ///
        /// Translation
        ///
        class translation {
        public:
            int set;        ///< Translation set ID
            charseq src;    ///< Source sequence
            charseq dst;    ///< Destination sequence
        };


        ///
        /// Translation set
        ///
        class transet {
        public:
            int set;            ///< ID
            std::wstring src;   ///< Source name
            std::wstring dst;   ///< Destination name
        };


        ///
        /// Key sequence
        ///
        class keyseq {
        public:
            ///
            /// Key code
            ///
            struct keycode {
                wchar_t key;            ///< Key
                bool shift;             ///< Shift modifier
                bool ctrl;              ///< Ctrl modifier
                bool alt;               ///< Alt modifier

                ///
                /// Translates keycode from Slovenian to English keyboard
                ///
                inline static wchar_t translate_slen(_In_ wchar_t key)
                {
                    switch (key) {
                    case L'Z': return L'Y';
                    case L'Y': return L'Z';
                    case  191: return  189;
                    case  189: return  191;
                    default  : return key;
                    }
                }
            };

        public:
            std::wstring chr;           ///< Character
            std::vector<keycode> seq;   ///< Key sequence
        };


        ///
        /// Language
        ///
        class language {
        public:
            ZRCola::langid_t lang;      ///< Language ID
            std::wstring name;          ///< Name
        };


        ///
        /// Language Character
        ///
        class langchar {
        public:
            std::wstring chr;           ///> Character
            ZRCola::langid_t lang;      ///< Language ID
        };


        ///
        /// Character group
        ///
        class chrgrp {
        public:
            int grp;                            ///< Character group ID
            int rank;                           ///< Rank
            std::wstring name;                  ///< Name
            std::vector<wchar_t> chars;         ///< Characters (zero-delimited)
            std::vector<unsigned __int16> show; ///< Bit vector if particular character from \c chars is displayed initially
        };


        ///
        /// Character data
        ///
        class character_data {
        public:
            inline character_data()
            {
                cat.data[0] = 0;
                cat.data[1] = 0;
            }

            inline character_data(_In_ const character_data &othr) :
                cat      (othr.cat),
                desc     (othr.desc),
                terms    (othr.terms),
                terms_rel(othr.terms_rel),
                rel      (othr.rel)
            {
            }

            ZRCola::chrcatid_t cat;             ///< Category ID
            std::wstring desc;                  ///< Character description
            std::set<std::wstring> terms;       ///< Search terms
            std::set<std::wstring> terms_rel;   ///< Relevant terms for relating characters
            std::vector<wchar_t> rel;           ///< Related characters (zero-delimited)
        };


        ///
        /// Character
        ///
        typedef std::pair<std::wstring, character_data> character;


        ///
        /// Character bank
        ///
        class character_bank : public std::map<std::wstring, character_data>
        {
        public:
            void build_related();

        protected:
            class build_related_worker : public winstd::win_handle
            {
            public:
                build_related_worker(_In_ const character_bank *cb, _In_ iterator from, _In_ iterator to);

                inline void join()
                {
                    if (m_h)
                        WaitForSingleObject(m_h, INFINITE);
                }

            private:
                // This class is non-copyable AND non-movable
                build_related_worker(_Inout_ build_related_worker &othr);
                build_related_worker(_Inout_ build_related_worker &&othr);
                build_related_worker& operator=(_Inout_ build_related_worker &othr);
                build_related_worker& operator=(_Inout_ build_related_worker &&othr);

            protected:
                unsigned int process();
                static unsigned int __stdcall process(_In_ void *param);

            protected:
                const character_bank *m_cb;
                iterator m_from, m_to;
                winstd::heap m_heap;
            };
        };


        ///
        /// Character description index key comparator
        ///
        struct character_desc_idx_less : public std::binary_function<std::wstring, std::wstring, bool>
        {
            inline bool operator()(const std::wstring& _Left, const std::wstring& _Right) const
            {
                size_t
                    _Left_len  = _Left .size(),
                    _Right_len = _Right.size();

                int r = _wcsncoll(_Left.c_str(), _Right.c_str(), std::min<size_t>(_Left_len, _Right_len));
                     if (r         != 0         ) return r < 0;
                else if (_Left_len <  _Right_len) return true;

                return false;
            }
        };


        ///
        /// Character description index
        ///
        class character_desc_idx : public std::map<std::wstring, std::vector<wchar_t>, character_desc_idx_less>
        {
        public:
            static void parse_keywords(const wchar_t *str, std::set<std::wstring> &terms);
            void add_keywords(const std::set<std::wstring> &terms, const std::wstring &chr, size_t sub = 0);
            inline void add_keywords(const wchar_t *str, const std::wstring &chr, size_t sub = 0)
            {
                std::set<std::wstring> terms;
                parse_keywords(str, terms);
                add_keywords(terms, chr, sub);
            }

            void save(ZRCola::textindex<wchar_t, wchar_t, unsigned __int32> &idx) const;

        protected:
            inline void add_keyword(const std::wstring &term, const std::wstring &chr)
            {
                iterator idx = find(term);
                if (idx == end()) {
                    // New keyword.
                    insert(std::make_pair(term, mapped_type(chr.data(), chr.data() + chr.length() + 1)));
                } else {
                    // Append to existing keyword.
                    auto &val = idx->second;
                    for (mapped_type::size_type i = 0, n = val.size(); ; i += wcsnlen(val.data() + i, n - i) + 1) {
                        if (i >= n) {
                            // End-of-values reached. Append character.
                            val.insert(val.end(), chr.data(), chr.data() + chr.length() + 1);
                            break;
                        } else if (chr.compare(val.data() + i) == 0) {
                            // Character already among the values.
                            break;
                        }
                    }
                }
            }
        };


        ///
        /// Character category
        ///
        class chrcat {
        public:
            ZRCola::chrcatid_t cat;     ///> Category ID
            int rank;                   ///< Rank
            std::wstring name;          ///< Name
        };


        ///
        /// Character tag
        ///
        class chrtag {
        public:
            std::wstring chr;           ///> Character
            int tag;                    ///< Tag ID
        };


        ///
        /// Tag name
        ///
        class tagname {
        public:
            int tag;                    ///< Tag ID
            std::map<LCID, std::list<std::wstring> > names; ///< Names
        };


    public:
        DBSource();
        virtual ~DBSource();

        ///
        /// Opens the database
        ///
        /// \param[in] filename  File name of the MDB database.
        ///
        /// \returns
        /// - true when open succeeds
        /// - false otherwise
        ///
        bool Open(LPCTSTR filename);

        ///
        /// Logs errors in database connections
        ///
        void LogErrors() const;

        ///
        /// Is recordset at end
        ///
        /// \param[out] rs  Recordset with results
        ///
        /// \returns
        /// - true when at end
        /// - false otherwise
        ///
        static inline bool IsEOF(const winstd::com_obj<ADORecordset>& rs)
        {
            VARIANT_BOOL eof = VARIANT_TRUE;
            return FAILED(rs->get_EOF(&eof)) || eof ? true : false;
        }

        ///
        /// Gets number of records in a recordset
        ///
        /// \param[out] rs  Recordset with results
        ///
        /// \returns Number of records
        ///
        static inline size_t GetRecordsetCount(const winstd::com_obj<ADORecordset>& rs)
        {
            ADO_LONGPTR count;
            return SUCCEEDED(rs->get_RecordCount(&count)) ? count : (size_t)-1;
        }

        ///
        /// Splits string to individual keywords
        ///
        /// \param[in ] str       String
        /// \param[out] keywords  Array of keywords
        ///
        /// \returns
        /// - true when successful
        /// - false otherwise
        ///
        static bool GetKeywords(const wchar_t *str, std::vector< std::wstring > &keywords);

        ///
        /// Gets boolean from ZRCola.zrc database
        ///
        /// \param[in]  f    Data field
        /// \param[out] val  Output boolean value
        ///
        /// \returns
        /// - true when successful
        /// - false otherwise
        ///
        bool GetValue(const winstd::com_obj<ADOField>& f, bool& val) const;

        ///
        /// Gets integer from ZRCola.zrc database
        ///
        /// \param[in]  f    Data field
        /// \param[out] val  Output integer value
        ///
        /// \returns
        /// - true when successful
        /// - false otherwise
        ///
        bool GetValue(const winstd::com_obj<ADOField>& f, int& val) const;

        ///
        /// Gets string from ZRCola.zrc database
        ///
        /// \param[in]  f    Data field
        /// \param[out] val  Output string value
        ///
        /// \returns
        /// - true when successful
        /// - false otherwise
        ///
        bool GetValue(const winstd::com_obj<ADOField>& f, std::wstring& val) const;

        ///
        /// Gets encoded Unicode character from ZRCola.zrc database
        ///
        /// \param[in]  f    Data field
        /// \param[out] chr  Output character
        ///
        /// \returns
        /// - true when successful
        /// - false otherwise
        ///
        bool GetUnicodeCharacter(const winstd::com_obj<ADOField>& f, wchar_t& chr) const;

        ///
        /// Gets encoded Unicode string from ZRCola.zrc database
        ///
        /// \param[in]  f    Data field
        /// \param[out] str  Output string
        ///
        /// \returns
        /// - true when successful
        /// - false otherwise
        ///
        bool GetUnicodeString(const winstd::com_obj<ADOField>& f, std::wstring& str) const;

        ///
        /// Gets language ID from ZRCola.zrc database
        ///
        /// \param[in]  f     Data field
        /// \param[out] lang  Language
        ///
        /// \returns
        /// - true when successful
        /// - false otherwise
        ///
        bool GetLanguage(const winstd::com_obj<ADOField>& f, langid_t& lang) const;

        ///
        /// Gets character category ID from ZRCola.zrc database
        ///
        /// \param[in]  f   Data field
        /// \param[out] cc  Character category
        ///
        /// \returns
        /// - true when successful
        /// - false otherwise
        ///
        bool GetChrCat(const winstd::com_obj<ADOField>& f, chrcatid_t& cc) const;

        ///
        /// Gets tag names from ZRCola.zrc database
        ///
        /// \param[in]  f      Data field
        /// \param[out] names  Output names
        ///
        /// \returns
        /// - true when successful
        /// - false otherwise
        ///
        bool GetTagNames(const winstd::com_obj<ADOField>& f, LCID lcid, std::list<std::wstring>& names) const;

        ///
        /// Returns character translations
        ///
        /// \param[out] rs  Recordset with results
        ///
        /// \returns
        /// - true when query succeeds
        /// - false otherwise
        ///
        bool SelectTranslations(winstd::com_obj<ADORecordset>& rs) const;

        ///
        /// Returns character translations by set
        ///
        /// \param[in ] set  Translation set ID
        /// \param[out] rs   Recordset with results
        ///
        /// \returns
        /// - true when query succeeds
        /// - false otherwise
        ///
        bool SelectTranslations(int set, winstd::com_obj<ADORecordset>& rs) const;

        ///
        /// Returns translation data
        ///
        /// \param[in]  rs  Recordset with results
        /// \param[out] t   Translation
        ///
        /// \returns
        /// - true when succeeded
        /// - false otherwise
        ///
        bool GetTranslation(const winstd::com_obj<ADORecordset>& rs, translation& t) const;

        ///
        /// Returns translation sets
        ///
        /// \param[out] rs  Recordset with results
        ///
        /// \returns
        /// - true when query succeeds
        /// - false otherwise
        ///
        bool SelectTranlationSets(winstd::com_obj<ADORecordset>& rs) const;

        ///
        /// Returns translation set data
        ///
        /// \param[in]  rs    Recordset with results
        /// \param[out] lang  Language
        ///
        /// \returns
        /// - true when succeeded
        /// - false otherwise
        ///
        bool GetTranslationSet(const winstd::com_obj<ADORecordset>& rs, transet& ts) const;

        ///
        /// Returns key sequences
        ///
        /// \param[out] rs  Recordset with results
        ///
        /// \returns
        /// - true when query succeeds
        /// - false otherwise
        ///
        bool SelectKeySequences(winstd::com_obj<ADORecordset>& rs) const;

        ///
        /// Returns key sequence data
        ///
        /// \param[in]  rs  Recordset with results
        /// \param[out] ks  Key sequence
        ///
        /// \returns
        /// - true when succeeded
        /// - false otherwise
        ///
        bool GetKeySequence(const winstd::com_obj<ADORecordset>& rs, keyseq& ks) const;

        ///
        /// Returns languages
        ///
        /// \param[out] rs  Recordset with results
        ///
        /// \returns
        /// - true when query succeeds
        /// - false otherwise
        ///
        bool SelectLanguages(winstd::com_obj<ADORecordset>& rs) const;

        ///
        /// Returns language data
        ///
        /// \param[in]  rs    Recordset with results
        /// \param[out] lang  Language
        ///
        /// \returns
        /// - true when succeeded
        /// - false otherwise
        ///
        bool GetLanguage(const winstd::com_obj<ADORecordset>& rs, language& lang) const;

        ///
        /// Returns language character
        ///
        /// \param[out] rs  Recordset with results
        ///
        /// \returns
        /// - true when query succeeds
        /// - false otherwise
        ///
        bool SelectLanguageCharacters(winstd::com_obj<ADORecordset>& rs) const;

        ///
        /// Returns language character data
        ///
        /// \param[in]  rs    Recordset with results
        /// \param[out] lang  Language character data
        ///
        /// \returns
        /// - true when succeeded
        /// - false otherwise
        ///
        bool GetLanguageCharacter(const winstd::com_obj<ADORecordset>& rs, langchar& lc) const;

        ///
        /// Returns character groups
        ///
        /// \param[out] rs  Recordset with results
        ///
        /// \returns
        /// - true when query succeeds
        /// - false otherwise
        ///
        bool SelectCharacterGroups(winstd::com_obj<ADORecordset>& rs) const;

        ///
        /// Returns character group data
        ///
        /// \param[in]  rs    Recordset with results
        /// \param[out] cg    Character group
        ///
        /// \returns
        /// - true when succeeded
        /// - false otherwise
        ///
        bool GetCharacterGroup(const winstd::com_obj<ADORecordset>& rs, chrgrp& cg) const;

        ///
        /// Returns characters
        ///
        /// \param[out] rs  Recordset with results
        ///
        /// \returns
        /// - true when query succeeds
        /// - false otherwise
        ///
        bool SelectCharacters(winstd::com_obj<ADORecordset>& rs) const;

        ///
        /// Returns character data
        ///
        /// \param[in]  rs    Recordset with results
        /// \param[out] chr   Character
        ///
        /// \returns
        /// - true when succeeded
        /// - false otherwise
        ///
        bool GetCharacter(const winstd::com_obj<ADORecordset>& rs, character& chr) const;

        ///
        /// Returns character categories
        ///
        /// \param[out] rs  Recordset with results
        ///
        /// \returns
        /// - true when query succeeds
        /// - false otherwise
        ///
        bool SelectCharacterCategories(winstd::com_obj<ADORecordset>& rs) const;

        ///
        /// Returns character category data
        ///
        /// \param[in]  rs    Recordset with results
        /// \param[out] cc    Character category
        ///
        /// \returns
        /// - true when succeeded
        /// - false otherwise
        ///
        bool GetCharacterCategory(const winstd::com_obj<ADORecordset>& rs, chrcat& cc) const;

        ///
        /// Returns character tags
        ///
        /// \param[out] rs  Recordset with results
        ///
        /// \returns
        /// - true when query succeeds
        /// - false otherwise
        ///
        bool SelectCharacterTags(winstd::com_obj<ADORecordset>& rs) const;

        ///
        /// Returns character tag data
        ///
        /// \param[in]  rs    Recordset with results
        /// \param[out] cc    Character tag
        ///
        /// \returns
        /// - true when succeeded
        /// - false otherwise
        ///
        bool GetCharacterTag(const winstd::com_obj<ADORecordset>& rs, chrtag& tc) const;

        ///
        /// Returns tag names
        ///
        /// \param[out] rs  Recordset with results
        ///
        /// \returns
        /// - true when query succeeds
        /// - false otherwise
        ///
        bool SelectTagNames(winstd::com_obj<ADORecordset>& rs) const;

        ///
        /// Returns tag name data
        ///
        /// \param[in]  rs    Recordset with results
        /// \param[out] tn    Tag name
        ///
        /// \returns
        /// - true when succeeded
        /// - false otherwise
        ///
        bool GetTagName(const winstd::com_obj<ADORecordset>& rs, tagname& tn) const;

    protected:
        std::basic_string<TCHAR> m_filename;    ///< Database filename
        winstd::com_obj<ADOConnection> m_db;    ///< Database
        _locale_t m_locale;                     ///< Database locale

        winstd::com_obj<ADOCommand> m_comCharacterGroup;    ///< ADO Command for GetCharacterGroup subquery
        winstd::com_obj<ADOParameter> m_pCharacterGroup1;   ///< \c m_comCharacterGroup parameter

        winstd::com_obj<ADOCommand> m_comTranslation;       ///< ADO Command for SelectTranslations subquery
        winstd::com_obj<ADOParameter> m_pTranslation1;      ///< \c m_comTranslations parameter

        std::set<std::wstring> m_terms_ignore;  ///< Terms to ignore when comparing characters
    };
};


inline ZRCola::translation_db& operator<<(_Inout_ ZRCola::translation_db &db, _In_ const ZRCola::DBSource::translation &rec)
{
    unsigned __int32 idx = db.data.size();
    wxASSERT_MSG((int)0xffff8000 <= rec.set && rec.set <= (int)0x00007fff, wxT("translation set index out of bounds"));
    db.data.push_back((unsigned __int16)rec.set);
    wxASSERT_MSG((int)0xffff8000 <= rec.dst.rank && rec.dst.rank <= (int)0x00007fff, wxT("destination character rank out of bounds"));
    db.data.push_back((unsigned __int16)rec.dst.rank);
    wxASSERT_MSG((int)0xffff8000 <= rec.src.rank && rec.src.rank <= (int)0x00007fff, wxT("source character rank out of bounds"));
    db.data.push_back((unsigned __int16)rec.src.rank);
    std::wstring::size_type n = rec.dst.str.length();
    wxASSERT_MSG(n <= 0xffff, wxT("destination overflow"));
    db.data.push_back((unsigned __int16)n);
    n += rec.src.str.length();
    wxASSERT_MSG(n <= 0xffff, wxT("source overflow"));
    db.data.push_back((unsigned __int16)n);
    db.data.insert(db.data.end(), rec.dst.str.cbegin(), rec.dst.str.cend());
    db.data.insert(db.data.end(), rec.src.str.cbegin(), rec.src.str.cend());
    db.idxSrc.push_back(idx);
    db.idxDst.push_back(idx);

    return db;
}


inline ZRCola::transet_db& operator<<(_Inout_ ZRCola::transet_db &db, _In_ const ZRCola::DBSource::transet &rec)
{
    unsigned __int32 idx = db.data.size();
    wxASSERT_MSG((int)0xffff8000 <= rec.set && rec.set <= (int)0x00007fff, wxT("translation set index out of bounds"));
    db.data.push_back((unsigned __int16)rec.set);
    std::wstring::size_type n = rec.src.length();
    wxASSERT_MSG(n <= 0xffff, wxT("translation set source name overflow"));
    db.data.push_back((unsigned __int16)n);
    n += rec.dst.length();
    wxASSERT_MSG(n <= 0xffff, wxT("translation set destination name overflow"));
    db.data.push_back((unsigned __int16)n);
    db.data.insert(db.data.end(), rec.src.cbegin(), rec.src.cend());
    db.data.insert(db.data.end(), rec.dst.cbegin(), rec.dst.cend());
    db.idxTranSet.push_back(idx);

    return db;
}


inline ZRCola::keyseq_db& operator<<(_Inout_ ZRCola::keyseq_db &db, _In_ const ZRCola::DBSource::keyseq &rec)
{
    unsigned __int32 idx = db.data.size();
    std::wstring::size_type n = rec.chr.length();
    wxASSERT_MSG(n <= 0xffff, wxT("character overflow"));
    db.data.push_back((unsigned __int16)n);
    n += rec.seq.size() * sizeof(ZRCola::keyseq_db::keyseq::key_t) / sizeof(wchar_t);
    wxASSERT_MSG(n <= 0xffff, wxT("key sequence overflow"));
    db.data.push_back((unsigned __int16)n);
    db.data.insert(db.data.end(), rec.chr.cbegin(), rec.chr.cend());
    for (auto kc = rec.seq.cbegin(), kc_end = rec.seq.cend(); kc != kc_end; ++kc) {
        db.data.push_back(kc->key);
        db.data.push_back(
            (kc->shift ? ZRCola::keyseq_db::keyseq::SHIFT : 0) |
            (kc->ctrl  ? ZRCola::keyseq_db::keyseq::CTRL  : 0) |
            (kc->alt   ? ZRCola::keyseq_db::keyseq::ALT   : 0));
    }
    db.idxChr.push_back(idx);
    db.idxKey.push_back(idx);

    return db;
}


inline ZRCola::language_db& operator<<(_Inout_ ZRCola::language_db &db, _In_ const ZRCola::DBSource::language &rec)
{
    unsigned __int32 idx = db.data.size();
    db.data.insert(db.data.end(), reinterpret_cast<const unsigned __int16*>(&rec.lang), reinterpret_cast<const unsigned __int16*>(&rec.lang + 1));
    std::wstring::size_type n = rec.name.length();
    wxASSERT_MSG(n <= 0xffff, wxT("language name overflow"));
    db.data.push_back((unsigned __int16)n);
    db.data.insert(db.data.end(), rec.name.cbegin(), rec.name.cend());
    db.idxLang.push_back(idx);

    return db;
}


inline ZRCola::langchar_db& operator<<(_Inout_ ZRCola::langchar_db &db, _In_ const ZRCola::DBSource::langchar &rec)
{
    unsigned __int32 idx = db.data.size();
    db.data.insert(db.data.end(), reinterpret_cast<const unsigned __int16*>(&rec.lang), reinterpret_cast<const unsigned __int16*>(&rec.lang + 1));
    std::wstring::size_type n = rec.chr.length();
    wxASSERT_MSG(n <= 0xffff, wxT("character overflow"));
    db.data.push_back((unsigned __int16)n);
    db.data.insert(db.data.end(), rec.chr.cbegin(), rec.chr.cend());
    db.idxChr .push_back(idx);
#ifdef ZRCOLA_LANGCHAR_LANG_IDX
    db.idxLang.push_back(idx);
#endif

    return db;
}


inline ZRCola::chrgrp_db& operator<<(_Inout_ ZRCola::chrgrp_db &db, _In_ const ZRCola::DBSource::chrgrp &rec)
{
    unsigned __int32 idx = db.data.size();
    wxASSERT_MSG((int)0xffff8000 <= rec.grp && rec.grp <= (int)0x00007fff, wxT("character group ID out of bounds"));
    db.data.push_back((unsigned __int16)rec.grp);
    wxASSERT_MSG((int)0xffff8000 <= rec.rank && rec.rank <= (int)0x00007fff, wxT("character group rank out of bounds"));
    db.data.push_back((unsigned __int16)rec.rank);
    std::wstring::size_type n = rec.name.length();
    wxASSERT_MSG(n <= 0xffff, wxT("character group name overflow"));
    db.data.push_back((unsigned __int16)n);
    n += rec.chars.size();
    wxASSERT_MSG(n <= 0xffff, wxT("character group characters overflow"));
    db.data.push_back((unsigned __int16)n);
    db.data.insert(db.data.end(), rec.name .cbegin(), rec.name .cend());
    db.data.insert(db.data.end(), rec.chars.cbegin(), rec.chars.cend());
    db.data.insert(db.data.end(), rec.show .cbegin(), rec.show .cend());
    db.idxRank.push_back(idx);

    return db;
}


inline ZRCola::character_db& operator<<(_Inout_ ZRCola::character_db &db, _In_ const ZRCola::DBSource::character &rec)
{
    unsigned __int32 idx = db.data.size();
    db.data.insert(db.data.end(), reinterpret_cast<const unsigned __int16*>(&rec.second.cat), reinterpret_cast<const unsigned __int16*>(&rec.second.cat + 1));
    std::wstring::size_type n = rec.first.length();
    wxASSERT_MSG(n <= 0xffff, wxT("character overflow"));
    db.data.push_back((unsigned __int16)n);
    n += rec.second.desc.length();
    wxASSERT_MSG(n <= 0xffff, wxT("character description overflow"));
    db.data.push_back((unsigned __int16)n);
    n += rec.second.rel.size();
    wxASSERT_MSG(n <= 0xffff, wxT("related characters overflow"));
    db.data.push_back((unsigned __int16)n);
    db.data.insert(db.data.end(), rec.first      .cbegin(), rec.first      .cend());
    db.data.insert(db.data.end(), rec.second.desc.cbegin(), rec.second.desc.cend());
    db.data.insert(db.data.end(), rec.second.rel .cbegin(), rec.second.rel .cend());
    db.idxChr.push_back(idx);

    return db;
}


inline ZRCola::chrcat_db& operator<<(_Inout_ ZRCola::chrcat_db &db, _In_ const ZRCola::DBSource::chrcat &rec)
{
    unsigned __int32 idx = db.data.size();
    db.data.insert(db.data.end(), reinterpret_cast<const unsigned __int16*>(&rec.cat), reinterpret_cast<const unsigned __int16*>(&rec.cat + 1));
    wxASSERT_MSG((int)0xffff8000 <= rec.rank && rec.rank <= (int)0x00007fff, wxT("character category rank out of bounds"));
    db.data.push_back((unsigned __int16)rec.rank);
    std::wstring::size_type n = rec.name.length();
    wxASSERT_MSG(n <= 0xffff, wxT("character category name overflow"));
    db.data.push_back((unsigned __int16)n);
    db.data.insert(db.data.end(), rec.name.cbegin(), rec.name.cend());
    db.idxChrCat.push_back(idx);
    db.idxRank  .push_back(idx);

    return db;
}


inline ZRCola::chrtag_db& operator<<(_Inout_ ZRCola::chrtag_db &db, _In_ const ZRCola::DBSource::chrtag &rec)
{
    unsigned __int32 idx = db.data.size();
    wxASSERT_MSG((int)0xffff8000 <= rec.tag && rec.tag <= (int)0x00007fff, wxT("tag out of bounds"));
    db.data.push_back((unsigned __int16)rec.tag);
    std::wstring::size_type n = rec.chr.length();
    wxASSERT_MSG(n <= 0xffff, wxT("character overflow"));
    db.data.push_back((unsigned __int16)n);
    db.data.insert(db.data.end(), rec.chr.cbegin(), rec.chr.cend());
    db.idxChr.push_back(idx);
    db.idxTag.push_back(idx);

    return db;
}


inline ZRCola::tagname_db& operator<<(_Inout_ ZRCola::tagname_db &db, _In_ const ZRCola::DBSource::tagname &rec)
{
    for (auto ln = rec.names.cbegin(), ln_end = rec.names.cend(); ln != ln_end; ++ln) {
        for (auto nm = ln->second.cbegin(), nm_end = ln->second.cend(); nm != nm_end; ++nm) {
            unsigned __int32 idx = db.data.size();
            wxASSERT_MSG((int)0xffff8000 <= rec.tag && rec.tag <= (int)0x00007fff, wxT("tag out of bounds"));
            db.data.push_back((unsigned __int16)rec.tag);
            db.data.push_back(LOWORD(ln->first));
            db.data.push_back(HIWORD(ln->first));
            std::wstring::size_type n = nm->length();
            wxASSERT_MSG(n <= 0xffff, wxT("tag name overflow"));
            db.data.push_back((unsigned __int16)n);
            db.data.insert(db.data.end(), nm->cbegin(), nm->cend());
            db.idxName.push_back(idx);
            db.idxTag .push_back(idx);
        }
    }

    return db;
}
