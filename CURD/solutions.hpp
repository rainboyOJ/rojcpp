//@desc solutions solution_codes solution_full_results 三个表的操作
#pragma once

#include "curd_base.hpp"

namespace CURD {
    
class solutions : public curd_base {

    //@desc 创建一条记录
    uint64_t create_solution(
            std::string_view problem_id,
            std::string_view language,
            std::string_view user_id,
            std::string_view code   //代码
            );



};

} // end namespace CURD

