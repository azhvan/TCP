#pragma once
#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind.hpp>

namespace ba = boost::asio;
using ba::ip::tcp;


class Client
{
  ba::io_service io_service_;
  tcp::socket socket_;

  std::string ip_;
  int port_;

  void help();
  void exit();  
  void connect(const std::string& connectintString);  
  void send(std::string msg);  
  void run();
public:
  Client();
};

