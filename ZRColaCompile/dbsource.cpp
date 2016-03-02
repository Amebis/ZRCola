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


bool ZRCola::DBSource::GetUnicodeCharacter(const ATL::CComPtr<ADOField>& f, wchar_t& chr) const
{
    wxASSERT_MSG(f, wxT("field is empty"));

    CComVariant v;
    wxVERIFY(SUCCEEDED(f->get_Value(&v)));

    // Parse the field. Must be exactly one Unicode code.
    wxVERIFY(SUCCEEDED(v.ChangeType(VT_BSTR)));
    UINT i = 0, n = ::SysStringLen(V_BSTR(&v));
    chr = 0;
    for (; i < n && V_BSTR(&v)[i]; i++) {
             if (L'0' <= V_BSTR(&v)[i] && V_BSTR(&v)[i] <= L'9') chr = chr*0x10 + (V_BSTR(&v)[i] - L'0');
        else if (L'A' <= V_BSTR(&v)[i] && V_BSTR(&v)[i] <= L'F') chr = chr*0x10 + (V_BSTR(&v)[i] - L'A' + 10);
        else if (L'a' <= V_BSTR(&v)[i] && V_BSTR(&v)[i] <= L'f') chr = chr*0x10 + (V_BSTR(&v)[i] - L'a' + 10);
        else break;
    }
    if (i <= 0 && 4 < i) {
        CComBSTR fieldname; wxVERIFY(SUCCEEDED(f->get_Name(&fieldname)));
        _ftprintf(stderr, wxT("%s: error ZCC0030: Syntax error in \"%.*ls\" field (\"%.*ls\"). Unicode code must be one to four hexadecimal characters long.\n"), m_filename.c_str(), fieldname.Length(), (BSTR)fieldname, n, V_BSTR(&v));
        return false;
    } else if (i != n) {
        CComBSTR fieldname; wxVERIFY(SUCCEEDED(f->get_Name(&fieldname)));
        _ftprintf(stderr, wxT("%s: error ZCC0031: Syntax error in \"%.*ls\" field (\"%.*ls\"). Extra trailing characters.\n"), m_filename.c_str(), fieldname.Length(), (BSTR)fieldname, n, V_BSTR(&v));
        return false;
    }

    return true;
}


bool ZRCola::DBSource::GetUnicodeString(const ATL::CComPtr<ADOField>& f, std::wstring& str) const
{
    wxASSERT_MSG(f, wxT("field is empty"));

    CComVariant v;
    wxVERIFY(SUCCEEDED(f->get_Value(&v)));

    // Parse the field. Must be "xxxx+xxxx+xxxx..." sequence.
    wxVERIFY(SUCCEEDED(v.ChangeType(VT_BSTR)));
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
            CComBSTR fieldname; wxVERIFY(SUCCEEDED(f->get_Name(&fieldname)));
            _ftprintf(stderr, wxT("%s: error ZCC0020: Syntax error in \"%.*ls\" field (\"%.*ls\"). Unicode code must be one to four hexadecimal characters long.\n"), m_filename.c_str(), fieldname.Length(), (BSTR)fieldname, n, V_BSTR(&v));
            return false;
        }
        str += c;

        // Skip delimiter(s) and whitespace.
        for (; i < n && V_BSTR(&v)[i] && (V_BSTR(&v)[i] == L'+' || _iswspace_l(V_BSTR(&v)[i], m_locale)); i++);
    }

    return true;
}


