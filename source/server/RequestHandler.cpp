#include "RequestHandler.hpp"

Response RequestHandler::handleRequest(const Request &request) {
  if (request.getMethod() == "GET") {
    return handleGet(request);
  } else if (request.getMethod() == "POST") {
    return handlePost(request);
  } else if (request.getMethod() == "DELETE") {
    return handleDelete(request);
  }

  return buildErrorResponse(405, "Method Not Allowed");
}

Response RequestHandler::handleGet(const Request &request) {
  Response response;
  std::string filePath = "." + request.getUri();

  if (!fileExists(filePath)) {
    return buildErrorResponse(404, "Not Found");
  }

  std::string content = readFile(filePath);

  response.setStatusCode(200);
  response.setReasonPhrase("OK");
  response.setBody(content);
  response.addHeader("Content-Type", "text/html");
  response.addHeader("Content-Length", intToString(content.size()));

  return response;
}

Response RequestHandler::handlePost(const Request &request) {
  Response response;
  std::string filePath = "./uploaded_data.txt";

  writeFile(filePath, request.getBody());

  response.setStatusCode(201);
  response.setReasonPhrase("Created");
  response.setBody("Data successfully saved to uploaded_data.txt.");
  response.addHeader("Content-Length", intToString(response.getBody().size()));

  return response;
}

Response RequestHandler::handleDelete(const Request &request) {
  Response response;
  std::string filePath = "." + request.getUri();

  if (!fileExists(filePath)) {
    return buildErrorResponse(404, "Not Found");
  }

  if (!deleteFile(filePath)) {
    return buildErrorResponse(500, "Internal Server Error");
  }

  response.setStatusCode(200);
  response.setReasonPhrase("OK");
  response.setBody("File successfully deleted.");
  response.addHeader("Content-Length", intToString(response.getBody().size()));

  return response;
}

Response RequestHandler::buildErrorResponse(int statusCode,
                                            const std::string &reason) {
  Response response;
  response.setStatusCode(statusCode);
  response.setReasonPhrase(reason);
  response.setBody("Error: " + reason);
  response.addHeader("Content-Length", intToString(response.getBody().size()));
  return response;
}

bool RequestHandler::fileExists(const std::string &filePath) const {
  std::ifstream file(filePath.c_str());
  return file.good();
}

std::string RequestHandler::readFile(const std::string &filePath) const {
  std::ifstream file(filePath.c_str());
  std::stringstream buffer;

  buffer << file.rdbuf();
  return buffer.str();
}

void RequestHandler::writeFile(const std::string &filePath,
                               const std::string &data) const {
  std::ofstream file(filePath.c_str(), std::ios::app);
  file << data;
}

bool RequestHandler::deleteFile(const std::string &filePath) const {
  return std::remove(filePath.c_str()) == 0;
}

std::string RequestHandler::intToString(int value) const {
  std::ostringstream oss;
  oss << value;
  return oss.str();
}