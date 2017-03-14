#ifndef SHAKA_PARSER_TOKENIZER_H
#define SHAKA_PARSER_TOKENIZER_H

namespace shaka {

class Tokenizer {
public:
    Tokenizer (std::istream& in) :
        in(in) {}

    Token get () {
        if (!tokens.empty()) {
            auto front = tokens.front();
            tokens.pop_front();
            return front;
        } else {
            try {
                this->read_next_token();
            } catch (const char* c) {
                std::cerr << c << std::endl;
                throw c;
            }
            auto front = tokens.front();
            tokens.pop_front();
            return front;
        }
    }

    void unget (Token t) {
        tokens.push_front(t);
    }

    void read_next_token () {
        Token t = parse_token();
        std::cout << "Parsed: " << static_cast<int>(t.type) << " :: " << t.str << std::endl;
        tokens.push_back(t); 
    }

    Token parse_paren_start() {
        // Begin parenthesis
        if (in.peek() == '(') {
            in.get();
            return Token(Token::Type::PAREN_START, "(");
        } else {
            throw std::runtime_error("Tokenizer: Could not parse Token.PAREN_START");
        }
    }

    Token parse_paren_end() {
        if (in.peek() == ')') {
            in.get();
            return Token(Token::Type::PAREN_END, ")");
        } else {
            throw std::runtime_error("Tokenizer: Could not parse Token.PAREN_END");
        }
    }

    Token parse_string() {
        // String
        if (in.peek() == '\"') {
            std::string buffer;
            // Read in the rest of the string.
            in.get();
            while (in.peek() != '\"') {
                buffer += in.peek();
                in.get();
            } in.get();
            return Token(Token::Type::STRING, buffer);
        } else {
            throw std::runtime_error("Tokenizer: Could not parse Token.STRING");
        }
    }

    void parse_line_comment() {
        std::cout << "What LINE COMMENT is this? \'" << in.peek() << "\'" << std::endl;
        // If it's a comment, read until the
        // end of the lie.
        while (in.peek() != '\n') {
            in.get();
        } in.get();
    }

