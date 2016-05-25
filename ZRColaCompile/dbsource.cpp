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



bool ZRCola::DBSource::character_desc_idx::add_keywords(const wchar_t *str, wchar_t chr, size_t sub)
{
    wxASSERT_MSG(str, wxT("string is NULL"));

    while (*str) {
        // Skip white space.
        for (;;) {
            if (*str == 0)
                return true;
            else if (!iswspace(*str))
                break;
            else
                str++;
        }

        // Get term.
        std::wstring term;
        if (*str == L'"') {
            const wchar_t *str_end = ++str;
            for (;;) {
                if (*str_end == 0) {
                    term.assign(str, str_end);
                    break;
                } else if (*str_end == L'"') {
                    term.assign(str, str_end);
                    str_end++;
                    break;
                } else
                    str_end++;
            }
            str = str_end;
        } else {
            const wchar_t *str_end = str + 1;
            for (; *str_end && !iswspace(*str_end); str_end++);
            term.assign(str, str_end);
            str = str_end;
        }

        if (!term.empty()) {
            std::transform(term.begin(), term.end(), term.begin(), std::towlower);
            if (sub) {
                std::wstring::size_type j_end = term.size();
                if (j_end >= sub) {
                    // Insert all keyword substrings "sub" or more characters long.
                    for (std::wstring::size_type i = 0, i_end = j_end - sub; i <= i_end; ++i) {
                        for (std::wstring::size_type j = i + sub; j <= j_end; ++j)
                            add_keyword(term.substr(i, j - i), chr);
                    }
                }
            } else {
                // Insert exact keyword only.
                add_keyword(term, chr);
            }
        }
    }

    return true;
}


void ZRCola::DBSource::character_desc_idx::save(ZRCola::textindex<wchar_t, wchar_t, unsigned __int32> &idx) const
{
    idx       .clear();
    idx.keys  .clear();
    idx.values.clear();

    // Pre-allocate memory.
    std::vector<wchar_t>::size_type size_keys   = 0;
    std::vector<wchar_t>::size_type size_values = 0;
    for (const_iterator i = cbegin(), i_end = cend(); i != i_end; ++i) {
        size_keys   += i->first.size();
        size_values += i->second.size();
    }
    idx       .reserve(size()     );
    idx.keys  .reserve(size_keys  );
    idx.values.reserve(size_values);

    // Convert the index.
    for (const_iterator i = cbegin(), i_end = cend(); i != i_end; ++i) {
        ZRCola::mappair_t<unsigned __int32> p = { idx.keys.size(), idx.values.size() };
        idx.push_back(p);
        idx.keys.insert(idx.keys.end(), i->first.cbegin(), i->first.cend());
        idx.values.insert(idx.values.end(), i->second.cbegin(), i->second.cend());
    }
}


ZRCola::DBSource::DBSource()
{
}


ZRCola::DBSource::~DBSource()
{
    if (m_pCharacterGroup1)
        m_pCharacterGroup1.Release();

    if (m_comCharacterGroup)
        m_comCharacterGroup.Release();

    if (m_db)
        m_db->Close();

    if (m_locale)
        _free_locale(m_locale);
}


bool ZRCola::DBSource::Open(LPCTSTR filename)
{
    wxASSERT_MSG(!m_db, wxT("database already open"));

    // Create COM object.
    HRESULT hr = ::CoCreateInstance(CLSID_CADOConnection, NULL, CLSCTX_ALL, IID_IADOConnection, (LPVOID*)&m_db);
    if (SUCCEEDED(hr)) {
        // Open the database.
        std::wstring cn;
        cn  = L"Driver={Microsoft Access Driver (*.mdb)};";
        cn += L"Dbq=";
        cn += filename;
        cn += L";Uid=;Pwd=;";
        hr = m_db->Open(ATL::CComBSTR(cn.c_str()));
        if (SUCCEEDED(hr)) {
            // Database open and ready.
            m_filename = filename;
            m_locale = _create_locale(LC_ALL, "Slovenian_Slovenia.1250");

            wxASSERT_MSG(!m_comCharacterGroup, wxT("ADO command already created"));

            // Create ADO command(s).
            wxVERIFY(SUCCEEDED(::CoCreateInstance(CLSID_CADOCommand, NULL, CLSCTX_ALL, IID_IADOCommand, (LPVOID*)&m_comCharacterGroup)));
            wxVERIFY(SUCCEEDED(m_comCharacterGroup->put_ActiveConnection(ATL::CComVariant(m_db))));
            wxVERIFY(SUCCEEDED(m_comCharacterGroup->put_CommandType(adCmdText)));
            wxVERIFY(SUCCEEDED(m_comCharacterGroup->put_CommandText(ATL::CComBSTR(L"SELECT [Znak] FROM [VRS_SkupineZnakov] WHERE [Skupina]=? ORDER BY [Rang] ASC, [Znak] ASC"))));
            {
                // Create and add command parameters.
                ATL::CComPtr<ADOParameters> params;
                wxVERIFY(SUCCEEDED(m_comCharacterGroup->get_Parameters(&params)));
                wxASSERT_MSG(!m_pCharacterGroup1, wxT("ADO command parameter already created"));
                wxVERIFY(SUCCEEDED(m_comCharacterGroup->CreateParameter(ATL::CComBSTR(L"@Skupina"), adVarWChar, adParamInput, 50, ATL::CComVariant(DISP_E_PARAMNOTFOUND, VT_ERROR), &m_pCharacterGroup1)));
                wxVERIFY(SUCCEEDED(params->Append(m_pCharacterGroup1)));
            }

            return true;
        } else {
            _ftprintf(stderr, wxT("%s: error ZCC0011: Could not open database (0x%x).\n"), (LPCTSTR)filename, hr);
            LogErrors();
        }
        m_db.Release();
    } else
        _ftprintf(stderr, wxT("%s: error ZCC0012: Creating ADOConnection object failed (0x%x).\n"), (LPCTSTR)filename, hr);

    return false;
}


