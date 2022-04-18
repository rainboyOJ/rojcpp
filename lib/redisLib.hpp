/**
 * 对redisConnectPool的相关封装
 * @warning 因为我觉得,一个小型的网站不需要使用Redis这种重型工具来作为缓存工具,
 *  所以这个文件没有被使用,也不会删除这个文件
 *
 *  实现使用的缓存是 ../third-party/netcore/core/fastCache.hpp 
 *  我自己实现的简单缓存
 *
 */

#pragma once
#include "../third-party/redisConnectPool/redisConnectPool.hpp"

//单例模式
class redisConnectPoolSingleton {
public:
    static redisConnectPool & GetPool(){
        static redisConnectPoolSingleton Inst(
                __config__::Redis_ip,
                __config__::Redis_port,
                __config__::Redis_poolsize
                );
        return  Inst.redisPool;
    }

private:
    redisConnectPoolSingleton(std::string_view ip,int port,int connNum) 
        :redisPool{ip,port,connNum}
    {}
    redisConnectPool redisPool;
};
