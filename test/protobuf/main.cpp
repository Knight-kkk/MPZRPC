#include "test.pb.h"
#include <iostream>
#include <string>
using namespace test;
using namespace std;


int main(){
    Response res;
    StatusCode* scode = res.mutable_rescode();
    scode -> set_code(123);
    scode -> set_errormsg("there is nothing wrong!\n");
    User* user = res.add_users();
    user -> set_id(1);
    user -> set_name("zhang san");
    user -> set_sex(User::MAN);
    user = res.add_users();
    user -> set_id(2);
    user -> set_name("li si");
    user -> set_sex(User::WOMAN);
    
    string str;
    res.SerializeToString(&str);
    cout << "Serialized String:" << str.c_str() << endl;

    // 反序列化
    Response deserialRes;
    deserialRes.ParseFromString(str);
    

    return 0;
}



int main1(){
    // 请求对象
    LonginRequest req;
    req.set_name("zhang san");
    req.set_pwd("123456");

    string strreq;
    // 序列化
    bool ret = req.SerializeToString(&strreq); 
    if(ret)   
        cout << "Serialized String:" << strreq.c_str() << endl;
    
    // 反序列化
    LonginRequest deserialreq;
    ret = deserialreq.ParseFromString(strreq);
    if(ret){
        cout << "Deserialized Req.name:" << deserialreq.name() << endl;
        cout << "Deserialized Req.pwd:" << deserialreq.pwd() << endl;
    }
    return  0;
}