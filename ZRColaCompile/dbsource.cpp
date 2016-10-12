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
using namespace winstd;


//////////////////////////////////////////////////////////////////////////
// ZRCola::DBSource::character_bank
//////////////////////////////////////////////////////////////////////////

ZRCola::DBSource::character_bank::character_bank() : vector<unique_ptr<ZRCola::DBSource::character> >()
{
    resize(0x10000);
}


void ZRCola::DBSource::character_bank::build_related()
{
    // Initialize ignore list.
    m_ignore.insert(L"letter");
    m_ignore.insert(L"modifier");
    m_ignore.insert(L"symbol");
    m_ignore.insert(L"accent");
    m_ignore.insert(L"with");
    m_ignore.insert(L"and");
    m_ignore.insert(L"capital");
    m_ignore.insert(L"small");
    m_ignore.insert(L"combining");

    SYSTEM_INFO si;
    GetSystemInfo(&si);

    // Launch workers.
    build_related_worker **workers = new build_related_worker*[si.dwNumberOfProcessors];
    size_type from = 0, to;
    for (DWORD i = 0; i < si.dwNumberOfProcessors; i++) {
        to = MulDiv(i + 1, 0x10000, si.dwNumberOfProcessors);
        workers[i] = new build_related_worker(this, from, to);
        from = to;
    }

    // Wait for workers.
    for (DWORD i = 0; i < si.dwNumberOfProcessors; i++) {
        if (workers[i]) {
            workers[i]->join();
            delete workers[i];
        }
    }

    delete workers; // This line of code sounds horrible, I know.
}


ZRCola::DBSource::character_bank::build_related_worker::build_related_worker(_In_ const character_bank *cb, _In_ size_type from, _In_ size_type to) :
    win_handle((HANDLE)_beginthreadex(NULL, 0, process, this, CREATE_SUSPENDED, NULL)),
    m_heap(HeapCreate(0, 0, 0)),
    m_cb(cb),
    m_from(from),
    m_to(to)
{
    // Now that members of this class are surely initialized, proceed.
    ResumeThread(m_h);
}


unsigned int ZRCola::DBSource::character_bank::build_related_worker::process()
{
    heap_allocator<wchar_t> al(m_heap);
    basic_string<wchar_t, char_traits<wchar_t>, heap_allocator<wchar_t> > rel(al);
    set<wstring, less<wstring>, heap_allocator<wstring> > matching(less<wstring>(), al);

    for (size_type i = m_from; i < m_to; i++) {
        auto &chr = *(m_cb->at(i).get());
        if (&chr == NULL) continue;

        rel.clear();

        // Remove all unexisting, inactive, or self related characters.
        for (auto j = chr.rel.length(); j--;) {
            wchar_t c = chr.rel[j];
            if (m_cb->at(c) && (wchar_t)j != c)
                rel += c;
        }

        // Add all characters that share enought keywords.
        for (size_type j = 0, j_end = m_cb->size(); j < j_end; j++) {
            if (i == j || rel.find((wchar_t)j) != wstring::npos)
                continue;
            const auto &chr2 = *(m_cb->at(j).get());
            if (&chr2 == NULL)
                continue;

            set<wstring>::size_type comparisons = 0;
            matching.clear();
            for (auto term = chr.terms.cbegin(), term_end = chr.terms.cend(); term != term_end; ++term) {
                // Test for ignored word(s).
                if (m_cb->m_ignore.find(*term) != m_cb->m_ignore.cend())
                    continue;
                for (auto term2 = chr2.terms.cbegin(), term2_end = chr2.terms.cend(); term2 != term2_end; ++term2) {
                    // Test for ignored word(s).
                    if (m_cb->m_ignore.find(*term2) != m_cb->m_ignore.cend())
                        continue;
                    comparisons++;
                    if (*term == *term2)
                        matching.insert(*term);
                }
            }

            if (comparisons) {
                // If 1/2 terms match, assume related.
                auto hits = matching.size();
                if (hits*hits*2 >= comparisons)
                    rel += chr2.chr;
            }
        }

        chr.rel.assign(rel.c_str(), rel.length());
    }

    return 0;
}


unsigned int __stdcall ZRCola::DBSource::character_bank::build_related_worker::process(_In_ void *param)
{
    return ((ZRCola::DBSource::character_bank::build_related_worker*)param)->process();
}


