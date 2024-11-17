#ifndef CONFIG_PARSER_H
#define CONFIG_PARSER_H

#include <string>
#include <vector>
#include <stdexcept>
#include <fstream>

class ConfigParser {
    std::vector<std::pair<std::string, std::vector<std::string>>> operations;

public:
    ConfigParser(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            throw std::runtime_error("Unable to open config file: " + filename);
        }

        std::string line;
        while (std::getline(file, line)) {
            if (line.empty() || line[0] == '#') continue;
            auto tokens = split(line, ' ');
            operations.emplace_back(tokens[0], std::vector<std::string>(tokens.begin() + 1, tokens.end()));
        }
    }

    const std::vector<std::pair<std::string, std::vector<std::string>>>& getOperations() const {
        return operations;
    }

private:
    static std::vector<std::string> split(const std::string& str, char delimiter) {
        std::vector<std::string> tokens;
        std::string token;
        for (char ch : str) {
            if (ch == delimiter) {
                if (!token.empty()) tokens.push_back(token);
                token.clear();
            } else {
                token += ch;
            }
        }
        if (!token.empty()) tokens.push_back(token);
        return tokens;
    }
};

#endif
