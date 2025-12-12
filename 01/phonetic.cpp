#include "phonetic.hpp"
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
    
    // Удаляем h и w
    std::string step2;
    step2 += text[0];
    for (size_t i = 1; i < text.size(); i++) {
        char ch = std::toupper(text[i]);
        if (ch == 'H' || ch == 'W') continue;
        step2 += text[i];
    }
    
    // Заменяем согласные на цифры 
    std::string step3;
    step3 += first;
    for (size_t i = 1; i < step2.size(); i++) {
        char ch = std::toupper(step2[i]);
        if (ch < 'A' || ch > 'Z') continue;
        
        char code = getCode(ch);
        if (code != '0') {
            step3 += code;
        } else {
            step3 += ch;
        }
    }
    
    // Схлопываем одинаковые цифры
    std::string step4;
    step4 += step3[0];
    for (size_t i = 1; i < step3.size(); i++) {
        if (step3[i] >= '0' && step3[i] <= '9') {
            if (step4.back() != step3[i]) {
                step4 += step3[i];
                
                if (step4.size() >= 4) {
                    break;
                }
            }
        } else {
            step4 += step3[i];
        }
    }
    
    // Удаляем гласные
    std::string step5;
    step5 += step4[0];
    for (size_t i = 1; i < step4.size(); i++) {
        char ch = std::toupper(step4[i]);
        if (ch == 'A' || ch == 'E' || ch == 'I' || ch == 'O' || ch == 'U' || ch == 'Y') {
            continue;
        }
        step5 += step4[i];
        if (step5.size() >= 4) {
            break;
        }
    }

    std::string res = step5;
    while (res.size() < 4) {
        res += '0';
    }
    if (res.size() > 4) {
        res.resize(4);
    }
    
    return res;
}

bool isEqual(std::string text1, std::string text2) {
    return convertTextToSound(text1) == convertTextToSound(text2);
}
