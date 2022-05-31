#include <iostream>

#include "srpc.pb.h"

using namespace std;

int main(){
    srpc::req data{};
    data.set_url("/user/login");
    data.set_body("hi");

    string out;
    data.SerializeToString(&out);

    cout<<out<<endl;
}