//
// posix_chat_client.cpp
// ~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2008 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//


//linuxƽ̨����

#include <cstdlib>
#include <cstring>
#include <iostream>

#include <boost/array.hpp>
#include <boost/bind.hpp>
#include <boost/asio.hpp>

#include "ChatMsg.h"

#if defined(BOOST_ASIO_HAS_POSIX_STREAM_DESCRIPTOR)

using boost::asio::ip::tcp;
namespace posix = boost::asio::posix;

class posix_chat_client
{
public:
	posix_chat_client(boost::asio::io_service& io_service,
		tcp::resolver::iterator endpoint_iterator)
		: socket_(io_service),
		input_(io_service, ::dup(STDIN_FILENO)),
		output_(io_service, ::dup(STDOUT_FILENO)),
		input_buffer_(ChatMsg::Max_Body_Length)
	{
		// Try connecting to the first endpoint.
		tcp::endpoint endpoint = *endpoint_iterator;
		socket_.async_connect(endpoint,
			boost::bind(&posix_chat_client::handle_connect, this,
			boost::asio::placeholders::error, ++endpoint_iterator));
	}

private:

	void handle_connect(const boost::system::error_code& error,
		tcp::resolver::iterator endpoint_iterator)
	{
		if (!error)
		{
			// Read the fixed-length header of the next message from the server.
			boost::asio::async_read(socket_,
				boost::asio::buffer(read_msg_.Data(), ChatMsg::Header_Length),
				boost::bind(&posix_chat_client::handle_read_header, this,
				boost::asio::placeholders::error));

			// Read a line of input entered by the user.
			boost::asio::async_read_until(input_, input_buffer_, '\n',
				boost::bind(&posix_chat_client::handle_read_input, this,
				boost::asio::placeholders::error,
				boost::asio::placeholders::bytes_transferred));
		}
		else if (endpoint_iterator != tcp::resolver::iterator())
		{
			// That endpoint didn't work, try the next one.
			socket_.close();
			tcp::endpoint endpoint = *endpoint_iterator;
			socket_.async_connect(endpoint,
				boost::bind(&posix_chat_client::handle_connect, this,
				boost::asio::placeholders::error, ++endpoint_iterator));
		}
	}

	void handle_read_header(const boost::system::error_code& error)
	{
		if (!error && read_msg_.DecodeHeader())
		{
			// Read the variable-length body of the message from the server.
			boost::asio::async_read(socket_,
				boost::asio::buffer(read_msg_.Body(), read_msg_.BodyLength()),
				boost::bind(&posix_chat_client::handle_read_body, this,
				boost::asio::placeholders::error));
		}
		else
		{
			close();
		}
	}

	void handle_read_body(const boost::system::error_code& error)
	{
		if (!error)
		{
			// Write out the message we just received, terminated by a newline.
			static char eol[] = { '\n' };
			boost::array<boost::asio::const_buffer, 2> buffers = {{
				boost::asio::buffer(read_msg_.Body(), read_msg_.BodyLength()),
					boost::asio::buffer(eol) }};
				boost::asio::async_write(output_, buffers,
					boost::bind(&posix_chat_client::handle_write_output, this,
					boost::asio::placeholders::error));
		}
		else
		{
			close();
		}
	}

	void handle_write_output(const boost::system::error_code& error)
	{
		if (!error)
		{
			// Read the fixed-length header of the next message from the server.
			boost::asio::async_read(socket_,
				boost::asio::buffer(read_msg_.Data(), ChatMsg::Header_Length),
				boost::bind(&posix_chat_client::handle_read_header, this,
				boost::asio::placeholders::error));
		}
		else
		{
			close();
		}
	}

	void handle_read_input(const boost::system::error_code& error,
		std::size_t length)
	{
		if (!error)
		{
			// Write the message (minus the newline) to the server.
			write_msg_.BodyLength(length - 1);
			input_buffer_.sgetn(write_msg_.Body(), length - 1);
			input_buffer_.consume(1); // Remove newline from input.
			write_msg_.EncodeHeader();
			boost::asio::async_write(socket_,
				boost::asio::buffer(write_msg_.Data(), write_msg_.Length()),
				boost::bind(&posix_chat_client::handle_write, this,
				boost::asio::placeholders::error));
		}
		else if (error == boost::asio::error::not_found)
		{
			// Didn't get a newline. Send whatever we have.
			write_msg_.BodyLength(input_buffer_.size());
			input_buffer_.sgetn(write_msg_.Body(), input_buffer_.size());
			write_msg_.EncodeHeader();
			boost::asio::async_write(socket_,
				boost::asio::buffer(write_msg_.Data(), write_msg_.Length()),
				boost::bind(&posix_chat_client::handle_write, this,
				boost::asio::placeholders::error));
		}
		else
		{
			close();
		}
	}

	void handle_write(const boost::system::error_code& error)
	{
		if (!error)
		{
			// Read a line of input entered by the user.
			boost::asio::async_read_until(input_, input_buffer_, '\n',
				boost::bind(&posix_chat_client::handle_read_input, this,
				boost::asio::placeholders::error,
				boost::asio::placeholders::bytes_transferred));
		}
		else
		{
			close();
		}
	}

	void close()
	{
		// Cancel all outstanding asynchronous operations.
		socket_.close();
		input_.close();
		output_.close();
	}

private:
	tcp::socket socket_;
	posix::stream_descriptor input_;
	posix::stream_descriptor output_;
	ChatMsg read_msg_;
	ChatMsg write_msg_;
	boost::asio::streambuf input_buffer_;
};

int main(int argc, char* argv[])
{
	try
	{
		if (argc != 3)
		{
			std::cerr << "Usage: posix_chat_client <host> <port>\n";
			return 1;
		}

		boost::asio::io_service io_service;

		tcp::resolver resolver(io_service);
		tcp::resolver::query query(argv[1], argv[2]);
		tcp::resolver::iterator iterator = resolver.resolve(query);

		posix_chat_client c(io_service, iterator);

		io_service.run();
	}
	catch (std::exception& e)
	{
		std::cerr << "Exception: " << e.what() << "\n";
	}

	return 0;
}

#else // defined(BOOST_ASIO_HAS_POSIX_STREAM_DESCRIPTOR)
int main() {}
#endif // defined(BOOST_ASIO_HAS_POSIX_STREAM_DESCRIPTOR)