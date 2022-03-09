#include "http_server.hpp"
#include "websocket_manager.h"
#include "sqlpool.hpp"

#include "__Entity__.hpp"
#include "__Routes__.hpp"

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

    //Server.set_http_handler<POST>("/user/register",[](request & req,response & res){
    //});
    User::regist(Server);
    

    Server.run();
    return 0;
}
