//@desc solutions solution_codes solution_full_results 三个表的操作
#pragma once

//#include "curd_base.hpp"
#include <string_view>
#include "modern_cppdb.hpp"

namespace CURD {
    
/**
 *table desc
 *  owner_id
 *  problem_id
 *  contest_id
 *  lang
 *  result
 *  time_used
 *  memory_used
 */
using namespace std::literals::string_view_literals;
class solutions {

    //创建一条空的记录
    static unsigned long long add(
            unsigned int owner_id,
            unsigned int problem_id,
            //unsigned int contest_id, //没有比赛就是0
            unsigned int lang
            //int result,
            //int time_used,
            //unsigned long long memory_used
            )
        {
            cppdb::query<
    "insert into solutions (owner_id,problem_id,lang) values(?,?,?) RETURNING id;"
                , unsigned long long> q;
            unsigned long long solution_id = q << owner_id << problem_id << lang << cppdb::exec ;
            return solution_id;
        }

    //创建一个新的题目代码的记录
    static void add_solution_codes( unsigned long long solution_id,
                             std::string_view codes)
    {
        cppdb::query<
        "insert into solution_codes (solution_id,code) values (?,\"?\");" , void > q;
        q << solution_id << codes << cppdb::exec;
    }

    //创建一个新 完整结果 的记录
    static
    void add_full_solution_result(unsigned long long solution_id)
    {
        cppdb::query<
    "insert into solution_full_results(solution_id) values (?);", void > q;
        q << solution_id << cppdb::exec;
    }
        
    //更新一条记录
    static void update() {
    }




};

} // end namespace CURD