    bool rule_hash(Token& result) {
        if (in.peek() == '#') {
            in.get();

            // <character>
            if (in.peek() == '\\') {
                in.get();
                std::cout << "in character" << std::endl;

                // what type of escape?
                if (std::isalpha(in.peek())) {
                    // If the first letter is 'x', then possibly hex digit
                    if (in.peek() == 'x') {
                        in.get();

                        // If the next character is a hex digit,
                        // then go into hex scalar value.
                        if (is_hex_digit(in.peek())) {
                            std::string buffer;
                            while(is_hex_digit(in.peek())) {
                                buffer += in.get();
                            }
                            int i = std::stoi(buffer, 0, 16);
                            buffer.clear();
                            buffer += static_cast<char>(i);
                            result = Token(Token::Type::CHARACTER, buffer);
                            return true;

                        // Otherwise, it's just a literal 'x'
                        } else {
                            result = Token(Token::Type::CHARACTER, "x"); 
                            return true;
                        }

                    // Otherwise, it's going to either be a single character escape
                    // or a named character escape.
                    } else {
                        std::cout << "in single/named char escape" << std::endl;
                        std::string buffer;
                        while (!is_delimiter(in.peek()) && in.peek() != EOF) {
                            buffer += in.get();
                        }
                        std::cout << "buffer: " << buffer << std::endl;
                        std::cout << "buffer.size(): " << buffer.size() << std::endl;
                        // single character escape?
                        if (buffer.size() == 1) {
                            result = Token(Token::Type::CHARACTER, buffer);

                        // named character escape?
                        } else if (buffer == "alarm") {
                            result = Token(Token::Type::CHARACTER, "\a");
                            return true;
                        } else if (buffer == "backspace") {
                            result = Token(Token::Type::CHARACTER, "\b");
                            return true;
                        } else if (buffer == "delete") {
                            result = Token(Token::Type::CHARACTER, "\x7F");
                            return true;
                        } else if (buffer == "escape") {
                            result = Token(Token::Type::CHARACTER, "\x1B");
                            return true;
                        } else if (buffer == "newline") {
                            result = Token(Token::Type::CHARACTER, "\n");
                            return true;
                        } else if (buffer == "null") {
                            result = Token(Token::Type::CHARACTER, "\0");
                            return true;
                        } else if (buffer == "return") {
                            result = Token(Token::Type::CHARACTER, "\r");
                            return true;
                        } else if (buffer == "space") {
                            result = Token(Token::Type::CHARACTER, " ");
                            return true;
                        } else if (buffer == "tab") {
                            result = Token(Token::Type::CHARACTER, "\t");
                            return true;

                        // Otherwise, error!
                        } else {
                            throw "Parser.parse_token: Bad character escape";
                            result = Token(Token::Type::END_OF_FILE);
                            return false;
                        }
                    } // character escape

                // <boolean> #t or #true? 
            } else if (in.peek() == 't') {

                std::string buffer;
                while (std::isalpha(in.peek())) {
                    buffer += in.get();
                }
                if (buffer == "true" || buffer == "t") {
                    result = Token(Token::Type::BOOLEAN_TRUE, "#t");
                    return true;
                } else {
                    throw "Parser.parse_token: invalid hash identifier/boolean does not match to true";
                    result = Token(Token::Type::END_OF_FILE);
                    return false;
                }
            // <boolean> #f or #false? 
            } else if (in.peek() == 'f') {
                std::string buffer;
                while (std::isalpha(in.peek())) {
                    buffer += in.get();
                }
                if (buffer == "false" || buffer == "f") {
                    result = Token(Token::Type::BOOLEAN_FALSE, "#f");
                    return true;
                } else {
                    throw "Parser.parse_token: invalid hash identifier/boolean does not match to true";
                    result = Token(Token::Type::END_OF_FILE);
                    return false;
                }
            // Nested comment must keep track
            // of depth.
            } else if (in.peek() == '|') {
                int depth_count = 1;
                while (depth_count > 0) {
                    if (in.peek() == '|') {
                        in.get();
                        if (in.peek() == '#') {
                            in.get();
                            depth_count--;
                        }
                    } else if (in.peek() == '#') {
                        in.get();
                        if (in.peek() == '|') {
                            in.get();
                            depth_count++;
                        }
                    } else {
                        std::cout << "What is this? \'" << in.peek() << "\'" << std::endl;
                        in.get();
                    }
                }
                return false;

            // Single datum comment.
            } else if (in.peek() == ';') {
                in.get();
                result = Token(Token::Type::DATUM_COMMENT, "#;");
                return true;

            // Directive
            } else if (in.peek() == '!') {
                in.get();
                
                // Read in the directive string.
                std::string buffer;
                while (!is_delimiter(in.peek())) {
                    buffer += in.get(); 
                }
                // fold-case directive?
                if (buffer == "fold-case") {
                    result = Token(Token::Type::DIRECTIVE, "fold-case");
                    return true;
                // no-fold-case directive?
                } else if (buffer == "no-fold-case") {
                    result = Token(Token::Type::DIRECTIVE, "no-fold-case");
                    return true;
                } else {
                    std::cerr << "BROKE ON: " << in.peek() << std::endl;
                    throw "Parser.parse_token: invalid directive";
                    result = Token(Token::Type::END_OF_FILE);
                    return false;
                }
            } else {
                std::cerr << "BROKE ON: " << in.peek() << std::endl;
                throw "Parser.parse_token: invalid hash directive";
                result = Token(Token::Type::END_OF_FILE);
                return false;
            }
        } else {
            std::cerr << "BROKE ON: " << in.peek() << std::endl;
            throw "Parser.parse_token: is not a hash? wrong";
            result = Token(Token::Type::END_OF_FILE);
            return false;

            }
        }
    }

