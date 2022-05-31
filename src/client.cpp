#include <iostream>
#include <memory>
#include <string>

#include <grpcpp/grpcpp.h>

#include "srpc.grpc.pb.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

using srpc::rpc;

class sRpcClient {
 public:
  sRpcClient(std::shared_ptr<Channel> channel): stub_(rpc::NewStub(channel)) {
  }

  // Assembles the client's payload, sends it and presents the response back
  // from the server.
  std::string call() {
    // Data we are sending to the server.
    srpc::req req{};
    req.set_url("/user/login");

    // Container for the data we expect from the server.
    srpc::res res;

    // Context for the client. It could be used to convey extra information to
    // the server and/or tweak certain RPC behaviors.
    ClientContext context;

    // The actual RPC.
    Status status = stub_->call(&context, req, &res);

    // Act upon its status.
    if (status.ok()) {
      return res.data();
    } else {
        std::cout<<status.error_details()<<std::endl;
      std::cout<<status.error_code()<<": "<<status.error_message()<<std::endl;
      return "RPC failed";
    }
  }

 private:
  std::unique_ptr<rpc::Stub> stub_;
};

int main(int argc, char** argv) {
  // Instantiate the client. It requires a channel, out of which the actual RPCs
  // are created. This channel models a connection to an endpoint (in this case,
  // localhost at port 50051). We indicate that the channel isn't authenticated
  // (use of InsecureChannelCredentials()).
    sRpcClient client(grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials()));

    std::string req = client.call();
    std::cout<<"req "<<req<<std::endl;

    return 0;
}