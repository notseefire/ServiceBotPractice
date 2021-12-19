/*
 * @Descripttion: 
 * @version: 1.0.0
 * @Author: CYKS
 * @Date: 2021-11-29 19:39:43
 * @LastEditors: CYKS
 * @LastEditTime: 2021-12-19 11:56:50
 */
#include "gtest/gtest.h"
#include "../parser/script.hpp"
#include "../parser/parser.hpp"
#include "../parser/lexer.hpp"

TEST(Parser, FileManager) {
    fs::path path("/home/cyks/Documents/2021Fall/Homework/Program/assginment/example_scripts");
    auto p = std::make_shared<ScriptManager>(ScriptManager(path));
    auto t = p->begin();
    EXPECT_EQ((*t).first, "scripts2");
    t++;
    EXPECT_EQ((*t).first, "scripts1");
    t++;
    EXPECT_EQ(t, p->end());
}

TEST(Parser, LexerTest) {
    fs::path path("/home/cyks/Documents/2021Fall/Homework/Program/assginment/example_scripts");
    auto p = std::make_shared<ScriptManager>(ScriptManager(path));
    LookupTableFactory factory;
    auto keywords = factory.get_keyword_table();
    auto operators = factory.get_operator_table();
    Lexer l(keywords, operators);
    auto token_streams = l.lex(*p);
}

TEST(Parser, PaserTest) {
    fs::path path("/home/cyks/Documents/2021Fall/Homework/Program/assginment/example_scripts");
    auto p = std::make_shared<ScriptManager>(ScriptManager(path));
    LookupTableFactory factory;
    auto keywords = factory.get_keyword_table();
    auto operators = factory.get_operator_table();
    Lexer l(keywords, operators);
    auto token_streams = l.lex(*p);
    Parser parser;
    try {
        auto token_table = parser.parser(token_streams);
        for(auto index = token_table.begin(); index != token_table.end(); index++) {
            std::cout << index->first << std::endl;
        }
    } catch (const char *e) {
        std::cout << e << std::endl;
    }
}