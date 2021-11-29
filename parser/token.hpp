/*
 * @Descripttion: 
 * @version: 1.0.0
 * @Author: CYKS
 * @Date: 2021-11-29 18:44:06
 * @LastEditors: CYKS
 * @LastEditTime: 2021-11-29 21:43:07
 */

#include <string>
#include <variant>

using namespace std;

enum struct reserved_token {
    IF, FI, ECHO, ASSIGN,
    ELIF, ELSE, DONE, DO, THEN,
    EQUAL, CALL,
};

struct identifier {
    string _value;
};

class Token {
private:
    using token_value = variant<string, reserved_token, identifier>;

    token_value _value;
    size_t _line;
    size_t _number;
public:
    Token(token_value token, size_t line, size_t number): _value(token), _line(line), _number(number) {}

    bool is_string();
    bool is_reserved_token();
    bool is_identifier();
    string get_string();
    identifier get_id();
    reserved_token get_token();
};