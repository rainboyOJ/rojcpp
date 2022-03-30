/**
 * @发送过来的judge的josn 信息
 */
#pragma once

#include "serializable.hpp"
#include "sqlpool.hpp"
#include "appException.hpp"
#include "judge_server/src/Send.hpp"

using namespace cppjson;

struct judgeEntiy {
    std::string language;   //语言
    std::string code;       //代码
    std::string pid;        //题目的编号
    int memoryLimt; //ms
    int timeLimt;   //mb

    config get_config()const 
    {
        config config=Serializable::get_config(this);
        config.update({
            {"language", language},
            {"code", code},
            {"memoryLimt", memoryLimt},
            {"timeLimt", memoryLimt},
            {"pid", pid}
        });
        return config;
    }
    MessageSendJudge convertToMessageSendJudge(std::string_view key){
        return  MessageSendJudge(key,code,language,pid,timeLimt,memoryLimt);
    }
};
