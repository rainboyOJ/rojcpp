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
            auto exist = netcore::Cache::get().exists(std::string(session_id));
            if( exist )
                return true;
        }

        //2. 检查 sql 里的 login_logs 是否登录
        //  可能是服务器重启导致 缓存失效
        if(session_id.length() != 0)
        {
            //TODO try cache
            const std::string session_id_str = std::string(std::string(session_id));
            uint64_t log_id =  rojcppForServer::__decrypt(session_id_str);
            CURD::login_logs login_log_table;
            auto [exist,expire_duration,user_id] 
                = login_log_table.exist_and_NoExpired(log_id);
            //重新缓存起来
            netcore::Cache::get().set(session_id_str,std::move(user_id),expire_duration);
            return true;
        }

        if (bwrite_on_fail) 
            MsgEntityHelper::sendErrorMesg(res, MSG::NEED_LOGIN);
        return false;
    }

    bool bwrite_on_fail{true};
};