    Token parse_token () {
        bool done = false;
        while (!done) {
            // Begin parenthesis
            if (in.peek() == '(') {
                return parse_paren_start();

            // End parenthesis
            } else if (in.peek() == ')') {
                return parse_paren_end();

            // String
            } else if (in.peek() == '\"') {
                return parse_string();

            // Line comment
            } else if (in.peek() == ';') {
                parse_line_comment();
                // Then, continue and get next token to parse.
                done = false;

            // End of file
            } else if (in.peek() == EOF) {
                std::cout << "What EOF is this? \'" << in.peek() << "\'" << std::endl;
                in.get();
                return Token(Token::Type::END_OF_FILE);

            // Skip whitespace
            } else if (std::isspace(in.peek())) {
                std::cout << "What space is this? \'" << in.peek() << "\'" << std::endl;
                in.get();
                done = false;

            // Comment or boolean begins with #
            } else if (in.peek() == '#') {
                Token result(Token::Type::END_OF_FILE);
                if (rule_hash(result)) {
                    return result;
                } else {
                    done = false;
                }

            // Identifier ==> <initial> <subsequent>*
            } else if (is_initial(in.peek())) {
                std::cout << "What INITIAL is this? \'" << in.peek() << "\'" << std::endl;
                std::string buffer;
                buffer += in.get();
                // Keep getting subsequent if needed.
                while (is_subsequent(in.peek())) {
                    buffer += in.get();
                }
                return Token(Token::Type::IDENTIFIER, buffer);

            // Identifier ==> <vertical line> <symbol element>* <vertical line>
            } else if (in.peek() == '|') {
                in.get();
                std::string buffer;
                while (handle_symbol_element(in, buffer));
                if (in.peek() == '|') {
                    in.get();
                    return Token(Token::Type::IDENTIFIER, buffer);
                } else {
                    throw "Parser.parse_token: invalid pipe-delimted identifier syntax";
                    return Token(Token::Type::END_OF_FILE);
                }

            // Identifier ==> <explicit sign> ...
            } else if (is_explicit_sign(in.peek())) {
                std::string buffer;
                buffer += in.get();
                // <sign subsequent> <subsequent>*
                if (is_sign_subsequent(in.peek())) {
                    buffer =+ in.get();
                    while (is_subsequent(in.peek())) {
                        buffer += in.get(); 
                    }
                    return Token(Token::Type::IDENTIFIER, buffer);
                // . <dot subsequent> <subsequent>*
                } else if (in.peek() == '.') {
                    buffer += in.get();
                    if (is_dot_subsequent(in.peek())) {
                        buffer += in.get();
                        while (is_subsequent(in.peek())) {
                            buffer += in.get(); 
                        }
                        return Token(Token::Type::IDENTIFIER, buffer);
                    // no <dot subsequent> ==> error!
                    } else {
                        throw "Parser.parse_token: bad dot subsequent identifier";
                        return Token(Token::Type::END_OF_FILE);
                    }

                // No other identifier? Just <explicit sign> is fine.
                } else {
                    return Token(Token::Type::IDENTIFIER, buffer);
                }
            
            // Identifier ==> . <dot subsequent> <subsequent>
            } else if (in.peek() == '.') {
                std::string buffer;
                buffer += in.get();
                if (is_dot_subsequent(in.peek())) {
                    buffer += in.get();
                    while (is_subsequent(in.peek())) {
                        buffer += in.get();
                    }
                    return Token(Token::Type::IDENTIFIER, buffer);
                } else {
                    throw "Parser.parse_token: bad dot subsequent identifier";
                    return Token(Token::Type::END_OF_FILE);
                }

            // NOT A VALID TOKEN!!!
            } else {
                std::cerr << "BROKE ON: " << in.peek() << std::endl;
                throw "Parser.parse_token: invalid token";
                return Token(Token::Type::END_OF_FILE);
            }
        }
        throw 1;
        return Token(Token::Type::END_OF_FILE);
    }

