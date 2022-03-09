#pragma once
#include "http_server.hpp"

#include "__Entity__.hpp"
#include "serializable.hpp"


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
    auto userJson = cppjson::Serializable::loads<userRegistJson>(
            std::string(req.body())
            );//注册
    SqlPoolRAII pool;
    sqlUserTable table(userJson);
    auto exits = table.Select(pool.get());
    if( exits ){
        MsgEntity ok(0,"login success");
        res.set_status_and_content(status_type::ok,
                ok.dumps()
                ,req_content_type::json);
    }
    else{
        MsgEntity ret(-1,"login fail");
        res.set_status_and_content(status_type::ok,
                ret.dumps()
                ,req_content_type::json);
    }
}


static void regist(rojcpp::http_server_ & http_server){
    http_server.set_http_handler<POST>("/user/register",user_register);
    http_server.set_http_handler<POST>("/user/login",user_login);
}


};
