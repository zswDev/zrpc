#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <functional>
#include <vector>

#include <grpc/support/log.h>
#include <grpcpp/grpcpp.h>

#include "srpc.grpc.pb.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using grpc::ClientAsyncResponseReader;
using grpc::CompletionQueue;

using srpc::rpc;

class Client {
private:

    struct AsyncClientCall {
        srpc::res res;
        ClientContext ctx;
        Status status;
        std::unique_ptr<ClientAsyncResponseReader<srpc::res>> res_reader;
        std::function<void(srpc::res& res)> callback;
    };
    CompletionQueue cq_;  
    std::unique_ptr<rpc::Stub> stub_;

public:
    Client(std::string& url): Client(grpc::CreateChannel(url, grpc::InsecureChannelCredentials())){
    }
    Client(std::shared_ptr<Channel> channel): stub_(rpc::NewStub(channel)) {
    }
    void call(srpc::req& req, std::function<void(srpc::res& res)> callback) {
        AsyncClientCall* client = new AsyncClientCall;
        client->callback = callback;
        client->res_reader = stub_->PrepareAsynccall(&client->ctx, req, &cq_);
        client->res_reader->StartCall();
        client->res_reader->Finish(&client->res, &client->status, (void*)client);
    }
    void start(){
        std::thread work = std::thread(Client::try_loop, this);
        work.join();
    }
private:
    void loop(){
        void* got_tag;

        bool ok = false;

        while(cq_.Next(&got_tag, &ok)) {

            AsyncClientCall* client = static_cast<AsyncClientCall*>(got_tag);
            if (client->status.ok()) {
                client->callback(client->res);
                std::cout<<client->res.data()<<std::endl;
            }else {
                std::cout<<client->status.error_details()<<std::endl;
                std::cout<<client->status.error_code()<<": "<<client->status.error_message()<<std::endl;
            }
            delete client;        
        }
    }
    void try_loop(){
         while(true) {
            try {
                this->loop();
            } catch(std::exception e) {
                std::cout<<"loop:"<<e.what()<<std::endl;
            }
        }
    }
};

class Register{
public:
    std::vector<std::string>& getAddrList(){
        std::vector<std::string> addrList= {"127.0.0.1:80"};
        return addrList;
    }
};

int main(int argc, char** argv) {

    // sRpcClient client(grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials()));

    // std::thread thread_ = std::thread(&sRpcClient::AsyncCompleteRpc, &client);
    std::string url = "127.0.0.1:80";
    Client* client = new Client(url);

    srpc::req req{};
    req.set_url("/user");

    for (int i=0;i<1024;i++) {
        client->call(req, [](srpc::res res)->void{
            std::cout<<res.data()<<std::endl;
        });
    }
    client->start();

    std::cout<<"exit"<<std::endl;

    return 0;
}