/*
 * @Descripttion: 
 * @version: 1.0.0
 * @Author: CYKS
 * @Date: 2021-11-29 19:39:43
 * @LastEditors: CYKS
 * @LastEditTime: 2021-11-29 22:32:10
 */
#include "gtest/gtest.h"
#include "../parser/script.hpp"

TEST(blaTest, test1) {
    EXPECT_EQ(1, 1);
}

TEST(FileLoadingTest, Constructor) {
    fs::path path("/home/cyks/Documents/2021Fall/Homework/Program/assginment/example_scripts");
    auto p = std::make_shared<ScriptManager>(ScriptManager(path));
    auto t = p->begin();
    EXPECT_EQ((*t).first, "scripts2");
    t++;
    EXPECT_EQ((*t).first, "scripts1");
    t++;
    EXPECT_EQ(t, p->end());
}