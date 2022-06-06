#include <iostream>
#include <thread>

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/beast/version.hpp>

#include <boost/asio/spawn.hpp>
#include <boost/asio/strand.hpp>

namespace beast = boost::beast;
namespace http = beast::http;
namespace websocket = beast::websocket;
namespace net = boost::asio;

using namespace std;
using tcp = boost::asio::ip::tcp;

void do_session(tcp::socket socket){
    try{
        websocket::stream<tcp::socket> ws{move(socket)};

        ws.set_option(websocket::stream_base::decorator(
            [](websocket::response_type& res){
                res.set(
                    http::field::server,
                    string(BOOST_BEAST_VERSION_STRING)
                    +
                    " websocket-server-sync");
            }
        ));

        ws.accept();

        for(;;){
            beast::flat_buffer buffer;

            ws.read(buffer);
            ws.text(ws.got_text());
            ws.write(buffer.data());
        }
    } catch(beast::system_error const& err) {
        cerr<<"boost_err: "<<err.code().message()<<endl;
    } catch(exception const& err) {
        cerr<<"err: "<< err.what()<<endl;
    }
}


int main(){
    auto const address = boost::asio::ip::make_address("0.0.0.0");
    unsigned short port = static_cast<unsigned short>(atoi("8081"));

    int num_workers = 1;

    boost::asio::io_context ioc{1};
    tcp::acceptor acceptor{ioc, {address, port}};

    for(;;) {
        tcp::socket socket{ioc};
        acceptor.accept(socket);
        cout<<"accept"<<endl;

        thread(&do_session, move(socket)).detach();
    }
}