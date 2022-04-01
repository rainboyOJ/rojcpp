/**
 * @desc 连接 judgeServer 是对
 *  ../third-party/judgercpp/judge_server/src/Client.hpp 的封装
 */

#pragma once


class judgeConnectSingleton {

    //得到返回的结果
    void handleJudgeResult();

private:
    int poolSize;
};
