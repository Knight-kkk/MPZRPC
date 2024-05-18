#include "zkutil.hpp"
#include <iostream>

int main(int argc, char** argv){
    if(argc < 3){
        std::cout << "Usgae:./zookeeperClient -c configfile\n";
        return -1;
    }
    ZkClient zkcli;
    zkcli.start("127.0.0.1:2181");
    char nodename[100] = {0};
    std::cout << "Input node path:";
    std::cin.getline(nodename, 100);
    char nodedata[100] = {0};
    std::cout << "Input node data:";
    std::cin.getline(nodedata, 100);
    zkcli.createNode(nodename,nodedata,100);
    std:: string strnodedata = zkcli.getData(nodename);
    std::cout << "获取测试节点的数据为：" << strnodedata << std::endl;
    return 0;
}