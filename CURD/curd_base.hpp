/**
 * 一些对于sql基础的操作
 */

#pragma once

#include "sqlpool.hpp"

namespace CURD {

class curd_base {
public:
    //curd_base() {}
    //virtual ~curd_base() {}
    const auto get() const { return sql.get(); }
private:
    SqlPoolRAII sql;
};

}

