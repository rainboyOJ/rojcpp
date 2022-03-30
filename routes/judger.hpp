/**
 * @desc 发送评测的相关评测
 */
#pragma once
#include "http_server.hpp"

#include "__Entity__.hpp"
#include "serializable.hpp"

using namespace rojcpp;

struct judgeRoutes {
    /**
     * @desc 处理评测信息
     */
    static void handleJudgeMsg(request & req,response & res);
    static void regist(rojcpp::http_server_ & http_server);
};
/**
 * @desc 用来处理用户发来的评测信息
 * 1. 验证数据
 *      代码长度
 *      pid 是否存在
 *      lang 语言
 * 2. 创建一个评测的sql
 * 3. 发送评测给judgeServer
 * 4. judge client的 handle 等待处理
 */
void judgeRoutes::handleJudgeMsg(request &req, response &res){

    auto judgeJson = cppjson::Serializable::loads<judgeEntiy>( std::string(req.body()) );
    std::cout << "language: "<< std::endl;
    std::cout << judgeJson.language << std::endl;

    //TODO 
    // 1. 创建一个solutions
    // 2. 创建一个redisCache

    solutionJson sJson;
    sJson.id = 1000;
    sJson.msg = "test";
    sJson.status = -1;
    res.set_status_and_content(status_type::ok,
            cppjson::Serializable::dumps(sJson)
            ,req_content_type::json);

}

void judgeRoutes::regist(rojcpp::http_server_ &http_server){
    http_server.set_http_handler<POST>("/handleJudge",handleJudgeMsg);
}
