/**
 * @desc 1.client 请求 solution 或 发送评测时的 返回信息
 */
#pragma once

#include "serializable.hpp"
#include "appException.hpp"
//#include "third-party/judgercpp/judger.h"
#include "judger.h"


/**
 * @desc 一个 测试点的结果
 */
struct pointResult :public result {

    //int cpu_time;
    //int real_time;
    //long memory;
    //int signal;
    //int exit_code;
    //int error;
    //int result;
    cppjson::config get_config()const 
    {
        cppjson::config config=cppjson::Serializable::get_config(this);
        config.update({
            {"cpu_time",  cpu_time  },
            {"real_time", real_time },
            {"memory",    memory    },
            {"signal",    exit_code },
            {"exit_code", exit_code },
            {"error",     error     },
            {"result",    result    }
        });
        return config;
    }
};


/**
 * @desc solution
 */

struct solutionJson {
    std::size_t id;  //solution 的id
    int status;      //评测/信息的状态
    std::string msg; //文字信息
    std::vector<pointResult> results;

    cppjson::config get_config()const 
    {
        cppjson::config config=cppjson::Serializable::get_config(this);
        config.update({
            {"id",      id      },
            {"status",  status  },
            {"msg",     msg     },
            {"results", results }
        });
        return config;
    }
};
