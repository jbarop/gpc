#include <iostream>
#include <cctype>
#include "tokenizer.hpp"

using namespace gpc;

/**
 * Characters which will be trimmed.
 */
static const std::string white_spaces(" \f\n\r\t\v");

/**
 * Returns a new string where the trailing and leading whitspaces are trimmed.
 */
static std::string string_trim(const std::string& str) {
    std::string result = str;
    std::string::size_type pos;
    
    pos = result.find_last_not_of(white_spaces);
    result.erase(pos + 1);
    pos = result.find_first_not_of(white_spaces);
    result.erase(0, pos);
    
    return result;
}

/**
 * Splits the 'source' string at 'delimiter' and stores the substrings in 'result'.
 */
static void string_split(const std::string& source, const std::string& delimiter, std::vector<std::string>& result) {
    size_t  start = 0, end = 0;
    
    while (end != std::string::npos) {
        end = source.find(delimiter, start);
        result.push_back(source.substr(start,(end == std::string::npos) ? std::string::npos : end - start));
        start = ((end > (std::string::npos - delimiter.size())) ?  std::string::npos : end + delimiter.size());
    }
}

/**
 * Returns true if a string only contains digits.
 */
static bool string_isdigit(const std::string& source) {
    for (std::string::const_iterator it = source.begin(); it != source.end(); it++) {
        if (!std::isdigit(*it)) {
            return false;
        }
    }
    
    return true;
}

token::token(enum token_type type, const std::string value)
    : type(type), value(value) {
}

/**
 * Because whitespaces do matter for lexical numbers we first split by all
 * possible operations and then inspect the remaining payload.
 *
 * Iterates recursive over the operation symbol table map.
 */
void tokenizer::tokenize(symbol_iterator_t it, const std::string& input) {
    if (it != operation_symbol_table.end()) {
        // at least one more item in the map left and more splitting needed
        token_type current_type = it->second;
        std::string current_value = it->first;
        std::vector<std::string> split_result;
        string_split(input, current_value, split_result);
        it++;
        for (unsigned int i = 0; i < split_result.size(); i++) {            
            if (i != 0) {
                // if we have more than 1 result than we have a match and
                // we have to remember the operation as token
                m_tokens.push_back(token(current_type, current_value));
            }
            // tokenize the result with the next entry in the operation symbol table
            tokenize(it, split_result[i]);
        }
    } else {
        // no more operations left so we're finnaly got an operand
        std::string operand = string_trim(input);
        
        if (operand.size() != 0) {
            if (string_isdigit(operand)) {
                m_tokens.push_back(token(TOKEN_DIGIT, string_trim(input)));
            } else {
                tokenize_lexical_number(string_trim(input));   
            }   
        }
    }
}

/**
 * A lexical number is splitted by whitespaces.
 *
 * Iterate over all possible lexical number strings for each part of the number.
 */
void tokenizer::tokenize_lexical_number(const std::string& input) {    
    std::vector<std::string> split_result;
    std::vector<std::string>::iterator it;
    string_split(input, " ", split_result);
    for (it = split_result.begin(); it != split_result.end(); it++) {
        if (it->length() != 0) {
            bool found = false;
            for (symbol_iterator_t symbol_it = lexical_number_symbol_table.begin(); symbol_it != lexical_number_symbol_table.end(); symbol_it++) {
                if (*it == symbol_it->first) {
                    m_tokens.push_back(token(symbol_it->second, symbol_it->first));
                    found = true;
                }
            }
            
            if (!found) {
                throw "Unkown token '" + *it + "'";
            }
            
        }
    }
}

tokenizer::tokenizer(const std::string& input) {
   tokenize(operation_symbol_table.begin(), input);

//    for (token_iterator_t it = m_tokens.begin(); it != m_tokens.end(); it++) {
//        std::cout << it->type << " - " << it->value << "\n";
//    }
}

std::vector<token>& tokenizer::tokens() {
    return m_tokens;
}

/**
 * Fill the operation symbol table with values.
 */