//////////////////////////////////////////////////////////////////////////
// ZRCola::DBSource::character_desc_idx
//////////////////////////////////////////////////////////////////////////

void ZRCola::DBSource::character_desc_idx::parse_keywords(const wchar_t *str, set<wstring> &terms)
{
    wxASSERT_MSG(str, wxT("string is NULL"));

    while (*str) {
        // Skip white space.
        for (;;) {
            if (*str == 0)
                return;
            else if (!iswspace(*str))
                break;
            else
                str++;
        }

        // Get term.
        wstring term;
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
            transform(term.begin(), term.end(), term.begin(), towlower);
            terms.insert(term);
        }
    }
}


void ZRCola::DBSource::character_desc_idx::add_keywords(const set<wstring> &terms, wchar_t chr, size_t sub)
{
    for (auto term = terms.cbegin(), term_end = terms.cend(); term != term_end; ++term) {
        if (sub) {
            wstring::size_type j_end = term->size();
            if (j_end >= sub) {
                // Insert all keyword substrings "sub" or more characters long.
                for (wstring::size_type i = 0, i_end = j_end - sub; i <= i_end; ++i) {
                    for (wstring::size_type j = i + sub; j <= j_end; ++j)
                        add_keyword(term->substr(i, j - i), chr);
                }
            }
        } else {
            // Insert exact keyword only.
            add_keyword(*term, chr);
        }
    }
}


void ZRCola::DBSource::character_desc_idx::save(ZRCola::textindex<wchar_t, wchar_t, unsigned __int32> &idx) const
{
    idx       .clear();
    idx.keys  .clear();
    idx.values.clear();

    // Pre-allocate memory.
    vector<wchar_t>::size_type size_keys   = 0;
    vector<wchar_t>::size_type size_values = 0;
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


//////////////////////////////////////////////////////////////////////////
// ZRCola::DBSource
//////////////////////////////////////////////////////////////////////////

ZRCola::DBSource::DBSource()
{
}


ZRCola::DBSource::~DBSource()
{
    // Manually release all COM objects related to the database before we close the database.
    m_pCharacterGroup1.free();
    m_comCharacterGroup.free();

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
        wstring cn;
        cn  = L"Driver={Microsoft Access Driver (*.mdb)};";
        cn += L"Dbq=";
        cn += filename;
        cn += L";Uid=;Pwd=;";
        hr = m_db->Open(bstr(cn.c_str()));
        if (SUCCEEDED(hr)) {
            // Database open and ready.
            m_filename = filename;
            m_locale = _create_locale(LC_ALL, "Slovenian_Slovenia.1250");

            wxASSERT_MSG(!m_comCharacterGroup, wxT("ADO command already created"));

            // Create ADO command(s).
            wxVERIFY(SUCCEEDED(::CoCreateInstance(CLSID_CADOCommand, NULL, CLSCTX_ALL, IID_IADOCommand, (LPVOID*)&m_comCharacterGroup)));
            wxVERIFY(SUCCEEDED(m_comCharacterGroup->put_ActiveConnection(variant(m_db))));
            wxVERIFY(SUCCEEDED(m_comCharacterGroup->put_CommandType(adCmdText)));
            wxVERIFY(SUCCEEDED(m_comCharacterGroup->put_CommandText(bstr(L"SELECT [VRS_SkupineZnakov].[Znak], [VRS_SkupineZnakov].[pogost] FROM [VRS_SkupineZnakov] LEFT JOIN [VRS_CharList] ON [VRS_SkupineZnakov].[Znak]=[VRS_CharList].[znak] WHERE [VRS_CharList].[aktiven]=1 AND [VRS_SkupineZnakov].[Skupina]=? ORDER BY [VRS_SkupineZnakov].[Rang] ASC, [VRS_SkupineZnakov].[Znak] ASC"))));
            {
                // Create and add command parameters.
                com_obj<ADOParameters> params;
                wxVERIFY(SUCCEEDED(m_comCharacterGroup->get_Parameters(&params)));
                wxASSERT_MSG(!m_pCharacterGroup1, wxT("ADO command parameter already created"));
                wxVERIFY(SUCCEEDED(m_comCharacterGroup->CreateParameter(bstr(L"@Skupina"), adVarWChar, adParamInput, 50, variant(DISP_E_PARAMNOTFOUND, VT_ERROR), &m_pCharacterGroup1)));
                wxVERIFY(SUCCEEDED(params->Append(m_pCharacterGroup1)));
            }

            return true;
        } else {
            _ftprintf(stderr, wxT("%s: error ZCC0011: Could not open database (0x%x).\n"), (LPCTSTR)filename, hr);
            LogErrors();
        }
        m_db.free();
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
            if (SUCCEEDED(errors->get_Item(variant(i), &err))) {
                // Write error number and description to the log.
                long num = 0;
                wxVERIFY(SUCCEEDED(err->get_Number(&num)));

                bstr desc;
                wxVERIFY(SUCCEEDED(err->get_Description(&desc)));

                _ftprintf(stderr, wxT("    error ADO%x: %ls\n"), num, (BSTR)desc);

                err->Release();
            }
        }

        errors->Release();
    }
}


