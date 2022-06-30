#include "http_server.hpp"
#include "websocket_manager.h"
#include "third-party/modern_cppdb/include/cppdb.hpp"

#include "__Entity__.hpp"


// ----- routes -----
#include "./routes/user.hpp"
//#include "./routes/judger.hpp"


using namespace netcore;

__START_LOG__

netcore::http_server OJServer{__config__::work_thread,8099};

int main(){
    LOG(ERROR) << "Server start";

    //数据库连接池
    // 执行一次,初始化SQL连接池
    const std::string connection_info_ = std::string(__config__::connection_info_);
    cppdb::pool_manager::init(connection_info_);
    
    //std::cout << "create sqlpool succ" << std::endl;
    //auto conn  = pool.GetConnecion();
    //std::string error{};
    //std::string query = "CREATE TABLE test(id int);";
    //conn->infoQuery(query,error);
    //std::cout << "error.leng " << error.length() << std::endl;
    
    cppjson::Serializable::Regist<userRegistJson>();//注册 用户的json
    cppjson::Serializable::Regist<MsgEntity>();//注册   返回消息的json
    cppjson::Serializable::Regist<judgeEntiy>();//注册  评测的json
    cppjson::Serializable::Regist<pointResult>();//注册  
    cppjson::Serializable::Regist<solutionJson>();//注册


    OJServer.set_http_handler<GET>("/helloworld",[](request & req,response & res){
            res.set_status_and_content(status_type::ok,"hello world,this rojcpp OJServer",req_content_type::string);
    });

    //OJServer.set_http_handler<POST>("/user/register",[](request & req,response & res){
    //});
    User::regist(OJServer);           // 用户相关的Routes
    //judgeRoutes::regist(OJServer);    // 评测相关的Routes
    

    OJServer.run();
    
    return 0;
}
