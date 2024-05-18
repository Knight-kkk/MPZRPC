#include "logger.hpp"
#include <thread>
#include <iostream>
#include <unistd.h>
void Logger::setLogLevel(LogLevel level){
    _level = level;
}

void Logger::Log(const std::string& logMsg){
    _logQueue.Push(logMsg);
}



Logger& Logger::instance(){
    static Logger logger;
    return logger;
} 

Logger::Logger(){

    // 起一个线程循环写日志文件
    std::thread writeLog([&](){
        while(true){
            char fileName[200];
            time_t now = time(nullptr);
            tm* localNow = localtime(&now);
            sprintf(fileName,"../log/%d-%d-%d.log", localNow -> tm_year + 1900, localNow -> tm_mon + 1, localNow -> tm_mday);
            FILE* fp = fopen(fileName, "wa+");
            if(fp == nullptr){
                std::cout << "fopen() 失败\n";
                exit(EXIT_FAILURE);
            }

            // 读取日志队列元素写日志
            char timeStamp[200];
            now = time(nullptr);
            localNow = localtime(&now);
            sprintf(timeStamp, "%d:%d:%d\t%s\t", localNow -> tm_hour, localNow -> tm_min, localNow -> tm_sec, _level == INFO ? "INFO" : "ERROR");
            std::string log = _logQueue.Pop();
            std::cout << log << std::endl;
            fputs(timeStamp ,fp);
            fputs(log.c_str(), fp);
            fputs("\n" , fp);
            fflush(fp);
            fclose(fp);
        }    
    });

    writeLog.detach();
}


