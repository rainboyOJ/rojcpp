#pragma once

#include "serializable.hpp"
#include "sqlpool.hpp"
#include "appException.hpp"

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
};
