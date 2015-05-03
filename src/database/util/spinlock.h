//
// Created by Daniel on 01.05.2015.
//

#ifndef PROJECT_SPINLOCK_H
#define PROJECT_SPINLOCK_H
#include <atomic>

class spinlock {
private:
    typedef enum {Locked, Unlocked} LockState;
    std::atomic<LockState> state_;

public:
    spinlock() : state_(Unlocked) {}

    void lock()
    {
        while (state_.exchange(Locked, std::memory_order_acquire) == Locked) {
            /* busy-wait */
        }
    }
    void unlock()
    {
        state_.store(Unlocked, std::memory_order_release);
    }
};
#endif //PROJECT_SPINLOCK_H
