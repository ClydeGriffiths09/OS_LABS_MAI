#pragma once

#include <string>

namespace utils {
    class StringProcessor {
        private:
            static const std::string VOWELS;
        public:
            static std::string RemoveVowels(const std::string& str);
            static bool IsVowel(char c);
    };
}