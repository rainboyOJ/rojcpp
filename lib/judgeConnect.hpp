/**
 * @desc 连接 judgeServer 是对
 *  ../third-party/judgercpp/judge_server/src/Client.hpp 的封装
 */

#pragma once

#include "../third-party/judgercpp/judge_server/src/Client.hpp"
//#include "./redisLib.hpp"
#include "./rojcppUtils.hpp"
#include "MsgEntity.hpp"
#include "fastCache.hpp"

#include "websocket_manager.h"

using rojcpp::Cache;


//过期时间
const int __KEY_EXPIRED_SECONS  = 60*30;

class judgeConnectSingleton {

public:
    static Client& Get(){
        static judgeConnectSingleton JCS(
                __config__::JUDGE_SERVER_CONNECT_SIZE,
                __config__::JUDGE_SERVER_IP,
                __config__::JUDGE_SERVER_PORT
                );
        return  JCS.judgeClient;
    }


private:
    explicit judgeConnectSingleton(
std::size_t connect_size,std::string_view judge_server_ip,int port
            )
        :judgeClient{connect_size,judge_server_ip,port}
    {
        //处理judgeServer返回过来的评测结果
        judgeClient.set_result_handle(
            []( MessageResultJudge & msgRes){
                std::string judgeKey( msgRes.get_key() );
                rojcppForServer::judgeCacheKeyFactory JKFC(judgeKey);

                // 对返回的信息进行处理
#ifdef  DEBUG
                std::cout << "==========judgeMessage_result: "<< std::endl;
                std::cout << msgRes << std::endl;
#endif
                // 1.如果 没有成功,结束
                if(msgRes.get_code() != judgeResult_id::SUCCESS) {
                    // TODO
                    // 1.1 写入redisCache 
                    // 1.2 写入mysql 数据库
                    // 1.3 进行webSocket的写入并且关闭
                    return;
                }

                //返回的是全部的测试点
                if( msgRes.get_msg() == "all") {
                    //auto pointInfo = rojcppForServer::get_single_point_info(msgRes.get_msg());
                    //auto pointJson = rojcppForServer::single_judge_result_info(pointInfo, *msgRes.begin());
                    //redisConnectPoolSingleton::GetPool().LSET(
                            //JKFC.get_results_array_key(),
                            //std::get<0>(pointInfo), //index
                            //pointJson
                            //);
                    //redisConnectPoolSingleton::GetPool().DEL(JKFC.get_is_judging_key().c_str());
                    //redisConnectPoolSingleton::GetPool().DEL(JKFC.get_point_size_key().c_str());
                    //redisConnectPoolSingleton::GetPool().DEL(JKFC.get_results_array_key().c_str());
                    Cache::get().del(JKFC.get_is_judging_key().c_str());
                    Cache::get().del(JKFC.get_point_size_key().c_str());
                    Cache::get().del(JKFC.get_results_array_key().c_str());

                    // TODO 最后一次发送
                    // TODO 清空 web_socket fd
                    
                    // 关闭web_socket
                    //rojcpp::WS_manager::get_instance().close_by_key(judgeKey);
                    //rojcpp::WS_manager::get_instance().close_by_key(judgeKey);
                    const MsgEntity close_msg("finish judge");
                    rojcpp::WS_manager::get_instance().send_msg_by_id( judgeKey, close_msg.dumps(),true);
                    return;
                }


                //返回的是测试点的数量
                if( rojcppForServer::str_begin_with(msgRes.get_msg(),"allSize") ) {

                    auto pointSize = rojcppForServer::str2int(
                                    rojcppForServer::get_allSize_size(msgRes.get_msg()) );

                    // 设置cahe 测试点数量
                    //redisConnectPoolSingleton::GetPool().SETEX(
                            //JKFC.get_point_size_key(),
                            //pointSize,
                            //60*10 //十分钟
                            //);
                    Cache::get().set(
                            JKFC.get_point_size_key(),
                            std::to_string(pointSize),
                            60*30 //30分钟
                            );

                    //创建 pointSize 个测试点
                    //redisConnectPoolSingleton::GetPool().RPUSH(
                                //JKFC.get_results_array_key(),
                                //"",
                                //pointSize
                                //);
                                //
                    // 不在这里创建,得接收到单个测试信息后,创建
                    //Cache::get().set(
                            //JKFC.get_results_array_key(),
                            //"",
                            //60*30 //30分钟
                            //);

                    return;
                }

                //只可能是返回的是单个的测试点
                
                // 是哪个测试点
                auto pointInfo = rojcppForServer::get_single_point_info(msgRes.get_msg());
                auto pointJson = rojcppForServer::single_judge_result_info(pointInfo, *msgRes.begin());
                //redisConnectPoolSingleton::GetPool().LSET(
                        //JKFC.get_results_array_key(),
                        //std::get<0>(pointInfo), //index
                        //pointJson
                        //);
                Cache::get().append(JKFC.get_results_array_key()
                        , pointJson
                        ,__KEY_EXPIRED_SECONS
                        );

                // TODO 发送 单个测试点的信息
                //
                std::ostringstream single_result_msg;
                single_result_msg << R"({"point":)" << std::get<0>(pointInfo) << ",";
                single_result_msg << R"("result":)" << pointJson << "}";
                rojcpp::WS_manager::get_instance().send_msg_by_id( judgeKey,single_result_msg.str() );

            });
    }
    Client judgeClient;
    int poolSize;
};
