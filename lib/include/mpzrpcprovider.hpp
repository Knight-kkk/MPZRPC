#pragma once
#include "google/protobuf/service.h"
#include <muduo/net/EventLoop.h>
#include <functional>
#include <muduo/net/TcpConnection.h>
#include <muduo/net/Buffer.h>
#include "google/protobuf/descriptor.h"
#include <unordered_map> 

struct ServiceInfo{
    google::protobuf::Service* service;
    std::unordered_map<std::string, const google::protobuf::MethodDescriptor*> methodMap;
};

class MprpcProvider
{
public:
    // 发布服务函数
    void NotifyService(google::protobuf::Service* service);
    // 启动服务函数
    void Run();
    // 执行完请求之后发送响应的回调函数
    void sendResponseCallback(const muduo::net::TcpConnectionPtr& conn, google::protobuf::Message* response);
private:
    // 使用muduo库步骤：
    // 声明loop
    // 初始化server
    // 设置server回调函数：连接回调函数和消息回调函数
    // 设置线程数量
    // server.start()
    muduo::net::EventLoop _loop;
    void _connectionCallback(const muduo::net::TcpConnectionPtr& conn);
    void _messageCallBack(const muduo::net::TcpConnectionPtr& conn, muduo::net::Buffer* buff, muduo::Timestamp time);
    std::unordered_map<std::string, ServiceInfo> _serviceMethodMap;
};


