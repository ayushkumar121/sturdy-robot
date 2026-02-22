//
// Created by ari on 2/22/26.
//

#include "Basic.h"

#include <fstream>
#include <iostream>
#include <sstream>

std::string Basic::readEntireFile(std::string_view fileName) {
    std::ifstream file(fileName.data());
    if (!file) {
        throw std::runtime_error("File not found");
    }

    file.seekg(0, std::ios::end);
    std::string content(file.tellg(), '\0');
    file.seekg(0, std::ios::beg);
    file.read(content.data(), content.size());

    return content;
}