#include "WebSocketServer.h"

session::session(tcp::socket&& socket, DAQHandler *daqHandler)
        : ws_(std::move(socket)), daqHandler_(daqHandler) {}

void session::run(){
        net::dispatch(ws_.get_executor(), beast::bind_front_handler(&session::on_run, shared_from_this()));
}
void session::on_run() {
    ws_.set_option(websocket::stream_base::timeout::suggested(beast::role_type::server));
    ws_.set_option(websocket::stream_base::decorator(
        [](websocket::response_type& res){
            res.set(http::field::server,
                std::string(BOOST_BEAST_VERSION_STRING) +
                    " websocket-server-async");
        }));
    ws_.async_accept(beast::bind_front_handler(&session::on_accept, shared_from_this()));
}

void session::on_accept(beast::error_code ec){
    if(ec) return fail(ec, "accept");
    do_read();
}

void session::do_read(){
    ws_.async_read(buffer_, beast::bind_front_handler(&session::on_read, shared_from_this()));
}

void session::on_read(beast::error_code ec, std::size_t bytes_transferred){
    boost::ignore_unused(bytes_transferred);
    if(ec == websocket::error::closed) return;
    if(ec)fail(ec, "read");
    std::string request = beast::buffers_to_string(buffer_.data());
    std::string response = daqHandler_->response(request.c_str());
    ws_.text(ws_.got_text());
    ws_.async_write(net::buffer(response), beast::bind_front_handler(&session::on_write, shared_from_this()));
}

void session::on_write(beast::error_code ec, std::size_t bytes_transferred){
    boost::ignore_unused(bytes_transferred);
    if(ec) return fail(ec, "write");
    buffer_.consume(buffer_.size());
    do_read();
}

//------------------------------------------------------------------------------

listener::listener(net::io_context& ioc, tcp::endpoint endpoint, DAQHandler *daqHandler)
    : ioc_(ioc), acceptor_(ioc), daqHandler_(daqHandler) {
    beast::error_code ec;
    acceptor_.open(endpoint.protocol(), ec);
    if(ec){
        fail(ec, "open");
        return;
    }
    acceptor_.set_option(net::socket_base::reuse_address(true), ec);
    if(ec){
        fail(ec, "set_option");
        return;
    }
    acceptor_.bind(endpoint, ec);
    if(ec){
        fail(ec, "bind");
        return;
    }
    acceptor_.listen(net::socket_base::max_listen_connections, ec);
    if(ec){
        fail(ec, "listen");
        return;
    }
}
void listener::run(){
    do_accept();
}
void listener::do_accept(){
    acceptor_.async_accept(net::make_strand(ioc_), beast::bind_front_handler(&listener::on_accept, shared_from_this()));
}
void listener::on_accept(beast::error_code ec, tcp::socket socket){
    if(ec){
        fail(ec, "accept");
    }else{
        std::make_shared<session>(std::move(socket), daqHandler_)->run();
    }
    cout<<"do accept PID "<<getpid()<<endl;
    do_accept();
}