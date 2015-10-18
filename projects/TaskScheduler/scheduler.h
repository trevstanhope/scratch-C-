
#include <map>
#include <functional>
#include <chrono>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <memory>
#include <boost/noncopyable.hpp>

class Scheduler : boost::noncopyable {
 private:
    std::multimap<std::chrono::system_clock::time_point, std::function<void()>> tasks;
    std::mutex mutex;
    std::unique_ptr<std::thread> thread;
    std::condition_variable blocker;
    bool go_on;
 public:
   Scheduler() : go_on(true) {
        thread.reset(new std::thread([this](){ this->ThreadLoop(); }));
    }
    ~Scheduler() {
      go_on = false;
      ScheduleAt(std::chrono::system_clock::now(), []() {});
      thread->join();
    }

    void ThreadLoop() {
      while (go_on) {
        std::function<void()> todo;
        {
          std::unique_lock<std::mutex> lock(mutex);
          auto now = std::chrono::system_clock::now();
          if (tasks.empty() == false && tasks.begin()->first <= now) {
            todo = tasks.begin()->second;
            tasks.erase(tasks.begin());
          }
        }

        // Run tasks while unlocked so tasks can schedule new tasks
        if (todo)
          todo();

        {
          std::unique_lock<std::mutex> lock(mutex);
          if (tasks.empty())
            blocker.wait(lock);
          else
            blocker.wait_until(lock, tasks.begin()->first);
        }
      }
    }

    void ScheduleAt(std::chrono::system_clock::time_point &time,
                    std::function<void()> func) {
      std::unique_lock<std::mutex> lock(mutex);
      auto it = tasks.insert(std::make_pair(time, func));
      if (it == tasks.begin())
        blocker.notify_one();
    }

    void ScheduleEvery(std::chrono::system_clock::duration interval,
                       std::function<void()> func) {
      std::function<void()> waitFunc = [this, interval, func]() {
        func();
        this->ScheduleEvery(interval, func);
      };
      ScheduleAt(std::chrono::system_clock::now() + interval, waitFunc);
    }
};
