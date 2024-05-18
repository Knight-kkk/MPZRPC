#pragma once
#include <unordered_map>
using namespace std;
#include <string>


// mprpcapplication配置类
class MprpcConfig{
public:
    // 从配置文件中加载键值对
    void loadConfig(char * configfile);
    // 根据key查找value
    string load(const string& key);

private:
    unordered_map<string, string> _configMap;
};