bool ZRCola::DBSource::GetKeySequence(const ATL::CComPtr<ADOField>& f, std::vector<keyseq::keycode>& seq) const
{
    wxASSERT_MSG(f, wxT("field is empty"));

    CComVariant v;
    wxVERIFY(SUCCEEDED(f->get_Value(&v)));
    wxVERIFY(SUCCEEDED(v.ChangeType(VT_BSTR)));

    // Convert to uppercase.
    _wcsupr_l(V_BSTR(&v), m_locale);

    // Parse the field. Must be comma delimited sequence of key codes.
    seq.clear();
    for (UINT i = 0, n = ::SysStringLen(V_BSTR(&v)); i < n && V_BSTR(&v)[i];) {
        keyseq::keycode kc = {};

        while (i < n && V_BSTR(&v)[i]) {
            // Parse key code.
            static const wchar_t str_shift[] = L"SHIFT+", str_ctrl[] = L"CTRL+", str_alt[] = L"ALT+";
            if (i + _countof(str_shift) <= n && wmemcmp(V_BSTR(&v) + i, str_shift, _countof(str_shift) - 1) == 0) {
                kc.shift = true;
                i += _countof(str_shift) - 1;
            } else if (i + _countof(str_ctrl) <= n && wmemcmp(V_BSTR(&v) + i, str_ctrl, _countof(str_ctrl) - 1) == 0) {
                kc.ctrl = true;
                i += _countof(str_ctrl) - 1;
            } else if (i + _countof(str_alt) <= n && wmemcmp(V_BSTR(&v) + i, str_alt, _countof(str_alt) - 1) == 0) {
                kc.alt = true;
                i += _countof(str_alt) - 1;
            } else {
                kc.key = V_BSTR(&v)[i];
                i++;
                break;
            }
        }
        if (i < n && V_BSTR(&v)[i] && V_BSTR(&v)[i] != L',' && !_iswspace_l(V_BSTR(&v)[i], m_locale)) {
            CComBSTR fieldname; wxVERIFY(SUCCEEDED(f->get_Name(&fieldname)));
            _ftprintf(stderr, wxT("%s: error ZCC0060: Syntax error in \"%.*ls\" field (\"%.*ls\"). Key sequences must be \"Ctrl+Alt+<key>\" formatted, delimited by commas and/or space.\n"), m_filename.c_str(), fieldname.Length(), (BSTR)fieldname, n, V_BSTR(&v));
            return false;
        }
        if (seq.size() > 0xffff) {
            _ftprintf(stderr, wxT("%s: warning ZCC0061: Key sequence \"%.*ls...\" too long. Ignored.\n"), (LPCTSTR)m_filename.c_str(), std::min<UINT>(n, 20), V_BSTR(&v));
            return false;
        }
        seq.push_back(kc);

        // Skip delimiter(s) and whitespace.
        for (; i < n && V_BSTR(&v)[i] && (V_BSTR(&v)[i] == L',' || _iswspace_l(V_BSTR(&v)[i], m_locale)); i++);
    }

    if (seq.empty()) {
        _ftprintf(stderr, wxT("%s: warning ZCC0062: Empty key sequence. Ignored.\n"), (LPCTSTR)m_filename.c_str());
        return false;
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
        wxVERIFY(SUCCEEDED(flds->get_Item(CComVariant(L"komb"), &f)));
        wxCHECK(GetUnicodeString(f, t.str), false);
    }

    {
        ATL::CComPtr<ADOField> f;
        wxVERIFY(SUCCEEDED(flds->get_Item(CComVariant(L"znak"), &f)));
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
    if (FAILED(rs->Open(ATL::CComVariant(L"SELECT DISTINCT [Znak], [tipka] FROM [wrd_KeyCodes]"), ATL::CComVariant(m_db), adOpenStatic, adLockReadOnly, adCmdText))) {
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
        wxVERIFY(SUCCEEDED(flds->get_Item(CComVariant(L"Znak"), &f)));
        wxCHECK(GetUnicodeCharacter(f, ks.chr), false);
    }

    {
        ATL::CComPtr<ADOField> f;
        wxVERIFY(SUCCEEDED(flds->get_Item(CComVariant(L"tipka"), &f)));
        wxCHECK(GetKeySequence(f, ks.seq), false);
    }

    return true;
}
