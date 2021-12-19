/*
 * @Descripttion:
 * @version: 1.0.0
 * @Author: CYKS
 * @Date: 2021-11-29 19:15:41
 * @LastEditors: CYKS
 * @LastEditTime: 2021-12-20 00:13:19
 */
#pragma once

#include <experimental/filesystem>
#include <iostream>
#include <map>
#include <utility>
#include <vector>

namespace fs = std::experimental::filesystem;
using namespace std;

/**
 * @brief 
 * manager search every script file under directory into `_container`.
 * visit script with iterator interface
 * 
 */
class ScriptManager {
 public:
  using iterator = vector<pair<string, fs::path>>::const_iterator;

  /**
   * @brief Construct a new Script Manager object with path.
   * 
   * @param path a file system path to directory
   */
  ScriptManager(fs::path path) : _path(path) {
    if (!fs::is_directory(path)) {
      throw "Path is not direcotry";
    }
    if (!fs::exists(path)) {
      throw "Directory path not exist";
    }
    for (const auto& entry : fs::directory_iterator(path)) {
      fs::path script_path = entry.path();
      if (script_path.extension() != ".sh") continue;

      cout << "+" + (string)script_path.filename() << endl;
      _container.push_back(make_pair(script_path.stem(), script_path));
    }
  }

  iterator begin() { return _container.begin(); }

  iterator end() { return _container.end(); }

 private:
  fs::path _path;
  vector<fs::path> _scriptTable;
  vector<pair<string, fs::path>> _container;
};
