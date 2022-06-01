#include <iostream>
#include <memory>
#include <string>

#include <grpcpp/grpcpp.h>

#include "srpc.grpc.pb.h"

using namespace std;

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using grpc::ServerCompletionQueue;
using grpc::ServerAsyncResponseWriter;

using srpc::rpc;

class ServerImpl{

public:
    ~ServerImpl(){
        server_->Shutdown();
        cq_->Shutdown();
    }

    void Run(){

        std::string address("0.0.0.0:50051");

        ServerBuilder builder;
        
        builder.AddListeningPort(address, grpc::InsecureServerCredentials());
        
        builder.RegisterService(&service_);

        cq_ = builder.AddCompletionQueue();

        server_ = builder.BuildAndStart();

        std::cout << "Server listening on " << address << std::endl;

        HandleRpcs();
    }

    private:
        class CallData{
            public:
            CallData(rpc::AsyncService* service, ServerCompletionQueue* cq)
            : service(service), cq(cq), res_write(&ctx), status(CREATE){
                Proceed();
            }
            void Proceed(){

                if (status == CREATE) {
                    status = PROCESS;
                    service->Requestcall(&ctx, &req, &res_write, cq, cq, this);
                } else if (status == PROCESS) {
                    status = FINISH;

                    new CallData(service, cq);

                    std::string prefix("hello");
                    res.set_data(prefix + req.url());
                    res_write.Finish(res, Status::OK, this);
                } else {
                    if (status == FINISH) {
                        std::cout<<"finish"<<std::endl;
                    }
                    delete this;
                }
            }
            private:
                rpc::AsyncService* service;
                ServerCompletionQueue* cq;
                ServerContext ctx;

                srpc::req req;
                srpc::res res;

                ServerAsyncResponseWriter<srpc::res> res_write;

                enum CallStatus {CREATE, PROCESS, FINISH};
                CallStatus status;
        };
    void HandleRpcs(){
        new CallData(&service_, cq_.get());
        void* tag;
        bool ok;
        while(true) {
            bool ret = cq_->Next(&tag, &ok);
            std::cout<<ret<<std::endl;

            static_cast<CallData*>(tag)->Proceed();
        }
    }

    std::unique_ptr<ServerCompletionQueue> cq_;
    rpc::AsyncService service_;
    std::unique_ptr<Server> server_;   
};


int main(int argc, char** argv){

    ServerImpl server;
    
    server.Run();
    return 0;
}

//protoc -I=./ --cpp_out=./include *.proto
//protoc --grpc_out=include --plugin=protoc-gen-grpc=/home/code/download/vcpkg-master/installed/x64-linux/tools/grpc/grpc_cpp_plugin *.proto