void ZRCola::DBSource::LogErrors() const
{
    wxASSERT_MSG(m_db, wxT("database does not exist"));

    // Get array of errors.
    ADOErrors *errors = NULL;
    if (SUCCEEDED(m_db->get_Errors(&errors))) {
        // Get number of errors.
        long n = 0;
        wxVERIFY(SUCCEEDED(errors->get_Count(&n)));

        // Iterate the errors.
        for (long i = 0; i < n; i++) {
            ADOError *err = NULL;
            if (SUCCEEDED(errors->get_Item(ATL::CComVariant(i), &err))) {
                // Write error number and description to the log.
                long num = 0;
                wxVERIFY(SUCCEEDED(err->get_Number(&num)));

                ATL::CComBSTR desc;
                wxVERIFY(SUCCEEDED(err->get_Description(&desc)));

                _ftprintf(stderr, wxT("    error ADO%x: %ls\n"), num, (BSTR)desc);

                err->Release();
            }
        }

        errors->Release();
    }
}


bool ZRCola::DBSource::GetValue(const ATL::CComPtr<ADOField>& f, bool& val) const
{
    wxASSERT_MSG(f, wxT("field is empty"));

    ATL::CComVariant v;
    wxVERIFY(SUCCEEDED(f->get_Value(&v)));
    wxCHECK(SUCCEEDED(v.ChangeType(VT_BOOL)), false);

    val = V_BOOL(&v) ? true : false;

    return true;
}


bool ZRCola::DBSource::GetValue(const ATL::CComPtr<ADOField>& f, int& val) const
{
    wxASSERT_MSG(f, wxT("field is empty"));

    ATL::CComVariant v;
    wxVERIFY(SUCCEEDED(f->get_Value(&v)));
    wxCHECK(SUCCEEDED(v.ChangeType(VT_I4)), false);

    val = V_I4(&v);

    return true;
}


bool ZRCola::DBSource::GetValue(const ATL::CComPtr<ADOField>& f, std::wstring& val) const
{
    wxASSERT_MSG(f, wxT("field is empty"));

    ATL::CComVariant v;
    wxVERIFY(SUCCEEDED(f->get_Value(&v)));
    if (V_VT(&v) != VT_NULL) {
        wxCHECK(SUCCEEDED(v.ChangeType(VT_BSTR)), false);

        val.reserve(::SysStringLen(V_BSTR(&v)));
        val = V_BSTR(&v);
    } else
        val.empty();

    return true;
}


