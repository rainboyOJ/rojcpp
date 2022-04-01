/**
 * @desc 一些基本操作的封装
 */

#pragma once
#include <string_view>

struct redisBase {

    /**
     * @desc 设定key 与 过期时间
     */
    static void SETEX(std::string_view key,std::string_view value,std::size_t expire= 60);

    /**
     * @得到一个指定类型的key值
     */
    template<typename T>
    static T GET(std::string_view key);
    
    /**
     * @desc 创建一个列表并添加元素
     */
    template<typename T>
    static void RPUSH(std::string_view key,T&& value);


    /**
     * @desc 基取list里的元素
     */
    static void LRANGE(std::string_view key,int start,int end);
    static void ALL_LRANGE(std::string_view key,int start,int end){ return LRANGE(key, 0, -1); }
};
