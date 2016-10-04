#ifndef STARTABLE_H
#define STARTABLE_H

#include <thread>

class Startable {
public:
    /**
     * Start thread
     * @return the thread started
     */
    std::thread start();
    /**
     * Stop the thread
     */
    void stop();

protected:
    bool m_bShouldStop = false;
    void run();
    virtual void update(double delta) = 0;
};

#endif
