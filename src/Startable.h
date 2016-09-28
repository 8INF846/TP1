#ifndef STARTABLE_H
#define STARTABLE_H

#include <thread>

class Startable {
public:
    std::thread start();
    void stop();

protected:
    bool m_bShouldStop = false;
    void run();
    virtual void update(double delta) = 0;
};

#endif
