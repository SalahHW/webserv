#include <map>
#include <string>

class HeaderBuilder {
public:
    HeaderBuilder();
    ~HeaderBuilder();

    void addHeader(const std::string& key, const std::string& value);
    std::string buildHeaders() const;

private:
    std::map<std::string, std::string> headers;
};