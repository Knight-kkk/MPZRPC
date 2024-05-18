#include "mpzrpcchannel.hpp"
#include <google/protobuf/service.h>
#include <google/protobuf/message.h>
#include <string>
#include "mpzrpcrequesthead.pb.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include "mpzrpcapplication.hpp"
#include "zkutil.hpp"

void mprpcChannel::CallMethod(const google::protobuf::MethodDescriptor* method,
                        google::protobuf::RpcController* controller, const google::protobuf::Message* request,
                        google::protobuf::Message* response, google::protobuf::Closure* done)
                {
                        // 完成序列化和请求发送
                        // 请求格式：
                        // headsize(4B) + service_name + method_name + args_size + args
                        // 首先需要获取args，从而得到args_size
                        // 之后获取服务名和方法名，得到请求头
                        // 最后得到整个请求
                        cout << __FILE__ << __LINE__ << endl;
                        std::string serviceName = method -> service() -> name();
                        std::string methodName = method -> name();
                        std::string args;
                        uint32_t argsSize = 0;
                        uint32_t headSize = 0;
                        if(! request -> SerializeToString(&args)){
                                std::cout << "序列化RPC参数失败\n";
                                exit(EXIT_FAILURE);
                        }
                        argsSize = args.size();
                        // 至此得到了服务名，方法名，参数和参数长度
                        // 填充RPC请求头之后序列化
                        mprpc::mprpcRequestHead head;
                        head.set_servicename(serviceName);
                        head.set_methodname(methodName);
                        head.set_argssize(argsSize);
                        std::string strHead;
                        if(!head.SerializeToString(&strHead)){
                                std::cout << "序列化RPC请求头失败\n";
                                exit(EXIT_FAILURE);
                        }
                        headSize = strHead.size();
                        // 至此得到headSize、serviceName、methodSize、argsSize、args等字段
                        std::string strSend;
                        std::cout << __FILE__ << __LINE__ << ",head size:" << headSize << endl;
                        // 将headSize的二进制串添加到发送的序列前
                        strSend.insert(0, (char*)(&headSize),4);
                        std::cout << __FILE__ << __LINE__ << ",strSend:" << strSend << endl;
                        strSend += strHead;
                        strSend += args;
                        
                        // 发送请求序列
                        int fd = socket(AF_INET, SOCK_STREAM, 0);
                        if(fd == -1){
                                controller -> SetFailed("socket() 执行失败\n");
                                // std::cout << "socket() 执行失败\n";
                                return ;      
                        }
                        
                        // std::string providerIP =  MprpcApplication::instance().getConfig().load("ProviderIP");
                        // std::string providerPort = MprpcApplication::instance().getConfig().load("ProviderPort");
                        // 需要从zookeeper server上获得rpc服务提供主机的地址和端口号
                        std::string zkserver = MprpcApplication::instance().getConfig().load("ZooKeeperIP") + ":" + MprpcApplication::instance().getConfig().load("ZooKeeperPort");
                        ZkClient zkclient;
                        zkclient.start(zkserver);
                        std::string nodepath = "/" + serviceName + "/" + methodName;
                        std::string rpcserver = zkclient.getData(nodepath.c_str());
                        if(rpcserver == "" || rpcserver.find(":") == -1){
                                std::cerr << "未找到该服务" << std::endl;
                                return ;
                        }else{
                                std::cout << "rpc server:" << rpcserver << std::endl;
                        }
                        int index = rpcserver.find(":");
                        struct sockaddr_in addr;
                        addr.sin_family = AF_INET;
                        addr.sin_addr.s_addr = inet_addr(rpcserver.substr(0,index).c_str());
                        addr.sin_port = htons(short(atoi(rpcserver.substr(index + 1, rpcserver.size() - index - 1).c_str())));
                        
                        std::cout << "providerIP:" << rpcserver.substr(0,index).c_str() << endl;
                        std::cout << "providerPort:" << rpcserver.substr(index + 1, rpcserver.size() - index - 1) << endl;

                        if(connect(fd, (sockaddr*)(&addr), sizeof(addr)) == -1){
                                // std::cout << "connect() 失败\n";
                                controller -> SetFailed("connect() 失败\n");
                                close(fd);
                                return ;    
                        }

                        std::cout << __FILE__ << __LINE__ << ",send message:" << strSend.c_str() << endl;
                        if(send(fd, strSend.c_str(), strSend.size(), 0) == -1){
                                // std::cout << "send() 失败\n";
                                close(fd);
                                controller -> SetFailed("send() 失败\n");
                                return ;
                        }

                        char buffer[1024] = {0};
                        int len = recv(fd, buffer, 1024, 0);
                        if(len <= 0){
                                // std::cout << "recv() 失败\n";
                                controller -> SetFailed("recv()失败\n");
                                close(fd);
                                return ;
                        }
                        if(!response -> ParseFromString(buffer)){
                                // std::cout << "ParseFromString() 失败\n";
                                close(fd);
                                controller -> SetFailed("ParseFromString() 失败\n");
                                return ; 
                        }
                        // 类HTTP短链接形式，请求一次之后断开
                        close(fd);
                }