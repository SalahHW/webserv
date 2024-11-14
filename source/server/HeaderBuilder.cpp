#include "HeaderBuilder.hpp"
#include <sstream>

HeaderBuilder::~HeaderBuilder() { }

HeaderBuilder::HeaderBuilder() { }

void HeaderBuilder::addHeader(const std::string& key, const std::string& value)
{
    headers[key] = value;
}

std::string HeaderBuilder::buildHeaders() const
{
    std::ostringstream headersStream;
    for (std::map<std::string, std::string>::const_iterator it = headers.begin(); it != headers.end(); ++it) {
        headersStream << it->first << ": " << it->second << "\r\n";
    }
    return headersStream.str();
}
