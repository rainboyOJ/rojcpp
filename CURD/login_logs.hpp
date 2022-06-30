/**
 * @desc login_logs 这个表的相关操作
 */
#pragma once

#include "modern_cppdb.hpp"

namespace CURD {


struct login_logs {
    
    //创建一条新的登录记录
    static
    unsigned long long add(
            unsigned long long user_id,
            std::string_view ip = ""
            ) {
        cppdb::query<"insert into login_logs (user_id,ip) values (?,'?') RETURNING id;", unsigned long long> q;
        return q<< user_id << ip << cppdb::exec;
    }

    //检查登录是否过期
    std::tuple<bool,unsigned long long,unsigned long long>
        exist_and_NoExpired(unsigned long long log_id)
    {
        using one_row = 
            cppdb::row_type<
                cppdb::column<"left_time", unsigned long long>,
                cppdb::column<"user_id", unsigned long long>
            >;

        cppdb::query<"select unix_timestamp (expireat) - unix_timestamp (),user_id from login_logs ll where ll.expireat > now() and ll.id = ?;", one_row> q;

        unsigned long long time_;

        try {
            one_row row =  q << log_id << cppdb::exec;
            return std::make_tuple(true,
                    cppdb::get<"left_time",one_row>(row),
                    cppdb::get<"user_id",one_row>(row)
                    );
        }
        catch(cppdb::cppdb_no_result) { //TODO 细分错误
            return std::make_tuple(false,0,0);
        }

    }
};

/*
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
*/

} // end namespace CURD


