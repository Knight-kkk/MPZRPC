#pragma once
// 只包含一次
#include "mpzrpcconfig.hpp"

// MprpcApplication是基础类，完成服务器初始化等工作
class MprpcApplication{
public:
    // 初始化函数
    static void init(int argc, char **argv);
    // 单例对象函数
    static MprpcApplication& instance();
    // 获取配置
    MprpcConfig getConfig();

private:
    MprpcApplication(){}
    // 禁止拷贝构造函数
    MprpcApplication(const MprpcApplication&) = delete;
    MprpcApplication(MprpcApplication&&) = delete;
        static MprpcConfig _config;
};