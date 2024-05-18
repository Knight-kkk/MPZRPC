#include "mpzrpcprovider.hpp"
#include <muduo/net/TcpServer.h>
#include "mpzrpcapplication.hpp"
#include <muduo/net/InetAddress.h>
#include <functional>
#include <string>
#include "mpzrpcrequesthead.pb.h"
#include "logger.hpp"
#include "zkutil.hpp"

void MprpcProvider::Run(){
    std::string ip = MprpcApplication::instance().getConfig().load("ProviderIP");
    std::uint16_t port = atoi(MprpcApplication::instance().getConfig().load("ProviderPort").c_str());
    // std::cout << __FILE__ << ":" << __LINE__ << ",ip:" << ip.c_str() << ",port:" << port << endl;
    muduo::net::InetAddress addr(ip.c_str(), port);
    muduo::net::TcpServer server(&_loop, addr, "TestProvider");
    server.setThreadNum(8);
    server.setConnectionCallback(std::bind(&MprpcProvider::_connectionCallback, this, std::placeholders::_1));
    server.setMessageCallback(std::bind(&MprpcProvider::_messageCallBack, this, std::placeholders::_1, placeholders::_2, placeholders::_3));
    server.start();
    std::cout << __FILE__ << __LINE__ << std::endl;
    std::cout << __FILE__ << __LINE__ << std::endl;
    // 连接zookeeper server
    std::string zkip = MprpcApplication::instance().getConfig().load("ZooKeeperIP");
    std::string zkport = MprpcApplication::instance().getConfig().load("ZooKeeperPort");
    ZkClient zkclient;
    std::string zkserver = zkip + ":" + zkport;
    std::cout << "zkserver:" << zkserver << std::endl;
    zkclient.start(zkserver);
    // 发布服务
    // 由于节点不能跨级创建，因此先创建服务名节点
    for(auto it = _serviceMethodMap.begin(); it != _serviceMethodMap.end();it++){
        std::string servicename = it -> first;
        std::string servicenode = "/" + servicename;
        std::string nodedata = ip + ":" + std::to_string(port);
        zkclient.createNode(servicenode.c_str(), nodedata.c_str(), nodedata.size(), 0);
        for(auto it2 = it -> second.methodMap.begin(); it2 != it -> second.methodMap.end();it2++){
            std::string methodname = it2 -> first;
            // 节点名：/服务/方法

            std::string nodepath = "/" + servicename + "/" + methodname;
            std::cout << nodepath << std::endl;
            zkclient.createNode(nodepath.c_str(), nodedata.c_str(), nodedata.size(), ZOO_EPHEMERAL);
            std::cout << "创建节点" << std::endl;
        }
    }
    _loop.loop();

}

void MprpcProvider::NotifyService(google::protobuf::Service* service){
    const google::protobuf::ServiceDescriptor* serviceDescPtr = service -> GetDescriptor();
    // 获取service name
    std::string servicename = serviceDescPtr -> name();
    ServiceInfo serviceinfo;
    serviceinfo.service = service;
    // 获取service 的method descriptor, 通过索引获得方法描述
    int methodCount = serviceDescPtr -> method_count();
    for(int i = 0; i < methodCount;i++){
        // 获取每个方法信息
        const google::protobuf::MethodDescriptor* methodDescPtr = serviceDescPtr -> method(i);
        // 获取方法名字
        serviceinfo.methodMap.insert({methodDescPtr -> name(), methodDescPtr});
        LOG_INFO("service name: %s, method name: %s", servicename.c_str(), methodDescPtr -> name().c_str());
    }
    _serviceMethodMap.insert({servicename, serviceinfo});
}

void MprpcProvider::_connectionCallback(const muduo::net::TcpConnectionPtr& conn){
    // 连接断开事件
    if(!conn -> connected()){
        // std::cout << "Connection client: " << conn -> peerAddress().toIpPort() << "closed\n"; 
        LOG_INFO("Connection client: %s closed", conn -> peerAddress().toIpPort().c_str());
        conn -> shutdown();
    }
    // 连接请求事件
    else{
        // std::cout << "New connection: " << conn -> peerAddress().toIpPort() << endl;
        LOG_INFO("New connection: %s", conn -> peerAddress().toIpPort().c_str());
    }
    return ; 
}

