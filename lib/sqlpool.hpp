/**
 * sql的连接池
 */
#pragma once
#include "ConnectionPool.h"

#ifndef USER_CONFIG
#include "__default_config.hpp"
#endif

//RAII
class SqlPoolRAII;

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


class SqlPoolRAII {
public:
    SqlPoolRAII(){
        conn = SQL_POOL::get_instance().GetConnecion();
    }
    ~SqlPoolRAII(){
        if( conn != nullptr)
            SQL_POOL::get_instance().ReleaseConnecion(conn);
    }
    auto  get() { return conn;}
    //操作sql连接
    SQLConnection* operator->(){
        return conn;
    }
private:
    SQLConnection* conn{nullptr};
};
