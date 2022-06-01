#include <iostream>
#include <memory>
#include <string>
#include <thread>

#include <grpc/support/log.h>
#include <grpcpp/grpcpp.h>

#include "srpc.grpc.pb.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using grpc::ClientAsyncResponseReader;
using grpc::CompletionQueue;

using srpc::rpc;

class sRpcClient {
public:
    sRpcClient(std::shared_ptr<Channel> channel): stub_(rpc::NewStub(channel)) {
    }

    void call(int i) {

        srpc::req req{};

        req.set_url("/user/login" + std::to_string(i));

        AsyncClientCall* client = new AsyncClientCall;

        client->res_reader = stub_->PrepareAsynccall(&client->ctx, req, &cq_);

        client->res_reader->StartCall();

        client->res_reader->Finish(&client->res, &client->status, (void*)client);
    }
    void AsyncCompleteRpc(){

        void* got_tag;

        bool ok = false;

        while(cq_.Next(&got_tag, &ok)) {

            AsyncClientCall* client = static_cast<AsyncClientCall*>(got_tag);

            if (client->status.ok()) {
                 std::cout<<client->res.data()<<std::endl;
            }else {
                std::cout<<client->status.error_details()<<std::endl;
                std::cout<<client->status.error_code()<<": "<<client->status.error_message()<<std::endl;
            }

            delete client;        
        }
    }
private:

    struct AsyncClientCall {
        srpc::res res;
        ClientContext ctx;
        Status status;

        std::unique_ptr<ClientAsyncResponseReader<srpc::res>> res_reader;
    };
    CompletionQueue cq_;  

    std::unique_ptr<rpc::Stub> stub_;
};

int main(int argc, char** argv) {

    sRpcClient client(grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials()));

    std::thread thread_ = std::thread(&sRpcClient::AsyncCompleteRpc, &client);

    for (int i=0;i<1024;i++) {
        client.call(i);
    }
    thread_.join();

    std::cout<<"exit"<<std::endl;

    return 0;
}