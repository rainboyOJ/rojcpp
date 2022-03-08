/**
 * 定义给user使用
 */


#pragma once
#include "serializable.hpp"
#include "sqlpool.hpp"

using namespace cppjson;
/**
 * 注册时使用
 */
struct userRegistJson {
    std::string username;
    std::string nickname;
    std::string password;
    std::string school;
    std::string email;

    config get_config()const 
    {
        config config=Serializable::get_config(this);
        config.update({
            {"username", username},
            {"nickname", nickname},
            {"password", password},
            {"school",   school},
            {"email",    email}
        });
        return config;
    }
};

/**
 * 创建一个用户
 * username
 * nickname
 * password
 * school
 * email
 * role
 * email_verified_at
 * status
 * remember_token
 */
struct sqlUserTable {
    std::string username;
    std::string nickname;
    std::string password;
    std::string school;
    std::string email;
    std::string role{"student"};
    //std::string email_verified_at;
    int status{0};
    //std::string remember_token;

    sqlUserTable(const userRegistJson & urj )
        :username{urj.username},
        nickname{urj.nickname},
        password{urj.password},
        email{urj.email}
    {}

    //创建
    void Insert(SQLConnection * conn){
        std::string error;
        std::ostringstream oss;
        oss << "insert into users (`username`,`nickname`,`password`,`school`,`email`,`role`,`status`) values (";
        oss << '\"' << username <<  "\",";
        oss << '\"' << nickname <<  "\",";
        oss << '\"' << password <<  "\",";
        oss << '\"' << school <<  "\",";
        oss << '\"' << email <<  "\",";
        oss << '\"' << role <<  "\",";
        oss  << status ; 
        oss << ")";
        std::cout << oss.str() << std::endl;
        auto ret = conn->infoQuery(oss.str(), error);
        //std::cout << "checkQuery ret " << ret << std::endl;
        if(error.length() )
            std::cout << error << std::endl;
    }
    void Delete(); //删除
    void Update();
    void Select(); //查询
};

