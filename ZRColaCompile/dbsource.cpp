﻿/*
    SPDX-License-Identifier: GPL-3.0-or-later
    Copyright © 2015-2022 Amebis
*/

#include "pch.h"

using namespace std;
using namespace stdex;
using namespace winstd;


//////////////////////////////////////////////////////////////////////////
// ZRCola::DBSource::character_bank
//////////////////////////////////////////////////////////////////////////

void ZRCola::DBSource::character_bank::build_related()
{
    SYSTEM_INFO si;
    GetSystemInfo(&si);

    // Launch workers.
    build_related_worker **workers = new build_related_worker*[si.dwNumberOfProcessors];
    size_type from = 0, total = size();
    iterator chr_from = begin(), chr_to;
    for (DWORD i = 0; i < si.dwNumberOfProcessors; i++, chr_from = chr_to) {
        size_type to = MulDiv(i + 1, total, si.dwNumberOfProcessors);
        for (chr_to = chr_from; from < to; from++, ++chr_to);
        workers[i] = new build_related_worker(this, chr_from, chr_to);
    }

    // Wait for workers.
    for (DWORD i = 0; i < si.dwNumberOfProcessors; i++) {
        if (workers[i]) {
            workers[i]->join();
            delete workers[i];
        }
    }

    delete [] workers; // This line of code sounds horrible, I know.
}


ZRCola::DBSource::character_bank::build_related_worker::build_related_worker(_In_ const character_bank *cb, _In_ iterator from, _In_ iterator to) :
    winstd::thread((HANDLE)_beginthreadex(NULL, 0, process, this, CREATE_SUSPENDED, NULL)),
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
    vector<wchar_t, heap_allocator<wchar_t> > rel(al);
    set<wstring, less<wstring>, heap_allocator<wstring> > matching(less<wstring>(), al);

    for (auto c = m_from; c != m_to; c++) {
        rel.clear();

        // Skip all unexisting, or self related characters.
        auto m_cb_end = m_cb->cend();
        for (std::vector<wchar_t>::const_pointer c_rel = c->second.rel.data(), c_rel_end = c_rel + c->second.rel.size(), c_rel_next = c_rel_end; c_rel < c_rel_end; c_rel = c_rel_next) {
            c_rel_next = c_rel + wcsnlen(c_rel, c_rel_end - c_rel) + 1;
            if (m_cb->find(c_rel) != m_cb_end && c->first.compare(c_rel) != 0)
                rel.insert(rel.end(), c_rel, c_rel_next);
        }

        // Add all characters that share enough keywords.
        for (auto c2 = m_cb->cbegin(), c2_end = m_cb->cend(); c2 != c2_end; ++c2) {
            if (c == c2)
                continue;
            bool already_present = false;
            for (std::vector<wchar_t>::const_pointer c_rel = rel.data(), c_rel_end = c_rel + rel.size(), c_rel_next = c_rel_end; c_rel < c_rel_end; c_rel = c_rel_next) {
                c_rel_next = c_rel + wcsnlen(c_rel, c_rel_end - c_rel) + 1;
                if (c2->first.compare(c_rel) == 0) {
                    already_present = true;
                    break;
                }
            }
            if (already_present)
                continue;

            set<wstring>::size_type comparisons = 0;
            matching.clear();
            for (auto term = c->second.terms_rel.cbegin(), term_end = c->second.terms_rel.cend(); term != term_end; ++term) {
                for (auto term2 = c2->second.terms_rel.cbegin(), term2_end = c2->second.terms_rel.cend(); term2 != term2_end; ++term2) {
                    comparisons++;
                    if (*term == *term2)
                        matching.insert(*term);
                }
            }

            if (comparisons) {
                // If 1/2 terms match, assume related.
                auto hits = matching.size();
                if (hits*hits*2 >= comparisons)
                    rel.insert(rel.end(), c2->first.data(), c2->first.data() + c2->first.length() + 1);
            }
        }

        c->second.rel.assign(rel.cbegin(), rel.cend());
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

void ZRCola::DBSource::character_desc_idx::parse_keywords(_In_ const wchar_t *str, _Inout_ set<wstring> &terms)
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


void ZRCola::DBSource::character_desc_idx::add_keywords(const set<wstring> &terms, const wstring &chr, size_t sub)
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


void ZRCola::DBSource::character_desc_idx::save(ZRCola::textindex<wchar_t, wchar_t, uint32_t> &idx) const
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
        ZRCola::mappair_t<uint32_t> p = { idx.keys.size(), idx.values.size() };
        idx.push_back(p);
        idx.keys.insert(idx.keys.end(), i->first.cbegin(), i->first.cend());
        idx.values.insert(idx.values.end(), i->second.cbegin(), i->second.cend());
    }
}


//////////////////////////////////////////////////////////////////////////
// ZRCola::DBSource
//////////////////////////////////////////////////////////////////////////

