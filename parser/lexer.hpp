/*
 * @Descripttion: 
 * @version: 1.0.0
 * @Author: CYKS
 * @Date: 2021-11-29 21:07:58
 * @LastEditors: CYKS
 * @LastEditTime: 2021-11-29 22:58:43
 */

#include <vector>
#include <map>
#include "token.hpp"
#include "script.hpp"
#include <memory>

using namespace std;

class Lexer {
public:
	using lookup_table = map<string, reserved_token>;
	using token_stream = vector<Token>;
	Lexer(unique_ptr<lookup_table> &keyword_table, unique_ptr<lookup_table> &operator_table);

	map<string, token_stream> lex(ScriptManager& manager);

private:
	unique_ptr<lookup_table> _keyword_table, _operator_table;
	map<string, token_stream> _token_table;
	
	token_stream lex_script(const fs::path& script_path);
};