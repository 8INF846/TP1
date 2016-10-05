#ifndef CONSOLE_H
#define CONSOLE_H

#include <fstream>

class Console {
public:
    static std::ostream& out(int log_level = 0);
    Console(const Console&) = delete;
    Console& operator=(const Console&) = delete;
private:
    static Console* s_pInstance;
    Console();
    ~Console();
    std::ofstream m_file;
};

#endif
