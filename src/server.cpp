#include <iostream>
#include <memory>
#include <string>

#include <grpcpp/grpcpp.h>

#include "srpc.grpc.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;

class sRpcService final : public srpc::rpc::Service{

    Status call(ServerContext *context,
                const srpc::req *request,
                srpc::res *reply) override{

        std::string prefix("Hi!");
        reply->set_data(prefix + request->url());
        return Status::OK;
    }
};

void Serve(){
    
    std::string address("0.0.0.0:50051");
    sRpcService serve;
    ServerBuilder builder;
    builder.AddListeningPort(address, grpc::InsecureServerCredentials());
    builder.RegisterService(&serve);

    std::unique_ptr<Server> server(builder.BuildAndStart());
    std::cout << "Server listening on " << address << std::endl;
    server->Wait();
}

int main(int argc, char** argv){
    Serve();
    return 0;
}


// int main(){
//     srpc::req req{};
//     req.set_param("aa");
//     req.set_path("/user/login");
//     string s{};
//     req.SerializeToString(&s);
//     cout<<s<<endl;
// }


//protoc -I=./ --cpp_out=./include *.proto
//protoc --grpc_out=include --plugin=protoc-gen-grpc=/home/code/download/vcpkg-master/installed/x64-linux/tools/grpc/grpc_cpp_plugin *.proto