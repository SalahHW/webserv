#include "Directive.hpp"

class ServerNameDirective : public Directive {
public:
    ~ServerNameDirective();
    ServerNameDirective(Block* currentContext, const std::string& fullDirectiveLine);
    ServerNameDirective(const ServerNameDirective& other);
    ServerNameDirective& operator=(const ServerNameDirective& other);

    virtual bool validateSpecific();
    virtual void displayInfo() const;
    virtual void apply(Server& server);

    void setServerName(const std::string& serverName);

    const std::string& getServername() const;

private:
    ServerNameDirective();

    std::string serverName;
};