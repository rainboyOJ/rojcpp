/**
 * sql的连接池
 */
#include "ConnectionPool.h"

#ifndef USER_CONFIG
#include "__default_config.hpp"
#endif

class SQL_POOL {
public:
    static ConnectionPool& get_instance(){
        static SQL_POOL sql_pool;
        return  sql_pool.sqlpool;
    }
private:
    SQL_POOL()
        :sqlpool(
        __config__::sqlServer,
        __config__::sqlPort,
        __config__::sqlUser,
        __config__::sqlPwd,
        __config__::dbName,
        __config__::connPoolNum
                )
    {
    }
    ConnectionPool sqlpool;
};

