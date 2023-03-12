#ifndef PX_DISASTER_THREAD_SAFE_QUEUE_HPP
#define PX_DISASTER_THREAD_SAFE_QUEUE_HPP
#include <mutex>
#include <condition_variable>
#include <queue>

namespace px::disaster::thread_safe {
  template<class T>
  class Queue {
  private:
    using Container = std::queue<T>;
    mutable std::mutex m_mutex;
    Container m_queue;
    std::condition_variable m_dataCond;

  public:
    Queue() {}
    Queue(const Queue &other) {
      std::lock_guard<std::mutex> lk(other.m_mutex);
      m_queue = other.m_queue;
    }
    Queue(Queue &&other) {
      std::lock_guard<std::mutex> lk(other.m_mutex);
      m_queue = std::move(other.m_queue);
    }

    void Push(const T &value) {
      std::lock_guard<std::mutex> lk(m_mutex);
      m_queue.push(value);
      m_dataCond.notify_one();
    }
    void Push(T &&value) {
      std::lock_guard<std::mutex> lk(m_mutex);
      m_queue.push(std::move(value));
      m_dataCond.notify_one();
    }

    void WaitAndPop(T &value) {
      std::unique_lock<std::mutex> lk(m_mutex);
      m_dataCond.wait(lk, [this]{return !m_queue.empty();});
      value = m_queue.front();
      m_queue.pop();
    }

    bool TryPop(T &value) {
      std::lock_guard<std::mutex> lk(m_mutex);
      if (m_queue.empty())
        return false;
      value = m_queue.front();
      m_queue.pop();
      return true;
    }

    bool Empty() const {
      std::lock_guard<std::mutex> lk(m_mutex);
      return m_queue.empty();
    }

    Container::size_type Size() const {
      std::lock_guard<std::mutex> lk(m_mutex);
      return m_queue.size();
    }
  };
}

#endif // PX_DISASTER_THREAD_SAFE_QUEUE_HPP