bool ZRCola::DBSource::GetUnicodeCharacter(const ATL::CComPtr<ADOField>& f, wchar_t& chr) const
{
    wxASSERT_MSG(f, wxT("field is empty"));

    ATL::CComVariant v;
    wxVERIFY(SUCCEEDED(f->get_Value(&v)));
    if (V_VT(&v) != VT_NULL) {
        wxCHECK(SUCCEEDED(v.ChangeType(VT_BSTR)), false);

        // Parse the field. Must be exactly one Unicode code.
        UINT i = 0, n = ::SysStringLen(V_BSTR(&v));
        chr = 0;
        for (; i < n && V_BSTR(&v)[i]; i++) {
                 if (L'0' <= V_BSTR(&v)[i] && V_BSTR(&v)[i] <= L'9') chr = chr*0x10 + (V_BSTR(&v)[i] - L'0');
            else if (L'A' <= V_BSTR(&v)[i] && V_BSTR(&v)[i] <= L'F') chr = chr*0x10 + (V_BSTR(&v)[i] - L'A' + 10);
            else if (L'a' <= V_BSTR(&v)[i] && V_BSTR(&v)[i] <= L'f') chr = chr*0x10 + (V_BSTR(&v)[i] - L'a' + 10);
            else break;
        }
        if (i <= 0 && 4 < i) {
            ATL::CComBSTR fieldname; wxVERIFY(SUCCEEDED(f->get_Name(&fieldname)));
            _ftprintf(stderr, wxT("%s: error ZCC0030: Syntax error in \"%.*ls\" field (\"%.*ls\"). Unicode code must be one to four hexadecimal characters long.\n"), m_filename.c_str(), fieldname.Length(), (BSTR)fieldname, n, V_BSTR(&v));
            return false;
        } else if (i != n) {
            ATL::CComBSTR fieldname; wxVERIFY(SUCCEEDED(f->get_Name(&fieldname)));
            _ftprintf(stderr, wxT("%s: error ZCC0031: Syntax error in \"%.*ls\" field (\"%.*ls\"). Extra trailing characters.\n"), m_filename.c_str(), fieldname.Length(), (BSTR)fieldname, n, V_BSTR(&v));
            return false;
        }
    } else
        chr = 0;

    return true;
}


bool ZRCola::DBSource::GetUnicodeString(const ATL::CComPtr<ADOField>& f, std::wstring& str) const
{
    wxASSERT_MSG(f, wxT("field is empty"));

    ATL::CComVariant v;
    wxVERIFY(SUCCEEDED(f->get_Value(&v)));
    wxCHECK(SUCCEEDED(v.ChangeType(VT_BSTR)), false);

    // Parse the field. Must be "xxxx+xxxx+xxxx..." sequence.
    str.clear();
    for (UINT i = 0, n = ::SysStringLen(V_BSTR(&v)); i < n && V_BSTR(&v)[i];) {
        // Parse Unicode code.
        UINT j = 0;
        wchar_t c = 0;
        for (; i < n && V_BSTR(&v)[i]; i++, j++) {
                 if (L'0' <= V_BSTR(&v)[i] && V_BSTR(&v)[i] <= L'9') c = c*0x10 + (V_BSTR(&v)[i] - L'0');
            else if (L'A' <= V_BSTR(&v)[i] && V_BSTR(&v)[i] <= L'F') c = c*0x10 + (V_BSTR(&v)[i] - L'A' + 10);
            else if (L'a' <= V_BSTR(&v)[i] && V_BSTR(&v)[i] <= L'f') c = c*0x10 + (V_BSTR(&v)[i] - L'a' + 10);
            else break;
        }
        if (j <= 0 || 4 < j) {
            ATL::CComBSTR fieldname; wxVERIFY(SUCCEEDED(f->get_Name(&fieldname)));
            _ftprintf(stderr, wxT("%s: error ZCC0020: Syntax error in \"%.*ls\" field (\"%.*ls\"). Unicode code must be one to four hexadecimal characters long.\n"), m_filename.c_str(), fieldname.Length(), (BSTR)fieldname, n, V_BSTR(&v));
            return false;
        }
        str += c;

        // Skip delimiter(s) and whitespace.
        for (; i < n && V_BSTR(&v)[i] && (V_BSTR(&v)[i] == L'+' || _iswspace_l(V_BSTR(&v)[i], m_locale)); i++);
    }

    return true;
}


