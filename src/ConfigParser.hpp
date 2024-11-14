#include <vector>
#include <memory>
#include <string>

#include "Converter.hpp"

class ConfigParser {
public:
    ConfigParser(const std::string& configPath);
    std::vector<std::unique_ptr<Converter>> parse();

private:
    std::string configPath;
};
