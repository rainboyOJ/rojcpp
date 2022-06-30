#pragma once
#include "http_server.hpp"

#include "__Entity__.hpp"
#include "serializable.hpp"
#include "../lib/rojcppUtils.hpp"
#include "../CURD/login_logs.hpp"
#include "../CURD/user_table.hpp"
#include "user_ap.hpp"



using namespace netcore;

struct User {



/**
 * method: POST
 * url: /usr/register
 * 注册
 */
static void user_register(request & req,response & res){
    LOG(DEBUG) << __PRETTY_FUNCTION__;
    
    LOG(DEBUG) << "body : " <<  req.body();
    userRegistJson userJson = cppjson::Serializable::loads<userRegistJson>(
            std::string(req.body())
            );//注册
    //输出
    //std::cout << "pool id "  << pool->getPoolId() << std::endl;
    //std::string error;
    //auto query_res = pool->infoQuery("show tables", error);
    //for (const auto& e : query_res) {
    //std::cout << e << std::endl;
    //}
    try {
        CURD::UserTable::add(
                userJson.username,
                userJson.nickname,
                userJson.password,
                userJson.email,
                userJson.school);
    }
    catch(std::exception & e){
        MsgEntityHelper::sendErrorMesg(res, e.what());
        return;
    }

    MsgEntityHelper::sendMesg(res, MSG::OK);
}

/**
 * method: POST
 * url: /usr/login
 * 接收的json {user}
 * 登录
 */
static void user_login(request & req,response & res){
    //TODO try cache json load error
    auto userJson = cppjson::Serializable::loads<userRegistJson>(
            std::string(req.body())
            );//注册

    if( UserAP_is_logined(false).before(req, res) == true){
        MsgEntityHelper::sendMesg(res, MSG::ALREADY_LOGINED);
        return;
    }

    try {
        cppdb::query<"select id from users where username = '?' and password = '?';", unsigned long long> q;

        unsigned long long user_id = q << userJson.username << userJson.password << cppdb::exec;

        //创建 sql login_logs
        uint64_t login_log_id= 0;
        {
            login_log_id = CURD::login_logs::add(user_id,"unknown");
            std::cout << "========================================" << std::endl;
            std::cout << "uuid " << login_log_id << std::endl;
            std::cout << "========================================" << std::endl;
        }
        auto session_id = rojcppForServer::__encrypt(login_log_id) ;
        res.create_session(session_id); // ?
        netcore::Cache::get().set(session_id,std::to_string(user_id),__config__::session_expire);
        MsgEntity ok(0,MSG::OK);

        std::cout << "user login 3 ======================" << std::endl;
        res.set_status_and_content(status_type::ok,
                ok.dumps()
                ,req_content_type::json);
    }
    catch(cppdb::cppdb_no_result & e) {
        MsgEntityHelper::sendErrorMesg(res, MSG::WRONG_PASSWORD_OR_USERNAME);
    }

}

/**
 * method: GET
 * url: /usr/logout
 * 注销登录
 */
static void user_logout(request & req,response & res){
    //auto weakptr = req.get_session();
    //if( weakptr.lock() ){
        //auto share_ptr = weakptr.lock();
        //if( share_ptr->get_data<bool>("logined") == true ){
            //share_ptr->set_data("logined", false);
        //}
    //}

    std::string_view session_id = req.get_cookie_value();
    if( session_id.length() != 0){
        Cache::get().del(std::string(session_id));
        res.create_session("");
        MsgEntityHelper::sendMesg(res, "LOGOUT SUCC");
    }
    else {
        MsgEntityHelper::sendErrorMesg(res, "NOT LOGINED");
    }

}

//@desc ping
static void ping(request &req,response & res){
    MsgEntityHelper::sendMesg(res, "pong");
}


static void regist(netcore::http_server & http_server){
    http_server.set_http_handler<POST>("/user/register",user_register);
    http_server.set_http_handler<POST>("/user/login",user_login);
    http_server.set_http_handler<GET>("/user/logout",user_logout);
    http_server.set_http_handler<GET>("/user/ping",ping,UserAP_is_logined{true});
}


};
