#include "Directive.hpp"

Directive::~Directive() { }

Directive::Directive(Block* currentContext, const std::string& fullDirectiveLine)
    : currentContext(currentContext)
    , fullDirectiveLine(fullDirectiveLine)
{
    tokenizeName();
}

Directive::Directive(const Directive& other)
    : currentContext(other.currentContext)
    , fullDirectiveLine(other.fullDirectiveLine)
    , name(other.name)
    , arguments(other.arguments)
    , minArgs(other.minArgs)
    , maxArgs(other.maxArgs)
    , contexts(other.contexts)
{
}

Directive& Directive::operator=(const Directive& other)
{
    if (this != &other) {
        fullDirectiveLine = other.fullDirectiveLine;
        name = other.name;
        currentContext = other.currentContext;
        arguments = other.arguments;
        minArgs = other.minArgs;
        maxArgs = other.maxArgs;
        contexts = other.contexts;
    }
    return *this;
}

void Directive::validate()
{
    isValid = validateContext() && validateArgsSize() && validateSpecific();
}

bool Directive::validateContext() const
{
    if (std::find(contexts.begin(), contexts.end(), currentContext->getName()) == contexts.end()) {
        std::cerr << "Error: Directive \"" << name << "\" cannot be used in \"" << currentContext << "\" block" << std::endl;
        return false;
    }
    return true;
}

bool Directive::validateArgsSize() const
{
    int argsSize = static_cast<int>(arguments.size());

    if (argsSize < minArgs || argsSize > maxArgs) {
        std::cerr << "Error: Directive \"" << name << "\" has an invalid number of arguments." << std::endl;
        return false;
    }
    return true;
}

void Directive::apply(Server& server)
{
    (void)server;
}

void Directive::apply(Location& location)
{
    (void)location;
}

void Directive::setFullDirectiveLine(const std::string& line)
{
    this->fullDirectiveLine = line;
}

void Directive::setName(const std::string& name)
{
    this->name = name;
}

void Directive::setCurrentContext(Block* context)
{
    this->currentContext = context;
}

void Directive::addArgument(const std::string& argument)
{
    this->arguments.push_back(argument);
}

void Directive::addContext(const std::string& context)
{
    this->contexts.push_back(context);
}

void Directive::setMinArgs(int min)
{
    this->minArgs = min;
}

void Directive::setMaxArgs(int max)
{
    this->maxArgs = max;
}

std::string Directive::getFullDirectiveLine() const
{
    return fullDirectiveLine;
}

std::string Directive::getName() const
{
    return name;
}

Block* Directive::getCurrentContext() const
{
    return this->currentContext;
}

std::vector<std::string> Directive::getArguments() const
{
    return arguments;
}

std::vector<std::string> Directive::getContexts() const
{
    return contexts;
}

int Directive::getMinArgs() const
{
    return minArgs;
}

int Directive::getMaxArgs() const
{
    return maxArgs;
}

bool Directive::good() const
{
    return isValid;
}

void Directive::tokenizeName()
{
    std::stringstream ss(fullDirectiveLine);
    std::string token;

    ss >> token;
    name = token;

    while (ss >> token) {
        arguments.push_back(token);
    }
}