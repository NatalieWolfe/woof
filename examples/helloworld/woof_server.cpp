#include "examples/helloworld/hello.woof.h"
#include "woof/async/future.h"
#include "woof/service/register.h"

class HelloHandler : public GreeterService::HelloHandler<> {
protected:
  woof::Future<void> run() {
    *response().mutable_message() = "Hello " + request().name();
    co_return;
  }
};

WOOF_REGISTER_HANDLER(HelloHandler);