bool ZRCola::DBSource::GetValue(const com_obj<ADOField>& f, bool& val) const
{
    wxASSERT_MSG(f, wxT("field is empty"));

    variant v;
    wxVERIFY(SUCCEEDED(f->get_Value(&v)));
    wxCHECK(SUCCEEDED(v.change_type(VT_BOOL)), false);

    val = V_BOOL(&v) ? true : false;

    return true;
}


bool ZRCola::DBSource::GetValue(const com_obj<ADOField>& f, int& val) const
{
    wxASSERT_MSG(f, wxT("field is empty"));

    variant v;
    wxVERIFY(SUCCEEDED(f->get_Value(&v)));
    wxCHECK(SUCCEEDED(v.change_type(VT_I4)), false);

    val = V_I4(&v);

    return true;
}


bool ZRCola::DBSource::GetValue(const com_obj<ADOField>& f, wstring& val) const
{
    wxASSERT_MSG(f, wxT("field is empty"));

    variant v;
    wxVERIFY(SUCCEEDED(f->get_Value(&v)));
    if (V_VT(&v) != VT_NULL) {
        wxCHECK(SUCCEEDED(v.change_type(VT_BSTR)), false);

        val.reserve(::SysStringLen(V_BSTR(&v)));
        val = V_BSTR(&v);
    } else
        val.clear();

    return true;
}


bool ZRCola::DBSource::GetUnicodeCharacter(const com_obj<ADOField>& f, wchar_t& chr) const
{
    wxASSERT_MSG(f, wxT("field is empty"));

    variant v;
    wxVERIFY(SUCCEEDED(f->get_Value(&v)));
    if (V_VT(&v) != VT_NULL) {
        wxCHECK(SUCCEEDED(v.change_type(VT_BSTR)), false);

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
            bstr fieldname; wxVERIFY(SUCCEEDED(f->get_Name(&fieldname)));
            _ftprintf(stderr, wxT("%s: error ZCC0030: Syntax error in \"%.*ls\" field (\"%.*ls\"). Unicode code must be one to four hexadecimal characters long.\n"), m_filename.c_str(), fieldname.length(), (BSTR)fieldname, n, V_BSTR(&v));
            return false;
        } else if (i != n) {
            bstr fieldname; wxVERIFY(SUCCEEDED(f->get_Name(&fieldname)));
            _ftprintf(stderr, wxT("%s: error ZCC0031: Syntax error in \"%.*ls\" field (\"%.*ls\"). Extra trailing characters.\n"), m_filename.c_str(), fieldname.length(), (BSTR)fieldname, n, V_BSTR(&v));
            return false;
        }
    } else
        chr = 0;

    return true;
}


bool ZRCola::DBSource::GetUnicodeString(const com_obj<ADOField>& f, wstring& str) const
{
    wxASSERT_MSG(f, wxT("field is empty"));

    variant v;
    wxVERIFY(SUCCEEDED(f->get_Value(&v)));
    wxCHECK(SUCCEEDED(v.change_type(VT_BSTR)), false);

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
            bstr fieldname; wxVERIFY(SUCCEEDED(f->get_Name(&fieldname)));
            _ftprintf(stderr, wxT("%s: error ZCC0020: Syntax error in \"%.*ls\" field (\"%.*ls\"). Unicode code must be one to four hexadecimal characters long.\n"), m_filename.c_str(), fieldname.length(), (BSTR)fieldname, n, V_BSTR(&v));
            return false;
        }
        str += c;

        // Skip delimiter(s) and whitespace.
        for (; i < n && V_BSTR(&v)[i] && (V_BSTR(&v)[i] == L'+' || _iswspace_l(V_BSTR(&v)[i], m_locale)); i++);
    }

    return true;
}


