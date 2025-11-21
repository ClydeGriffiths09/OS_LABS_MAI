#include <stringprocessor.h>

namespace utils {
const std::string StringProcessor::VOWELS = "aeiouAEIOU";

bool StringProcessor::IsVowel(char c) {
    return VOWELS.find(c) != std::string::npos;
}

std::string StringProcessor::RemoveVowels(const std::string& str) {
    std::string res;

    for (char c : str) {
        if (!IsVowel(c)) {
            res += c;
        }
    }

    return res;
}
}