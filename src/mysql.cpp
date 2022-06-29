#include <iostream>

#ifdef __cplusplus
extern "C" {
#endif

#include <mysql.h>

#ifdef __cplusplus
}
#endif
int main(){
    const char* user="root";
    const char* passwd="admin123";
    const char* table="test";
    const char* host="127.0.0.1";
    unsigned int port = 3306;
   
    MYSQL* conn = mysql_init(nullptr);
    conn = mysql_real_connect(conn, host, user, passwd, table, port, NULL, 0);
    if (conn == nullptr) {
        return -1;
    }
    std::cout<<"ok"<<std::endl;

    int res = mysql_query(conn,"select * from user");
    std::cout<<"res:"<<res<<std::endl;

    MYSQL_RES* result = mysql_store_result(conn);
    std::cout<<"result:"<<res<<std::endl;

    if (result) {
        MYSQL_ROW sql_row = mysql_fetch_row(result);
        while (sql_row != nullptr){
            std::cout<<"id:"<<sql_row[0];
            std::cout<<",name:"<<sql_row[1];
            std::cout<<",sex:"<<sql_row[2]<<std::endl;

            sql_row = mysql_fetch_row(result);
        } 
    }
    mysql_free_result(result);
    mysql_close(conn);
}