/**
 * @desc 常用工具函数
 */
#pragma once

#include <string>
#include <string_view>
#include "plusaes.hpp"
#include "../third-party/judgercpp/judger.h"

namespace rojcppForServer {
    

/**
 * unescape \n \r
 */
std::string unescape_newline(std::string_view str){
    std::string ret;

    std::size_t str_index = 0;
    while ( str_index  < str.length() ) {
        if( str[str_index] == '\\' && str_index+1 < str.length() ) {
            char next = str[str_index+1];
            if( next == 'n' ){
                ret.push_back('\n');
                str_index++;
            }
            else if ( next == 'r' ){
                ret.push_back('\r');
                str_index++;
            }
        }
        else
            ret.push_back( str[str_index]);
        str_index++;
    }
    return ret;
}


/**
 * @desc s1的开头包含s2
 */
bool str_begin_with (std::string_view s1,std::string_view s2){
    if( s1.size() < s2.size() ) return false;
    for(int i = 0;i< s2.size() ;++i){
        if( s1[i] != s2[i])
            return false;
    }
    return true;
}

/**
 * @desc 得到 allSize:10 这个字符串的数值10
 *
 */
std::string_view get_allSize_size(std::string_view s1){
    auto iter = s1.begin()+8;
    return std::string_view(iter,s1.end()-iter);
}

int str2int(std::string_view _str){
    int a = 0;
    int  flag = 1; 
    for (const auto& ch : _str) {
        if( ch < '0' || ch > '9'){ if( ch == '-') flag = -1;  }
        if( ch >= '0' && ch <= '9'){ a = a*10 + ch-'0'; }
    }
    return a;
}

//得到单个测试点的信息
// 编号,输入文件名,输出文件名
std::tuple<int,std::string,std::string > get_single_point_info(std::string_view msg){
    int number_comma = 0;
    std::string tmp;
    std::tuple<int,std::string,std::string >  ret;
    for (const auto& e : msg) {
        if( e == ',') {
            number_comma++;
            if(number_comma == 1) //是数字
                std::get<0>(ret) = str2int(tmp);
            else if(number_comma == 2) //是数字
                std::get<1>(ret) = std::move(tmp); //是输入文件名
            tmp.clear();
        }
        else tmp += e;
    }
    std::get<2>(ret) = std::move(tmp); //是输出文件名
    return ret;
}


//struct result {
    //int cpu_time;
    //int real_time;
    //long memory;
    //int signal;
    //int exit_code;
    //int error;
    //int result;
//};

std::string judge_result_dumps2Json(const result & res){
    std::ostringstream oss;
    oss << "{" ;
    oss << R"("cpu_time":)" << res.cpu_time << ",";
    oss << R"("real_time":)" << res.real_time<< ",";
    oss << R"("memory":)" << res.memory<< ",";
    oss << R"("signal":)" << res.signal<< ",";
    oss << R"("exit_code":)" << res.exit_code<< ",";
    oss << R"("error":)" << res.error<< ",";
    oss << R"("result":)" << res.result;
    oss << "}" ;
    return oss.str();
}

std::string single_judge_result_info(std::tuple<int,std::string,std::string> Info,const result &res){
    std::ostringstream oss;
    oss << "{" ;
    oss << R"("point":)" << std::get<0>(Info) << ",";
    oss << R"("in":)" << std::get<1>(Info) << ",";
    oss << R"("out":)" << std::get<2>(Info) << ",";
    oss << R"("result":)" << judge_result_dumps2Json(res) ;
    return oss.str();
}

// ============ 在redisCache过程的生成key值

struct judgeCacheKeyFactory {
    explicit judgeCacheKeyFactory(std::string_view key)
        :baseName{key}
    {}

    std::string get_point_size_key(){
        return append_key_suffix("_point_size");
    }

    std::string get_results_array_key(){
        return append_key_suffix("_results");
    }
    std::string get_is_judging_key(){
        return append_key_suffix("_is_judging");
    }

    //@desc 缓存 结果的key,不是从judgeSer
    std::string get_Cache_result_key(){
        return append_key_suffix("_cache_result");
    }

    std::string append_key_suffix(std::string_view suffix){
        std::ostringstream oss;
        oss << baseName << suffix;
        return oss.str();
    }

    std::string baseName;
};


//加密 对数字进行加密
std::string __encrypt(uint64_t number, 
        const unsigned char * key = __config__::ENCRYT_KEY , 
        const int key_size = 16){
    std::string data = std::to_string(number);
    plusaes::Error e;
    const bool padding = true;
    const std::size_t encrypted_size = (padding) ? data.size() + (16 - data.size() % 16) : data.size();

    //创建加密后存储空间
    std::vector<unsigned char> encrypted(encrypted_size);

    e = plusaes::encrypt_ecb((unsigned char*)data.data(), (unsigned long)data.size(), key, key_size, &encrypted[0], (unsigned long)encrypted.size(), padding);
    //TODO throw KEY_ERROR
    //if(e != plusaes::kErrorOk){
        //std::cout << "err" << std::endl;
    //}
    // 把加密的后转成16进制的字符串
    std::string ret{};

    auto toHex = [](unsigned char c) -> char {
        unsigned char  t = c & 0x0f;
        if( t >= 0 and t <= 9 ) return t+'0';
        return t-10 +'A';
    };

    for (const auto& e : encrypted) {
        unsigned char t = (e&0xff);
        ret += toHex(t >> 4);
        ret += toHex(t);
    }
    return ret;
}

uint64_t __decrypt(const std::string & s,
        const unsigned char * key = __config__::ENCRYT_KEY , 
        const int key_size = 16){
    const bool padding = true;
    const int encrypted_size = s.length() / 2;
    std::vector<unsigned char> encrypted(encrypted_size);
    std::vector<unsigned char> decrypted(encrypted_size);

    auto strToNum = [](const char a[]) -> unsigned char {
        unsigned char t = 0;
        for(int i=0;i<=1;++i){
            if( '0' <=a[i] && a[i] <= '9')
                t = (t<<4) | ((a[i]-'0') & 0x0f);
            else if( 'A' <=a[i] && a[i] <= 'F')
                t = (t<<4) | ((a[i]-'A' + 10) & 0x0f);
        }
        return t;
    };

    for(int i =0 ;i< s.length() ;i += 2){
        encrypted[i / 2] = strToNum(&s[i]);
    }

    plusaes::Error e;
    unsigned long padded = 0;
    if (padding) {
        e = plusaes::decrypt_ecb(encrypted.data(), (unsigned long)encrypted.size(), key, key_size, &decrypted[0], (unsigned long)decrypted.size(), &padded);
    }
    else {
        e = plusaes::decrypt_ecb(encrypted.data(), (unsigned long)encrypted.size(), key, key_size, &decrypted[0], (unsigned long)decrypted.size(), 0);
    }

    std::string num_str(decrypted.begin(),decrypted.end()-padded);
    return std::stoull(num_str);
}

} // end namespace rojcpp
