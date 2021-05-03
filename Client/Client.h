#pragma once
#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind.hpp>

namespace ba = boost::asio;
using ba::ip::tcp;


class Client
{
  ba::io_service io_service_;
  boost::shared_ptr<tcp::socket> socket_;

  std::string ip_;
  int port_;

  void help();
  void exit();
  
  void connect(const std::string& connectintString);
  void handle_connect(boost::shared_ptr<tcp::socket> socket, 
    const boost::system::error_code& error);
  
  void send(std::string msg);
  void handle_send(const boost::system::error_code& error,
    size_t bytes_transferred,
    std::string msg);
  
  void run();
public:
  Client();
};

