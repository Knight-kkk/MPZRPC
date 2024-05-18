#include <iostream>
#include <string>
#include "service1.pb.h"
#include "mpzrpcapplication.hpp"
#include "mpzrpcprovider.hpp"
using namespace std;
// 该程序演示如何将本地服务转化为rpc服务
// 首先在proto文件中声明本地方法参数和返回值
// 随后声明本地方法
// protoc生成头文件和代码
// 本地方法所在类继承头文件中相应类
// 重写头文件中虚函数


// 远程方法调用流程
// caller ====> 请求 ====> caller ====> 调用实现的虚函数

class Service1: public examples::service1{
public:
    bool func1(const string& str1, const string& str2){
        cout << "Input1:" << str1 << endl;
        cout << "Input2:" << str2 << endl;
        return true;
    }

    void func1(::google::protobuf::RpcController* controller,
                    const ::examples::args* request,   // 序列化的参数 
                    ::examples::retval* response,   // 响应
                    ::google::protobuf::Closure* done)  // 回调函数
    {
        // 得到远程调用参数
        string input1 = request -> input1();
        string input2 = request -> input2();
        // 调用本地方法
        bool ret = func1(input1, input2);

        // 回复响应
        examples::msgcode *code = response -> mutable_retcode();
        code -> set_code(0);
        code -> set_msg("nothing wrong");
        response -> set_ret(ret); 

        // 执行回调函数, 将执行结果序列化之后发送回去
        done -> Run();
    }

void func2(::google::protobuf::RpcController* controller,
                    const ::examples::args* request,   // 序列化的参数 
                    ::examples::retval* response,   // 响应
                    ::google::protobuf::Closure* done)  // 回调函数
    {
    
    }

void func3(::google::protobuf::RpcController* controller,
                    const ::examples::args* request,   // 序列化的参数 
                    ::examples::retval* response,   // 响应
                    ::google::protobuf::Closure* done)  // 回调函数
    {
    
    }


private:

};

int main1(){
    Service1 service1;
    service1.func1("local", "func");
    return 0;
}


int main(int argc, char ** argv){
    MprpcApplication::init(argc, argv);
    MprpcProvider provider;
    provider.NotifyService(new Service1());
    provider.Run();
    return  0;
}