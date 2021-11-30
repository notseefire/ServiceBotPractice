/*
 * @Descripttion: 
 * @version: 1.0.0
 * @Author: CYKS
 * @Date: 2021-11-29 19:39:43
 * @LastEditors: CYKS
 * @LastEditTime: 2021-11-30 15:41:50
 */
#include "gtest/gtest.h"
#include "../parser/script.hpp"
#include "../parser/lexer.hpp"

TEST(Paser, FileManager) {
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
    (l.lex(*p));
}

TEST(Paser, PaserTest) {

}