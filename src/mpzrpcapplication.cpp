#include "mpzrpcapplication.hpp"
#include <iostream>
using namespace std;
#include <unistd.h>


MprpcConfig MprpcApplication::_config;

void help(){
    std::cout << "Usage: ./main [config file]\n";
    exit(-EXIT_FAILURE);
}
// 初始化函数
void MprpcApplication::init(int argc, char ** argv){
    
    if(argc < 3){
        help();
    }
    
    int  c = -1;
    while((c = getopt(argc, argv, "c:")) != -1){
        switch (c)
        {
        case 'c':   //config
            //  load config file
            cout << "config:" << optarg << endl;
            _config.loadConfig(optarg);
            break;
        case '?':
            
        case ':':
            help();
            exit(EXIT_FAILURE);
        default:
            break;
        }
    }

}

MprpcApplication& MprpcApplication::instance(){
    static MprpcApplication ma;
    return ma;
}


MprpcConfig MprpcApplication::getConfig(){
    return _config;
}
