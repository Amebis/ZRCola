/*
    Copyright 2017 Amebis

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
// ZRCola::DecompParser
//////////////////////////////////////////////////////////////////////////

ZRCola::DecompParser::DecompParser(_In_ const std::wstring &decomposed) : m_decomposed(decomposed)
{
    if (m_decomposed.empty()) {
        m_sym = 0;
        m_idx = 0;
    } else {
        m_sym = m_decomposed[0];
        m_idx = 1;
    }
}


void ZRCola::DecompParser::ParsePrefixes(_Inout_ std::vector<wchar_t> &prefix)
{
    for (;;) {
        if (
            m_sym == 0x203F                    || // Undertie
            0xE0F0 <= m_sym && m_sym <= 0xE0F3 || // 1st Set of Prefix Modifiers
            m_sym == 0xE0F6                    || // Above
            m_sym == 0xE0F7                    || // Below
            0xE0F8 <= m_sym && m_sym <= 0xE0FC)   // 2nd Set of Prefix Modifiers
        {
            // Prefix modifier
            prefix.push_back(m_sym);
            NextChar();
        } else
            break;
    }
}


void ZRCola::DecompParser::ParseModifiers(_Inout_ std::vector<wchar_t> &mod)
{
    for (;;) {
        if (0xE000 <= m_sym && m_sym <= 0xE05B || // 1st Set of Modifiers
            0xE063 <= m_sym && m_sym <= 0xE0BE || // 2nd Set of Modifiers
            m_sym == 0x0023                    || // Number Sign
            m_sym == 0x003A                    || // Colon
            m_sym == 0x02DE                    || // Rhotic Hook
            m_sym == 0x2019                    || // Right Single Quotation Mark
            0x02B0 <= m_sym && m_sym <= 0x02FF || // Standard Unicode Modifiers
            m_sym == 0xE0C0                    || // Circle
            m_sym == 0xE0C2                    || // Square
            m_sym == 0xE0CA                    || // Diamond
            m_sym == 0xE0CD)                      // Not Circle
        {
            mod.push_back(m_sym);
            NextChar();
        } else
            break;
    }
}


ZRCola::DecompParser::Expression* ZRCola::DecompParser::ParseParentheses(_In_ wchar_t open, _In_ wchar_t close)
{
    if (m_sym == open) {
        // Parenthesis Start
        NextChar();
        if (m_sym == 0xE037) {
            // EVA Parenthesis End
            unique_ptr<Character> res(new Character(close));
            NextChar();
            return res.release();
        } else {
            unique_ptr<Parentheses> res(new Parentheses(open, close, ParseExpression()));
            if (m_sym == close) {
                // Parenthesis End
                NextChar();
            } else if (m_sym == open && m_idx < m_decomposed.length() && m_decomposed[m_idx] == 0xE037) {
                // Decomposed Parenthesis End
                NextChar();
                NextChar();
            } else
                throw invalid_argument(string_printf("%lc is unexpected. Should end with %lc.", m_sym, close));
            return res.release();
        }
    } else
        return NULL;
}


ZRCola::DecompParser::Expression* ZRCola::DecompParser::ParseCharacter()
{
    Expression *res_p;
    if ((res_p = ParseParentheses(0xE0C5, 0xE0C6)) != NULL || // Superscript
        (res_p = ParseParentheses(0xE0CE, 0xE0CF)) != NULL || // Subscript
        (res_p = ParseParentheses(0xE2E0, 0xE2E1)) != NULL || // EVA Double
        (res_p = ParseParentheses(0xE2E2, 0xE2E3)) != NULL || // EVA Condensed
        (res_p = ParseParentheses(0xE2E4, 0xE2E5)) != NULL || // EVA Emphasis
        (res_p = ParseParentheses(0xE2E6, 0xE2E7)) != NULL || // EVA Strike
        (res_p = ParseParentheses(0xE2E8, 0xE2E9)) != NULL || // EVA Undeal
        (res_p = ParseParentheses(0xE2EA, 0xE2EB)) != NULL || // EVA Italic
        (res_p = ParseParentheses(0xE2EC, 0xE2ED)) != NULL || // EVA Superscript
        (res_p = ParseParentheses(0xE2EE, 0xE2EF)) != NULL)   // EVA Subscript
    {
        return res_p;
    }

    unique_ptr<Character> res(new Character());

    ParsePrefixes(res->m_prefix);

    if (m_sym < 0xE000 || 0xE0FC < m_sym ||
        m_sym == 0xE0D6 ||                      // Clockwise Top Semicircle Arrow
        m_sym == 0xE0D7)                        // Anticlockwise Top Semicircle Arrow
    {
        // Base Character
        res->m_char = m_sym;
        NextChar();
    }

    ParseModifiers(res->m_mod);

    return res.release();
}


ZRCola::DecompParser::Expression* ZRCola::DecompParser::ParseTerm()
{
    unique_ptr<Ligature> res(new Ligature(ParseCharacter()));
    while (m_sym == 0xE0C4) {
        // Ligature
        std::unique_ptr<Ligature::Element> res2(new Ligature::Element(m_sym));
        NextChar();
        ParseModifiers(res2->m_mod);
        res2->m_el.reset(ParseCharacter());
        res->m_rest.push_back(std::move(res2));
    }

    if (res->m_rest.empty()) {
        // A ligature of, well one single character, is rather a character alone.
        return res->m_first.release();
    } else
        return res.release();
}


ZRCola::DecompParser::Expression* ZRCola::DecompParser::ParseExpression()
{
    unique_ptr<Expression> res(ParseTerm());
    if (0xE05C <= m_sym && m_sym <= 0xE062 ||
        m_sym == 0xE0C1                    || // Two Characters in Circle
        m_sym == 0xE0C3                    || // Two Characters in Square
        m_sym == 0xE0C7                    || // Breved below
        m_sym == 0xE0CB                    || // Inverse tilded
        m_sym == 0xE0CC                    || // Diagonally Barred
        0xE0D0 <= m_sym && m_sym <= 0xE0D5)
    {
        // Joiner of two terms
        unique_ptr<Joined2> res_j(new Joined2(m_sym, res.release()));
        NextChar();

        ParseModifiers(res_j->m_mod);

        res_j->m_second.reset(ParseTerm());
        return res_j.release();
    } else if (
        m_sym == 0xE0C8 || // Inverse breved
        m_sym == 0xE0C9)   // Breved below
    {
        // Joiner of three terms
        unique_ptr<Joined3> res_j(new Joined3(m_sym, res.release()));
        NextChar();

        ParseModifiers(res_j->m_mod);

        res_j->m_second.reset(ParseTerm());
        res_j->m_third.reset(ParseTerm());
        return res_j.release();
    } else
        return res.release();
}


ZRCola::DecompParser::Expression* ZRCola::DecompParser::Parse()
{
    unique_ptr<Expression> res(ParseExpression());
    if (m_sym)
        throw invalid_argument("Unexpected trailing character(s).");
    return res.release();
}
