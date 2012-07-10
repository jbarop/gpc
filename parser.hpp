//
//  parser.hpp
//  gpc
//
//  Created by Johannes Barop on 19.12.11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#ifndef __GPC_PARSER_HPP_INCLUDED__
#define __GPC_PARSER_HPP_INCLUDED__

#include "tokenizer.hpp"
#include "ast.hpp"

namespace gpc {

    /**
     * Convient typedef for easier access to the number table map type.
     */
    typedef std::map<std::string, int> number_table_t;

    /**
     * Convient typedef for easier access to the number table map iterator type.
     */
    typedef number_table_t::const_iterator number_iterator_t;

    /**
     * Transforms a list of token to a syntax tree which can be evaluated to calculate the result.
     */
    class parser {
    public:

        /**
         * Construct a new parser by parsing the given tokens.
         */
        parser(token_vector_t& tokens);

        /**
         * Cleanup.
         */
        ~parser();

        /**
         * Return the syntax tree.
         */
        node* ast();
        
    private:

        /**
         * Map of lexical numbers to thier integer value
         */
        static number_table_t number_table;

        /**
         * List of tokens to analyize.
         */
        token_vector_t m_tokens;

        /**
         * Current active token.
         */
        token_iterator_t m_current_token;

        /**
         * Root node of the syntax tree.
         */
        node* m_root;

        /**
         * Grammer: Parse a calculator expression.
         */
        node* parse_expression();

        /**
         * Grammer: Parse a term (+|-).
         */
        node* parse_term();

        /**
         * Grammer: Parse a factor (*|/)
         */
        node* parse_factor();

        /**
         * Grammer: Parse a digital number (0123456789).
         */
        int parse_digit_number();

        /**
         * Grammer: Parse a lexical number (one hundred and seven).
         */
        int parse_lexical_number();

        /**
         * Grammer: Parse a lexical numeric without multipliers like 'hundred'.
         */
        int parse_lexical_onner_or_teenie_or_tenner();

        /**
         * Grammer: Parse a lexical numeric (1-19).
         */
        bool parse_lexical_onner_or_teenie(int& result);

        /**
         * Grammer: Parse a lexical numeric (20-99).
         */
        bool parse_lexical_tenner(int& result);

    };

}


#endif //__GPC_PARSER_HPP_INCLUDED__
