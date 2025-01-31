#include "ClientTimeOutDirective.hpp"

ClientTimeOutDirective::~ClientTimeOutDirective() { }

ClientTimeOutDirective::ClientTimeOutDirective(Block* currentContext, const std::string& fullDirectiveLine)
    : Directive(currentContext, fullDirectiveLine)
    , time(0)
{
  setName("client_tiemout");
  setMinArgs(1);
  setMaxArgs(1);
  addContext("server");
  addContext("location");
  validate();
}

ClientTimeOutDirective::ClientTimeOutDirective(const ClientTimeOutDirective& other)
    : Directive(other)
    , time(other.time)
{
}

ClientTimeOutDirective& ClientTimeOutDirective::operator=(const ClientTimeOutDirective& other)
{
  if (this != &other)
  {
    Directive::operator=(other);
    time = other.time;
  }
  return *this;
}

bool ClientTimeOutDirective::validateSpecific()
{
  const std::vector<std::string>& arguments = getArguments();
  const std::string& arg = arguments[0];

  char lastChar = arg[arg.size() - 1];

  if (lastChar == 's' || lastChar == 'S')
  {
    std::string timeStr = arg.substr(0, arg.size() - 1);
  }
  else if (isdigit(lastChar))
  {
  }
  else
  {
    std::cerr << "Error: Directive \"" << getName()
              << "\" has an invalid argument." << std::endl;
    return false;
  }

  if (!utils::convertToInt(arguments[0].c_str(), time) || time <= 0)
  {
    std::cerr << "Error: Directive \"" << getName()
              << "\" has an invalid argument." << std::endl;
    return false;
  }
  return true;
}

void ClientTimeOutDirective::displayInfo() const
{
  std::cout << "Name : " << this->getName() << std::endl
            << "- TimeOut : " << time << "s" << std::endl;
}

void ClientTimeOutDirective::apply(Server& server)
{
  server.setClientTimeOut(time);
}

void ClientTimeOutDirective::apply(Location& location)
{
  location.setClientTimeOut(time);
}

void ClientTimeOutDirective::setTimeOut(size_t time)
{
  this->time = time;
}

size_t ClientTimeOutDirective::getTimeOut() const
{
  return time;
}