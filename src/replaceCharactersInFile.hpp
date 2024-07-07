#include <iostream>
#include <fstream>
#include <string>
#include <map>
bool replaceCharactersInFile(const std::string& filePath, const std::map<char, char>& replaceMap);

// 替换文件中的字符
bool replaceCharactersInFile(const std::string& filePath, const std::map<char, char>& replaceMap) {
    std::ifstream inputFile(filePath, std::ios::in); // 打开输入文件
    if (!inputFile.is_open()) {
        std::cerr << "无法打开文件: " << filePath << std::endl;
        return false;
    }

    std::string tempFilePath = filePath + ".tmp"; // 创建一个临时文件来存储更改
    std::ofstream outputFile(tempFilePath, std::ios::out); // 打开输出文件
    if (!outputFile.is_open()) {
        std::cerr << "无法创建临时文件: " << tempFilePath << std::endl;
        inputFile.close();
        return false;
    }

    char ch;
    while (inputFile.get(ch)) { // 读取文件中的每个字符
        auto it = replaceMap.find(ch); // 查找是否有替换规则
        if (it != replaceMap.end()) {
            outputFile << it->second; // 如果有，写入替换后的字符
        } else {
            outputFile << ch; // 否则写入原始字符
        }
    }

    inputFile.close();
    outputFile.close();

    // 替换原文件
    remove(filePath.c_str()); // 删除原文件
    rename(tempFilePath.c_str(), filePath.c_str()); // 将临时文件重命名为原文件名

    return true;
}