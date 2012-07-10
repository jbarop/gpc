#ifndef __GPC_TOKENIZER_HPP_INCLUDED__
#define __GPC_TOKENIZER_HPP_INCLUDED__

#include <string>
#include <vector>
#include <map>

namespace gpc {

    /**
     * Diferent kinds of tokens.
     */
    enum token_type {
        /**
         * '-' operation.
         *
         * Also used for the unary -!
         */
        TOKEN_MINUS,

        /**
         * '+' operation.
         */
        TOKEN_PLUS,

        /**
         * '*' operation.
         */
        TOKEN_MULTIPLY,

        /**
         * '/' operation.
         */
        TOKEN_DIVIDE,

        /**
         * A number containing digits.
         *
         * For example 123.
         */
        TOKEN_DIGIT,

        /**
         * Lexical numbers from 'zero' to 'nine'.
         */
        TOKEN_LEXICAL_ONNER,

        /**
         * Lexical numbers from 'ten' to 'nineteen'.
         */
        TOKEN_LEXICAL_TEENS,

        /**
         * Lexical tenners.
         *
         * For example 'twenty', 'thirty', 'forty', ...
         */
        TOKEN_LEXICAL_TENNER,

        /**
         * Lexical multipliers.
         *
         * For example 'hundred', 'thousand', ...
         */
        TOKEN_LEXICAL_MULTIPLIER,

        /**
         * 'and' string in english numbers.
         *
         * For example: five hundred *and* six.
         */
        TOKEN_LEXICAL_AND
    };

    /**
     * Convient typedef for easier access to the symbol table map type.
     */
    typedef std::map<const char*, token_type> symbol_table_t;

    /**
     * Convient typedef for easier access to the symbol table map iterator type.
     */
    typedef symbol_table_t::const_iterator symbol_iterator_t;

    /**
     * A token from the input string.
     *
     * The input string is splitted into tokens to be analyzed by the parser.
     */
    struct token {
        token(enum token_type type, std::string value);
        enum token_type type;
        std::string value;
    };

    /**
     * Convient typedef for easier access to the token vector type.
     */
    typedef std::vector<token> token_vector_t;

    /**
     * Convient typedef for easier access to the token vector iterator type.
     */
    typedef token_vector_t::iterator token_iterator_t;

    /**
     * Responsible for splitting the input string in a list of tokens.
     */
    class tokenizer {
    public:

        /**
         * Construct a new tokenizer by tokenizing the given string.
         */
        tokenizer(const std::string& input);

        /**
         * Get a reference to the token list.
         */
        std::vector<token>& tokens();

    private:
        /**
         * Map of operations (plus, minus, etc) to their token type.
         */
        static symbol_table_t operation_symbol_table;

        /**
         * Map of lexical numbers strings to thier token type.
         */
        static symbol_table_t lexical_number_symbol_table;
        std::vector<token> m_tokens;

        void tokenize(symbol_iterator_t it, const std::string& input);
        void tokenize_lexical_number(const std::string& input);
    };

}

#endif //__GPC_TOKENIZER_HPP_INCLUDED__
