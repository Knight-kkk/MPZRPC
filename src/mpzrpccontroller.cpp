#include "mpzrpccontroller.hpp"

bool mprpcController::Failed() const{
    return _failed;
}

std::string mprpcController::ErrorText() const{
    return _errorText;
}

void mprpcController::SetFailed(const std::string& reason){
    _failed = true; 
    _errorText = reason;
}

void mprpcController::Reset(){
    _errorText = "";
    _failed = false;
}