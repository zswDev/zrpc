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

void fail(beast::error_code ecode, char const* what){
    cerr<<what<<" : "<<ecode.message()<<endl;
}

class session: public enable_shared_from_this<session> {
private:
    websocket::stream<beast::tcp_stream> ws_;
    beast::flat_buffer buffer_;

public:
    explicit session(tcp::socket&& socket): ws_(move(socket)){

    }

    void run(){
        net::dispatch(
            ws_.get_executor(),
            beast::bind_front_handler(
                &session::on_run,
                shared_from_this()));
    }

    void on_run(){
        ws_.set_option(
            websocket::stream_base::timeout::suggested(
                beast::role_type::server
            ));

        ws_.set_option(
            websocket::stream_base::decorator(
                [](websocket::response_type& res){
                    res.set(
                        http::field::server,
                        string(BOOST_BEAST_VERSION_STRING)
                        +
                        " websocket-server-async"
                    );
                }
            )
        );

        ws_.async_accept(
            beast::bind_front_handler(
                &session::on_accept,
                shared_from_this()
            )
        );
    }

    void on_accept(beast::error_code ecode){
        if (ecode) {
            fail(ecode, "accept");
            return;
        }
        do_read();
    }

    void do_read(){
        ws_.async_read(
            buffer_,
            beast::bind_front_handler(
                &session::on_read,
                shared_from_this()
            )
        );
    }
    void on_read(
        beast::error_code ecode,
        size_t bytes_tramsferred
    ){
        boost::ignore_unused(bytes_tramsferred);
        if (ecode == websocket::error::closed) {
            return;
        }
        if (ecode) {
            fail(ecode, "read");
            return;
        }

        ws_.text(ws_.got_text());
        ws_.async_write(
            buffer_.data(),
            beast::bind_front_handler(
                &session::on_write,
                shared_from_this()
            )
        );
    }

    void on_write(
        beast::error_code ecode,
        size_t bytes_transferred
    ){
        boost::ignore_unused(bytes_transferred);
        if (ecode) {
            fail(ecode, "write");
            return;
        }
        buffer_.consume(buffer_.size());
        
        do_read();
    }
};

class listener: public enable_shared_from_this<listener>{
private:
    net::io_context& ioc_;
    tcp::acceptor acceptor_;

public:
    listener(
        net::io_context& ioc,
        tcp::endpoint endpoint
    ) : ioc_(ioc), acceptor_(ioc) {

        beast::error_code ecode;

        acceptor_.open(endpoint.protocol(), ecode);
        if (ecode) {
            fail(ecode, ",open");
            return;
        }

        acceptor_.set_option(
            net::socket_base::reuse_address(true),
            ecode);
        if (ecode) {
            fail(ecode, ",set_option");
            return;
        }

        acceptor_.bind(endpoint, ecode);
        if(ecode){
            fail(ecode, ",bind");
            return;
        }

        acceptor_.listen(
            net::socket_base::max_listen_connections,
            ecode);
        if (ecode) {
            fail(ecode, "listen");
            return;
        }
    }
    void run() {
        do_accept();
    }   

private:
    void do_accept(){
        acceptor_.async_accept(
            net::make_strand(ioc_),
            beast::bind_front_handler(
                &listener::on_accept,
                shared_from_this()
            )
        );
    }
    void on_accept(beast::error_code ecode, tcp::socket socket){
        if (ecode) {
            fail(ecode, "accept");
        } else {
            make_shared<session>(move(socket))->run();
        }
        do_accept();
    }
};

int main(){
    auto const address = boost::asio::ip::make_address("0.0.0.0");
    unsigned short port = static_cast<unsigned short>(atoi("8081"));

    int num_workers = 1;

    boost::asio::io_context ioc{1};
   
    make_shared<listener>(ioc, tcp::endpoint{address, port})->run();

    // vector<thread> v;
    // v.emplace_back([&ioc]{
    //     ioc.run();
    // });

    ioc.run();
}