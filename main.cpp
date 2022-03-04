#include "http_server.hpp"
#include "websocket_manager.h"

using namespace rojcpp;
rojcpp::http_server_ Server;


int main(){
    Server.set_http_handler<GET>("/helloworld",[](request & req,response & res){
            res.set_status_and_content(status_type::ok,"hello world,this rojcpp Server",req_content_type::string);
    });

    Server.run();
    return 0;
}
