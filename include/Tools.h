#ifndef TOOLS_H
#define TOOLS_H

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <iostream>
#include <string>
#include <vector>

// 2. إزالة using namespace std; من هنا (خطأ كبير في ملفات الهيدر)
// سنستخدم std:: بشكل مباشر

inline bool isNumber(const std::string& s) {
    if (s.empty()) return false;
    for (char c : s) {
        if (!isdigit(static_cast<unsigned char>(c))) return false;
    }
    return true;
}

// أضف هذه الدالة في ملف Tools.h
inline void replaceAll(std::string& str, const std::string& from, const std::string& to) {
    if (from.empty()) return;
    size_t start_pos = 0;
    while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // للانتقال للكلمة التالية
    }
}

inline std::string removeisquote(const std::string& s) {
    if (s.size() >= 2) {
        return s.substr(1, s.size() - 2);
    }
    return s;
}

inline bool isquote(char c) {
    return c == '"' || c == '\'';
}

// دالة Ferror سليمة منطقياً لكن يجب استخدام std::
inline void Ferror(const std::string& message) {
    HANDLE hConsole = GetStdHandle(STD_ERROR_HANDLE);
    // تعيين اللون الأحمر
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
    std::cerr << message << std::endl;
    // إرجاع اللون الأبيض الافتراضي
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    exit(1);
}
inline bool CheckLogical(std::string Value) {
    return (Value == "true")? true: !(Value == "false");
}
inline bool isbrackets(const char &brakets) {
    char Bracketas[] = {'(', ')', '[', ']', '{', '}'};
    for (char c : Bracketas) {
        if (c == brakets) return true;
    }
    return false;
}

inline bool isoperator(const char& ch) {
    char operators[] = {'+', '-', '*', '/', '='};
    for (char c : operators) {
        if (c == ch) return true;
    }
    return false;
}

template<typename T>
void print(T First) {
    std::cout << First << std::endl;
}

inline void PrintSpace(int indent = 0) {
    for (int i = 0; i < indent; i++) std::cout << " ";
}

#endif