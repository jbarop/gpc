#include <iostream>
#include <sstream>
#include "parser.hpp"

using namespace gpc;

parser::parser(token_vector_t& tokens) : m_tokens(tokens), m_current_token(m_tokens.begin()), m_root(parse_expression())  {    
    if (m_current_token != m_tokens.end()) {
        throw "Expected EOL|+|- but got '" + m_current_token->value + "'";
    }
}

parser::~parser() {
    delete m_root;
}

node* parser::ast() {
    return m_root;
}

node* parser::parse_expression() {
    node* result = parse_term();

    while (m_current_token != m_tokens.end()) {
        if (m_current_token->type == TOKEN_PLUS) {
            m_current_token++;
            result = new add_op_node(result, parse_term());
        } else if (m_current_token->type == TOKEN_MINUS)  {
            m_current_token++;
            result = new sub_op_node(result, parse_term());
        } else {
            return result;
        }
    }

    return result;
}

node* parser::parse_term() {
    node* result = parse_factor();

    while (m_current_token != m_tokens.end()) {
        if (m_current_token->type == TOKEN_MULTIPLY) {
            m_current_token++;
            result = new mul_op_node(result, parse_factor());
        } else if (m_current_token->type == TOKEN_DIVIDE) {
            m_current_token++;
            result = new div_op_node(result, parse_factor());
        } else {
            return result;
        }
    }

    return result;
}

node* parser::parse_factor() {
    if (m_current_token == m_tokens.end()) {
        throw "Expected a number but got EOL";
    }

    if (m_current_token->type == TOKEN_MINUS) {
        m_current_token++;
        return new unary_minus_node(parse_factor());
    } else if (m_current_token->type == TOKEN_DIGIT) {
        return new number_node(parse_digit_number());
    } else {
        return new number_node(parse_lexical_number());
    }
}

int parser::parse_digit_number() {    
    int result;
    std::stringstream ss(m_current_token->value);
    ss >> result;
    m_current_token++;

    return result;
}

int parser::parse_lexical_number() {
    int result = parse_lexical_onner_or_teenie_or_tenner();

    while (m_current_token != m_tokens.end() && m_current_token->type == TOKEN_LEXICAL_MULTIPLIER) {
        result *= number_table[m_current_token->value];
        m_current_token++;
    }

    if ((m_current_token->type == TOKEN_LEXICAL_AND) || (m_current_token->type == TOKEN_LEXICAL_ONNER) || (m_current_token->type == TOKEN_LEXICAL_TEENS) || (m_current_token->type == TOKEN_LEXICAL_TENNER)) {
        if (m_current_token->type == TOKEN_LEXICAL_AND) {
            m_current_token++; // skip the 'and'
        }
        result += parse_lexical_number();
    }

    return result;
}

int parser::parse_lexical_onner_or_teenie_or_tenner() {
    int result;

    if (!parse_lexical_onner_or_teenie(result)) {
        if (!parse_lexical_tenner(result)) {
            throw "Expected lexical number";
        }
    }

    return result;
}

bool parser::parse_lexical_onner_or_teenie(int& result) {
    if (m_current_token != m_tokens.end() && (m_current_token->type == TOKEN_LEXICAL_ONNER || m_current_token->type == TOKEN_LEXICAL_TEENS)) {
        result = number_table[m_current_token->value];
        m_current_token++;
        return true;
    }

    return false;
}

bool parser::parse_lexical_tenner(int& result) {
    if (m_current_token != m_tokens.end() && m_current_token->type == TOKEN_LEXICAL_TENNER) {
        result = number_table[m_current_token->value];
        m_current_token++;
        if (m_current_token != m_tokens.end() && m_current_token->type == TOKEN_LEXICAL_ONNER) {
            int onner = number_table[m_current_token->value];
            m_current_token++;
            if (onner == 0) {
                throw "Expected one|two|three|... but got zero.";
            }
            result += onner;
        }
        return true;
    }

    return false;
}

/**
 * Fill the number table with values.
 */
static number_table_t create_number_table()
{
    number_table_t result;

    result.insert(std::make_pair("zero", 0));
    result.insert(std::make_pair("one", 1));
    result.insert(std::make_pair("two", 2));
    result.insert(std::make_pair("three", 3));
    result.insert(std::make_pair("four", 4));
    result.insert(std::make_pair("five", 5));
    result.insert(std::make_pair("six", 6));
    result.insert(std::make_pair("seven", 7));
    result.insert(std::make_pair("eight", 8));
    result.insert(std::make_pair("nine", 9));

    result.insert(std::make_pair("ten", 10));
    result.insert(std::make_pair("eleven", 11));
    result.insert(std::make_pair("twelve", 12));
    result.insert(std::make_pair("thirteen", 13));
    result.insert(std::make_pair("fourteen", 14));
    result.insert(std::make_pair("fifteen", 15));
    result.insert(std::make_pair("sixteen", 16));
    result.insert(std::make_pair("seventeen", 17));
    result.insert(std::make_pair("eighteen", 18));
    result.insert(std::make_pair("nineteen", 19));

    result.insert(std::make_pair("twenty", 20));
    result.insert(std::make_pair("thirty", 30));
    result.insert(std::make_pair("forty", 40));
    result.insert(std::make_pair("fifty", 50));
    result.insert(std::make_pair("sixty", 60));
    result.insert(std::make_pair("seventy", 70));
    result.insert(std::make_pair("eighty", 80));
    result.insert(std::make_pair("ninety", 90));

    result.insert(std::make_pair("hundred", 100));
    result.insert(std::make_pair("thousand", 1000));
    result.insert(std::make_pair("million", 1000000));

    result.insert(std::make_pair("and", TOKEN_LEXICAL_AND));

    return result;
}

number_table_t parser::number_table = create_number_table();
