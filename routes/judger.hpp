/**
 * @desc 发送评测的相关评测
 */
#pragma once
#include "http_server.hpp"

#include "__Entity__.hpp"
#include "serializable.hpp"
#include "judgeConnect.hpp"
#include "rojcppUtils.hpp"
#include "fastCache.hpp"
#include "user_ap.hpp"

using  netcore::Cache;
using namespace netcore;

struct judgeRoutes {
    /**
     * @desc 处理评测信息
     */
    static void handleJudgeMsg(request & req,response & res);
    // @desc 连接上webSocket后返回的信息
    static void handleResultWS(request & req,response & res);
    // @desc 使用Get返回的信息 GET /judge_result?id=
    static void handleResult(request & req,response & res);
    static void regist(netcore::http_server & http_server);

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

    //TODO 整理代码,把 sql的操作封装出来,成一个单个类
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
        //redisConnectPoolSingleton::GetPool().SETEX(keyoss.str(),1,60*30);
        netcore::Cache::get().set(keyoss.str(),"1",60*30); //保存 30分钟
    }
    // 3. 发送评测给judgeServer
    judgeConnectSingleton::Get().send(
            solution_id, //key
            rojcppForServer::unescape_newline(judgeJson.code),
            "cpp", //language TODO language transfer
            judgeJson.pid,
            1000,// time TODO
            128 // memoryLimit
            );
    

    MsgEntity msgRet(solution_id);
    res.set_status_and_content(status_type::ok,
            msgRet.dumps()
            ,req_content_type::json);
}


/**
 * websocket的连接,用来返回评测的结果
 */
void judgeRoutes::handleResultWS(request &req, response &res){
    //assert(req.get_content_type() == content_type::websocket);
    if(req.get_content_type() != content_type::websocket){
        //TODO 不是websocket不处理
        return;
    }

    //在这里检查cookie
    if(UserAP_is_logined(false).before(req,res) == false){
            WS_manager::get_instance().send_ws_string(
                    req.get_conn()->GetFd() 
                    , std::string(MSG::NEED_LOGIN), true);
        return;
    }

    auto id = req.get_query_value<uint64_t>("id");
    //注册fd 到WS_manager
    WS_manager::get_instance().regist(
            std::to_string(id)
            , req.get_conn()->GetFd() );
    //LOG_DEBUG("websocket connected! regist id %lld and fd :%d in WS_manager", id, req.get_conn()->GetFd() );

    //TODO 需要一个连接关闭时 把 fd 关闭
    
    req.on(ws_open, [](request& req){
        std::cout << "websocket start" << std::endl;
        //TODO 把fd 注册到 redisCache 对应的key上,不一定是redisCache
    });

    // 对发送过来的的ws信息,暂时不进行处理
    req.on(ws_message, [](request& req) {
        std::cout << "ws_message :test" << std::endl;
        auto part_data = req.get_part_data();
        //echo
        std::string str = std::string(part_data.data(), part_data.length());
        //req.get_conn()->send_ws_string(std::move(str));
        WS_manager::get_instance().send_ws_string(req.get_conn()->GetFd(),std::move(str));
        std::cout << part_data.data() << std::endl;
    });
    //如何主动关闭? close?

    // @desc 反注册 websocket
    req.on(ws_error, [](request& req){
            std::cout << "====================== ws_error" << std::endl;
            WS_manager::get_instance().unregist(
                    std::string( req.get_query_value("id"))
                    , req.get_conn()->GetFd() );
    });

    // @desc 反注册 websocket
    req.on(ws_close, [](request& req){
            std::cout << "====================== ws_close" << std::endl;

            WS_manager::get_instance().unregist(
                    std::string( req.get_query_value("id"))
                    , req.get_conn()->GetFd() );
    });
}


void judgeRoutes::handleResult(request & req,response & res){
    try {
        auto id = req.get_query_value<uint64_t>("id");
        //TODO 没有id 返回错误 403
        auto str_id = std::to_string(id);
        rojcppForServer::judgeCacheKeyFactory JKFC(str_id);

        //1 在redisCache 有评测
        //if(redisConnectPoolSingleton::GetPool().isExitsKey( JKFC.get_is_judging_key() )) {
        if( Cache::get().exists(JKFC.get_is_judging_key())) {
            //TODO 检测user 是否是提交judge的use是同一个user
            MsgEntityHelper::sendMesg(res, "NEED_WEB_SOCKET_CONNECT");
            return;
        }

        //2 先在redisCache 里找结果
        //auto cache_result = redisConnectPoolSingleton::GetPool().GET<std::string>(JKFC.get_Cache_result_key()) ;
        auto [get_cache_succ,cache_result] = Cache::get().get(JKFC.get_Cache_result_key());
        if(  get_cache_succ ){
            MsgEntityHelper::sendMesg(res, cache_result);
            return;
        }

        //3 在sqldb 里查找
        //TODO
        //if () {
        //  设置进cache 里
        //}

        //4 没有找到

        MsgEntityHelper::sendErrorMesg(res, "NOT_FOUND");
    }
    catch(std::exception & e){
        MsgEntityHelper::sendErrorMesg(res, "wrong id or other error");
    }
}


void judgeRoutes::regist(netcore::http_server &http_server){
    http_server.set_http_handler<POST>("/handleJudge",handleJudgeMsg,UserAP_is_logined{});

    http_server.regist_ws_conn_check("/judgews",[](netcore::request & req,netcore::response & res) -> bool
    {
        //检查id
        std::cout << "handleResultWS=====================" << std::endl;
        try {
            //得到请求的哪个题目的 result,id
            auto id = req.get_query_value<uint64_t>("id");
            //LOG_DEBUG("at ws_conn_check for /judgews, id is %lld",id);

            //TODO 应该在redisCache 里注册 是哪个id
            //TODO 应该检查是否有哪个client fd注册过 这个id
            //TODO 只允许一个id 对应一个fd
        }
        catch(std::exception & e){
            return  false;
        }
        return true;
    });

    http_server.set_http_handler<GET,POST>("/judgews",handleResultWS);
    http_server.set_http_handler<GET>("/judge_result",handleResult,UserAP_is_logined{});
}