bool ZRCola::DBSource::GetLanguage(const ATL::CComPtr<ADOField>& f, ZRCola::langid_t& lang) const
{
    wxASSERT_MSG(f, wxT("field is empty"));

    ATL::CComVariant v;
    wxVERIFY(SUCCEEDED(f->get_Value(&v)));
    wxCHECK(SUCCEEDED(v.ChangeType(VT_BSTR)), false);

    // Convert to lowercase.
    _wcslwr_l(V_BSTR(&v), m_locale);

    // Parse the field.
    size_t n = wcsnlen(V_BSTR(&v), ::SysStringLen(V_BSTR(&v)));
    if (n != 3) {
            ATL::CComBSTR fieldname; wxVERIFY(SUCCEEDED(f->get_Name(&fieldname)));
            _ftprintf(stderr, wxT("%s: error ZCC0080: Syntax error in \"%.*ls\" field (\"%.*ls\"). Language ID must be exactly three (3) characters long.\n"), m_filename.c_str(), fieldname.Length(), (BSTR)fieldname, n, V_BSTR(&v));
            return false;
    }
    for (size_t i = 0;; i++) {
        if (i < sizeof(lang)) {
            if (i < n) {
                wchar_t c = V_BSTR(&v)[i];
                if ((unsigned short)c > 0x7f) {
                    ATL::CComBSTR fieldname; wxVERIFY(SUCCEEDED(f->get_Name(&fieldname)));
                    _ftprintf(stderr, wxT("%s: error ZCC0081: Syntax error in \"%.*ls\" field (\"%.*ls\"). Language ID must contain ASCII characters only.\n"), m_filename.c_str(), fieldname.Length(), (BSTR)fieldname, n, V_BSTR(&v));
                    return false;
                }
                lang.data[i] = (char)c;
            } else
                lang.data[i] = 0;
        } else
            break;
    }

    return true;
}


bool ZRCola::DBSource::GetChrCat(const ATL::CComPtr<ADOField>& f, chrcatid_t& cc) const
{
    wxASSERT_MSG(f, wxT("field is empty"));

    ATL::CComVariant v;
    wxVERIFY(SUCCEEDED(f->get_Value(&v)));
    if (V_VT(&v) != VT_NULL) {
        wxCHECK(SUCCEEDED(v.ChangeType(VT_BSTR)), false);

        // Parse the field.
        size_t n = wcsnlen(V_BSTR(&v), ::SysStringLen(V_BSTR(&v)));
        if (n < 1 || 2 < n) {
                ATL::CComBSTR fieldname; wxVERIFY(SUCCEEDED(f->get_Name(&fieldname)));
                _ftprintf(stderr, wxT("%s: error ZCC0110: Syntax error in \"%.*ls\" field (\"%.*ls\"). Character category ID must be one (1) or two (2) characters long.\n"), m_filename.c_str(), fieldname.Length(), (BSTR)fieldname, n, V_BSTR(&v));
                return false;
        }
        for (size_t i = 0;; i++) {
            if (i < sizeof(cc)) {
                if (i < n) {
                    wchar_t c = V_BSTR(&v)[i];
                    if ((unsigned short)c > 0x7f) {
                        ATL::CComBSTR fieldname; wxVERIFY(SUCCEEDED(f->get_Name(&fieldname)));
                        _ftprintf(stderr, wxT("%s: error ZCC0111: Syntax error in \"%.*ls\" field (\"%.*ls\"). Character category ID must contain ASCII characters only.\n"), m_filename.c_str(), fieldname.Length(), (BSTR)fieldname, n, V_BSTR(&v));
                        return false;
                    }
                    cc.data[i] = (char)c;
                } else
                    cc.data[i] = 0;
            } else
                break;
        }
    } else
        memset(cc.data, 0, sizeof(cc));

    return true;
}


bool ZRCola::DBSource::SelectTranslations(ATL::CComPtr<ADORecordset> &rs) const
{
    // Create a new recordset.
    if (rs) rs.Release();
    wxCHECK(SUCCEEDED(::CoCreateInstance(CLSID_CADORecordset, NULL, CLSCTX_ALL, IID_IADORecordset, (LPVOID*)&rs)), false);

    // Open it.
    if (FAILED(rs->Open(ATL::CComVariant(
        L"SELECT [komb], [znak], [rang_znak] "
        L"FROM [VRS_ReplChar] "
        L"WHERE [rang_komb]=1 "
        L"ORDER BY [znak], [rang_znak], [komb]"), ATL::CComVariant(m_db), adOpenStatic, adLockReadOnly, adCmdText)))
    {
        _ftprintf(stderr, wxT("%s: error ZCC0040: Error loading compositions from database. Please make sure the file is ZRCola.zrc compatible.\n"), m_filename.c_str());
        LogErrors();
        return false;
    }

    return true;
}


bool ZRCola::DBSource::GetTranslation(const ATL::CComPtr<ADORecordset>& rs, ZRCola::DBSource::translation& t) const
{
    wxASSERT_MSG(rs, wxT("recordset is empty"));

    ATL::CComPtr<ADOFields> flds;
    wxVERIFY(SUCCEEDED(rs->get_Fields(&flds)));

    {
        ATL::CComPtr<ADOField> f;
        wxVERIFY(SUCCEEDED(flds->get_Item(ATL::CComVariant(L"komb"), &f)));
        wxCHECK(GetUnicodeString(f, t.str), false);
    }

    {
        ATL::CComPtr<ADOField> f;
        wxVERIFY(SUCCEEDED(flds->get_Item(ATL::CComVariant(L"znak"), &f)));
        wxCHECK(GetUnicodeCharacter(f, t.chr), false);
    }

    {
        ATL::CComPtr<ADOField> f;
        wxVERIFY(SUCCEEDED(flds->get_Item(ATL::CComVariant(L"rang_znak"), &f)));
        wxCHECK(GetValue(f, t.rank), false);
    }

    return true;
}


