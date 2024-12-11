//
// Created by Muhammad chandra zulfikar on 11/12/24.
//
#include "string_helper.h"
#include <iostream>
#include <sstream>

std::string trim(const std::string& str) {
    size_t const first = str.find_first_not_of(' ');
    size_t const last = str.find_last_not_of(' ');

    if (first == std::string::npos || last == std::string::npos) {
        return ""; // Empty string
    }
    return str.substr(first, (last - first + 1));
}

std::map<std::string, std::string> StringHelper::parseKeyValue(const std::string& message) {
    std::map<std::string, std::string> kvPairs;
    std::stringstream ss(message);
    std::string item;

    while (std::getline(ss, item, ',')) {
        size_t const pos = item.find('=');
        if (pos != std::string::npos) {
            std::string const key = trim(item.substr(0, pos));
            std::string const value = trim(item.substr(pos + 1));

            kvPairs[key] = value;
        }
    }

    return kvPairs;
}


