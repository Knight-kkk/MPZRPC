#pragma once
#include <queue>
#include <mutex>
#include <condition_variable>

// 模板类,声明和定义在同一个文件中
template<typename T>
class LogQueue{
public:
    void Push(const T& elem){
        std::lock_guard<std::mutex> lock(_mutex);
        _logQueue.push(elem);
        _conditionVariable.notify_one();
    }
    
    T Pop(){
        std::unique_lock<std::mutex> lock(_mutex);
        while(_logQueue.empty()){
            _conditionVariable.wait(lock);
        }
        T elem = _logQueue.front();
        _logQueue.pop();
        return elem;
    }

private:
    std::mutex _mutex;
    std::condition_variable _conditionVariable; 
    std::queue<T> _logQueue;   
};


