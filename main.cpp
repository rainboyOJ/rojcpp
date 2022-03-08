#include "http_server.hpp"
#include "websocket_manager.h"
#include "sqlpool.hpp"

#include "__Entity__.hpp"

using namespace rojcpp;
rojcpp::http_server_ Server;



int main(){
    // 执行一次
    SQL_POOL::get_instance();
    //std::cout << "create sqlpool succ" << std::endl;
    //auto conn  = pool.GetConnecion();
    //std::string error{};
    //std::string query = "CREATE TABLE test(id int);";
    //conn->infoQuery(query,error);
    //std::cout << "error.leng " << error.length() << std::endl;
    
    cppjson::Serializable::Regist<userRegistJson>();//注册
    cppjson::Serializable::Regist<MsgEntity>();//注册


    Server.set_http_handler<GET>("/helloworld",[](request & req,response & res){
            res.set_status_and_content(status_type::ok,"hello world,this rojcpp Server",req_content_type::string);
    });

    Server.set_http_handler<POST>("/user/register",[](request & req,response & res){
            //std::cout << req.body() << std::endl;
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
    });

    Server.run();
    return 0;
}
