/**
 * @desc 和user这个表相关的CURD
 */

#include <string_view>
#include "modern_cppdb.hpp"


namespace CURD {

/**
 * table desc
 *
 * username| nickname| password| school| email| role{"student"}|
 */
using namespace std::literals::string_view_literals;
struct UserTable {

    /**
     * @desc 创建一个用户
     */
    static void add(
            std::string_view username,
            std::string_view nickname,
            std::string_view password,
            std::string_view email,
            std::string_view school = ""sv,
            std::string_view role = "student"sv
            ) 
    {

        //INSERT INTO rojcpp.users (created_at,updated_at,deleted_at,username,nickname,password,school,email,`role`,email_verified_at,status,remember_token) VALUES
        cppdb::query<"INSERT INTO users (username,nickname,password,school,email,role) VALUES ('?','?','?','?','?','?');", void> q;
        q << username << nickname << password << email << school << role << cppdb::exec;
    }

    static void find(){
    }

    static void del() {
    }

    static void update() {
    }

};

} // namespace CURD