void MprpcProvider::_messageCallBack(const muduo::net::TcpConnectionPtr& conn, muduo::net::Buffer* buff, muduo::Timestamp time){
    // 解析请求消息
    // 消息结构：   |       4B      | Request Head           | Argument |
    //             |  头部长度字段  | 服务名，方法名，参数长度 |    参数  |
    std::cout << __FILE__ << __LINE__ << endl;
    std::string message = buff->retrieveAllAsString(); 
    // 读取请求消息前四个字节
    std::uint32_t headSize = 0;
    message.copy((char*)(&headSize), 4, 0);
    std::cout << __FILE__ << __LINE__ << ",head size:" << headSize << endl;
    // 根据头部长度从后面读取消息头
    mprpc::mprpcRequestHead requestHead;
    if(requestHead.ParseFromString(message.substr(4, headSize))){
        std::string serviceName = requestHead.servicename();
        std::string methodName = requestHead.methodname();
        std::uint32_t argsSize = requestHead.argssize();
        // 查找服务和方法
        auto itService = _serviceMethodMap.find(serviceName);
        // 未找到服务

        if(itService == _serviceMethodMap.end()){
            // std::cout << "未找到服务:" << serviceName << endl;
            LOG_ERROR("Service %s not found", serviceName.c_str());
            return ;
        }
        auto itMethod = itService -> second.methodMap.find(methodName);
        // 未找到方法
        if(itMethod == itService -> second.methodMap.end()){
            // std::cout << "未找到方法：" << methodName << endl;
            LOG_ERROR("Method not found", methodName.c_str());
            return ;
        }
        // 解析参数    
        google::protobuf::Service* service = itService -> second.service; 
        const google::protobuf::MethodDescriptor* methodDescptr = itMethod -> second;
        std::string arguments = message.substr(4 + headSize, argsSize);
        // 获取请求消息和响应消息类型
        google::protobuf::Message* request = service -> GetRequestPrototype(methodDescptr).New();
        google::protobuf::Message* response = service -> GetResponsePrototype(methodDescptr).New();
        // 调用函数
        std::cout << __FILE__ << __LINE__ << endl;
        google::protobuf::Closure* done = google::protobuf::NewCallback<MprpcProvider, 
                                                                        const muduo::net::TcpConnectionPtr&, 
                                                                        google::protobuf::Message* >
                                                                        (   
                                                                            this, 
                                                                            &MprpcProvider::sendResponseCallback, 
                                                                            conn, 
                                                                            response
                                                                        );
        // 调用本地方法
        service -> CallMethod(methodDescptr, nullptr, request, response, done);
        // 输出解析结果
        // std::cout << "Head Length: " << headSize << endl;
        // std::cout << "Service Name: " << serviceName << endl;
        // std::cout << "Method Name: " << methodName << endl;
        // std::cout << "Argument Length: " << argsSize << endl;
        LOG_INFO("Head length: %d, service name: %s, method name: %s, argument length: %d", headSize, serviceName.c_str(), methodName.c_str(), argsSize);
    }
    // std::cout << __FILE__ << __LINE__ << endl;
    LOG_INFO("%s:%s:%d", __FILE__, __FUNCTION__, __LINE__);
    return ;
}  


// 发送RPC请求结果消息的回调函数
void MprpcProvider::sendResponseCallback(const muduo::net::TcpConnectionPtr& conn, google::protobuf::Message* response){
    string strSend;
    // std::cout << __FILE__ << __LINE__ << "send message:" << strSend << endl;
    LOG_INFO("%s:%d, send message: %s", strSend.c_str());
    response -> SerializePartialToString(&strSend);
    conn -> send(strSend);
}