bool ZRCola::DBSource::SelectKeySequences(ATL::CComPtr<ADORecordset> &rs) const
{
    // Create a new recordset.
    if (rs) rs.Release();
    wxCHECK(SUCCEEDED(::CoCreateInstance(CLSID_CADORecordset, NULL, CLSCTX_ALL, IID_IADORecordset, (LPVOID*)&rs)), false);

    // Open it.
    if (FAILED(rs->Open(ATL::CComVariant(
        L"SELECT DISTINCT [VRS_KeyCodes].[Znak], [VRS_CharGroup].[CharGroup], IIF([VRS_CharGroup].[Arg1] IS NOT NULL, [VRS_CharGroup].[Arg1], 0)+IIF([VRS_CharGroup].[Arg2] IS NOT NULL, [VRS_CharGroup].[Arg2], 0)+IIF([VRS_CharGroup].[Arg3] IS NOT NULL, [VRS_CharGroup].[Arg3], 0) AS [Modifiers], IIF([VRS_CharGroup].[Arg4] IS NOT NULL, [VRS_CharGroup].[Arg4], 0) AS [KeyCodePre], [VRS_KeyCodes].[KeyCode], [VRS_KeyCodes].[Shift] "
        L"FROM [VRS_KeyCodes] LEFT JOIN [VRS_CharGroup] ON [VRS_CharGroup].[CharGroup]=[VRS_KeyCodes].[CharGroup] "
        L"ORDER BY [VRS_CharGroup].[CharGroup], [VRS_KeyCodes].[KeyCode], [VRS_KeyCodes].[Shift], [VRS_KeyCodes].[Znak]"), ATL::CComVariant(m_db), adOpenStatic, adLockReadOnly, adCmdText)))
    {
        _ftprintf(stderr, wxT("%s: error ZCC0050: Error loading key sequences from database. Please make sure the file is ZRCola.zrc compatible.\n"), m_filename.c_str());
        LogErrors();
        return false;
    }

    return true;
}


bool ZRCola::DBSource::GetKeySequence(const ATL::CComPtr<ADORecordset>& rs, ZRCola::DBSource::keyseq& ks) const
{
    wxASSERT_MSG(rs, wxT("recordset is empty"));

    ATL::CComPtr<ADOFields> flds;
    wxVERIFY(SUCCEEDED(rs->get_Fields(&flds)));

    {
        ATL::CComPtr<ADOField> f;
        wxVERIFY(SUCCEEDED(flds->get_Item(ATL::CComVariant(L"Znak"), &f)));
        wxCHECK(GetUnicodeCharacter(f, ks.chr), false);
    }

    int modifiers;
    {
        ATL::CComPtr<ADOField> f;
        wxVERIFY(SUCCEEDED(flds->get_Item(ATL::CComVariant(L"Modifiers"), &f)));
        wxCHECK(GetValue(f, modifiers), false);
    }

    int keycode1;
    {
        ATL::CComPtr<ADOField> f;
        wxVERIFY(SUCCEEDED(flds->get_Item(ATL::CComVariant(L"KeyCodePre"), &f)));
        wxCHECK(GetValue(f, keycode1), false);
    }

    int keycode;
    {
        ATL::CComPtr<ADOField> f;
        wxVERIFY(SUCCEEDED(flds->get_Item(ATL::CComVariant(L"KeyCode"), &f)));
        wxCHECK(GetValue(f, keycode), false);
    }

    bool shift;
    {
        ATL::CComPtr<ADOField> f;
        wxVERIFY(SUCCEEDED(flds->get_Item(ATL::CComVariant(L"Shift"), &f)));
        wxCHECK(GetValue(f, shift), false);
    }

    ks.seq.clear();
    if (keycode1) {
        // First key in the sequence is complete.
        keyseq::keycode kc1 = {
            keyseq::keycode::translate_slen(keycode1),
            (modifiers & 0x100) != 0,
            (modifiers & 0x200) != 0,
            (modifiers & 0x400) != 0 };
        ks.seq.push_back(kc1);

        keyseq::keycode kc2 = { keyseq::keycode::translate_slen(keycode), shift };
        ks.seq.push_back(kc2);
    } else {
        // First key in the sequence is only modifier(s).
        keyseq::keycode kc1 = {
            keyseq::keycode::translate_slen(keycode),
            shift || (modifiers & 0x100) != 0,
                     (modifiers & 0x200) != 0,
                     (modifiers & 0x400) != 0 };
        ks.seq.push_back(kc1);
    }

    return true;
}


