#pragma once

#include <coroutine>
#include <functional>
#include <list>
#include <thread>

namespace woof::co {

/**
 * A per-thread singleton coroutine scheduling service.
 *
 * To get an instance of this class, use `Scheduler::this_thread` or
 * `Scheduler::for_thread`.
 */
class Scheduler {
public:
  Scheduler(Scheduler&&) = delete;
  Scheduler& operator=(Scheduler&&) = delete;
  Scheduler(const Scheduler&) = delete;
  Scheduler& operator=(const Scheduler&) = delete;
  virtual ~Scheduler() = default;

  /**
   * Fetches the Scheduler instance operating on the current thread.
   */
  static Scheduler& this_thread();

  /**
   * Fetches the Scheduler instance operating on the given thread.
   */
  static Scheduler& for_thread(std::thread::id thread_id);

  /**
   * Enqueues the given coroutine handle to be executed using the micro task
   * queue.
   *
   * @param coro
   *  A coroutine to schedule for immediate execution.
   */
  virtual void schedule(std::coroutine_handle<> coro) = 0;

  /**
   * Runs the event loop until it is empty or stop is called.
   */
  virtual void run() = 0;

  /**
   * Signals the event loop to stop. Safe to call from any thread.
   */
  virtual void stop() = 0;

protected:
  Scheduler() = default;

  /**
   * Enqueues the given callback onto the micro task queue for execution.
   *
   * If the micro task queue is currently being run, the task will execute
   * during that cycle, thus it is possible to starve the event loop. To avoid
   * this, tight async loops should inject timeouts at some interval within the
   * loop.
   *
   * @tparam MicroTask
   *  A functor taking no parameters and returning no value.
   *
   * @param task
   *  The task to enqueue.
   */
  template <typename MicroTask>
  void enqueue_micro_task(MicroTask&& task) {
    _micro_task_queue.push_back(std::forward<MicroTask>(task));
  }

  /**
   * Runs every task in the micro task queue.
   */
  void run_micro_task_queue();

private:
  std::list<std::function<void()>> _micro_task_queue;
};

}
