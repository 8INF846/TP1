#include <iostream>
#include "Console.h"
#include "Settings.h"

Console *Console::s_pInstance = 0;

Console::Console() : m_file("log.txt", std::ios::out) {}

Console::~Console() {
    m_file.close();
}

std::ostream& Console::out(int log_level) {
    if(!s_pInstance) {
        s_pInstance = new Console;
    }

    if(log_level > Settings::LOG_LEVEL) {
        return s_pInstance->m_file;
    }
    return std::cout;
}