ZRCola::DBSource::DBSource() :
    m_locale(nullptr)
{
    // Initialize ignore list.
    m_terms_ignore.insert(L"letter");
    m_terms_ignore.insert(L"modifier");
    m_terms_ignore.insert(L"symbol");
    m_terms_ignore.insert(L"accent");
    m_terms_ignore.insert(L"with");
    m_terms_ignore.insert(L"and");
    m_terms_ignore.insert(L"capital");
    m_terms_ignore.insert(L"small");
    m_terms_ignore.insert(L"combining");
}


ZRCola::DBSource::~DBSource()
{
    // Manually release all COM objects related to the database before we close the database.
    m_pTranslationSets1.free();
    m_comTranslationSets.free();
    m_pTranslation1.free();
    m_comTranslation.free();
    m_pCharacterGroup1.free();
    m_comCharacterGroup.free();
    m_pHighlight1.free();
    m_comHighlight.free();

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
#pragma warning(push)
#pragma warning(disable: 6387) // Connection15::Open() declaration is wrong: it defaults username and password parameters to NULL, but annotates them as required non-NULL.
        hr = m_db->Open(bstr(cn.c_str()));
#pragma warning(pop)
        if (SUCCEEDED(hr)) {
            // Database open and ready.
            m_filename = filename;
            m_locale = _create_locale(LC_ALL, "Slovenian_Slovenia.1250");

            // Create ADO command(s).
            wxASSERT_MSG(!m_comCharacterGroup, wxT("ADO command already created"));
            wxVERIFY(SUCCEEDED(::CoCreateInstance(CLSID_CADOCommand, NULL, CLSCTX_ALL, IID_IADOCommand, (LPVOID*)&m_comCharacterGroup)));
            wxVERIFY(SUCCEEDED(m_comCharacterGroup->put_ActiveConnection(variant(m_db))));
            wxVERIFY(SUCCEEDED(m_comCharacterGroup->put_CommandType(adCmdText)));
            wxVERIFY(SUCCEEDED(m_comCharacterGroup->put_CommandText(bstr(L"SELECT [VRS_SkupineZnakov].[Znak], [VRS_SkupineZnakov].[pogost] "
                L"FROM [VRS_SkupineZnakov] "
                L"LEFT JOIN [VRS_CharList] ON [VRS_SkupineZnakov].[Znak]=[VRS_CharList].[znak] "
                L"WHERE [VRS_CharList].[aktiven]=1 AND [VRS_SkupineZnakov].[Skupina]=? "
                L"ORDER BY [VRS_SkupineZnakov].[Rang] ASC, [VRS_SkupineZnakov].[Znak] ASC"))));
            {
                // Create and add command parameters.
                com_obj<ADOParameters> params;
                wxVERIFY(SUCCEEDED(m_comCharacterGroup->get_Parameters(&params)));
                wxASSERT_MSG(!m_pCharacterGroup1, wxT("ADO command parameter already created"));
                wxVERIFY(SUCCEEDED(m_comCharacterGroup->CreateParameter(bstr(L"@Skupina"), adVarWChar, adParamInput, 50, variant(DISP_E_PARAMNOTFOUND, VT_ERROR), &m_pCharacterGroup1)));
                wxVERIFY(SUCCEEDED(params->Append(m_pCharacterGroup1)));
            }

            wxASSERT_MSG(!m_comTranslation, wxT("ADO command already created"));
            wxVERIFY(SUCCEEDED(::CoCreateInstance(CLSID_CADOCommand, NULL, CLSCTX_ALL, IID_IADOCommand, (LPVOID*)&m_comTranslation)));
            wxVERIFY(SUCCEEDED(m_comTranslation->put_ActiveConnection(variant(m_db))));
            wxVERIFY(SUCCEEDED(m_comTranslation->put_CommandType(adCmdText)));
            wxVERIFY(SUCCEEDED(m_comTranslation->put_CommandText(bstr(L"SELECT [Komb1] AS [komb], [rang_komb1] AS [rang_komb], '' AS [Kano], 0 AS [Kanoniziraj], [Komb2] AS [znak], [rang_komb2] AS [rang_znak] "
                L"FROM [VRS_ScriptRepl2] "
                L"WHERE [Script]=? "
                L"ORDER BY [Komb2], [rang_komb2], [rang_komb1], [Komb1]"))));
            {
                // Create and add command parameters.
                com_obj<ADOParameters> params;
                wxVERIFY(SUCCEEDED(m_comTranslation->get_Parameters(&params)));
                wxASSERT_MSG(!m_pTranslation1, wxT("ADO command parameter already created"));
                wxVERIFY(SUCCEEDED(m_comTranslation->CreateParameter(bstr(L"@Script"), adSmallInt, adParamInput, 0, variant(DISP_E_PARAMNOTFOUND, VT_ERROR), &m_pTranslation1)));
                wxVERIFY(SUCCEEDED(params->Append(m_pTranslation1)));
            }

            wxASSERT_MSG(!m_comTranslationSets, wxT("ADO command already created"));
            wxVERIFY(SUCCEEDED(::CoCreateInstance(CLSID_CADOCommand, NULL, CLSCTX_ALL, IID_IADOCommand, (LPVOID*)&m_comTranslationSets)));
            wxVERIFY(SUCCEEDED(m_comTranslationSets->put_ActiveConnection(variant(m_db))));
            wxVERIFY(SUCCEEDED(m_comTranslationSets->put_CommandType(adCmdText)));
            wxVERIFY(SUCCEEDED(m_comTranslationSets->put_CommandText(bstr(L"SELECT [Script] "
                L"FROM [VRS_Script2SeqScr] "
                L"WHERE [ID]=? "
                L"ORDER BY [Rank] ASC"))));
            {
                // Create and add command parameters.
                com_obj<ADOParameters> params;
                wxVERIFY(SUCCEEDED(m_comTranslationSets->get_Parameters(&params)));
                wxASSERT_MSG(!m_pTranslationSets1, wxT("ADO command parameter already created"));
                wxVERIFY(SUCCEEDED(m_comTranslationSets->CreateParameter(bstr(L"@ID"), adSmallInt, adParamInput, 0, variant(DISP_E_PARAMNOTFOUND, VT_ERROR), &m_pTranslationSets1)));
                wxVERIFY(SUCCEEDED(params->Append(m_pTranslationSets1)));
            }

            wxASSERT_MSG(!m_comHighlight, wxT("ADO command already created"));
            wxVERIFY(SUCCEEDED(::CoCreateInstance(CLSID_CADOCommand, NULL, CLSCTX_ALL, IID_IADOCommand, (LPVOID*)&m_comHighlight)));
            wxVERIFY(SUCCEEDED(m_comHighlight->put_ActiveConnection(variant(m_db))));
            wxVERIFY(SUCCEEDED(m_comHighlight->put_CommandType(adCmdText)));
            wxVERIFY(SUCCEEDED(m_comHighlight->put_CommandText(bstr(L"SELECT [komb] "
                L"FROM [VRS_HighlightChars2] "
                L"WHERE [group]=? "
                L"ORDER BY [komb]"))));
            {
                // Create and add command parameters.
                com_obj<ADOParameters> params;
                wxVERIFY(SUCCEEDED(m_comHighlight->get_Parameters(&params)));
                wxASSERT_MSG(!m_pHighlight1, wxT("ADO command parameter already created"));
                wxVERIFY(SUCCEEDED(m_comHighlight->CreateParameter(bstr(L"@group"), adSmallInt, adParamInput, 0, variant(DISP_E_PARAMNOTFOUND, VT_ERROR), &m_pHighlight1)));
                wxVERIFY(SUCCEEDED(params->Append(m_pHighlight1)));
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


bool ZRCola::DBSource::GetValue(const com_obj<ADOField>& f, short& val) const
{
    wxASSERT_MSG(f, wxT("field is empty"));

    variant v;
    wxVERIFY(SUCCEEDED(f->get_Value(&v)));
    wxCHECK(SUCCEEDED(v.change_type(VT_I2)), false);

    val = V_I2(&v);

    return true;
}


bool ZRCola::DBSource::GetValue(const com_obj<ADOField>& f, string& val) const
{
    wxASSERT_MSG(f, wxT("field is empty"));

    variant v;
    wxVERIFY(SUCCEEDED(f->get_Value(&v)));
    if (V_VT(&v) != VT_NULL) {
        wxCHECK(SUCCEEDED(v.change_type(VT_BSTR)), false);

        WideCharToMultiByte(CP_ACP, 0, V_BSTR(&v), ::SysStringLen(V_BSTR(&v)), val, NULL, NULL);
    } else
        val.clear();

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
    str.clear();
    if (V_VT(&v) != VT_NULL) {
        wxCHECK(SUCCEEDED(v.change_type(VT_BSTR)), false);

        // Parse the field. Must be "xxxx+xxxx+xxxx..." sequence.
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
    }

    return true;
}


bool ZRCola::DBSource::GetNormPerm(const winstd::com_obj<ADOField>& f, normperm& np) const
{
    wxASSERT_MSG(f, wxT("field is empty"));

    variant v;
    wxVERIFY(SUCCEEDED(f->get_Value(&v)));
    np.clear();
    if (V_VT(&v) != VT_NULL) {
        wxCHECK(SUCCEEDED(v.change_type(VT_BSTR)), false);

        // Parse the field. Must be "nnnn,nnnn,nnnn..." sequence.
        for (UINT i = 0, n = ::SysStringLen(V_BSTR(&v)); i < n && V_BSTR(&v)[i];) {
            // Parse Unicode code.
            UINT j = 0;
            std::vector<size_t> p;
            for (; i < n && V_BSTR(&v)[i]; i++, j++) {
                if (L'0' <= V_BSTR(&v)[i] && V_BSTR(&v)[i] <= L'9') p.push_back(V_BSTR(&v)[i] - L'0');
                else break;
            }
            if (j <= 0) {
                bstr fieldname; wxVERIFY(SUCCEEDED(f->get_Name(&fieldname)));
                _ftprintf(stderr, wxT("%s: error ZCC0150: Syntax error in \"%.*ls\" field (\"%.*ls\"). Permutation sequence must be at least one decimal digit long.\n"), m_filename.c_str(), fieldname.length(), (BSTR)fieldname, n, V_BSTR(&v));
                return false;
            }
            np.insert(std::move(p));

            // Skip delimiter(s) and whitespace.
            for (; i < n && V_BSTR(&v)[i] && (V_BSTR(&v)[i] == L',' || _iswspace_l(V_BSTR(&v)[i], m_locale)); i++);
        }
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


bool ZRCola::DBSource::GetTagNames(const winstd::com_obj<ADOField>& f, LCID lcid, list<wstring>& names) const
{
    wxASSERT_MSG(f, wxT("field is empty"));

    variant v;
    wxVERIFY(SUCCEEDED(f->get_Value(&v)));
    wxCHECK(SUCCEEDED(v.change_type(VT_BSTR)), false);

    // Parse the field. Must be "name, name, name..." sequence.
    names.clear();
    for (UINT i = 0, i_end = ::SysStringLen(V_BSTR(&v)); i < i_end && V_BSTR(&v)[i];) {
        if (iswspace(V_BSTR(&v)[i])) {
            // Skip leading white space.
            i++; continue;
        }

        // Parse name.
        UINT j = i, j_end = i;
        for (; i < i_end && V_BSTR(&v)[i]; i++) {
            if (V_BSTR(&v)[i] == L',' || V_BSTR(&v)[i] == L';') {
                // Delimiter found.
                i++; break;
            } else if (!iswspace(V_BSTR(&v)[i])) {
                // Remember last non-white space character.
                j_end = i + 1;
            }
        }
        wstring name(V_BSTR(&v) + j, V_BSTR(&v) + j_end);
        for (auto n = names.cbegin(), n_end = names.cend(); ; ++n) {
            if (n == n_end) {
                // Add name to the list.
                names.push_back(std::move(name));
                break;
            } else if (ZRCola::tagname_db::tagname::CompareName(lcid, n->data(), (uint16_t)n->length(), name.data(), (uint16_t)name.length()) == CSTR_EQUAL) {
                // Name is already on the list.
                break;
            }
        }
    }

    return true;
}


bool ZRCola::DBSource::SelectNormPermSets(winstd::com_obj<ADORecordset>& rs) const
{
    // Create a new recordset.
    rs.free();
    wxCHECK(SUCCEEDED(::CoCreateInstance(CLSID_CADORecordset, NULL, CLSCTX_ALL, IID_IADORecordset, (LPVOID*)&rs)), false);

    // Open it.
    if (FAILED(rs->Open(variant(
        L"SELECT [oblika], [oblike] "
        L"FROM [VRS_CharCanoOblike] "
        L"ORDER BY [oblika], [oblike]"), variant(m_db), adOpenStatic, adLockReadOnly, adCmdText)))
    {
        _ftprintf(stderr, wxT("%s: error ZCC0160: Error loading normalization permutation sets from database. Please make sure the file is ZRCola.zrc compatible.\n"), m_filename.c_str());
        LogErrors();
        return false;
    }

    return true;
}


bool ZRCola::DBSource::GetNormPerm(const winstd::com_obj<ADORecordset>& rs, std::string& norm, normperm& np) const
{
    wxASSERT_MSG(rs, wxT("recordset is empty"));

    com_obj<ADOFields> flds;
    wxVERIFY(SUCCEEDED(rs->get_Fields(&flds)));

    {
        com_obj<ADOField> f;
        wxVERIFY(SUCCEEDED(flds->get_Item(variant(L"oblika"), &f)));
        wxCHECK(GetValue(f, norm), false);
    }

    {
        com_obj<ADOField> f;
        wxVERIFY(SUCCEEDED(flds->get_Item(variant(L"oblike"), &f)));
        wxCHECK(GetNormPerm(f, np), false);
    }

    // Verify all lengths match.
    size_t n = norm.length();
    for (auto p = np.cbegin(), p_end = np.cend(); p != p_end; ++p) {
        if (p->size() != n) {
            _ftprintf(stderr, wxT("%s: error ZCC0170: Inconsistent normalization sequence \"%.*hs\" permutation length. Please make sure all permutation lengths match normalization sequence length (%u).\n"), m_filename.c_str(), n, norm.c_str(), n);
            return false;
        }
    }

    return true;
}


bool ZRCola::DBSource::SelectTranslations(com_obj<ADORecordset> &rs) const
{
    // Create a new recordset.
    rs.free();
    wxCHECK(SUCCEEDED(::CoCreateInstance(CLSID_CADORecordset, NULL, CLSCTX_ALL, IID_IADORecordset, (LPVOID*)&rs)), false);

    // Open it.
    if (FAILED(rs->Open(variant(
        L"SELECT [komb], [rang_komb], [Kano], [Kanoniziraj], [znak], [rang_znak] "
        L"FROM [VRS_ReplChar] "
        L"WHERE [rang_komb]=1 "
        L"ORDER BY [znak], [rang_znak], [rang_komb], [komb]"), variant(m_db), adOpenStatic, adLockReadOnly, adCmdText)))
    {
        _ftprintf(stderr, wxT("%s: error ZCC0040: Error loading translations from database. Please make sure the file is ZRCola.zrc compatible.\n"), m_filename.c_str());
        LogErrors();
        return false;
    }

    return true;
}


bool ZRCola::DBSource::SelectTranslations(short set, winstd::com_obj<ADORecordset>& rs) const
{
    // Create a new recordset.
    rs.free();
    wxVERIFY(SUCCEEDED(::CoCreateInstance(CLSID_CADORecordset, NULL, CLSCTX_ALL, IID_IADORecordset, (LPVOID*)&rs)));
    wxVERIFY(SUCCEEDED(rs->put_CursorLocation(adUseClient)));
    wxVERIFY(SUCCEEDED(rs->put_CursorType(adOpenForwardOnly)));
    wxVERIFY(SUCCEEDED(rs->put_LockType(adLockReadOnly)));

    // Open it.
    wxVERIFY(SUCCEEDED(m_pTranslation1->put_Value(variant(set))));
    if (FAILED(rs->Open(variant(m_comTranslation), variant(DISP_E_PARAMNOTFOUND, VT_ERROR)))) {
        _ftprintf(stderr, wxT("%s: error ZCC0100: Error loading translations from database. Please make sure the file is ZRCola.zrc compatible.\n"), m_filename.c_str());
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
        wxCHECK(GetUnicodeString(f, t.src.str), false);
    }

    {
        com_obj<ADOField> f;
        wxVERIFY(SUCCEEDED(flds->get_Item(variant(L"rang_komb"), &f)));
        wxCHECK(GetValue(f, t.src.rank), false);
    }

    {
        bool norm;
        com_obj<ADOField> f;
        wxVERIFY(SUCCEEDED(flds->get_Item(variant(L"Kanoniziraj"), &f)));
        wxCHECK(GetValue(f, norm), false);
        if (norm) {
            com_obj<ADOField> f2;
            wxVERIFY(SUCCEEDED(flds->get_Item(variant(L"Kano"), &f2)));
            wxCHECK(GetValue(f2, t.norm), false);
        } else
            t.norm.clear();
    }

    {
        com_obj<ADOField> f;
        wxVERIFY(SUCCEEDED(flds->get_Item(variant(L"rang_znak"), &f)));
        wxCHECK(GetValue(f, t.dst.rank), false);
    }

    {
        com_obj<ADOField> f;
        wxVERIFY(SUCCEEDED(flds->get_Item(variant(L"znak"), &f)));
        wxCHECK(GetUnicodeString(f, t.dst.str), false);
    }

    return true;
}


bool ZRCola::DBSource::SelectTranlationSets(com_obj<ADORecordset> &rs) const
{
    // Create a new recordset.
    rs.free();
    wxCHECK(SUCCEEDED(::CoCreateInstance(CLSID_CADORecordset, NULL, CLSCTX_ALL, IID_IADORecordset, (LPVOID*)&rs)), false);

    // Open it.
    if (FAILED(rs->Open(variant(
        L"SELECT DISTINCT [entCode], [Src_En], [Dst_En] "
        L"FROM [VRS_Script2] "
        L"ORDER BY [entCode], [Src_En], [Dst_En]"), variant(m_db), adOpenStatic, adLockReadOnly, adCmdText)))
    {
        _ftprintf(stderr, wxT("%s: error ZCC0060: Error loading translation sets from database. Please make sure the file is ZRCola.zrc compatible.\n"), m_filename.c_str());
        LogErrors();
        return false;
    }

    return true;
}


bool ZRCola::DBSource::GetTranslationSet(const com_obj<ADORecordset>& rs, ZRCola::DBSource::transet& ts) const
{
    wxASSERT_MSG(rs, wxT("recordset is empty"));

    com_obj<ADOFields> flds;
    wxVERIFY(SUCCEEDED(rs->get_Fields(&flds)));

    {
        com_obj<ADOField> f;
        wxVERIFY(SUCCEEDED(flds->get_Item(variant(L"entCode"), &f)));
        wxCHECK(GetValue(f, ts.set), false);
    }

    {
        com_obj<ADOField> f;
        wxVERIFY(SUCCEEDED(flds->get_Item(variant(L"Src_En"), &f)));
        wxCHECK(GetValue(f, ts.src), false);
    }

    {
        com_obj<ADOField> f;
        wxVERIFY(SUCCEEDED(flds->get_Item(variant(L"Dst_En"), &f)));
        wxCHECK(GetValue(f, ts.dst), false);
    }

    return true;
}


bool ZRCola::DBSource::SelectTranlationSeqs(com_obj<ADORecordset> &rs) const
{
    // Create a new recordset.
    rs.free();
    wxCHECK(SUCCEEDED(::CoCreateInstance(CLSID_CADORecordset, NULL, CLSCTX_ALL, IID_IADORecordset, (LPVOID*)&rs)), false);

    // Open it.
    if (FAILED(rs->Open(variant(
        L"SELECT DISTINCT [ID], [Descr], [Rank] "
        L"FROM [VRS_Script2Seq] "
        L"ORDER BY [Rank], [Descr]"), variant(m_db), adOpenStatic, adLockReadOnly, adCmdText)))
    {
        _ftprintf(stderr, wxT("%s: error ZCC0060: Error loading translation sequences from database. Please make sure the file is ZRCola.zrc compatible.\n"), m_filename.c_str());
        LogErrors();
        return false;
    }

    return true;
}


bool ZRCola::DBSource::GetTranslationSeq(const com_obj<ADORecordset>& rs, ZRCola::DBSource::transeq& ts) const
{
    wxASSERT_MSG(rs, wxT("recordset is empty"));

    com_obj<ADOFields> flds;
    wxVERIFY(SUCCEEDED(rs->get_Fields(&flds)));

    {
        com_obj<ADOField> f;
        wxVERIFY(SUCCEEDED(flds->get_Item(variant(L"ID"), &f)));
        wxCHECK(GetValue(f, ts.seq), false);
    }

    {
        com_obj<ADOField> f;
        wxVERIFY(SUCCEEDED(flds->get_Item(variant(L"Rank"), &f)));
        wxCHECK(GetValue(f, ts.rank), false);
    }

    {
        com_obj<ADOField> f;
        wxVERIFY(SUCCEEDED(flds->get_Item(variant(L"Descr"), &f)));
        wxCHECK(GetValue(f, ts.name), false);
    }

    // Read translation sequence sets from database.
    wxVERIFY(SUCCEEDED(m_pTranslationSets1->put_Value(variant(ts.seq))));
    com_obj<ADORecordset> rs_chars;
    wxVERIFY(SUCCEEDED(::CoCreateInstance(CLSID_CADORecordset, NULL, CLSCTX_ALL, IID_IADORecordset, (LPVOID*)&rs_chars)));
    wxVERIFY(SUCCEEDED(rs_chars->put_CursorLocation(adUseClient)));
    wxVERIFY(SUCCEEDED(rs_chars->put_CursorType(adOpenForwardOnly)));
    wxVERIFY(SUCCEEDED(rs_chars->put_LockType(adLockReadOnly)));
    if (FAILED(rs_chars->Open(variant(m_comTranslationSets), variant(DISP_E_PARAMNOTFOUND, VT_ERROR)))) {
        _ftprintf(stderr, wxT("%s: error ZCC0140: Error loading character group characters from database. Please make sure the file is ZRCola.zrc compatible.\n"), m_filename.c_str());
        LogErrors();
        return false;
    }

    {
        ts.sets.clear();
        com_obj<ADOFields> flds2;
        wxVERIFY(SUCCEEDED(rs_chars->get_Fields(&flds2)));
        com_obj<ADOField> f_set;
        wxVERIFY(SUCCEEDED(flds2->get_Item(variant(L"Script"), &f_set)));
        size_t n = 0;
        for (VARIANT_BOOL eof = VARIANT_TRUE; SUCCEEDED(rs_chars->get_EOF(&eof)) && !eof; rs_chars->MoveNext(), n++) {
            short set;
            wxCHECK(GetValue(f_set, set), false);
            ts.sets.push_back(set);
        }
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
        wxCHECK(GetUnicodeString(f, ks.chr), false);
    }

    short modifiers;
    {
        com_obj<ADOField> f;
        wxVERIFY(SUCCEEDED(flds->get_Item(variant(L"Modifiers"), &f)));
        wxCHECK(GetValue(f, modifiers), false);
    }

    short keycode1;
    {
        com_obj<ADOField> f;
        wxVERIFY(SUCCEEDED(flds->get_Item(variant(L"KeyCodePre"), &f)));
        wxCHECK(GetValue(f, keycode1), false);
    }

    short keycode;
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
            keyseq::keycode::translate_slen(static_cast<wchar_t>(keycode1)),
            (modifiers & 0x100) != 0,
            (modifiers & 0x200) != 0,
            (modifiers & 0x400) != 0 };
        ks.seq.push_back(kc1);

        keyseq::keycode kc2 = { keyseq::keycode::translate_slen(static_cast<wchar_t>(keycode)), shift };
        ks.seq.push_back(kc2);
    } else {
        // First key in the sequence is only modifier(s).
        keyseq::keycode kc1 = {
            keyseq::keycode::translate_slen(static_cast<wchar_t>(keycode)),
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
        wxCHECK(GetLanguage(f, lang.lang), false);
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
        wxCHECK(GetUnicodeString(f, lc.chr), false);
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
    wstring grp;

    {
        com_obj<ADOField> f;
        wxVERIFY(SUCCEEDED(flds->get_Item(variant(L"id"), &f)));
        wxCHECK(GetValue(f, cg.grp), false);
    }

    {
        com_obj<ADOField> f;
        wxVERIFY(SUCCEEDED(flds->get_Item(variant(L"Skupina"), &f)));
        wxCHECK(GetValue(f, grp), false);
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
    wxVERIFY(SUCCEEDED(m_pCharacterGroup1->put_Value(variant(grp.c_str()))));
    com_obj<ADORecordset> rs_chars;
    wxVERIFY(SUCCEEDED(::CoCreateInstance(CLSID_CADORecordset, NULL, CLSCTX_ALL, IID_IADORecordset, (LPVOID*)&rs_chars)));
    wxVERIFY(SUCCEEDED(rs_chars->put_CursorLocation(adUseClient)));
    wxVERIFY(SUCCEEDED(rs_chars->put_CursorType(adOpenForwardOnly)));
    wxVERIFY(SUCCEEDED(rs_chars->put_LockType(adLockReadOnly)));
    if (FAILED(rs_chars->Open(variant(m_comCharacterGroup), variant(DISP_E_PARAMNOTFOUND, VT_ERROR)))) {
        _ftprintf(stderr, wxT("%s: error ZCC0140: Error loading character group characters from database. Please make sure the file is ZRCola.zrc compatible.\n"), m_filename.c_str());
        LogErrors();
        return false;
    }

    {
        cg.chars.clear();
        cg.show.clear();
        com_obj<ADOFields> flds2;
        wxVERIFY(SUCCEEDED(rs_chars->get_Fields(&flds2)));
        com_obj<ADOField> f_char, f_show;
        wxVERIFY(SUCCEEDED(flds2->get_Item(variant(L"Znak"  ), &f_char)));
        wxVERIFY(SUCCEEDED(flds2->get_Item(variant(L"pogost"), &f_show)));
        size_t n = 0;
        for (VARIANT_BOOL eof = VARIANT_TRUE; SUCCEEDED(rs_chars->get_EOF(&eof)) && !eof; rs_chars->MoveNext(), n++) {
            wstring c;
            wxCHECK(GetUnicodeString(f_char, c), false);
            cg.chars.insert(cg.chars.end(), c.data(), c.data() + c.length() + 1);
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
    wstring c;
    chr.second.terms.clear();
    chr.second.terms_rel.clear();
    chr.second.rel.clear();

    {
        com_obj<ADOField> f;
        wxVERIFY(SUCCEEDED(flds->get_Item(variant(L"znak"), &f)));
        wxCHECK(GetUnicodeString(f, chr.first), false);
    }

    {
        com_obj<ADOField> f;
        wxVERIFY(SUCCEEDED(flds->get_Item(variant(L"znak_v"), &f)));
        wxCHECK(GetUnicodeString(f, c), false);
        if (!c.empty() && c != chr.first)
            chr.second.rel.insert(chr.second.rel.end(), c.data(), c.data() + c.length() + 1);
    }

    {
        com_obj<ADOField> f;
        wxVERIFY(SUCCEEDED(flds->get_Item(variant(L"znak_m"), &f)));
        wxCHECK(GetUnicodeString(f, c), false);
        if (!c.empty() && c != chr.first)
            chr.second.rel.insert(chr.second.rel.end(), c.data(), c.data() + c.length() + 1);
    }

    {
        com_obj<ADOField> f;
        wxVERIFY(SUCCEEDED(flds->get_Item(variant(L"opis_en"), &f)));
        wxCHECK(GetValue(f, chr.second.desc), false);
        ZRCola::DBSource::character_desc_idx::parse_keywords(chr.second.desc.c_str(), chr.second.terms);
        for (auto term = chr.second.terms.cbegin(), term_end = chr.second.terms.cend(); term != term_end; ++term) {
            if (m_terms_ignore.find(*term) != m_terms_ignore.cend())
                continue;
            chr.second.terms_rel.insert(*term);
        }
    }

    {
        com_obj<ADOField> f;
        wxVERIFY(SUCCEEDED(flds->get_Item(variant(L"kat"), &f)));
        wxCHECK(GetChrCat(f, chr.second.cat), false);
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

    {
        com_obj<ADOField> f;
        wxVERIFY(SUCCEEDED(flds->get_Item(variant(L"kat"), &f)));
        wxCHECK(GetChrCat(f, cc.cat), false);
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


bool ZRCola::DBSource::SelectCharacterTags(winstd::com_obj<ADORecordset>& rs) const
{
    // Create a new recordset.
    rs.free();
    wxCHECK(SUCCEEDED(::CoCreateInstance(CLSID_CADORecordset, NULL, CLSCTX_ALL, IID_IADORecordset, (LPVOID*)&rs)), false);

    // Open it.
    if (FAILED(rs->Open(variant(
        L"SELECT DISTINCT [znak], [oznaka] "
        L"FROM [VRS_CharTags] "
        L"ORDER BY [znak], [oznaka]"), variant(m_db), adOpenStatic, adLockReadOnly, adCmdText)))
    {
        _ftprintf(stderr, wxT("%s: error ZCC0130: Error loading character tags from database. Please make sure the file is ZRCola.zrc compatible.\n"), m_filename.c_str());
        LogErrors();
        return false;
    }

    return true;
}


bool ZRCola::DBSource::GetCharacterTag(const winstd::com_obj<ADORecordset>& rs, chrtag& ct) const
{
    wxASSERT_MSG(rs, wxT("recordset is empty"));

    com_obj<ADOFields> flds;
    wxVERIFY(SUCCEEDED(rs->get_Fields(&flds)));

    {
        com_obj<ADOField> f;
        wxVERIFY(SUCCEEDED(flds->get_Item(variant(L"znak"), &f)));
        wxCHECK(GetUnicodeString(f, ct.chr), false);
    }

    {
        com_obj<ADOField> f;
        wxVERIFY(SUCCEEDED(flds->get_Item(variant(L"oznaka"), &f)));
        wxCHECK(GetValue(f, ct.tag), false);
    }

    return true;
}


bool ZRCola::DBSource::SelectTagNames(winstd::com_obj<ADORecordset>& rs) const
{
    // Create a new recordset.
    rs.free();
    wxCHECK(SUCCEEDED(::CoCreateInstance(CLSID_CADORecordset, NULL, CLSCTX_ALL, IID_IADORecordset, (LPVOID*)&rs)), false);

    // Open it.
    if (FAILED(rs->Open(variant(
        L"SELECT DISTINCT [oznaka], [opis_en], [opis_sl], [opis_ru] "
        L"FROM [VRS_Tags] "
        L"ORDER BY [oznaka]"), variant(m_db), adOpenStatic, adLockReadOnly, adCmdText)))
    {
        _ftprintf(stderr, wxT("%s: error ZCC0130: Error loading tags from database. Please make sure the file is ZRCola.zrc compatible.\n"), m_filename.c_str());
        LogErrors();
        return false;
    }

    return true;
}


bool ZRCola::DBSource::GetTagName(const winstd::com_obj<ADORecordset>& rs, tagname& tn) const
{
    wxASSERT_MSG(rs, wxT("recordset is empty"));

    com_obj<ADOFields> flds;
    wxVERIFY(SUCCEEDED(rs->get_Fields(&flds)));
    tn.names.clear();

    {
        com_obj<ADOField> f;
        wxVERIFY(SUCCEEDED(flds->get_Item(variant(L"oznaka"), &f)));
        wxCHECK(GetValue(f, tn.tag), false);
    }

    {
        com_obj<ADOField> f;
        wxVERIFY(SUCCEEDED(flds->get_Item(variant(L"opis_en"), &f)));
        LCID lcid = MAKELCID(MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT), SORT_DEFAULT);
        list<wstring> names;
        wxCHECK(GetTagNames(f, lcid, names), false);
        tn.names.insert(std::move(pair<LCID, list<wstring> >(lcid, std::move(names))));
    }

    {
        com_obj<ADOField> f;
        wxVERIFY(SUCCEEDED(flds->get_Item(variant(L"opis_sl"), &f)));
        LCID lcid = MAKELCID(MAKELANGID(LANG_SLOVENIAN, SUBLANG_DEFAULT), SORT_DEFAULT);
        list<wstring> names;
        wxCHECK(GetTagNames(f, lcid, names), false);
        tn.names.insert(std::move(pair<LCID, list<wstring> >(lcid, std::move(names))));
    }

    {
        com_obj<ADOField> f;
        wxVERIFY(SUCCEEDED(flds->get_Item(variant(L"opis_ru"), &f)));
        LCID lcid = MAKELCID(MAKELANGID(LANG_RUSSIAN, SUBLANG_DEFAULT), SORT_DEFAULT);
        list<wstring> names;
        wxCHECK(GetTagNames(f, lcid, names), false);
        tn.names.insert(std::move(pair<LCID, list<wstring> >(lcid, std::move(names))));
    }

    return true;
}


bool ZRCola::DBSource::SelectHighlights(short set, winstd::com_obj<ADORecordset>& rs) const
{
    // Create a new recordset.
    rs.free();
    wxVERIFY(SUCCEEDED(::CoCreateInstance(CLSID_CADORecordset, NULL, CLSCTX_ALL, IID_IADORecordset, (LPVOID*)&rs)));
    wxVERIFY(SUCCEEDED(rs->put_CursorLocation(adUseClient)));
    wxVERIFY(SUCCEEDED(rs->put_CursorType(adOpenForwardOnly)));
    wxVERIFY(SUCCEEDED(rs->put_LockType(adLockReadOnly)));

    // Open it.
    wxVERIFY(SUCCEEDED(m_pHighlight1->put_Value(variant(set))));
    if (FAILED(rs->Open(variant(m_comHighlight), variant(DISP_E_PARAMNOTFOUND, VT_ERROR)))) {
        _ftprintf(stderr, wxT("%s: error ZCC0101: Error loading highlights from database. Please make sure the file is ZRCola.zrc compatible.\n"), m_filename.c_str());
        LogErrors();
        return false;
    }

    return true;
}


bool ZRCola::DBSource::GetHighlight(const com_obj<ADORecordset>& rs, ZRCola::DBSource::highlight& h) const
{
    wxASSERT_MSG(rs, wxT("recordset is empty"));

    com_obj<ADOFields> flds;
    wxVERIFY(SUCCEEDED(rs->get_Fields(&flds)));

    {
        com_obj<ADOField> f;
        wxVERIFY(SUCCEEDED(flds->get_Item(variant(L"komb"), &f)));
        wxCHECK(GetUnicodeString(f, h.chr), false);
    }

    return true;
}
