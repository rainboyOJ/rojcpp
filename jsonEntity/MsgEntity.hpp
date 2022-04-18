/**
 * @desc server发送给client的msg
 */
#pragma once

#include "serializable.hpp"
#include "sqlpool.hpp"
#include "appException.hpp"
#include "response.hpp"

using namespace cppjson;

struct MsgEntity {
    int code {0};
    std::string msg;
    MsgEntity() = default;
    explicit MsgEntity(std::string_view msg) : msg{msg} {}
    explicit MsgEntity(int code ,std::string_view msg) :code{code} ,msg{msg} {}

    config get_config()const 
    {
        config config=Serializable::get_config(this);

        config.update({
            {"code", code},
            {"msg", msg}
        });

        return config;
    }

    std::string dumps() const {
        std::ostringstream oss;
        oss << "{ \"code\": ";
        oss << code << ", \"msg\": ";
        oss << '"' << msg << '"' << "}";
        return oss.str();
    }
};

//助手函数
struct MsgEntityHelper {

    //@desc 正常的信息
    static inline void sendMesg(rojcpp::response & res , std::string_view msg) {
        res.set_status_and_content( 
                rojcpp::status_type::ok
                , MsgEntity(0,msg).dumps()
                , rojcpp::req_content_type::json);
    }
    //@desc 错误的信息
    static inline void sendErrorMesg(rojcpp::response & res , std::string_view msg){
        res.set_status_and_content( 
                rojcpp::status_type::ok
                , MsgEntity(-1,msg).dumps()
                , rojcpp::req_content_type::json);
    }
};
