
#include <iostream>
#include <sw/redis++/connection.h>
#include <sw/redis++/redis++.h>

#ifdef __cplusplus 
extern "C" {
#endif
   // #include <hiredis/hiredis.h>
#ifdef __cplusplus
}
#endif

using namespace std;

using namespace sw::redis;

// void test_hiredis(){
//     redisContext* conn = redisConnect("10.10.10.7", 7002);
//     if (conn->err) {
//         cout<<conn->errstr<<endl;
//         exit(0);
//     }

//     redisReply* reply = (redisReply*)redisCommand(conn, "set test_1 a1");
//     cout<<reply->str<<endl;
//     freeReplyObject(reply);

//     reply = (redisReply*)redisCommand(conn, "get test_1");
//     cout<<reply->str<<endl;
//     freeReplyObject(reply);

//     redisFree(conn);
// }

void test_redisxx(){
    ConnectionOptions conn_opts;
    conn_opts.host = "10.10.10.7";  // Required.
    conn_opts.port = 7001;
    conn_opts.db = 0;

    ConnectionPoolOptions pool_opts;
    pool_opts.size = 5;
    pool_opts.wait_timeout =std::chrono::milliseconds(100);

    auto* conn = new RedisCluster(conn_opts, pool_opts);
    auto ret = conn->set("test_1", "a1");
    cout<<ret<<endl;

    auto val = conn->get("test_1");
    cout<<val.value()<<endl;
}

int main(){
    test_redisxx();
}