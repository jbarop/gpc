#include <iostream>
#include <cstdlib>
#include "parser.hpp"

using namespace gpc;

static void error(const std::string& msg) {
#ifdef NYAN_CAT_IS_WATCHING
    std::cout << "ERROR: " << msg << "\n";
#else
    std::cout << "ERROR\n";
#endif
}

int main (int argc, const char* argv[]) {
    
    std::string line;
    while(std::cin) {
        std::getline(std::cin, line);
        
        if (line.length() == 0) {
            break;
        }
        
        try {
            parser parser(tokenizer(line).tokens());
            std::cout << parser.ast()->eval() << "\n";
        } catch(const char* exception) {
            error(exception);
        } catch(const std::string& exception) {
            error(exception);
        }
    };

    return EXIT_SUCCESS;
}

