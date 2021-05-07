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
  socket_.cancel();
  socket_.close();
}

void Client::connect(const std::string& connectingString)
{
  auto colonPos = connectingString.find(':');
  if ( colonPos == std::string::npos)
  {
    std::cerr << "Port doesn`t find\n";
    return;
  }
  ip_ = connectingString.substr(0, colonPos);
  std::string portPart = connectingString.substr(colonPos + 1);
  port_ = std::stoi(portPart);

  boost::system::error_code error;
  socket_.connect(tcp::endpoint(boost::asio::ip::address_v4::from_string(ip_), port_), error);
  if (error)
  {
    std::cerr << "Connect failed: " << error.message() << "\n";
    return;
  }
  std::cout << "Connect to " << ip_ << ":" << port_ << "\n";
}

void Client::send(std::string msg)
{
  if (ip_ == "" || port_ == -1)
  {
    std::cerr << "Firstly connect!\n";
    return;
  }
  boost::system::error_code error;
  if(!socket_.is_open())
    socket_.connect(tcp::endpoint(boost::asio::ip::address_v4::from_string(ip_), port_));
  socket_.write_some(ba::buffer(msg), error);
  if (error)
  {
    std::cerr << "Send failed: " << error.message() << "\n";
  }
    std::cout << "Message sent to " << ip_ << ":" << port_ << " with data: " << msg << "\n";
  socket_.close();
}

Client::Client() : 
  socket_(io_service_),
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
      if(socket_.is_open())
        exit();
      return;
    }
    if (command.find("connect") != std::string::npos)
    {
      std::string connectingString = "";
      for (int i = 8; i < command.length(); i++) 
      {
        if (command[i] == ' ')
          continue;
        connectingString = connectingString + command[i];
      }
      connect(connectingString);
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