static symbol_table_t create_operation_symbol_table()
{
    symbol_table_t result;
    
    result.insert(std::make_pair("+", TOKEN_PLUS));
    result.insert(std::make_pair("plus", TOKEN_PLUS));
    result.insert(std::make_pair("-", TOKEN_MINUS));
    result.insert(std::make_pair("minus", TOKEN_MINUS));
    result.insert(std::make_pair("*", TOKEN_MULTIPLY));
    result.insert(std::make_pair("times", TOKEN_MULTIPLY));
    result.insert(std::make_pair("/", TOKEN_DIVIDE));
    result.insert(std::make_pair("divided by", TOKEN_DIVIDE));    

    return result;
}

/**
 * Fill the lexical number symbol table with values.
 */
static symbol_table_t create_lexical_number_symbol_table()
{
    symbol_table_t result;
    
    result.insert(std::make_pair("zero", TOKEN_LEXICAL_ONNER));
    result.insert(std::make_pair("one", TOKEN_LEXICAL_ONNER));
    result.insert(std::make_pair("two", TOKEN_LEXICAL_ONNER));
    result.insert(std::make_pair("three", TOKEN_LEXICAL_ONNER));
    result.insert(std::make_pair("four", TOKEN_LEXICAL_ONNER));
    result.insert(std::make_pair("five", TOKEN_LEXICAL_ONNER));
    result.insert(std::make_pair("six", TOKEN_LEXICAL_ONNER));
    result.insert(std::make_pair("seven", TOKEN_LEXICAL_ONNER));
    result.insert(std::make_pair("eight", TOKEN_LEXICAL_ONNER));
    result.insert(std::make_pair("nine", TOKEN_LEXICAL_ONNER));
    
    result.insert(std::make_pair("ten", TOKEN_LEXICAL_TEENS));
    result.insert(std::make_pair("eleven", TOKEN_LEXICAL_TEENS));
    result.insert(std::make_pair("twelve", TOKEN_LEXICAL_TEENS));
    result.insert(std::make_pair("thirteen", TOKEN_LEXICAL_TEENS));
    result.insert(std::make_pair("fourteen", TOKEN_LEXICAL_TEENS));
    result.insert(std::make_pair("fifteen", TOKEN_LEXICAL_TEENS));
    result.insert(std::make_pair("sixteen", TOKEN_LEXICAL_TEENS));
    result.insert(std::make_pair("seventeen", TOKEN_LEXICAL_TEENS));
    result.insert(std::make_pair("eighteen", TOKEN_LEXICAL_TEENS));
    result.insert(std::make_pair("nineteen", TOKEN_LEXICAL_TEENS));
    
    result.insert(std::make_pair("twenty", TOKEN_LEXICAL_TENNER));
    result.insert(std::make_pair("thirty", TOKEN_LEXICAL_TENNER));
    result.insert(std::make_pair("forty", TOKEN_LEXICAL_TENNER));
    result.insert(std::make_pair("fifty", TOKEN_LEXICAL_TENNER));
    result.insert(std::make_pair("sixty", TOKEN_LEXICAL_TENNER));
    result.insert(std::make_pair("seventy", TOKEN_LEXICAL_TENNER));
    result.insert(std::make_pair("eighty", TOKEN_LEXICAL_TENNER));
    result.insert(std::make_pair("ninety", TOKEN_LEXICAL_TENNER));
    
    result.insert(std::make_pair("hundred", TOKEN_LEXICAL_MULTIPLIER));
    result.insert(std::make_pair("thousand", TOKEN_LEXICAL_MULTIPLIER));
    result.insert(std::make_pair("million", TOKEN_LEXICAL_MULTIPLIER));
    
    result.insert(std::make_pair("and", TOKEN_LEXICAL_AND));
    
    return result;
}

symbol_table_t tokenizer::operation_symbol_table = create_operation_symbol_table();
symbol_table_t tokenizer::lexical_number_symbol_table = create_lexical_number_symbol_table();
