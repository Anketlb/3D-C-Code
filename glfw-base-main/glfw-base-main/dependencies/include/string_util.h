#include<vector>
#include<string>

using namespace::std;

//Split a string on a delimiter
//Courtesy Gemini
vector<string> split_by_string(const string& text, const string& delimiter) {
    vector<string> tokens;
    
    // Safety check: if delimiter is empty, return the entire string as one element
    if (delimiter.empty()) {
        tokens.push_back(text);
        return tokens;
    }

    size_t start = 0;
    size_t end = text.find(delimiter);

    while (end != string::npos) {
        // Extract the substring between the current positions
        tokens.push_back(text.substr(start, end - start));
        
        // Advance past the entire length of the delimiter string
        start = end + delimiter.length();
        end = text.find(delimiter, start);
    }

    // Capture the final remaining piece after the last delimiter
    tokens.push_back(text.substr(start));

    return tokens;
}

bool starts_with(string_view text, string_view prefix) {
    // Check if the prefix fits, then compare the exact substring length
    return text.size() >= prefix.size() && text.substr(0, prefix.size()) == prefix;
}