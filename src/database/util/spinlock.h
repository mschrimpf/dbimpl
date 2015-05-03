//
// Created by Daniel on 01.05.2015.
//

#ifndef PROJECT_SPINLOCK_H
#define PROJECT_SPINLOCK_H
#include <boost/atomic.hpp>

class spinlock {
private:
    typedef enum {Locked, Unlocked} LockState;
    boost::atomic<LockState> state_;

public:
    spinlock() : state_(Unlocked) {}

    void lock()
    {
        while (state_.exchange(Locked, boost::memory_order_acquire) == Locked) {
            /* busy-wait */
        }
    }
    void unlock()
    {
        state_.store(Unlocked, boost::memory_order_release);
    }
};
#endif //PROJECT_SPINLOCK_H
