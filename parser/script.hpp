/*
 * @Descripttion: 
 * @version: 1.0.0
 * @Author: CYKS
 * @Date: 2021-11-29 19:15:41
 * @LastEditors: CYKS
 * @LastEditTime: 2021-11-29 22:47:00
 */
#include <experimental/filesystem>
#include <vector>
#include <map>
#include <iostream>
#include <utility>

namespace fs = std::experimental::filesystem;
using namespace std;

class ScriptManager {
public:
    using iterator = vector<pair<string, fs::path>>::const_iterator;

    ScriptManager(fs::path path): _path(path) {
        if(!fs::is_directory(path)) {
            throw "Path is not direcotry";
        }
        if(!fs::exists(path)) {
            throw "Directory path not exist";
        }
        for (const auto & entry: fs::directory_iterator(path)) {
            fs::path script_path = entry.path();
            if(script_path.extension() != ".sh")
                continue;

            cout << "+" + (string)script_path.filename() << endl;
            _container.push_back(make_pair(script_path.stem(), script_path));
        }
    }

    iterator begin() {
        return _container.begin();
    }
    
    iterator end() {
        return _container.end();
    }


private:
    fs::path _path;
    vector<fs::path> _scriptTable;
    vector<pair<string, fs::path>> _container;
};
