#include <iostream>

// #ifdef __cplusplus
// extern "C" {
// #endif

//#include <mongoc.h>

// #ifdef __cplusplus
// }
// #endif

#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <bsoncxx/json.hpp>
#include <bsoncxx/builder/stream/document.hpp>

using namespace std;

/*
void test_mongoc(){
    mongoc_init();

    mongoc_client_t* client = mongoc_client_new("mongodb://root:Admin123Abc123@10.10.10.5:37017");

    mongoc_database_t* database = mongoc_client_get_database(client, "pirate_treasure");
    mongoc_collection_t* collection = mongoc_client_get_collection(client, "pirate_treasure", "User_0");
    
    bson_t* command = BCON_NEW("ping", BCON_INT32(1));

    bson_t reply;
    bson_error_t error;
    bool ret= mongoc_client_command_simple(client, "admin", command, NULL, &reply, &error);
    if (!ret) {
        cout<<error.message<<endl;
        return;
    }
    char* str = bson_as_json(&reply, NULL);
    cout<<str<<endl;

    bson_t* insert = BCON_NEW("hello", BCON_UTF8("world"));
    
    ret = mongoc_collection_insert(collection, MONGOC_INSERT_NONE, insert, NULL, &error);
    if (!ret) {
        cout<<error.message<<endl;
        return;
    }

    bson_destroy(insert);
    bson_destroy(&reply);
    bson_destroy(command);
    bson_free(str);

    mongoc_collection_destroy(collection);
    mongoc_database_destroy(database);
    mongoc_client_destroy(client);
    mongoc_cleanup();
}*/

void test_mongocxx(){
    mongocxx::instance inst{};
    mongocxx::client conn{mongocxx::uri("mongodb://root:Admin123Abc123@10.10.10.5:37017")};

    bsoncxx::builder::stream::document doc{};
    doc<<"hello"<<"world";

    auto collection = conn["pirate_treasure"]["User_0"];
    
    collection.insert_one(doc.view());
    auto cursor = collection.find({});
    for (auto &&data : cursor) {
        cout<<bsoncxx::to_json(data)<<endl;
    }

}

int main(){
    //test_mongoc();
    test_mongocxx();
}