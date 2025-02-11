#include "RequestRouter.hpp"

std::string RequestRouter::route(const Request& request)
{
  const std::string& method = request.getMethod();

  if (method == "GET")
  {
    return GetRequestHandler::handle(request);
  }
  else if (method == "POST")
  {
    return PostRequestHandler::handle(request);
  }
  else if (method == "DELETE")
  {
    return DeleteRequestHandler::handle(request);
  }
  else
  {
    std::cerr << "Error: Unsupported HTTP method " << method << std::endl;
    return ErrorRequestHandler::generateErrorResponse(405);
  }
}