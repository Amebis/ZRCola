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


ZRCola::DBSource::DBSource()
{
}


ZRCola::DBSource::~DBSource()
{
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
    wxCHECK(SUCCEEDED(v.ChangeType(VT_BSTR)), false);

    val.reserve(::SysStringLen(V_BSTR(&v)));
    val = V_BSTR(&v);

    return true;
}


bool ZRCola::DBSource::GetUnicodeCharacter(const ATL::CComPtr<ADOField>& f, wchar_t& chr) const
{
    wxASSERT_MSG(f, wxT("field is empty"));

    ATL::CComVariant v;
    wxVERIFY(SUCCEEDED(f->get_Value(&v)));
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


bool ZRCola::DBSource::GetKeyCode(const ATL::CComPtr<ADOField>& f, ZRCola::DBSource::keyseq::keycode& kc) const
{
    wxASSERT_MSG(f, wxT("field is empty"));

    ATL::CComVariant v;
    wxVERIFY(SUCCEEDED(f->get_Value(&v)));
    wxCHECK(SUCCEEDED(v.ChangeType(VT_BSTR)), false);

    // Convert to uppercase.
    _wcsupr_l(V_BSTR(&v), m_locale);

    // Parse the field.
    memset(&kc, 0, sizeof(kc));
    for (UINT i = 0, n = ::SysStringLen(V_BSTR(&v)); i < n && V_BSTR(&v)[i];) {
        // Parse key code.
        if (i) {
            // Check for "+" separator.
            if (V_BSTR(&v)[i] != L'+') {
                ATL::CComBSTR fieldname; wxVERIFY(SUCCEEDED(f->get_Name(&fieldname)));
                _ftprintf(stderr, wxT("%s: error ZCC0070: Syntax error in \"%.*ls\" field (\"%.*ls\"). Key codes must be \"Ctrl+Alt+<key>\" formatted.\n"), m_filename.c_str(), fieldname.Length(), (BSTR)fieldname, n, V_BSTR(&v));
            }
            i++;
            if (i >= n || !V_BSTR(&v)[i]) {
                ATL::CComBSTR fieldname; wxVERIFY(SUCCEEDED(f->get_Name(&fieldname)));
                _ftprintf(stderr, wxT("%s: error ZCC0071: Syntax error in \"%.*ls\" field (\"%.*ls\"). Trailing separator \"+\" found.\n"), m_filename.c_str(), fieldname.Length(), (BSTR)fieldname, n, V_BSTR(&v));
            }
        }

        static const wchar_t str_shift[] = L"SHIFT", str_ctrl[] = L"CTRL", str_alt[] = L"ALT";
        if (i + _countof(str_shift) - 1 <= n && wmemcmp(V_BSTR(&v) + i, str_shift, _countof(str_shift) - 1) == 0) {
            kc.shift = true;
            i += _countof(str_shift) - 1;
        } else if (i + _countof(str_ctrl) - 1 <= n && wmemcmp(V_BSTR(&v) + i, str_ctrl, _countof(str_ctrl) - 1) == 0) {
            kc.ctrl = true;
            i += _countof(str_ctrl) - 1;
        } else if (i + _countof(str_alt) - 1 <= n && wmemcmp(V_BSTR(&v) + i, str_alt, _countof(str_alt) - 1) == 0) {
            kc.alt = true;
            i += _countof(str_alt) - 1;
        } else {
            kc.key = V_BSTR(&v)[i];
            i++;
        }
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
                lang[i] = (char)c;
            } else
                lang[i] = 0;
        } else
            break;
    }

    return true;
}



bool ZRCola::DBSource::SelectTranslations(ATL::CComPtr<ADORecordset> &rs) const
{
    // Create a new recordset.
    if (rs) rs.Release();
    wxCHECK(SUCCEEDED(::CoCreateInstance(CLSID_CADORecordset, NULL, CLSCTX_ALL, IID_IADORecordset, (LPVOID*)&rs)), false);

    // Open it.
    if (FAILED(rs->Open(ATL::CComVariant(L"SELECT [komb], [znak] FROM [VRS_ReplChar] WHERE [rang_komb]=1"), ATL::CComVariant(m_db), adOpenStatic, adLockReadOnly, adCmdText))) {
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

    return true;
}


bool ZRCola::DBSource::SelectKeySequences(ATL::CComPtr<ADORecordset> &rs) const
{
    // Create a new recordset.
    if (rs) rs.Release();
    wxCHECK(SUCCEEDED(::CoCreateInstance(CLSID_CADORecordset, NULL, CLSCTX_ALL, IID_IADORecordset, (LPVOID*)&rs)), false);

    // Open it.
    if (FAILED(rs->Open(ATL::CComVariant(L"SELECT DISTINCT [VRS_KeyCodes].[Znak], [VRS_CharGroup].[Name] AS [CharGroup], [VRS_KeyCodes].[KeyCode], [VRS_KeyCodes].[Shift] FROM [VRS_KeyCodes] LEFT JOIN [VRS_CharGroup] ON [VRS_CharGroup].[CharGroup]=[VRS_KeyCodes].[CharGroup]"), ATL::CComVariant(m_db), adOpenStatic, adLockReadOnly, adCmdText))) {
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

    keyseq::keycode kc1;
    {
        ATL::CComPtr<ADOField> f;
        wxVERIFY(SUCCEEDED(flds->get_Item(ATL::CComVariant(L"CharGroup"), &f)));
        wxCHECK(GetKeyCode(f, kc1), false);
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
    if (kc1.key) {
        // First key in the sequence is complete.
        ks.seq.push_back(kc1);
        keyseq::keycode kc2 = { keycode, shift };
        ks.seq.push_back(kc2);
    } else {
        // First key in the sequence is only modifier(s).
        kc1.key = keycode;
        if (shift) kc1.shift = true;
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
    if (FAILED(rs->Open(ATL::CComVariant(L"SELECT DISTINCT [entCode] FROM [VRS_Jezik]"), ATL::CComVariant(m_db), adOpenStatic, adLockReadOnly, adCmdText))) {
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

    return true;
}


bool ZRCola::DBSource::SelectLanguageCharacters(ATL::CComPtr<ADORecordset> &rs) const
{
    // Create a new recordset.
    if (rs) rs.Release();
    wxCHECK(SUCCEEDED(::CoCreateInstance(CLSID_CADORecordset, NULL, CLSCTX_ALL, IID_IADORecordset, (LPVOID*)&rs)), false);

    // Open it.
    if (FAILED(rs->Open(ATL::CComVariant(L"SELECT DISTINCT [znak], [lang] FROM [VRS_CharLocal]"), ATL::CComVariant(m_db), adOpenStatic, adLockReadOnly, adCmdText))) {
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
