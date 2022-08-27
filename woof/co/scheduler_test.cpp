#include "woof/co/scheduler.h"

#include "gtest/gtest.h"

namespace woof::co {

class TestScheduler : public Scheduler {
public:
  void schedule(std::coroutine_handle<> coro) override {}

  template <typename Func>
  void enqueue(Func&& func) {
    enqueue_micro_task(std::forward<Func>(func));
  }

  void run() override {
    run_micro_task_queue();
  }

  void stop() override {}
};

TEST(Scheduler, RunsMicroTask) {
  TestScheduler scheduler;
  bool executed = false;
  scheduler.enqueue([&]() { executed = true; });
  EXPECT_FALSE(executed);
  scheduler.run();
  EXPECT_TRUE(executed);
}

TEST(Scheduler, RunsMultipleMicroTasks) {
  TestScheduler scheduler;
  int executions = 0;
  scheduler.enqueue([&]() { ++executions; });
  scheduler.enqueue([&]() { ++executions; });
  scheduler.enqueue([&]() { ++executions; });
  EXPECT_EQ(executions, 0);
  scheduler.run();
  EXPECT_EQ(executions, 3);
}

TEST(Scheduler, RunsInOrderOfEnqueueing) {
  TestScheduler scheduler;
  int executions = 0;
  scheduler.enqueue([&]() { EXPECT_EQ(++executions, 1); });
  scheduler.enqueue([&]() { EXPECT_EQ(++executions, 2); });
  scheduler.enqueue([&]() { EXPECT_EQ(++executions, 3); });
  scheduler.run();
}

TEST(Scheduler, RunsLateEnqueuedItems) {
  TestScheduler scheduler;
  bool inner_called = false;
  scheduler.enqueue([&]() {
    scheduler.enqueue([&]() { inner_called = true; });
  });
  scheduler.run();
  EXPECT_TRUE(inner_called);
}

}
