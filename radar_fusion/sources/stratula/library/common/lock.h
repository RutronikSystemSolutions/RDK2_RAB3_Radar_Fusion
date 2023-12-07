#ifndef LOCK_H
#define LOCK_H 1


#include <stdbool.h>

typedef volatile int LockType;


/* \brief reset the lock to a defined value before using it */
static inline void reset_lock(LockType *lock)
{
    *lock = 0;
}

/* \brief check if a lock is currently acquired */
static inline bool is_locked(LockType *lock)
{
    return (*lock > 0);
}

/* \brief try to acquire lock without blocking */
static inline bool try_lock(LockType *lock)
{
    if (*lock == 0)
    {
        (*lock)++;
        if (*lock == 1)
        {
            return true;
        }
        (*lock)--;
    }

    return false;
}

/* \brief acquire lock to prevent access conflicts */
static inline void lock(LockType *lock)
{
    while (!try_lock(lock))
    {
    }
}

/* \brief release lock so that other functions can now access it */
static inline void unlock(LockType *lock)
{
    (*lock)--;
}


#endif /* LOCK_H */
