#include <iostream>

#include <SQLiteCpp/SQLiteCpp.h>


int main(){
    SQLite::Database db("test.db3", SQLite::OPEN_CREATE|SQLite::OPEN_READWRITE);
    int ret = 0;
    // ret = db.exec("create table user("
    //     "id int primary key not null,"
    //     "name text not null,"
    //     "sex int not null"
    // ")");
    // std::cout<<"ret:"<<ret<<std::endl;
    
    // ret = db.exec("insert into user(id, name, sex) values(1,\"aab\",1),(2,\"xyz\",2)");
    // std::cout<<"ret:"<<ret<<std::endl;

    SQLite::Statement query(db, "select * from user where id=?");
    query.bind(1, 2);
    while(query.executeStep()){
        std::cout<<query.getColumn(0)<<","<<query.getColumn(1)<<","<<query.getColumn(2)<<std::endl;
    }
}