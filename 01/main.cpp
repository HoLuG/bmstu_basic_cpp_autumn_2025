#include <iostream>
#include <string>
#include <cassert>
#include <cctype>

char getCode(char ch) {
    switch (ch) {
        case 'B': case 'F': case 'P': case 'V': return '1';
        case 'C': case 'G': case 'J': case 'K':
        case 'Q': case 'S': case 'X': case 'Z': return '2';
        case 'D': case 'T': return '3';
        case 'L': return '4';
        case 'M': case 'N': return '5';
        case 'R': return '6';
        default: return '0';
    }
}

std::string convertTextToSound(std::string text) {
    if (text.empty()) return "";

    char first = std::toupper(text[0]);
    std::string res;
    res += first;

    char lastCode = getCode(first);

    for (size_t i = 1; i < text.size(); i++) {
        char ch = std::toupper(text[i]);
        if (ch < 'A' || ch > 'Z') continue;

        char code = getCode(ch);

        if (code == '0') continue; // h, w, a, e, i, o, u, y

        if (code != lastCode) {
            res += code;
            lastCode = code;
        }
    }

    while (res.size() < 4) res += '0';
    if (res.size() > 4) res.resize(4);

    return res;
}

bool isEqual(std::string text1, std::string text2) {
    return convertTextToSound(text1) == convertTextToSound(text2);
}

int main() {
    std::string text1{"Ashcraft"};
    std::string text2{"Ashcroft"};

    assert(isEqual(text1, text2));
    assert(convertTextToSound("Ashcraft") == std::string("A261"));
    assert(convertTextToSound("Ashcroft") == std::string("A261"));

    assert(isEqual("Gauss", "Ghosh"));
    assert(isEqual("Knuth", "Kant"));
    assert(!isEqual("Hello", "World"));

    assert(convertTextToSound("A") == std::string("A000"));
    assert(convertTextToSound("Ab") == std::string("A100"));
    assert(convertTextToSound("Ash") == std::string("A200"));
    assert(convertTextToSound("Aw") == std::string("A000"));

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
