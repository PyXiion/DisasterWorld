#ifndef PX_DISASTER_THREAD_SAFE_DEQUE_HPP
#define PX_DISASTER_THREAD_SAFE_DEQUE_HPP
#include <mutex>
#include <condition_variable>
#include <deque>

namespace px::disaster::thread_safe {
  template<class T>
  class Deque {
  public:
    Deque() {}
    Deque(const Deque &other) {
      std::lock_guard<std::mutex> lk(other.m_mutex);
      m_deque = other.m_deque;
    }

    void PushBack(T value) {
      std::lock_guard<std::mutex> lk(m_mutex);
      m_deque.push_back(value);
      m_backCond.notify_one();
    }

    void PushFront(T value) {
      std::lock_guard<std::mutex> lk(m_mutex);
      m_deque.push_front(value);
      m_frontCond.notify_one();
    }

    void WaitAndPopBack(T &value) {
      std::unique_lock<std::mutex> lk(m_mutex);
      m_backCond.wait(lk, [this]{return !m_deque.empty();});
      value = m_deque.back();
      m_deque.pop_back();
    }

    void WaitAndPopFront(T &value) {
      std::unique_lock<std::mutex> lk(m_mutex);
      m_frontCond.wait(lk, [this]{return !m_deque.empty();});
      value = m_deque.front();
      m_deque.pop_front();
    }

    bool TryPopBack(T &value) {
      std::lock_guard<std::mutex> lk(m_mutex);
      if (m_deque.empty())
        return false;
      value = m_deque.back();
      m_deque.pop_back();
      return true;
    }

    bool TryPopFront(T &value) {
      std::lock_guard<std::mutex> lk(m_mutex);
      if (m_deque.empty())
        return false;
      value = m_deque.front();
      m_deque.pop_front();
      return true;
    }

    bool Empty() const {
      std::lock_guard<std::mutex> lk(m_mutex);
      return m_deque.empty();
    }

  private:
    mutable std::mutex m_mutex;
    std::deque<T> m_deque;  

    std::condition_variable m_backCond;
    std::condition_variable m_frontCond;
  };
}

#endif // PX_DISASTER_THREAD_SAFE_DEQUE_HPP