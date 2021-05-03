#include "Client.h"
#include <string>

void Client::help()
{
  std::string help =
    "Commands available:\n"
    " - help\n"
    "        This help message\n"
    " - exit\n"
    "        Quit the session\n"
    " - connect <string>\n"
    "        Connect to server\n"
    " - send <string>\n"
    "        Send message\n";
  std::cout << help;
}

void Client::exit()
{
  socket_->cancel();
  socket_->close();
}

void Client::connect(const std::string& connectintString)
{
  this->ip_ = "";
  bool endIp = false;
  std::string port = "";
  for (int j = 0; j < connectintString.length(); j++) {
    if (connectintString[j] == ':') {
      endIp = true;
      j++;
    }
    !endIp ?
      ip_ += connectintString[j] :
      port += connectintString[j];
  }
  this->port_ = std::stoi(port);

  socket_.reset(new tcp::socket(io_service_));
  
  std::cout << "Connect to " << ip_ << ":" << port << "\n";
  socket_->async_connect(tcp::endpoint(boost::asio::ip::address_v4::from_string(ip_), this->port_),
    boost::bind(&Client::handle_connect, this,
      socket_,
      boost::asio::placeholders::error));
}

void Client::handle_connect(boost::shared_ptr<tcp::socket> socket,
                            const boost::system::error_code& error)
{
  if (!error)
  {
    std::cout << "Successfully connected\n";
  }
  else
  {
    std::cerr << "Connected error: " << error.message() << std::endl;
    socket_->close();
  }
}

void Client::send(std::string msg)
{
  boost::system::error_code error;
  socket_->async_write_some(ba::buffer(msg),
    boost::bind(&Client::handle_send, this,
      boost::asio::placeholders::error,
      boost::asio::placeholders::bytes_transferred,
      msg));
}

void Client::handle_send(const boost::system::error_code& error, 
                         size_t bytes_transferred, 
                         std::string msg)
{
  if (!error)
  {
    std::cout << "Message sent to " + ip_ + ":" << port_ << " with data: " + msg + "\n";
  }
  else
  {
    std::cerr << "Connected error: " << error.message() << std::endl;
  }
}

Client::Client() : 
  ip_(""),
  port_(-1)
{
  run();
}

void Client::run()
{
  std::string command;

  while (1)
  {
    std::cout << "Client> ";

    std::getline(std::cin, command);

    if (command.find("help") != std::string::npos)
    {
      help();
    }
    if (command.find("exit") != std::string::npos)
    {
      exit();
      return;
    }
    if (command.find("connect") != std::string::npos)
    {
      std::string connectingString = "";
      for (int i = 8; i < command.length(); i++) 
      {
        connectingString = connectingString + command[i];
      }
      connect(connectingString);
      io_service_.run();
    }
    if (command.find("send") != std::string::npos)
    {
      std::string msg = "";
      for (int i = 5; i < command.length(); i++)
      {
        msg = msg + command[i];
      }
      send(msg);

    }
  }
}
