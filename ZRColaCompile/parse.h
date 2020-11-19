/*
    Copyright 2018-2020 Amebis

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

#include <sal.h>
#include <list>
#include <memory>
#include <string>
#include <vector>


namespace ZRCola {
    ///
    /// Decomposed sequence parser
    ///
    class DecompParser
    {
    public:
        ///
        /// Base class for expressions
        ///
        class Expression
        {
        };


        ///
        /// Expression in "parentheses"
        ///
        class Parentheses : public Expression
        {
        public:
            inline Parentheses(_In_opt_ wchar_t open = 0, _In_opt_ wchar_t close = 0, _In_opt_ Expression *expr = NULL) : m_open(open), m_close(close), m_expr(expr) {}

        public:
            wchar_t m_open, m_close;
            std::unique_ptr<Expression> m_expr;
        };


        ///
        /// Operator
        ///
        class Operator : public Expression
        {
        public:
            inline Operator(_In_opt_ wchar_t chr = 0) : m_char(chr) {}

            wchar_t m_char;
            std::vector<wchar_t> m_mod;
        };


        ///
        /// Character
        ///
        class Character : public Operator
        {
        public:
            inline Character(_In_opt_ wchar_t chr = 0) : Operator(chr) {}

            std::vector<wchar_t> m_prefix;
        };


        ///
        /// Ligature
        ///
        class Ligature : public Expression
        {
        public:
            class Element : public Operator
            {
            public:
                inline Element(_In_opt_ wchar_t chr = 0) : Operator(chr) {}

                std::unique_ptr<Expression> m_el;
            };

            inline Ligature(_In_opt_ Expression *first = NULL) : m_first(first) {}

            std::unique_ptr<Expression> m_first;
            std::list<std::unique_ptr<Element> > m_rest;
        };


        ///
        /// Two joined terms
        ///
        class Joined2 : public Operator
        {
        public:
            Joined2(_In_opt_ wchar_t chr = 0, _In_opt_ Expression *first = NULL) : Operator(chr), m_first(first) {}

            std::unique_ptr<Expression> m_first, m_second;
        };


        ///
        /// Three joined terms
        ///
        class Joined3 : public Joined2
        {
        public:
            Joined3(_In_opt_ wchar_t chr = 0, _In_opt_ Expression *first = NULL) : Joined2(chr, first) {}

            std::unique_ptr<Expression> m_third;
        };


    public:
        DecompParser(_In_ const std::wstring &decomposed);

    protected:
        inline void NextChar();
        void ParsePrefixes(_Inout_ std::vector<wchar_t> &prefix);
        void ParseModifiers(_Inout_ std::vector<wchar_t> &mod);
        Expression* ParseParentheses(_In_ wchar_t open, _In_ wchar_t close);
        Expression* ParseCharacter();
        Expression* ParseTerm();
        Expression* ParseExpression();

    public:
        Expression* Parse();

    protected:
        std::wstring::size_type m_idx;      ///< Index of current character
        const std::wstring &m_decomposed;   ///< Decomposed character sequence to analyse
        wchar_t m_sym;                      ///< Current character
    };
}


inline void ZRCola::DecompParser::NextChar()
{
    m_sym = m_idx < m_decomposed.length() ? m_decomposed[m_idx++] : 0;
}
