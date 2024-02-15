#ifndef __SERVER_THREAD_HPP__
#define __SERVER_THREAD_HPP__

#include <thread>
#include <functional>
#include <memory>
#include <pthread.h>
#include <stdint.h>

namespace Server {

// lock guard for mutex
template<typename T>
class ScopedLock {
public:
    ScopedLock(T& mutex): m_mutex{mutex}{
        m_mutex.lock();
        m_locked = true;
    }

    ~ScopedLock() {
        unlock();
    }

    void lock() {
        if (!m_locked) {
            m_mutex.lock();
            m_locked = true;
        }
    }

    void unlock() {
        if (m_locked) {
            m_mutex.unlock();
            m_locked = false;
        }
    }
private:
    T& m_mutex;
    bool m_locked;
};

// lock guard for read_mutex
template<typename T>
class ScopedRdLock {
public:
    ScopedRdLock(T& mutex): m_mutex{mutex}{
        m_mutex.rdlock();
        m_locked = true;
    }

    ~ScopedRdLock() {
        unlock();
    }

    void lock() {
        if (!m_locked) {
            m_mutex.rdlock();
            m_locked = true;
        }
    }

    void unlock() {
        if (m_locked) {
            m_mutex.unlock();
            m_locked = false;
        }
    }
private:
    T& m_mutex;
    bool m_locked;
};

// lock guard for write mutex
template<typename T>
class ScopedWriteLock {
public:
    ScopedWriteLock(T& mutex): m_mutex{mutex}{
        m_mutex.wrlock();
        m_locked = true;
    }

    ~ScopedWriteLock() {
        unlock();
    }

    void lock() {
        if (!m_locked) {
            m_mutex.wrlock();
            m_locked = true;
        }
    }

    void unlock() {
        if (m_locked) {
            m_mutex.unlock();
            m_locked = false;
        }
    }
private:
    T& m_mutex;
    bool m_locked;
};

class Mutex {
public:
    using Lock = ScopedLock<Mutex>;
    Mutex() {
        pthread_mutex_init(&m_mutex, nullptr);
    }

    ~Mutex() {
        pthread_mutex_destroy(&m_mutex);
    }

    void lock() {
        pthread_mutex_lock(&m_mutex);
    }

    void unlock() {
        pthread_mutex_unlock(&m_mutex);
    }

private:
    pthread_mutex_t m_mutex;
};

class NullMutex {
public:
    using Lock = ScopedLock<Mutex>;

    NullMutex() {};
    ~NullMutex() {};

    void lock() {};
    void unlock() {}
};

class RWMutex {
public:
    using ReadLock = ScopedRdLock<RWMutex>;
    using WriteLock = ScopedWriteLock<RWMutex>;

    RWMutex() {
        pthread_rwlock_init(&m_lock, nullptr);
    }

    ~RWMutex() {
        pthread_rwlock_destroy(&m_lock);
    }

    void rdlock() {
        pthread_rwlock_rdlock(&m_lock);
    }

    void wrlock() {
        pthread_rwlock_wrlock(&m_lock);
    }

    void unlock() {
        pthread_rwlock_unlock(&m_lock);
    }
private:
    pthread_rwlock_t m_lock;
};

class NullRWMutex {
public:
    using Lock = ScopedLock<Mutex>;

    NullRWMutex() {};
    ~NullRWMutex() {};

    void rdlock() {};
    void wrlock() {};
    void unlock() {};
};

class Semaphore {
public:
    Semaphore(uint32_t count = 0);

    ~Semaphore();

    void wait();
    void notify();
private:
    Semaphore(const Semaphore&) = delete;
    Semaphore(const Semaphore&&) = delete;
    Semaphore& operator=(const Semaphore&) = delete;
    
private:
    sem_t m_semaphore;  
};

class Spinlock {
public:
    using Lock = ScopedLock<Spinlock>;
    
    Spinlock() {
        pthread_spin_init(&m_mutex, 0);
    }
    
    ~Spinlock() {
        pthread_spin_destroy(&m_mutex);
    }

    void lock() {
        pthread_spin_lock(&m_mutex);
    }

    void unlock() {
        pthread_spin_unlock(&m_mutex);
    }
private:
    pthread_spinlock_t m_mutex;
};

class CASLock {
public:
    using Lock = ScopedRdLock<CASLock>;
    CASLock() {
        m_mutex.clear();
    };

    ~CASLock() {

    };

    void lock() {
        while(std::atomic_flag_test_and_set_explicit(&m_mutex, std::memory_order_acquire)) {}
    };

    void unlock() {
        std::atomic_flag_clear_explicit(&m_mutex, std::memory_order_release);
    };

private:
    volatile std::atomic_flag m_mutex;
};

class Thread {
public:
    using ptr = std::shared_ptr<Thread>;
    
    Thread(std::function<void()> cb, const std::string& name);

    ~Thread();

    pid_t getId() const { return m_id; };
    const std::string& getName() const { return m_name; };

    void join();

    /* static method to config/retrieve thread info */ 
    // Get current thread
    static Thread* getThis();

    // Get the name of thread (used by logger)
    static const std::string& GetName();

    // Set thread name of current thread
    static void setName(const std::string& name);

private:
    Thread(const Thread&) = delete;
    Thread(const Thread&&) = delete;
    Thread& operator=(const Thread&) = delete;

    // accept and run a thread 
    static void* run(void* arg);
private:
    pid_t m_id = -1;
    pthread_t m_thread = 0;
    std::function<void()> m_cb;
    std::string m_name;

    Semaphore m_semaphore;
};

}

#endif