#include "PostRequestHandler.hpp"

std::string PostRequestHandler::handle(const Request& request)
{
	std::string responseBody = "Data received: " + request.getBody();

    std::ostringstream oss;
    oss << responseBody.size();

    std::string response = "HTTP/1.1 200 OK\r\n";
    response += "Content-Type: text/plain\r\n";
    response += "Content-Length: " + oss.str() + "\r\n";
    response += "\r\n";
    response += responseBody;

    return response;
}