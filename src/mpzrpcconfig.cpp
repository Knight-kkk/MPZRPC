#include "mpzrpcconfig.hpp"
#include <string>
#include <iostream>
using namespace std;

// trim函数去除字符串头尾空格
void trim(string& str){
    int idx = str.find_first_not_of(' ');
    if(idx > 0){
        // 这一行开始位置存在空格
        str = str.substr(idx, str.size() - idx);
    }
    idx = str.find_last_not_of(' ');
    if(idx != -1){
        // 行结束位置存在空格
        str = str.substr(0, idx + 1);
    }    
}


void MprpcConfig::loadConfig(char * configfile){
    FILE* fp = fopen(configfile, "r");
    if(fp == nullptr){
        std::cerr << "config file doew not exist!\n";
        exit(EXIT_FAILURE);
    }

    while(!feof(fp)){
        // 按照行读取文件，区分三种情况：
        // 第一，注释行
        // 第二，配置项
        // 第三，无效行
        char line[101] = {0};
        fgets(line, 101, fp);
        string sline(line);
        trim(sline);
        // cout << "line:" << sline << endl;
        // 注释行或者无效行
        if(sline.size() == 0 || sline[0] == '#'){
            continue;
        }

        // 配置行
        int idx = sline.find('=');
        if(idx == -1){
            continue;
        }
        string key = sline.substr(0, idx);
        trim(key);
        string value = sline.substr(idx + 1, sline.size() - idx);
        trim(value);
        if((idx = value.find('\n')) != -1){
            value= value.substr(0, idx);
        }
        cout << "key:" << key << ",value:" << value << endl;
        _configMap[key] = value;
    }
}


string MprpcConfig::load(const string& key){
    auto it = _configMap.find(key);
    if(it == _configMap.end()){
        return "";
    }
    return it -> second;
}

