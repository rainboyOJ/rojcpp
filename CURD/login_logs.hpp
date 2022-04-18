/**
 * @desc login_logs 这个表的相关操作
 */
#pragma once
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

    std::tuple<bool,uint64_t,std::string> exist_and_NoExpired(uint64_t log_id){
        //select unix_timestamp (expireat) - unix_timestamp (),expireat  from rojcpp.login_logs ll where ll.expireat > now() and ll.id =11;
        std::ostringstream oss;
        oss << "select unix_timestamp(expireat) - unix_timestamp(), user_id from login_logs ll where ll.expireat > now() and ll.id=";
        oss << log_id << ";";
        std::string error;
        std::vector< std::vector<std::string> > queryRes 
            = get()->selectQuery(oss.str(), error);

        //std::cout << "+++++++++++++++++++++" << std::endl;
        //for (const auto& e : queryRes[0]) {
            //std::cout << e << std::endl;
        //}
        //std::cout << "+++++++++++++++++++++" << std::endl;
        
        if( queryRes.size() > 0 && queryRes[0].size() > 1)
            return std::make_tuple(true,std::stoull(queryRes[0][0]),queryRes[0][1]);
        else
            return std::make_tuple(false,0,"");
    }
};


} // end namespace CURD