bool ZRCola::DBSource::SelectLanguages(ATL::CComPtr<ADORecordset> &rs) const
{
    // Create a new recordset.
    if (rs) rs.Release();
    wxCHECK(SUCCEEDED(::CoCreateInstance(CLSID_CADORecordset, NULL, CLSCTX_ALL, IID_IADORecordset, (LPVOID*)&rs)), false);

    // Open it.
    if (FAILED(rs->Open(ATL::CComVariant(
        L"SELECT DISTINCT [entCode], [Jezik_En] "
        L"FROM [VRS_Jezik] "
        L"ORDER BY [entCode], [Jezik_En]"), ATL::CComVariant(m_db), adOpenStatic, adLockReadOnly, adCmdText)))
    {
        _ftprintf(stderr, wxT("%s: error ZCC0060: Error loading languages from database. Please make sure the file is ZRCola.zrc compatible.\n"), m_filename.c_str());
        LogErrors();
        return false;
    }

    return true;
}


bool ZRCola::DBSource::GetLanguage(const ATL::CComPtr<ADORecordset>& rs, ZRCola::DBSource::language& lang) const
{
    wxASSERT_MSG(rs, wxT("recordset is empty"));

    ATL::CComPtr<ADOFields> flds;
    wxVERIFY(SUCCEEDED(rs->get_Fields(&flds)));

    {
        ATL::CComPtr<ADOField> f;
        wxVERIFY(SUCCEEDED(flds->get_Item(ATL::CComVariant(L"entCode"), &f)));
        wxCHECK(GetLanguage(f, lang.id), false);
    }

    {
        ATL::CComPtr<ADOField> f;
        wxVERIFY(SUCCEEDED(flds->get_Item(ATL::CComVariant(L"Jezik_En"), &f)));
        wxCHECK(GetValue(f, lang.name), false);
    }

    return true;
}


bool ZRCola::DBSource::SelectLanguageCharacters(ATL::CComPtr<ADORecordset> &rs) const
{
    // Create a new recordset.
    if (rs) rs.Release();
    wxCHECK(SUCCEEDED(::CoCreateInstance(CLSID_CADORecordset, NULL, CLSCTX_ALL, IID_IADORecordset, (LPVOID*)&rs)), false);

    // Open it.
    if (FAILED(rs->Open(ATL::CComVariant(
        L"SELECT DISTINCT [znak], [lang] "
        L"FROM [VRS_CharLocal] "
        L"ORDER BY [znak], [lang]"), ATL::CComVariant(m_db), adOpenStatic, adLockReadOnly, adCmdText)))
    {
        _ftprintf(stderr, wxT("%s: error ZCC0090: Error loading language characters from database. Please make sure the file is ZRCola.zrc compatible.\n"), m_filename.c_str());
        LogErrors();
        return false;
    }

    return true;
}


bool ZRCola::DBSource::GetLanguageCharacter(const ATL::CComPtr<ADORecordset>& rs, ZRCola::DBSource::langchar& lc) const
{
    wxASSERT_MSG(rs, wxT("recordset is empty"));

    ATL::CComPtr<ADOFields> flds;
    wxVERIFY(SUCCEEDED(rs->get_Fields(&flds)));

    {
        ATL::CComPtr<ADOField> f;
        wxVERIFY(SUCCEEDED(flds->get_Item(ATL::CComVariant(L"znak"), &f)));
        wxCHECK(GetUnicodeCharacter(f, lc.chr), false);
    }

    {
        ATL::CComPtr<ADOField> f;
        wxVERIFY(SUCCEEDED(flds->get_Item(ATL::CComVariant(L"lang"), &f)));
        wxCHECK(GetLanguage(f, lc.lang), false);
    }

    return true;
}


