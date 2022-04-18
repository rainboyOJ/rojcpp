#pragma once
#include "http_server.hpp"

#include "__Entity__.hpp"
#include "serializable.hpp"
#include "../lib/rojcppUtils.hpp"
#include "../CURD/login_logs.hpp"
#include "user_ap.hpp"


using namespace rojcpp;

struct User {


/**
 * method: POST
 * url: /usr/register
 * 注册
 */
static void user_register(request & req,response & res){
    auto userJson = cppjson::Serializable::loads<userRegistJson>(
            std::string(req.body())
            );//注册
    //输出
    SqlPoolRAII pool;
    //std::cout << "pool id "  << pool->getPoolId() << std::endl;
    //std::string error;
    //auto query_res = pool->infoQuery("show tables", error);
    //for (const auto& e : query_res) {
    //std::cout << e << std::endl;
    //}
    sqlUserTable table(userJson);
    try {
        table.Insert(pool.get());
    }
    catch(std::exception & e){

        MsgEntity err(-1,e.what());
        std::cerr << " Exception : " << e.what() << "\n";
        res.set_status_and_content(status_type::ok,
                cppjson::Serializable::dumps(err)
                ,req_content_type::json);
        return;
    }

    MsgEntity ok(0,"ok");
    res.set_status_and_content(status_type::ok,
            cppjson::Serializable::dumps(ok)
            ,req_content_type::json);
}

/**
 * method: POST
 * url: /usr/login
 * 接收的json {user}
 * 登录
 */
static void user_login(request & req,response & res){
    std::cout << "user login ======================" << std::endl;
    //TODO try cache json load error
    auto userJson = cppjson::Serializable::loads<userRegistJson>(
            std::string(req.body())
            );//注册

    if( UserAP_is_logined().before(req, res) == true){
        MsgEntityHelper::sendMesg(res, "already logined!");
        return;
    }

    SqlPoolRAII pool;
    sqlUserTable table(userJson);
    auto user_id = table.Select(pool.get());
    if( user_id != 0 ){

        //创建 sql login_logs
        uint64_t login_log_id= 0;
        {
            CURD::login_logs login_table;
            login_log_id = login_table.create(user_id, "unknown");  //TODO req should has ip
            std::cout << "========================================" << std::endl;
            std::cout << "uuid " << login_log_id << std::endl;
            std::cout << "========================================" << std::endl;
        }
        auto session_id = rojcppForServer::__encrypt(login_log_id) ;
        res.create_session(session_id);
        rojcpp::Cache::get().set(session_id,std::to_string(user_id),__config__::session_expire);
        MsgEntity ok(0,"login success");

        std::cout << "user login 3 ======================" << std::endl;
        res.set_status_and_content(status_type::ok,
                ok.dumps()
                ,req_content_type::json);
    }
    else{
        MsgEntity ret(-1,"wrong password or username");
        res.set_status_and_content(status_type::ok,
                ret.dumps()
                ,req_content_type::json);
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


static void regist(rojcpp::http_server_ & http_server){
    http_server.set_http_handler<POST>("/user/register",user_register);
    http_server.set_http_handler<POST>("/user/login",user_login);
    http_server.set_http_handler<GET>("/user/logout",user_logout);
}


};
