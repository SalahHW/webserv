#ifndef LOCATION_HPP
#define LOCATION_HPP

class Location {
private:
    std::string path;
    std::string rootDirectory;
    std::string indexFile;
    std::string redirectionPath;
    bool autoIndex;
    bool getAccepted;
    bool postAccepted;
    bool deleteAccepted;
    int clientMaxBodySize;
};

#endif