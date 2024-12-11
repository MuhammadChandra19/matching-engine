//
// Created by Muhammad chandra zulfikar on 11/12/24.
//

#ifndef STRING_HELPER_H
#define STRING_HELPER_H

#include <map>
#include <string>

class StringHelper {
public:
    // Method to parse key-value pair messages into a map
    static std::map<std::string, std::string> parseKeyValue(const std::string& message);

};


#endif //STRING_HELPER_H
