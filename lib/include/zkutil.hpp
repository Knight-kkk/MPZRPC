#pragma once
#include <zookeeper/zookeeper.h>
#include <string>

class ZkClient{
public:
    ZkClient();
    ~ZkClient();
    void start(const std::string& zkserver);
    void createNode(const char* node, const char* data, int datasize, int state = 0);
    std::string getData(const char* node);
private:
    zhandle_t* _zkhandle;
};