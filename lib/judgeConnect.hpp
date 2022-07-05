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
#include "../CURD/judger_table.hpp"


using netcore::Cache;


//转换enum class 到对应的数据
template<typename T>
auto to_underlying(T e) noexcept 
    ->typename std::underlying_type<T>::type
{
    return static_cast<typename std::underlying_type<T>::type>(e);
}


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
#ifdef __NETCORE__DEBUG__
                LOG(DEBUG) << "==========judgeMessage_result: ";
                std::cout << msgRes << std::endl;
                std::cout << "-->>><<" << std::endl;
#endif
                // 1.如果 没有成功,结束
                if(msgRes.get_code() != judgeResult_id::SUCCESS) {
                    LOG(DEBUG) << "judge failed!";
                    // TODO
                    // 1.1 写入Cache 
                    // 1.2 写入mysql 数据库
                    CURD::judgeTable::update_solution(
                            msgRes.get_key(),
                            msgRes.get_code(),
                            0, //time_used
                            0, //memory_used, 
                            0, //score
                            ""sv, //mini_result
                            ""sv //full_result
                            );

                    return;
                }

                //返回的是全部的测试点
                if( msgRes.get_msg() == "all") {
                    LOG(DEBUG) << "get judge ALL Result!";
                    //新的过期时间
                    Cache::get().new_expire(JKFC.get_is_judging_key().c_str(),30);
                    Cache::get().new_expire(JKFC.get_point_size_key().c_str(),30);
                    Cache::get().new_expire(JKFC.get_results_array_key().c_str(),30);

                    auto [exist,result_str] 
                        = Cache::get().get(JKFC.get_results_array_key());
                    LOG(DEBUG) << result_str;
                    

                    
                    //const MsgEntity close_msg(MSG::FINISH_JUDGE);
                    //rojcpp::WS_manager::get_instance().send_msg_by_id( judgeKey, close_msg.dumps(),true);
                    //TODO 发送评测的

                    // 扫描得到所有的数据
                    enum class short_desc_resut: int {
                            RJ_WAIT=-3,
                            WAIT=2,
                            RUN=-1,
                            AC=0,
                            WA=1,
                            PE=2,
                            TLE=3,
                            MLE=4,
                            OLE=5,
                            RE=6,
                            SE=7
                    };

                 // -3:RJ WAIT -2:WAIT -1:RUN 0:AC 1:WA 2:PE 3:TLE 4:MLE 5:OLE 6:RE 7:SE
                    unsigned long long sum_real_time = 0;
                    unsigned long long sum_cpu_time = 0;
                    unsigned long long sum_memory = 0;
                    std::string full_result = "";
                    std::string mini_result;
                    int final_result = 0;
                    int total_size = msgRes.results_size();
                    mini_result.reserve(total_size);
                    int right_size = 0;

                    auto result_to_mini_result = [&mini_result](int result) ->void {
                        switch(result) {
                            case RESULT_MEAN::WRONG_ANSWER :
                                mini_result += 'W';
                                break;
                            case RESULT_MEAN::ACCEPT :
                                mini_result += 'A';
                                break;
                            case RESULT_MEAN::CPU_TIME_LIMIT_EXCEEDED :
                            case RESULT_MEAN::REAL_TIME_LIMIT_EXCEEDED:
                                mini_result += 'T';
                                break;
                            case RESULT_MEAN::RUNTIME_ERROR:
                                mini_result += 'R';
                                break;
                            case RESULT_MEAN::MEMORY_LIMIT_EXCEEDED:
                                mini_result += 'M';
                                break;
                            case RESULT_MEAN::SYSTEM_ERROR:
                                mini_result += 'S';
                                break;
                            default:
                                mini_result += 'X'; //表示未知情况
                                break;

                        };
                    };


                    //扫描结果,判断是哪个类型的结果
                    for (const auto& e : msgRes) {
                        sum_real_time += e.real_time;
                        sum_cpu_time += e.cpu_time;
                        sum_memory += e.memory;
                        if( final_result == 0 && e.result != 0 )
                            final_result = e.result;
                        if(e.result == RESULT_MEAN::ACCEPT)
                            right_size++;
                        result_to_mini_result(e.result);
                    }

                    if( exist )
                        CURD::judgeTable::update_solution(
                                msgRes.get_key(), //solution_id
                                final_result ,//int result
                                sum_cpu_time, //unsigned long long time_used
                                sum_memory, //unsigned long long memory_used
                                total_size == 0 ? 0 : 100*right_size / total_size, //int score
                                mini_result, //std::string_view mini_result
                                result_str //std::string_view full_result)
                            );
                    return;
                }


                //返回的是测试点的数量
                if( rojcppForServer::str_begin_with(msgRes.get_msg(),"allSize") ) {
                    LOG(DEBUG) << "get judge Size Count!";

                    auto pointSize = rojcppForServer::str2int(
                                    rojcppForServer::get_allSize_size(msgRes.get_msg()) );

                    Cache::get().set(
                            JKFC.get_point_size_key(),
                            std::to_string(pointSize),
                            60*30 //30分钟
                            );
                    return;
                }

                
                //把 messageResultJudge里的msg字符串转成相应的信息
                // 第几个点,输入数据文件名,正确的输出数据文件名
                std::tuple<int,std::string,std::string > pointInfo 
                    = rojcppForServer::get_single_point_info(msgRes.get_msg());

                //只可能是返回的是单个的测试点
                LOG(DEBUG) << "get judge single point :" << std::get<0>(pointInfo);

                //把结果转成JSON字符串
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

            });
    }
    Client judgeClient;
    int poolSize;
};
