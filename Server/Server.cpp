#include "Server.h"
#include <iostream>
#include <string>
#include <thread>

void Server::help()
{
  std::string help =
    "Commands available:\n" 
    " - help\n" 
    "        This help message\n"
    " - exit\n"
    "        Quit the session\n"
    " - start\n"
    "        Start server\n"
    " - setPort\n"
    "        Set port for server\n"
    " - stop\n"
    "        Stop server\n";
  std::cout << help;
} 

void Server::exit()
{

}

void Server::start()
{
  if (port_ == -1) {
    std::cout << "Firstly set port!\n";
    return;
  }

  acceptor_->listen(5);
  spawn(io_service_, [&](yield_context yield)
    {
      while (1)
      {
        tcp::socket socket_(acceptor_->get_executor());
        acceptor_->async_accept(socket_, yield);
        spawn(yield, [s = std::move(socket_), this](yield_context yield) mutable {
          char buf[1024] = {};
          size_t bytes = s.async_read_some(ba::buffer(buf), yield);
          std::cout << "\rMessage received from IP: " << s.remote_endpoint().address() << ", with data: " << std::string(buf, buf + bytes) << 
            "\nServer> ";
        });
      };
    });
  
  threads.create_thread(boost::bind(&ba::io_service::run, &io_service_));
  std::cout << "Server started on port " << port_ << "\n";
}

void Server::setPort(int port)
{
  this->port_ = port;
  acceptor_->bind(tcp::endpoint(tcp::v4(), port_));
  std::cout << "Server will be on port " << this->port_ << "\n";
}

void Server::stop()
{
  io_service_.stopped();
  std::cout << "Server stopped on port " << port_ << "\n";
}

Server::Server() :
  port_(-1)
{
  acceptor_ = new tcp::acceptor(io_service_);
  acceptor_->open(tcp::v4());
  acceptor_->set_option(tcp::acceptor::reuse_address(true));
  run();
}

void Server::run()
{
  std::string command;
  while (1) {
    std::cout << "Server> ";
    
    std::getline(std::cin, command);
    
    if (command.find("help") != std::string::npos)
    {
      help();
    }
    if (command.find("exit") != std::string::npos)
    {
      return;
    }
    if (command.find("start") != std::string::npos)
    {
      start();
    }
    if (command.find("setPort") != std::string::npos)
    {
      std::string port = "";
      for (int i = 8; i < command.length(); i++) 
      {
        port = port + command[i];
      }
      setPort(std::stoi(port));
    }
    if (command.find("stop") != std::string::npos)
    {
      stop();
    }
  }
}