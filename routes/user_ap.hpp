/**
 * 检查用户是否登录的ap
 */
#pragma once
#include "http_server.hpp"
#include "../jsonEntity/MsgEntity.hpp"
#include "../CURD/login_logs.hpp"
#include "../lib/rojcppUtils.hpp"
#include "fastCache.hpp"

//using namespace rojcpp;

//@desc 检查是否登录
struct UserAP_is_logined {

    UserAP_is_logined() =default;
    UserAP_is_logined(bool b) :bwrite_on_fail{b} {}

    bool before(netcore::request & req, netcore::response & res){
        using namespace netcore;
        std::string_view session_id = req.get_cookie_value();
        if( session_id.length() != 0 ){
            //auto exist = netcore::Cache::get().exists(std::string(session_id));
            auto [exists,val] = netcore::Cache::get().get(std::string(session_id));
            //设定req 里的 req.user_id
            if( exists ) {
                req.set_user_id(std::stoull(val));
                return true;
            }
        }

        //2. 检查 sql 里的 login_logs 是否登录
        //  可能是服务器重启导致 缓存失效
        if(session_id.length() != 0)
        {
            //TODO try cache
            const std::string session_id_str = std::string(session_id);
            uint64_t log_id =  rojcppForServer::__decrypt(session_id_str);
            auto [exist,expire_duration,user_id] 
                = CURD::login_logs::exist_and_NoExpired(log_id);
            if( exist ) {
                //重新缓存起来
                netcore::Cache::get().set(session_id_str,std::to_string(user_id),expire_duration);
                req.set_user_id(user_id);
                return true;
            }
        }

        if (bwrite_on_fail) 
            MsgEntityHelper::sendErrorMesg(res, MSG::NEED_LOGIN);
        return false;
    }

    bool bwrite_on_fail{true};
};
