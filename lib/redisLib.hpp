/**
 * 对redisConnectPool的相关封装
 */

#include "./third-party/redisConnectPool/redisConnectPool.hpp"

//单例模式
class redisConnectPoolSingleton {
public:
    static redisPool & GetPool(){
        static redisConnectPoolSingleton Inst(
                __config__::Redis_ip,
                __config__::Redis_port,
                __config__::Redis_poolsize
                );
        return  Inst;
    }

private:
    redisConnectPoolSingleton(std::string_view ip,int port,int connNum) 
        :redisPool{ip,port,connNum}
    {}
    redisConnectPool redisPool;
};
