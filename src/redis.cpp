
#include <iostream>
#include <sw/redis++/connection.h>
#include <sw/redis++/redis++.h>
#include <thread>


#ifdef __cplusplus 
extern "C" {
#endif

    #include <unistd.h>
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
RedisCluster* createConn(){
    ConnectionOptions conn_opts;
    conn_opts.host = "10.10.10.7";  // Required.
    conn_opts.port = 7001;
    conn_opts.db = 0;

    ConnectionPoolOptions pool_opts;
    pool_opts.size = 5;
    pool_opts.wait_timeout =std::chrono::milliseconds(100);

    auto* conn = new RedisCluster(conn_opts, pool_opts);
    return conn;
}

Redis* createConn2(){
    ConnectionOptions conn_opts;
    conn_opts.host = "127.0.0.1";  // Required.
    conn_opts.port = 6001;
    conn_opts.db = 0;

    ConnectionPoolOptions pool_opts;
    pool_opts.size = 5;
    pool_opts.wait_timeout =std::chrono::milliseconds(100);

    auto* conn = new Redis(conn_opts, pool_opts);
    return conn;
}
void test_redisxx(){
    auto conn = createConn2();

    auto sub = conn->subscriber();
    sub.on_message([](string channel, string msg){
        cout<<"on:"<<channel<<","<<msg<<endl;
    });
    sub.subscribe("test1");
    
    while(true){
        sub.consume();
    }

    // auto ret = conn->set("test_1", "a1");
    // cout<<ret<<endl;
    // auto val = conn->get("test_1");
    // cout<<val.value()<<endl;
}
void test_redisxx2(){
    auto conn = createConn2();
    conn->publish("test1", "hi");
}

int main(){
    thread work = thread(test_redisxx);
    sleep(1);
    test_redisxx2();
    work.join();
}