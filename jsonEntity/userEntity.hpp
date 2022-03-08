/**
 * 定义给user使用
 */


#include "serializable.hpp"

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
