/**
 * @desc login_logs 这个表的相关操作
 */
#include "curd_base.hpp"

namespace CURD {


class login_logs : public curd_base {

public:
    //TODO 改变session expired 的日期
    //创建一条记录
    //@retval 返回创建的这条记录的 uuid
    uint64_t create(uint64_t user_id,std::string_view ip){
        std::ostringstream oss;
        oss << "insert into login_logs (user_id,ip) values (";
        oss << user_id << ",\"";
        oss << ip << "\") RETURNING id;";
        //insert into  rojcpp.login_logs (user_id ,ip) values (10,"1.1.1.1")
        std::string error;
        std::vector<std::string> queryRes
            = get()->infoQuery(oss.str(), error);
        // TODO maybe error
        return std::stoull(queryRes[0]);
    }

    std::tuple<bool,std::string> find(){
    }
};


} // end namespace CURD