    bool is_delimiter(char c) {
        return (
            std::isspace(c)
            || c == '|'
            || c == '('
            || c == ')'
            || c == '\"'
            || c == ';'
        );
    }

    bool is_special_initial (char c) {
        return (
            c == '!' ||
            c == '$' ||
            c == '%' ||
            c == '&' ||
            c == '*' ||
            c == '/' ||
            c == '!' ||
            c == ':' ||
            c == '>' ||
            c == '=' ||
            c == '>' ||
            c == '?' ||
            c == '^' ||
            c == '_' ||
            c == '!'
        );
    }

    bool is_special_subsequent (char c) {
        return (
            c == '.'
            || c == '@'
            || is_explicit_sign(c)
        );
    }

    bool is_explicit_sign (char c) {
        return (c == '+' || c == '-');
    }

    bool is_letter (char c) {
        return (std::isalpha(c));
    }

    bool is_subsequent (char c) {
        return (is_initial(c)
            || is_digit(c)
            || is_special_subsequent(c));
    }

    bool is_initial (char c) {
        return (is_letter(c) || is_special_initial(c));
    }

    bool is_digit (char c) {
        return (std::isdigit(c));
    }

    bool handle_symbol_element(std::istream& in, std::string& interm) {

        // Terminating symbol
        if (in.peek() == '|') {
            return false;

        // Escape sequence
        } else if (in.peek() == '\\') {
            in.get();
            // inline_hex_escape?
            if (in.peek() == 'x') {
                in.get();
                return(handle_inline_hex_escape(in, interm));

            // mnemonic_escape?
            } else if (in.peek() == 'a') {
                in.get();
                interm += '\a';
                return true;
            } else if (in.peek() == 'b') {
                in.get();
                interm += '\b';
                return true;
            } else if (in.peek() == 't') {
                in.get();
                interm += '\t';
                return true;
            } else if (in.peek() == 'n') {
                in.get();
                interm += '\n';
                return true;
            } else if (in.peek() == 'r') {
                in.get();
                interm += '\r';
                return true;

            // escape for pipe character?
            } else if (in.peek() == '|') {
                in.get();
                interm += '|';
                return true;

            // otherwise, invalid!
            } else {
                throw "Parser.mnemonic_escape: invalid escape character";
                return false;
            }

        // Otherwise, just consume the next character.
        } else {
            interm += in.get();
            return true;
        }
    }

    bool is_dot_subsequent(char c) {
        return (is_sign_subsequent(c) || c == '.');
    }

    bool handle_inline_hex_escape(std::istream& in, std::string& interm) {
        char c = in.peek();
        std::string buffer;
        while (
            std::isdigit(c)
            || c == 'a'
            || c == 'b'
            || c == 'c'
            || c == 'd'
            || c == 'e'
            || c == 'f'
        ) {
            in.get(); 
            buffer += c;
            c = in.peek(); 
        }
        // Correct form ==> push character onto interm string.
        if (in.peek() == ';') {
            int i = std::stoi(buffer, 0, 16);
            interm += static_cast<char>(i);
            return true;

        // incorect form ==> error!
        } else {
            throw "Parser.handle_inline_hex_escape: no terminating \';\' character";
            return false;
        }
    }

    bool is_sign_subsequent(char c) {
        return (
            is_initial(c)
            || is_explicit_sign(c)
            || c == '@'
        );
    }

    bool is_hex_digit(char c) {
        return (
            std::isdigit(c)
            || c == 'a'
            || c == 'b'
            || c == 'c'
            || c == 'd'
            || c == 'e'
            || c == 'f'
        );
    }

    friend std::ostream& operator<< (std::ostream&, Token);

private:
    std::istream&     in;
    std::deque<Token> tokens;
};

std::ostream& operator<< (std::ostream& out, Token rhs) {
    out << "Token(" << static_cast<int>(rhs.type) << "," << rhs.str << ")";
    return out;
}

} // namespace shaka

#endif // SHAKA_PARSER_TOKENIZER_H