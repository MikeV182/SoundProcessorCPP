#include "../src/ConfigParser.hpp"
#include <gtest/gtest.h>
#include <fstream>
#include <stdexcept>

std::string createTempConfigFile(const std::string& content) {
    std::string filename = "temp_config.txt";
    std::ofstream file(filename);
    file << content;
    file.close();
    return filename;
}

TEST(ConfigParserTest, ValidConfigFile) {
    std::string content = "# This is a comment\noperation1 arg1 arg2 arg3\noperation2 arg4 arg5\n# Another comment\noperation3";
    std::string filename = createTempConfigFile(content);

    ConfigParser parser(filename);
    auto operations = parser.getOperations();

    ASSERT_EQ(operations.size(), 3);
    EXPECT_EQ(operations[0].first, "operation1");
    EXPECT_EQ(operations[0].second, (std::vector<std::string>{"arg1", "arg2", "arg3"}));
    EXPECT_EQ(operations[1].first, "operation2");
    EXPECT_EQ(operations[1].second, (std::vector<std::string>{"arg4", "arg5"}));
    EXPECT_EQ(operations[2].first, "operation3");
    EXPECT_EQ(operations[2].second.size(), 0); // operation3 has no arguments
}

TEST(ConfigParserTest, EmptyFile) {
    std::string content = "";
    std::string filename = createTempConfigFile(content);

    ConfigParser parser(filename);
    auto operations = parser.getOperations();

    EXPECT_TRUE(operations.empty());
}

TEST(ConfigParserTest, OnlyCommentsAndEmptyLines) {
    std::string content = "# Comment line 1\n# Comment line 2\n\n# Comment line 3";
    std::string filename = createTempConfigFile(content);

    ConfigParser parser(filename);
    auto operations = parser.getOperations();

    EXPECT_TRUE(operations.empty());
}

TEST(ConfigParserTest, InvalidFile) {
    EXPECT_THROW(ConfigParser("non_existent_file.cfg"), std::runtime_error);
}

TEST(ConfigParserTest, LinesWithTrailingSpaces) {
    std::string content = "operation1 arg1 arg2\noperation2     arg3\noperation3";
    std::string filename = createTempConfigFile(content);

    ConfigParser parser(filename);
    auto operations = parser.getOperations();

    ASSERT_EQ(operations.size(), 3);
    EXPECT_EQ(operations[0].first, "operation1");
    EXPECT_EQ(operations[0].second, (std::vector<std::string>{"arg1", "arg2"}));
    EXPECT_EQ(operations[1].first, "operation2");
    EXPECT_EQ(operations[1].second, (std::vector<std::string>{"arg3"}));
    EXPECT_EQ(operations[2].first, "operation3");
    EXPECT_EQ(operations[2].second.size(), 0);
}

TEST(ConfigParserTest, ComplexFile) {
    std::string content = "# Complex file with mixed entries\noperation1 arg1 arg2 arg3\n# Comment about operation2\noperation2\noperation3 arg4\n";
    std::string filename = createTempConfigFile(content);

    ConfigParser parser(filename);
    auto operations = parser.getOperations();

    ASSERT_EQ(operations.size(), 3);
    EXPECT_EQ(operations[0].first, "operation1");
    EXPECT_EQ(operations[0].second, (std::vector<std::string>{"arg1", "arg2", "arg3"}));
    EXPECT_EQ(operations[1].first, "operation2");
    EXPECT_EQ(operations[1].second.size(), 0);
    EXPECT_EQ(operations[2].first, "operation3");
    EXPECT_EQ(operations[2].second, (std::vector<std::string>{"arg4"}));
}
