#include "zkutil.hpp"
#include "logger.hpp"
#include <semaphore.h>
#include <iostream>

ZkClient::ZkClient(): _zkhandle(nullptr){

}



// 析构函数关闭zk连接句柄
ZkClient::~ZkClient(){
    if(_zkhandle != nullptr){
        zookeeper_close(_zkhandle);
    }
}

// 连接zookeeper server成功后的回调函数
void connCallBack(zhandle_t *zh, int type, int state, const char* path, void* watcherCtx){
    // std::cout << "zk回调函数\n";
    
    if(type == ZOO_SESSION_EVENT){
        std::cout << __FILE__ << __LINE__ <<std::endl;
        if(state == ZOO_CONNECTED_STATE){
            // init时使用信号量set了context
            // std::cout << __FILE__ << __LINE__ << std::endl;
            sem_t *sem = (sem_t*)zoo_get_context(zh);
            sem_post(sem);
            // std::cout << __FILE__ << __LINE__ << std::endl;
        }
    }
}

// 连接zookeeper server的函数，异步方式
void  ZkClient::start(const std::string& zkserver){
    // 获取zookeeper server地址和端口信息
    // std::cout << __FILE__ << __LINE__ <<std::endl;
    // 设置连接超时时间为30秒
    // std::cout << zkserver << std::endl;

    // 等待连接成功的信号
    sem_t sem;
    sem_init(&sem, 0 , 0);
    // std::cout << __FILE__ << __LINE__ << std::endl;
    _zkhandle = zookeeper_init(zkserver.c_str(), connCallBack, 30000, nullptr, &sem, 0);
    sem_wait(&sem);
    if(_zkhandle == nullptr){
        // std::cout << __FILE__ << __LINE__ << std::endl;
        std::cerr << __FILE__ << __LINE__ << "ZK init完成" << std::endl;
        LOG_ERROR("zookeeper_init() 失败");
        exit(EXIT_FAILURE);
    }

    // std::cout << __FILE__ << __LINE__ << std::endl;
    
    LOG_INFO("zookeeper_init() 完成");
}



// 获取指定路径的节点的信息
std::string ZkClient::getData(const char* node){
    char buffer[1024] = {0};
    int buffersize = sizeof(buffer);
    int flag = zoo_get(_zkhandle, node, 0 , buffer, &buffersize, nullptr);
    if(flag != ZOK){
        LOG_ERROR("zoo_get() 失败");
        return "";
    }
    return buffer;
}

// 创建节点
void ZkClient::createNode(const char* node, const char* data, int datasize, int state){
    char buffer[1024];
    int buffersize = sizeof(buffer);
    // 首先判断节点是否存在
    int flag;
    if((flag = zoo_exists(_zkhandle, node, 0, nullptr)) != ZNONODE){
        LOG_ERROR("zoo_create()失败, 原因是: 结点%s已经存在", node);
        return ;
    }
    flag = zoo_create(_zkhandle, node, data, datasize, &ZOO_OPEN_ACL_UNSAFE, state, buffer, buffersize);
    if(flag == ZOK){
        LOG_INFO("成功创建ZooKeeper节点:%s", buffer);
    }else{
        LOG_ERROR("创建ZooKeeper节点: %s 失败", node);
    }
}