bool ZRCola::DBSource::SelectCharacterGroups(ATL::CComPtr<ADORecordset>& rs) const
{
    // Create a new recordset.
    if (rs) rs.Release();
    wxCHECK(SUCCEEDED(::CoCreateInstance(CLSID_CADORecordset, NULL, CLSCTX_ALL, IID_IADORecordset, (LPVOID*)&rs)), false);

    // Open it.
    if (FAILED(rs->Open(ATL::CComVariant(
        L"SELECT DISTINCT [id], [Skupina], [opis_en], [Rang] "
        L"FROM [VRS_SkupinaZnakov] "
        L"ORDER BY [Rang], [opis_en]"), ATL::CComVariant(m_db), adOpenStatic, adLockReadOnly, adCmdText)))
    {
        _ftprintf(stderr, wxT("%s: error ZCC0090: Error loading character groups from database. Please make sure the file is ZRCola.zrc compatible.\n"), m_filename.c_str());
        LogErrors();
        return false;
    }

    return true;
}


bool ZRCola::DBSource::GetCharacterGroup(const ATL::CComPtr<ADORecordset>& rs, chrgrp& cg) const
{
    wxASSERT_MSG(rs, wxT("recordset is empty"));

    ATL::CComPtr<ADOFields> flds;
    wxVERIFY(SUCCEEDED(rs->get_Fields(&flds)));
    std::wstring id;

    {
        ATL::CComPtr<ADOField> f;
        wxVERIFY(SUCCEEDED(flds->get_Item(ATL::CComVariant(L"id"), &f)));
        wxCHECK(GetValue(f, cg.id), false);
    }

    {
        ATL::CComPtr<ADOField> f;
        wxVERIFY(SUCCEEDED(flds->get_Item(ATL::CComVariant(L"Skupina"), &f)));
        wxCHECK(GetValue(f, id), false);
    }

    {
        ATL::CComPtr<ADOField> f;
        wxVERIFY(SUCCEEDED(flds->get_Item(ATL::CComVariant(L"Rang"), &f)));
        wxCHECK(GetValue(f, cg.rank), false);
    }

    {
        ATL::CComPtr<ADOField> f;
        wxVERIFY(SUCCEEDED(flds->get_Item(ATL::CComVariant(L"opis_en"), &f)));
        wxCHECK(GetValue(f, cg.name), false);
    }

    // Read character list from database.
    wxVERIFY(SUCCEEDED(m_pCharacterGroup1->put_Value(ATL::CComVariant(id.c_str()))));
    ATL::CComPtr<ADORecordset> rs_chars;
    wxVERIFY(SUCCEEDED(::CoCreateInstance(CLSID_CADORecordset, NULL, CLSCTX_ALL, IID_IADORecordset, (LPVOID*)&rs_chars)));
    wxVERIFY(SUCCEEDED(rs_chars->put_CursorLocation(adUseClient)));
    wxVERIFY(SUCCEEDED(rs_chars->put_CursorType(adOpenForwardOnly)));
    wxVERIFY(SUCCEEDED(rs_chars->put_LockType(adLockReadOnly)));
    if (FAILED(rs_chars->Open(ATL::CComVariant(m_comCharacterGroup), ATL::CComVariant(DISP_E_PARAMNOTFOUND, VT_ERROR)))) {
        _ftprintf(stderr, wxT("%s: error ZCC0100: Error loading character group characters from database. Please make sure the file is ZRCola.zrc compatible.\n"), m_filename.c_str());
        LogErrors();
        return false;
    }

    {
        cg.chars.clear();
        ATL::CComPtr<ADOFields> flds;
        wxVERIFY(SUCCEEDED(rs_chars->get_Fields(&flds)));
        ATL::CComPtr<ADOField> f;
        wxVERIFY(SUCCEEDED(flds->get_Item(ATL::CComVariant(L"Znak"), &f)));
        for (VARIANT_BOOL eof = VARIANT_TRUE; SUCCEEDED(rs_chars->get_EOF(&eof)) && !eof; rs_chars->MoveNext()) {
            wchar_t c;
            wxCHECK(GetUnicodeCharacter(f, c), false);
            cg.chars += c;
        }
    }

    return true;
}


bool ZRCola::DBSource::SelectCharacters(ATL::CComPtr<ADORecordset>& rs) const
{
    // Create a new recordset.
    if (rs) rs.Release();
    wxCHECK(SUCCEEDED(::CoCreateInstance(CLSID_CADORecordset, NULL, CLSCTX_ALL, IID_IADORecordset, (LPVOID*)&rs)), false);

    // Open it.
    if (FAILED(rs->Open(ATL::CComVariant(
        L"SELECT DISTINCT [znak], [opis_en], [klj_bes_en], [kat], [znak_v], [znak_m] "
        L"FROM [VRS_CharList] "
        L"WHERE "
        L"[aktiven]=1 AND " // Active characters only
        L"[kat]<>'g' "      // Ignore "Other, Control" category!
        L"ORDER BY [znak]"), ATL::CComVariant(m_db), adOpenStatic, adLockReadOnly, adCmdText)))
    {
        _ftprintf(stderr, wxT("%s: error ZCC0120: Error loading characters from database. Please make sure the file is ZRCola.zrc compatible.\n"), m_filename.c_str());
        LogErrors();
        return false;
    }

    return true;
}


