/**
 * @desc 发送评测的相关评测
 */
#pragma once
#include "http_server.hpp"

#include "__Entity__.hpp"
#include "serializable.hpp"
#include "redisLib.hpp"

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
    std::ostringstream ossSolutionSql;
    ossSolutionSql << "insert into solutions (owner_id,problem_id,lang) values(";
    //TODO check language is suport
    // 1 cpp
    // 2 python3
    ossSolutionSql <<  10 << ","; // owner_id
    ossSolutionSql <<  judgeJson.pid << ","; // problem_id
    ossSolutionSql <<  judgeJson.language ;
    ossSolutionSql << ") RETURNING id"; // 得到插入的id

    std::string solution_id;

    //TODO 
    // 1. 创建一个solutions
    {
        SqlPoolRAII sql;
        std::string error;
        //创建一个solutions
        std::vector<std::string> queryRes = sql.get()->infoQuery(ossSolutionSql.str(), error);
        std::cout << ossSolutionSql.str() << std::endl;
        std::cout << "query result : \n" ;
        for (const auto& e : queryRes) {
            std::cout << e << std::endl;
        }
        if( error.length() !=0 ){
            std::cout << "error: " << std::endl;
            std::cout << error << std::endl;
        }
        solution_id = queryRes[0];
        //插入code_solutions ,code_full_results;
        ossSolutionSql.str("");
        ossSolutionSql << "insert into solution_codes (solution_id,code) values (";
        ossSolutionSql << queryRes[0] << ",";
        ossSolutionSql << '\"' << sql.get()->escapeString(judgeJson.code) << '\"';
        //ossSolutionSql << '\"' << "this is Code " << '\"';
        ossSolutionSql << ")";

        std::cout << ossSolutionSql.str() << std::endl;
        queryRes = sql.get()->infoQuery(ossSolutionSql.str(),error);
        if( error.length() != 0) {
            std::cout << error << std::endl;
            error.clear();
        }

        ossSolutionSql.str("");
        ossSolutionSql << "insert into solution_full_results(solution_id) values (";
        ossSolutionSql << solution_id << ")";
        std::cout << ossSolutionSql.str() << std::endl;
        queryRes = sql.get()->infoQuery(ossSolutionSql.str(),error);
        if( error.length() != 0) {
            std::cout << error << std::endl;
        }
    }

    // 2. 创建一个redisCache
    {
        std::ostringstream keyoss;
        keyoss << solution_id << "_is_judging";
        redisConnectPoolSingleton::GetPool().SETEX(keyoss.str(),1,60*30);
    }
    // 3. 发送评测给judgeServer
    MsgEntity msgRet(solution_id);
    res.set_status_and_content(status_type::ok,
            msgRet.dumps()
            ,req_content_type::json);
}

void judgeRoutes::regist(rojcpp::http_server_ &http_server){
    http_server.set_http_handler<POST>("/handleJudge",handleJudgeMsg);
}
