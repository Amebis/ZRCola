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

#include <atlbase.h>
#include <adoint.h>
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
        /// Translation
        ///
        class translation {
        public:
            wchar_t chr;                ///< Composed character
            std::wstring str;           ///< Decomposed string
            int rank;                   ///< Decomposition rank
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
            };

        public:
            wchar_t chr;                ///< Character
            std::vector<keycode> seq;   ///< Key sequence
        };


        ///
        /// Language
        ///
        class language {
        public:
            ZRCola::langid_t id;        ///< Language ID
            std::wstring name;          ///< Language name
        };


        ///
        /// Language Character
        ///
        class langchar {
        public:
            wchar_t chr;                ///> Character
            ZRCola::langid_t lang;      ///< Language ID
        };


        ///
        /// Character group
        ///
        class chrgrp {
        public:
            int id;                     ///< Character group ID
            int rank;                   ///< Character group rank
            std::wstring name;          ///< Character group name
            std::wstring chars;         ///< Character group characters
        };


        ///
        /// Character
        ///
        class character {
        public:
            wchar_t chr;                ///< Character
            ZRCola::chrcatid_t cat;     ///> Category ID
            std::wstring desc;          ///< Character description
            std::wstring keywords;      ///< Additional keywords
            std::wstring rel;           ///< Related characters
        };


        ///
        /// Character category
        ///
        class chrcat {
        public:
            ZRCola::chrcatid_t id;      ///> Category ID
            int rank;                   ///< Character category rank
            std::wstring name;          ///< Character category name
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
        static inline bool IsEOF(const ATL::CComPtr<ADORecordset>& rs)
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
        static inline size_t GetRecordsetCount(const ATL::CComPtr<ADORecordset>& rs)
        {
            ADO_LONGPTR count;
            return SUCCEEDED(rs->get_RecordCount(&count)) ? count : (size_t)-1;
        }


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
        bool GetValue(const ATL::CComPtr<ADOField>& f, bool& val) const;


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
        bool GetValue(const ATL::CComPtr<ADOField>& f, int& val) const;


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
        bool GetValue(const ATL::CComPtr<ADOField>& f, std::wstring& val) const;


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
        bool GetUnicodeCharacter(const ATL::CComPtr<ADOField>& f, wchar_t& chr) const;


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
        bool GetUnicodeString(const ATL::CComPtr<ADOField>& f, std::wstring& str) const;


        ///
        /// Gets encoded key from ZRCola.zrc database
        ///
        /// \param[in]  f   Data field
        /// \param[out] kc  Output key code
        ///
        /// \returns
        /// - true when successful
        /// - false otherwise
        ///
        bool GetKeyCode(const ATL::CComPtr<ADOField>& f, keyseq::keycode& kc) const;


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
        bool GetLanguage(const ATL::CComPtr<ADOField>& f, langid_t& lang) const;


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
        bool GetChrCat(const ATL::CComPtr<ADOField>& f, chrcatid_t& cc) const;


        ///
        /// Returns character translations
        ///
        /// \param[out] rs  Recordset with results
        ///
        /// \returns
        /// - true when query succeeds
        /// - false otherwise
        ///
        bool SelectTranslations(ATL::CComPtr<ADORecordset>& rs) const;


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
        bool GetTranslation(const ATL::CComPtr<ADORecordset>& rs, translation& t) const;


        ///
        /// Returns key sequences
        ///
        /// \param[out] rs  Recordset with results
        ///
        /// \returns
        /// - true when query succeeds
        /// - false otherwise
        ///
        bool SelectKeySequences(ATL::CComPtr<ADORecordset>& rs) const;


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
        bool GetKeySequence(const ATL::CComPtr<ADORecordset>& rs, keyseq& ks) const;


        ///
        /// Returns languages
        ///
        /// \param[out] rs  Recordset with results
        ///
        /// \returns
        /// - true when query succeeds
        /// - false otherwise
        ///
        bool SelectLanguages(ATL::CComPtr<ADORecordset>& rs) const;


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
        bool GetLanguage(const ATL::CComPtr<ADORecordset>& rs, language& lang) const;


        ///
        /// Returns language character
        ///
        /// \param[out] rs  Recordset with results
        ///
        /// \returns
        /// - true when query succeeds
        /// - false otherwise
        ///
        bool SelectLanguageCharacters(ATL::CComPtr<ADORecordset>& rs) const;


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
        bool GetLanguageCharacter(const ATL::CComPtr<ADORecordset>& rs, langchar& lc) const;


        ///
        /// Returns character groups
        ///
        /// \param[out] rs  Recordset with results
        ///
        /// \returns
        /// - true when query succeeds
        /// - false otherwise
        ///
        bool SelectCharacterGroups(ATL::CComPtr<ADORecordset>& rs) const;


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
        bool GetCharacterGroup(const ATL::CComPtr<ADORecordset>& rs, chrgrp& cg) const;

        ///
        /// Returns characters
        ///
        /// \param[out] rs  Recordset with results
        ///
        /// \returns
        /// - true when query succeeds
        /// - false otherwise
        ///
        bool SelectCharacters(ATL::CComPtr<ADORecordset>& rs) const;


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
        bool GetCharacter(const ATL::CComPtr<ADORecordset>& rs, character& chr) const;

        ///
        /// Returns character categories
        ///
        /// \param[out] rs  Recordset with results
        ///
        /// \returns
        /// - true when query succeeds
        /// - false otherwise
        ///
        bool SelectCharacterCategories(ATL::CComPtr<ADORecordset>& rs) const;


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
        bool GetCharacterCategory(const ATL::CComPtr<ADORecordset>& rs, chrcat& cc) const;

    protected:
        std::basic_string<TCHAR> m_filename;    ///< Database filename
        ATL::CComPtr<ADOConnection> m_db;       ///< Database
        _locale_t m_locale;                     ///< Database locale

        ATL::CComPtr<ADOCommand> m_comCharacterGroup;   ///< ADO Command for GetCharacterGroup subquery
        ATL::CComPtr<ADOParameter> m_pCharacterGroup1;  ///< \c m_comCharacterGroup parameter
    };
};
