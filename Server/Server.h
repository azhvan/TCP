#pragma once
#include <boost/asio.hpp>
#include <boost/asio/spawn.hpp>
#include <boost/thread.hpp>

namespace ba = boost::asio;
using ba::ip::tcp;
using ba::yield_context;

class Server
{
private:
  int port_;
  ba::io_service io_service_;
  tcp::acceptor *acceptor_;
  boost::thread_group threads;
  void help();
  void exit();
  void start();
  void setPort(int port);
  void stop();
public:
  Server();
  void run();
};