bool ZRCola::DBSource::GetCharacter(const ATL::CComPtr<ADORecordset>& rs, character& chr) const
{
    wxASSERT_MSG(rs, wxT("recordset is empty"));

    ATL::CComPtr<ADOFields> flds;
    wxVERIFY(SUCCEEDED(rs->get_Fields(&flds)));
    wchar_t c;
    chr.rel.clear();

    {
        ATL::CComPtr<ADOField> f;
        wxVERIFY(SUCCEEDED(flds->get_Item(ATL::CComVariant(L"znak"), &f)));
        wxCHECK(GetUnicodeCharacter(f, chr.chr), false);
    }

    {
        ATL::CComPtr<ADOField> f;
        wxVERIFY(SUCCEEDED(flds->get_Item(ATL::CComVariant(L"znak_v"), &f)));
        wxCHECK(GetUnicodeCharacter(f, c), false);
        if (c && c != chr.chr)
            chr.rel += c;
    }

    {
        ATL::CComPtr<ADOField> f;
        wxVERIFY(SUCCEEDED(flds->get_Item(ATL::CComVariant(L"znak_m"), &f)));
        wxCHECK(GetUnicodeCharacter(f, c), false);
        if (c && c != chr.chr)
            chr.rel += c;
    }

    {
        ATL::CComPtr<ADOField> f;
        wxVERIFY(SUCCEEDED(flds->get_Item(ATL::CComVariant(L"opis_en"), &f)));
        wxCHECK(GetValue(f, chr.desc), false);
    }

    {
        ATL::CComPtr<ADOField> f;
        wxVERIFY(SUCCEEDED(flds->get_Item(ATL::CComVariant(L"klj_bes_en"), &f)));
        wxCHECK(GetValue(f, chr.keywords), false);
    }

    {
        ATL::CComPtr<ADOField> f;
        wxVERIFY(SUCCEEDED(flds->get_Item(ATL::CComVariant(L"kat"), &f)));
        wxCHECK(GetChrCat(f, chr.cat), false);
    }

    return true;
}


bool ZRCola::DBSource::SelectCharacterCategories(ATL::CComPtr<ADORecordset>& rs) const
{
    // Create a new recordset.
    if (rs) rs.Release();
    wxCHECK(SUCCEEDED(::CoCreateInstance(CLSID_CADORecordset, NULL, CLSCTX_ALL, IID_IADORecordset, (LPVOID*)&rs)), false);

    // Open it.
    if (FAILED(rs->Open(ATL::CComVariant(
        L"SELECT DISTINCT [kat], [opis_en], [Rang] "
        L"FROM [VRS_CharCategory] "
        L"WHERE [kat]<>'g' "    // Ignore "Other, Control" category!
        L"ORDER BY [Rang], [opis_en]"), ATL::CComVariant(m_db), adOpenStatic, adLockReadOnly, adCmdText)))
    {
        _ftprintf(stderr, wxT("%s: error ZCC0130: Error loading character categories from database. Please make sure the file is ZRCola.zrc compatible.\n"), m_filename.c_str());
        LogErrors();
        return false;
    }

    return true;
}


bool ZRCola::DBSource::GetCharacterCategory(const ATL::CComPtr<ADORecordset>& rs, chrcat& cc) const
{
    wxASSERT_MSG(rs, wxT("recordset is empty"));

    ATL::CComPtr<ADOFields> flds;
    wxVERIFY(SUCCEEDED(rs->get_Fields(&flds)));
    std::wstring id;

    {
        ATL::CComPtr<ADOField> f;
        wxVERIFY(SUCCEEDED(flds->get_Item(ATL::CComVariant(L"kat"), &f)));
        wxCHECK(GetChrCat(f, cc.id), false);
    }

    {
        ATL::CComPtr<ADOField> f;
        wxVERIFY(SUCCEEDED(flds->get_Item(ATL::CComVariant(L"Rang"), &f)));
        wxCHECK(GetValue(f, cc.rank), false);
    }

    {
        ATL::CComPtr<ADOField> f;
        wxVERIFY(SUCCEEDED(flds->get_Item(ATL::CComVariant(L"opis_en"), &f)));
        wxCHECK(GetValue(f, cc.name), false);
    }

    return true;
}
