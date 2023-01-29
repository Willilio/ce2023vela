/**
 * Utility class deals with all things string related, performing tasks that might be built into another language such
 * as python or Java, etc.
 * */

#ifndef CE2023VELA_STR_H
#define CE2023VELA_STR_H

// Inclusions
#include <cstring>

// util namespace to prevent a mix-up when naming variables etc.
namespace util {

    class str {

    public:

        // The default split character for the split function is a whitespace ' '
        constexpr static auto DEFAULT_SPLIT_CHAR = " ";

        /**
         * The count occurrence function takes in a string (represented by a char*) and a char. The function searches
         * a string for a a specific character. It returns the number of occurrences of the character in the string
         * @param search_string is the string to be searched
         * @param search_char is the character to search for
         */
        static int count_occurrences(const char *search_string, const char *search_char);

        /**
         * The split function takes in a string (represented by a char*) which it then scans for a certain character,
         * at which point it stores the string in a list of strings. It returns this list, represented as a char**
         * @param to_split is the full string to be divided
         * @param split_char is the character which divides the string into separate parts
         * */
        static char **split(char *to_split, const char *split_char);

        /**
         * The split function takes in a string (represented by a char*) which it then scans for a whitespace, at which
         * point it stores the string in a list of strings. It returns this list, represented as a char**
         * @param to_split is the full string to be divided by whitespaces
         * */
        static char **split(char *to_split);


    };

} // util

#endif //CE2023VELA_STR_H
