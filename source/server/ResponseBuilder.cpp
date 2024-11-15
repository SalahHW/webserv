#include "ResponseBuilder.hpp"
#include <dirent.h>
#include <fstream>
#include <sstream>
#include <sys/stat.h>
#include <unistd.h>

ResponseBuilder::ResponseBuilder(RequestParsed& requestParsed, const Server& server)
    : requestParsed(requestParsed)
    , server(server)
{
}

ResponseBuilder::~ResponseBuilder() { }

std::string ResponseBuilder::buildResponse()
{
    prepareResponse();

    std::string statusLine = headerBuilder.buildStatusLine();
    std::string headers = headerBuilder.buildHeaders();

    return statusLine + headers + "\r\n" + body;
}

void ResponseBuilder::prepareResponse()
{
    HttpStatusCode statusCode = requestParsed.statusCode;

    if (statusCode == OK) {
        prepareSuccessResponse();
    } else if (statusCode == MOVED_PERMANENTLY || statusCode == FOUND) {
        prepareRedirectionResponse();
    } else if (statusCode >= 400 && statusCode < 500) {
        prepareClientErrorResponse();
    } else if (statusCode >= 500) {
        prepareServerErrorResponse();
    } else {
        prepareServerErrorResponse();
    }
}

void ResponseBuilder::prepareSuccessResponse()
{
    if (!findMatchingLocation(requestParsed.uri, matchingLocation)) {
        requestParsed.statusCode = PAGE_NOT_FOUND;
        prepareClientErrorResponse();
        return;
    }

    std::string filePath = matchingLocation.getRootDirectory() + requestParsed.uri;

    if (isDirectory(filePath)) {
        if (matchingLocation.getAutoIndex()) {
            body = generateDirectoryListing(filePath, requestParsed.uri);
            headerBuilder.setContentType("text/html");
        } else {
            filePath += "/" + matchingLocation.getIndexFile();
        }
    }

    if (body.empty()) {
        body = readFile(filePath);
        headerBuilder.setContentType(getContentType(filePath));
    }

    headerBuilder.setStatusCode(OK);
}

void ResponseBuilder::prepareRedirectionResponse()
{
    headerBuilder.setStatusCode(requestParsed.statusCode);
    headerBuilder.addHeader("Location", matchingLocation.getRedirectionPath());
    body = "<html><body><h1>Redirecting...</h1></body></html>";
}

void ResponseBuilder::prepareClientErrorResponse()
{
    headerBuilder.setStatusCode(requestParsed.statusCode);

    std::map<int, std::string>::const_iterator it = server.getErrorPages().find(requestParsed.statusCode);
    if (it != server.getErrorPages().end()) {
        body = readFile(it->second);
        headerBuilder.setContentType(getContentType(it->second));
    } else {
        body = "<html><body><h1>" + getReasonPhrase(requestParsed.statusCode) + "</h1></body></html>";
        headerBuilder.setContentType("text/html");
    }
}

void ResponseBuilder::prepareServerErrorResponse()
{
    headerBuilder.setStatusCode(INTERNAL_SERVER_ERROR);
    body = "<html><body><h1>500 Internal Server Error</h1></body></html>";
    headerBuilder.setContentType("text/html");
}

bool ResponseBuilder::findMatchingLocation(const std::string& uri, Location& matchingLocation)
{
    const std::map<std::string, Location>& locations = server.getLocations();
    size_t longestMatch = 0;
    bool found = false;

    for (std::map<std::string, Location>::const_iterator it = locations.begin(); it != locations.end(); ++it) {
        const std::string& locationPath = it->first;
        if (uri.compare(0, locationPath.length(), locationPath) == 0 && locationPath.length() > longestMatch) {
            longestMatch = locationPath.length();
            matchingLocation = it->second;
            found = true;
        }
    }
    return found;
}

bool ResponseBuilder::isDirectory(const std::string& path)
{
    struct stat s;
    if (stat(path.c_str(), &s) == 0) {
        return S_ISDIR(s.st_mode);
    }
    return false;
}

std::string ResponseBuilder::generateDirectoryListing(const std::string& directoryPath, const std::string& uri)
{
    std::ostringstream html;
    html << "<html><body><h1>Index of " << uri << "</h1><ul>";

    DIR* dir = opendir(directoryPath.c_str());
    if (!dir) {
        return "<html><body><h1>403 Forbidden</h1></body></html>";
    }

    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL) {
        html << "<li><a href=\"" << uri << entry->d_name << "\">" << entry->d_name << "</a></li>";
    }

    closedir(dir);
    html << "</ul></body></html>";
    return html.str();
}

std::string ResponseBuilder::readFile(const std::string& filePath)
{
    std::ifstream file(filePath.c_str());
    if (!file.is_open()) {
        return "<html><body><h1>404 Not Found</h1></body></html>";
    }

    std::ostringstream content;
    content << file.rdbuf();
    file.close();
    return content.str();
}

std::string ResponseBuilder::getContentType(const std::string& filePath)
{
    size_t dotPos = filePath.find_last_of('.');
    if (dotPos == std::string::npos) {
        return "application/octet-stream";
    }

    std::string extension = filePath.substr(dotPos + 1);
    if (extension == "html") {
        return "text/html";
    } else if (extension == "css") {
        return "text/css";
    } else if (extension == "js") {
        return "application/javascript";
    } else if (extension == "png") {
        return "image/png";
    } else if (extension == "jpg" || extension == "jpeg") {
        return "image/jpeg";
    }
    return "application/octet-stream";
}

std::string ResponseBuilder::getReasonPhrase(int code)
{
    switch (code) {
    case 200:
        return "OK";
    case 301:
        return "Moved Permanently";
    case 302:
        return "Found";
    case 400:
        return "Bad Request";
    case 403:
        return "Forbidden";
    case 404:
        return "Not Found";
    case 500:
        return "Internal Server Error";
    default:
        return "Unknown Status";
    }
}

// Implement other methods similarly, ensuring to use requestParsed.statusCode
// ...
