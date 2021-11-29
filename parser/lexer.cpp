/*
 * @Descripttion: 
 * @version: 1.0.0
 * @Author: CYKS
 * @Date: 2021-11-29 21:07:50
 * @LastEditors: CYKS
 * @LastEditTime: 2021-11-29 23:03:59
 */

#include "lexer.hpp"

using namespace std;

Lexer::Lexer(unique_ptr<lookup_table> &keyword_table,
	unique_ptr<lookup_table> &operator_table) :
	_keyword_table(move(keyword_table)),
	_operator_table(move(operator_table)) {
}

map<string, Lexer::token_stream> Lexer::lex(ScriptManager& manager) {
	for (auto t = manager.begin(); t != manager.end(); t++) {
		Lexer::token_stream stream = lex_script((*t).second);
		_token_table[static_cast<string>((*t).second)] = stream;
	}
}

Lexer::token_stream Lexer::lex_script(const fs::path& script_path) {

}