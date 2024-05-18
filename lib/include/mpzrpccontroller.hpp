#include <google/protobuf/service.h>


class mprpcController : public google::protobuf::RpcController{
public:
    void Reset();
    bool Failed() const;
    std::string ErrorText() const;
    void StartCancel(){}
    void SetFailed(const std::string& reason);
    bool IsCanceled() const {}
    void NotifyOnCancel(google::protobuf::Closure* callback){}
    mprpcController(){}
    ~mprpcController(){}
private:
    bool _failed = false;
    std::string _errorText = "";
};
