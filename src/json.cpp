#include <iostream>

#include <nlohmann/json.hpp>

using namespace std;

using json = nlohmann::json;

int main(){
    json obj = {
        {"a",1}
    };
    obj["b"]= {1.0,2,3};

    cout<<obj.dump(4)<<endl;
}