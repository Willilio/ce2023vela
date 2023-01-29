//
// Created by wcirw on 1/18/2023.
//

#include "str.h"
#include <cstring>
#include <iostream>

namespace util {

    // Count occurrences of a char in a string
    int str::count_occurrences(const char *search_string, const char *search_char) {
        int count = 0, char_pos = 0;
        while (search_string[char_pos] != '\0') {
            if (search_string[char_pos] == *search_char) {
                count++;
            }
            char_pos++;
        }
        return count;
    }

    // Split with a controllable split_char
    char **str::split(char *to_split, const char *split_char) {
        int num_parts = str::count_occurrences(to_split, split_char) + 1;
        char **output = new char *[num_parts];
        char *remaining_str = strtok(to_split, split_char);
        int found_subs = 0;
        for (int i=0; i<num_parts; i++) {
            output[found_subs] = remaining_str;
            remaining_str = strtok(nullptr, split_char);
            found_subs++;
        }
        return output;
    }

    // Equivalent to split(char * to_split, split_char=' ')
    char **str::split(char *to_split) {
        return split(to_split, DEFAULT_SPLIT_CHAR);
    }


} // util