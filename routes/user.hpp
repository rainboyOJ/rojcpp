#pragma once
#include "http_server.hpp"

#include "__Entity__.hpp"
#include "serializable.hpp"


using namespace rojcpp;

struct User {

// POST
// /user/register
static void user_register(request & req,response & res){
    auto userJson = cppjson::Serializable::loads<userRegistJson>(
            std::string(req.body())
            );//注册
    //输出
    std::cout << userJson.username << std::endl;
    std::cout << userJson.nickname << std::endl;
    std::cout << userJson.password<< std::endl;
    SqlPoolRAII pool;
    std::cout << "pool id "  << pool->getPoolId() << std::endl;
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

static void regist(rojcpp::http_server_ & http_server){
    http_server.set_http_handler<POST>("/user/register",user_register);
}

};
