#include "woof/co/scheduler.h"

namespace woof::co {

void Scheduler::run_micro_task_queue() {
  if (_micro_task_queue.empty()) return;

  while (!_micro_task_queue.empty()) {
    auto itr = _micro_task_queue.begin();
    (*itr)();
    _micro_task_queue.erase(itr);
  }
}

}
