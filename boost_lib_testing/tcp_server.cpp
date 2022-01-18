#include <iostream>
#include <string>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

#define DEFAULT_COMMUNICATION_PORT	(6300)

class tcp_connection : public boost::enable_shared_from_this<tcp_connection>
{
public:

  	static boost::shared_ptr<tcp_connection> create(boost::asio::execution_context& exec_context)
  	{
    	return boost::shared_ptr<tcp_connection>(new tcp_connection(exec_context));
  	}

  	tcp::socket& socket()
  	{
    	return socket_;
  	}

  void start()
  {
    message_ = "tcp server start response";

    boost::asio::async_write(socket_, boost::asio::buffer(message_),
        boost::bind(&tcp_connection::handle_write, shared_from_this(),
          	boost::asio::placeholders::error,
          	boost::asio::placeholders::bytes_transferred));
  }

private:
  tcp_connection(boost::asio::execution_context& exec_context)
    : socket_(exec_context)
  {
  }

  void handle_write(const boost::system::error_code& /*error*/,
      size_t /*bytes_transferred*/)
  {
  }

  tcp::socket socket_;
  std::string message_;
};

class tcp_server
{
public:
  tcp_server(boost::asio::io_service& io_service)
    : acceptor_(io_service, tcp::endpoint(tcp::v4(), DEFAULT_COMMUNICATION_PORT))
  {
    start_accept();
  }

private:
  	void start_accept()
  	{
    	boost::shared_ptr<tcp_connection> new_connection =
      		tcp_connection::create(acceptor_.get_executor().context());

    	acceptor_.async_accept(new_connection->socket(),
        	boost::bind(&tcp_server::handle_accept, this, new_connection,
          		boost::asio::placeholders::error));
  	}

  	void handle_accept(boost::shared_ptr<tcp_connection> new_connection,
    	const boost::system::error_code& error)
  	{
    	if (!error)
    	{
      		new_connection->start();
    	}

    	start_accept();
  	}

  	tcp::acceptor acceptor_;
};

int main()
{
	try
  	{
    	boost::asio::io_service io_service;
    	tcp_server server(io_service);
    	io_service.run();
  	}
  	catch (std::exception& e)
  	{
    	std::cerr << e.what() << std::endl;
  	}

  	return 0;
}