bool ZRCola::DBSource::GetLanguage(const com_obj<ADOField>& f, ZRCola::langid_t& lang) const
{
    wxASSERT_MSG(f, wxT("field is empty"));

    variant v;
    wxVERIFY(SUCCEEDED(f->get_Value(&v)));
    wxCHECK(SUCCEEDED(v.change_type(VT_BSTR)), false);

    // Convert to lowercase.
    _wcslwr_l(V_BSTR(&v), m_locale);

    // Parse the field.
    size_t n = wcsnlen(V_BSTR(&v), ::SysStringLen(V_BSTR(&v)));
    if (n != 3) {
            bstr fieldname; wxVERIFY(SUCCEEDED(f->get_Name(&fieldname)));
            _ftprintf(stderr, wxT("%s: error ZCC0080: Syntax error in \"%.*ls\" field (\"%.*ls\"). Language ID must be exactly three (3) characters long.\n"), m_filename.c_str(), fieldname.length(), (BSTR)fieldname, n, V_BSTR(&v));
            return false;
    }
    for (size_t i = 0;; i++) {
        if (i < sizeof(lang)) {
            if (i < n) {
                wchar_t c = V_BSTR(&v)[i];
                if ((unsigned short)c > 0x7f) {
                    bstr fieldname; wxVERIFY(SUCCEEDED(f->get_Name(&fieldname)));
                    _ftprintf(stderr, wxT("%s: error ZCC0081: Syntax error in \"%.*ls\" field (\"%.*ls\"). Language ID must contain ASCII characters only.\n"), m_filename.c_str(), fieldname.length(), (BSTR)fieldname, n, V_BSTR(&v));
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


bool ZRCola::DBSource::GetChrCat(const com_obj<ADOField>& f, chrcatid_t& cc) const
{
    wxASSERT_MSG(f, wxT("field is empty"));

    variant v;
    wxVERIFY(SUCCEEDED(f->get_Value(&v)));
    if (V_VT(&v) != VT_NULL) {
        wxCHECK(SUCCEEDED(v.change_type(VT_BSTR)), false);

        // Parse the field.
        size_t n = wcsnlen(V_BSTR(&v), ::SysStringLen(V_BSTR(&v)));
        if (n < 1 || 2 < n) {
                bstr fieldname; wxVERIFY(SUCCEEDED(f->get_Name(&fieldname)));
                _ftprintf(stderr, wxT("%s: error ZCC0110: Syntax error in \"%.*ls\" field (\"%.*ls\"). Character category ID must be one (1) or two (2) characters long.\n"), m_filename.c_str(), fieldname.length(), (BSTR)fieldname, n, V_BSTR(&v));
                return false;
        }
        for (size_t i = 0;; i++) {
            if (i < sizeof(cc)) {
                if (i < n) {
                    wchar_t c = V_BSTR(&v)[i];
                    if ((unsigned short)c > 0x7f) {
                        bstr fieldname; wxVERIFY(SUCCEEDED(f->get_Name(&fieldname)));
                        _ftprintf(stderr, wxT("%s: error ZCC0111: Syntax error in \"%.*ls\" field (\"%.*ls\"). Character category ID must contain ASCII characters only.\n"), m_filename.c_str(), fieldname.length(), (BSTR)fieldname, n, V_BSTR(&v));
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


bool ZRCola::DBSource::SelectTranslations(com_obj<ADORecordset> &rs) const
{
    // Create a new recordset.
    rs.free();
    wxCHECK(SUCCEEDED(::CoCreateInstance(CLSID_CADORecordset, NULL, CLSCTX_ALL, IID_IADORecordset, (LPVOID*)&rs)), false);

    // Open it.
    if (FAILED(rs->Open(variant(
        L"SELECT [komb], [znak], [rang_znak] "
        L"FROM [VRS_ReplChar] "
        L"WHERE [rang_komb]=1 "
        L"ORDER BY [znak], [rang_znak], [komb]"), variant(m_db), adOpenStatic, adLockReadOnly, adCmdText)))
    {
        _ftprintf(stderr, wxT("%s: error ZCC0040: Error loading compositions from database. Please make sure the file is ZRCola.zrc compatible.\n"), m_filename.c_str());
        LogErrors();
        return false;
    }

    return true;
}


bool ZRCola::DBSource::GetTranslation(const com_obj<ADORecordset>& rs, ZRCola::DBSource::translation& t) const
{
    wxASSERT_MSG(rs, wxT("recordset is empty"));

    com_obj<ADOFields> flds;
    wxVERIFY(SUCCEEDED(rs->get_Fields(&flds)));

    {
        com_obj<ADOField> f;
        wxVERIFY(SUCCEEDED(flds->get_Item(variant(L"komb"), &f)));
        wxCHECK(GetUnicodeString(f, t.decomp.str), false);
    }

    {
        com_obj<ADOField> f;
        wxVERIFY(SUCCEEDED(flds->get_Item(variant(L"znak"), &f)));
        wxCHECK(GetUnicodeCharacter(f, t.chr), false);
    }

    {
        com_obj<ADOField> f;
        wxVERIFY(SUCCEEDED(flds->get_Item(variant(L"rang_znak"), &f)));
        wxCHECK(GetValue(f, t.decomp.rank), false);
    }

    return true;
}


bool ZRCola::DBSource::SelectKeySequences(com_obj<ADORecordset> &rs) const
{
    // Create a new recordset.
    rs.free();
    wxCHECK(SUCCEEDED(::CoCreateInstance(CLSID_CADORecordset, NULL, CLSCTX_ALL, IID_IADORecordset, (LPVOID*)&rs)), false);

    // Open it.
    if (FAILED(rs->Open(variant(
        L"SELECT DISTINCT [VRS_KeyCodes].[Znak], [VRS_CharGroup].[CharGroup], IIF([VRS_CharGroup].[Arg1] IS NOT NULL, [VRS_CharGroup].[Arg1], 0)+IIF([VRS_CharGroup].[Arg2] IS NOT NULL, [VRS_CharGroup].[Arg2], 0)+IIF([VRS_CharGroup].[Arg3] IS NOT NULL, [VRS_CharGroup].[Arg3], 0) AS [Modifiers], IIF([VRS_CharGroup].[Arg4] IS NOT NULL, [VRS_CharGroup].[Arg4], 0) AS [KeyCodePre], [VRS_KeyCodes].[KeyCode], [VRS_KeyCodes].[Shift] "
        L"FROM [VRS_KeyCodes] LEFT JOIN [VRS_CharGroup] ON [VRS_CharGroup].[CharGroup]=[VRS_KeyCodes].[CharGroup] "
        L"ORDER BY [VRS_CharGroup].[CharGroup], [VRS_KeyCodes].[KeyCode], [VRS_KeyCodes].[Shift], [VRS_KeyCodes].[Znak]"), variant(m_db), adOpenStatic, adLockReadOnly, adCmdText)))
    {
        _ftprintf(stderr, wxT("%s: error ZCC0050: Error loading key sequences from database. Please make sure the file is ZRCola.zrc compatible.\n"), m_filename.c_str());
        LogErrors();
        return false;
    }

    return true;
}


bool ZRCola::DBSource::GetKeySequence(const com_obj<ADORecordset>& rs, ZRCola::DBSource::keyseq& ks) const
{
    wxASSERT_MSG(rs, wxT("recordset is empty"));

    com_obj<ADOFields> flds;
    wxVERIFY(SUCCEEDED(rs->get_Fields(&flds)));

    {
        com_obj<ADOField> f;
        wxVERIFY(SUCCEEDED(flds->get_Item(variant(L"Znak"), &f)));
        wxCHECK(GetUnicodeCharacter(f, ks.chr), false);
    }

    int modifiers;
    {
        com_obj<ADOField> f;
        wxVERIFY(SUCCEEDED(flds->get_Item(variant(L"Modifiers"), &f)));
        wxCHECK(GetValue(f, modifiers), false);
    }

    int keycode1;
    {
        com_obj<ADOField> f;
        wxVERIFY(SUCCEEDED(flds->get_Item(variant(L"KeyCodePre"), &f)));
        wxCHECK(GetValue(f, keycode1), false);
    }

    int keycode;
    {
        com_obj<ADOField> f;
        wxVERIFY(SUCCEEDED(flds->get_Item(variant(L"KeyCode"), &f)));
        wxCHECK(GetValue(f, keycode), false);
    }

    bool shift;
    {
        com_obj<ADOField> f;
        wxVERIFY(SUCCEEDED(flds->get_Item(variant(L"Shift"), &f)));
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


bool ZRCola::DBSource::SelectLanguages(com_obj<ADORecordset> &rs) const
{
    // Create a new recordset.
    rs.free();
    wxCHECK(SUCCEEDED(::CoCreateInstance(CLSID_CADORecordset, NULL, CLSCTX_ALL, IID_IADORecordset, (LPVOID*)&rs)), false);

    // Open it.
    if (FAILED(rs->Open(variant(
        L"SELECT DISTINCT [entCode], [Jezik_En] "
        L"FROM [VRS_Jezik] "
        L"ORDER BY [entCode], [Jezik_En]"), variant(m_db), adOpenStatic, adLockReadOnly, adCmdText)))
    {
        _ftprintf(stderr, wxT("%s: error ZCC0060: Error loading languages from database. Please make sure the file is ZRCola.zrc compatible.\n"), m_filename.c_str());
        LogErrors();
        return false;
    }

    return true;
}


bool ZRCola::DBSource::GetLanguage(const com_obj<ADORecordset>& rs, ZRCola::DBSource::language& lang) const
{
    wxASSERT_MSG(rs, wxT("recordset is empty"));

    com_obj<ADOFields> flds;
    wxVERIFY(SUCCEEDED(rs->get_Fields(&flds)));

    {
        com_obj<ADOField> f;
        wxVERIFY(SUCCEEDED(flds->get_Item(variant(L"entCode"), &f)));
        wxCHECK(GetLanguage(f, lang.id), false);
    }

    {
        com_obj<ADOField> f;
        wxVERIFY(SUCCEEDED(flds->get_Item(variant(L"Jezik_En"), &f)));
        wxCHECK(GetValue(f, lang.name), false);
    }

    return true;
}


bool ZRCola::DBSource::SelectLanguageCharacters(com_obj<ADORecordset> &rs) const
{
    // Create a new recordset.
    rs.free();
    wxCHECK(SUCCEEDED(::CoCreateInstance(CLSID_CADORecordset, NULL, CLSCTX_ALL, IID_IADORecordset, (LPVOID*)&rs)), false);

    // Open it.
    if (FAILED(rs->Open(variant(
        L"SELECT DISTINCT [znak], [lang] "
        L"FROM [VRS_CharLocal] "
        L"ORDER BY [znak], [lang]"), variant(m_db), adOpenStatic, adLockReadOnly, adCmdText)))
    {
        _ftprintf(stderr, wxT("%s: error ZCC0090: Error loading language characters from database. Please make sure the file is ZRCola.zrc compatible.\n"), m_filename.c_str());
        LogErrors();
        return false;
    }

    return true;
}


bool ZRCola::DBSource::GetLanguageCharacter(const com_obj<ADORecordset>& rs, ZRCola::DBSource::langchar& lc) const
{
    wxASSERT_MSG(rs, wxT("recordset is empty"));

    com_obj<ADOFields> flds;
    wxVERIFY(SUCCEEDED(rs->get_Fields(&flds)));

    {
        com_obj<ADOField> f;
        wxVERIFY(SUCCEEDED(flds->get_Item(variant(L"znak"), &f)));
        wxCHECK(GetUnicodeCharacter(f, lc.chr), false);
    }

    {
        com_obj<ADOField> f;
        wxVERIFY(SUCCEEDED(flds->get_Item(variant(L"lang"), &f)));
        wxCHECK(GetLanguage(f, lc.lang), false);
    }

    return true;
}


bool ZRCola::DBSource::SelectCharacterGroups(com_obj<ADORecordset>& rs) const
{
    // Create a new recordset.
    rs.free();
    wxCHECK(SUCCEEDED(::CoCreateInstance(CLSID_CADORecordset, NULL, CLSCTX_ALL, IID_IADORecordset, (LPVOID*)&rs)), false);

    // Open it.
    if (FAILED(rs->Open(variant(
        L"SELECT DISTINCT [id], [Skupina], [opis_en], [Rang] "
        L"FROM [VRS_SkupinaZnakov] "
        L"ORDER BY [Rang], [opis_en]"), variant(m_db), adOpenStatic, adLockReadOnly, adCmdText)))
    {
        _ftprintf(stderr, wxT("%s: error ZCC0090: Error loading character groups from database. Please make sure the file is ZRCola.zrc compatible.\n"), m_filename.c_str());
        LogErrors();
        return false;
    }

    return true;
}


bool ZRCola::DBSource::GetCharacterGroup(const com_obj<ADORecordset>& rs, chrgrp& cg) const
{
    wxASSERT_MSG(rs, wxT("recordset is empty"));

    com_obj<ADOFields> flds;
    wxVERIFY(SUCCEEDED(rs->get_Fields(&flds)));
    wstring id;

    {
        com_obj<ADOField> f;
        wxVERIFY(SUCCEEDED(flds->get_Item(variant(L"id"), &f)));
        wxCHECK(GetValue(f, cg.id), false);
    }

    {
        com_obj<ADOField> f;
        wxVERIFY(SUCCEEDED(flds->get_Item(variant(L"Skupina"), &f)));
        wxCHECK(GetValue(f, id), false);
    }

    {
        com_obj<ADOField> f;
        wxVERIFY(SUCCEEDED(flds->get_Item(variant(L"Rang"), &f)));
        wxCHECK(GetValue(f, cg.rank), false);
    }

    {
        com_obj<ADOField> f;
        wxVERIFY(SUCCEEDED(flds->get_Item(variant(L"opis_en"), &f)));
        wxCHECK(GetValue(f, cg.name), false);
    }

    // Read character list from database.
    wxVERIFY(SUCCEEDED(m_pCharacterGroup1->put_Value(variant(id.c_str()))));
    com_obj<ADORecordset> rs_chars;
    wxVERIFY(SUCCEEDED(::CoCreateInstance(CLSID_CADORecordset, NULL, CLSCTX_ALL, IID_IADORecordset, (LPVOID*)&rs_chars)));
    wxVERIFY(SUCCEEDED(rs_chars->put_CursorLocation(adUseClient)));
    wxVERIFY(SUCCEEDED(rs_chars->put_CursorType(adOpenForwardOnly)));
    wxVERIFY(SUCCEEDED(rs_chars->put_LockType(adLockReadOnly)));
    if (FAILED(rs_chars->Open(variant(m_comCharacterGroup), variant(DISP_E_PARAMNOTFOUND, VT_ERROR)))) {
        _ftprintf(stderr, wxT("%s: error ZCC0100: Error loading character group characters from database. Please make sure the file is ZRCola.zrc compatible.\n"), m_filename.c_str());
        LogErrors();
        return false;
    }

    {
        cg.chars.clear();
        com_obj<ADOFields> flds;
        wxVERIFY(SUCCEEDED(rs_chars->get_Fields(&flds)));
        com_obj<ADOField> f_char, f_show;
        wxVERIFY(SUCCEEDED(flds->get_Item(variant(L"Znak"  ), &f_char)));
        wxVERIFY(SUCCEEDED(flds->get_Item(variant(L"pogost"), &f_show)));
        for (VARIANT_BOOL eof = VARIANT_TRUE; SUCCEEDED(rs_chars->get_EOF(&eof)) && !eof; rs_chars->MoveNext()) {
            wchar_t c;
            wxCHECK(GetUnicodeCharacter(f_char, c), false);
            size_t n = cg.chars.length();
            cg.chars += c;
            bool show;
            wxCHECK(GetValue(f_show, show), false);
            if ((n % 16) == 0)
                cg.show.push_back(show ? 1 : 0);
            else if (show)
                cg.show[n / 16] |= 1 << (n % 16);
        }
    }

    return true;
}


bool ZRCola::DBSource::SelectCharacters(com_obj<ADORecordset>& rs) const
{
    // Create a new recordset.
    rs.free();
    wxCHECK(SUCCEEDED(::CoCreateInstance(CLSID_CADORecordset, NULL, CLSCTX_ALL, IID_IADORecordset, (LPVOID*)&rs)), false);

    // Open it.
    if (FAILED(rs->Open(variant(
        L"SELECT DISTINCT [znak], [opis_en], [kat], [znak_v], [znak_m] "
        L"FROM [VRS_CharList] "
        L"WHERE "
        L"[aktiven]=1 AND " // Active characters only
        L"[kat]<>'g' "      // Ignore "Other, Control" category!
        L"ORDER BY [znak]"), variant(m_db), adOpenStatic, adLockReadOnly, adCmdText)))
    {
        _ftprintf(stderr, wxT("%s: error ZCC0120: Error loading characters from database. Please make sure the file is ZRCola.zrc compatible.\n"), m_filename.c_str());
        LogErrors();
        return false;
    }

    return true;
}


bool ZRCola::DBSource::GetCharacter(const com_obj<ADORecordset>& rs, character& chr) const
{
    wxASSERT_MSG(rs, wxT("recordset is empty"));

    com_obj<ADOFields> flds;
    wxVERIFY(SUCCEEDED(rs->get_Fields(&flds)));
    wchar_t c;
    chr.rel.clear();

    {
        com_obj<ADOField> f;
        wxVERIFY(SUCCEEDED(flds->get_Item(variant(L"znak"), &f)));
        wxCHECK(GetUnicodeCharacter(f, chr.chr), false);
    }

    {
        com_obj<ADOField> f;
        wxVERIFY(SUCCEEDED(flds->get_Item(variant(L"znak_v"), &f)));
        wxCHECK(GetUnicodeCharacter(f, c), false);
        if (c && c != chr.chr)
            chr.rel += c;
    }

    {
        com_obj<ADOField> f;
        wxVERIFY(SUCCEEDED(flds->get_Item(variant(L"znak_m"), &f)));
        wxCHECK(GetUnicodeCharacter(f, c), false);
        if (c && c != chr.chr)
            chr.rel += c;
    }

    {
        com_obj<ADOField> f;
        wxVERIFY(SUCCEEDED(flds->get_Item(variant(L"opis_en"), &f)));
        wxCHECK(GetValue(f, chr.desc), false);
    }
    ZRCola::DBSource::character_desc_idx::parse_keywords(chr.desc.c_str(), chr.terms);

    {
        com_obj<ADOField> f;
        wxVERIFY(SUCCEEDED(flds->get_Item(variant(L"kat"), &f)));
        wxCHECK(GetChrCat(f, chr.cat), false);
    }

    return true;
}


bool ZRCola::DBSource::SelectCharacterCategories(com_obj<ADORecordset>& rs) const
{
    // Create a new recordset.
    rs.free();
    wxCHECK(SUCCEEDED(::CoCreateInstance(CLSID_CADORecordset, NULL, CLSCTX_ALL, IID_IADORecordset, (LPVOID*)&rs)), false);

    // Open it.
    if (FAILED(rs->Open(variant(
        L"SELECT DISTINCT [kat], [opis_en], [Rang] "
        L"FROM [VRS_CharCategory] "
        L"WHERE [kat]<>'g' "    // Ignore "Other, Control" category!
        L"ORDER BY [Rang], [opis_en]"), variant(m_db), adOpenStatic, adLockReadOnly, adCmdText)))
    {
        _ftprintf(stderr, wxT("%s: error ZCC0130: Error loading character categories from database. Please make sure the file is ZRCola.zrc compatible.\n"), m_filename.c_str());
        LogErrors();
        return false;
    }

    return true;
}


bool ZRCola::DBSource::GetCharacterCategory(const com_obj<ADORecordset>& rs, chrcat& cc) const
{
    wxASSERT_MSG(rs, wxT("recordset is empty"));

    com_obj<ADOFields> flds;
    wxVERIFY(SUCCEEDED(rs->get_Fields(&flds)));
    wstring id;

    {
        com_obj<ADOField> f;
        wxVERIFY(SUCCEEDED(flds->get_Item(variant(L"kat"), &f)));
        wxCHECK(GetChrCat(f, cc.id), false);
    }

    {
        com_obj<ADOField> f;
        wxVERIFY(SUCCEEDED(flds->get_Item(variant(L"Rang"), &f)));
        wxCHECK(GetValue(f, cc.rank), false);
    }

    {
        com_obj<ADOField> f;
        wxVERIFY(SUCCEEDED(flds->get_Item(variant(L"opis_en"), &f)));
        wxCHECK(GetValue(f, cc.name), false);
    }

    return true;
}
