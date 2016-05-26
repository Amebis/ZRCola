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
#include <list>
#include <map>
#include <memory>
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
            inline character()
            {
                chr = 0;
                cat.data[0] = 0;
                cat.data[1] = 0;
            }

            inline character(_In_ const character &othr) :
                chr  (othr.chr),
                cat  (othr.cat),
                desc (othr.desc),
                terms(othr.terms),
                rel  (othr.rel)
            {
            }

            inline bool operator==(_In_ const character &othr) const
            {
                return
                    chr   == othr.chr   &&
                    cat   == othr.cat   &&
                    desc  == othr.desc  &&
                    terms == othr.terms &&
                    rel   == othr.rel;
            }

            inline bool operator!=(_In_ const character &othr) const
            {
                return !operator==(othr);
            }

            wchar_t chr;                    ///< Character
            ZRCola::chrcatid_t cat;         ///< Category ID
            std::wstring desc;              ///< Character description
            std::list<std::wstring> terms;  ///< Search terms
            std::wstring rel;               ///< Related characters
        };


        ///
        /// Character bank
        ///
        class character_bank : public std::vector<std::unique_ptr<character> >
        {
        public:
            character_bank();
            void build_related();

        protected:
            class build_related_worker : public std::unique_ptr<void, stdex::CloseHandle_delete<void> >
            {
            public:
                typedef std::unique_ptr<void, stdex::CloseHandle_delete<void> > thread_type;

            public:
                build_related_worker(_In_ character_bank *cb, _In_ size_type from, _In_ size_type to);

                inline void join()
                {
                    HANDLE h = get();
                    if (h)
                        WaitForSingleObject(h, INFINITE);
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
                character_bank *m_cb;
                size_type m_from, m_to;
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
            static void parse_keywords(const wchar_t *str, std::list<std::wstring> &terms);
            void add_keywords(const std::list<std::wstring> &terms, wchar_t chr, size_t sub = 0);
            inline void add_keywords(const wchar_t *str, wchar_t chr, size_t sub = 0)
            {
                std::list<std::wstring> terms;
                parse_keywords(str, terms);
                add_keywords(terms, chr, sub);
            }

            void save(ZRCola::textindex<wchar_t, wchar_t, unsigned __int32> &idx) const;

        protected:
            inline void add_keyword(const std::wstring &term, wchar_t chr)
            {
                iterator idx = find(term);
                if (idx == end()) {
                    // New keyword.
                    insert(std::make_pair(term, std::vector<wchar_t>(1, chr)));
                } else {
                    // Append to existing keyword.
                    std::vector<wchar_t> &val = idx->second;
                    for (std::vector<wchar_t>::iterator i = val.begin(), i_end = val.end(); ; ++i) {
                        if (i == i_end) {
                            // End-of-values reached. Append character.
                            val.push_back(chr);
                            break;
                        } else if (*i == chr) {
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
