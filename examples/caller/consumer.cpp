#include "service1.pb.h"
#include "mpzrpcapplication.hpp"
#include "mpzrpcchannel.hpp"
#include <google/protobuf/message.h>
#include <google/protobuf/service.h>
#include "mpzrpccontroller.hpp"

int main(int argc, char ** argv){
    MprpcApplication::init(argc, argv);
    std::cout << __FILE__ << __LINE__ << endl;
    examples::service1_Stub stub(new mprpcChannel);
    std::cout << __FILE__ << __LINE__ << endl;
    examples::args request;
    std::cout << __FILE__ << __LINE__ << endl;    
    const std::string str1 = "input1";
    std::cout << __FILE__ << __LINE__ << endl;
    request.set_input1(str1);
    std::cout << __FILE__ << __LINE__ << endl;
    std::string str2 = "input2";
    request.set_input2(str2);
    std::cout << __FILE__ << __LINE__ << endl;
    examples::retval response;
    mprpcController controller;
    stub.func1(&controller, &request, &response, nullptr);
    
    // 解析远程调用结果
    if(controller.Failed()){
        std::cout << "RPC失败\n"; 
    }else{
        if(response.mutable_retcode() -> code() == 0){
            std::cout << "响应正常" << endl;
            std::cout << "远程调用结果是:" << response.ret() << endl;
        }else{
            std::cout << "响应错误, 错误原因是: " << response.mutable_retcode() -> msg() << endl;
        }    
    }
    return 0;
}