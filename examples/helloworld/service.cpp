#include <iostream>
#include <memory>
#include <string_view>

#include "examples/helloworld/hello.grpc.pb.h"
#include "grpcpp/grpcpp.h"

constexpr std::string_view ADDRESS = "0.0.0.0:54321";

class Server {
public:
  ~Server() {
    _server->Shutdown();
    _queue->Shutdown();
  }

  void run() {
    build_server();

    // Spawn a new Receiver instance to serve new clients.
    (new Receiver(_service, *_queue))->tick();

    void* tag;  // uniquely identifies a request.
    bool ok;
    while (true) {
      // Block waiting to read the next event from the completion queue. The
      // event is uniquely identified by its tag, which in this case is the
      // memory address of a Receiver instance.
      // The return value of Next should always be checked. This return value
      // tells us whether there is any kind of event or cq_ is shutting down.
      if (!_queue->Next(&tag, &ok)) return; // Do something else here...
      if (!ok) return; // Do something else here...
      static_cast<Receiver*>(tag)->tick();
    }
  }

private:
  // Class encompasing the state and logic needed to serve a request.
  class Receiver {
   public:
    // Take in the "service" instance (in this case representing an asynchronous
    // server) and the completion queue "cq" used for asynchronous communication
    // with the gRPC runtime.
    Receiver(
      GreeterService::AsyncService& service,
      grpc::ServerCompletionQueue& queue
    ) : _service(service), _queue(queue), _responder(&_ctx), _status(CREATE) {}

    void tick() {
      if (_status == CREATE) {
        // Make this instance progress to the PROCESS state.
        _status = PROCESS;

        // As part of the initial CREATE state, we *request* that the system
        // start processing SayHello requests. In this request, "this" acts are
        // the tag uniquely identifying the request (so that different Receiver
        // instances can serve different requests concurrently), in this case
        // the memory address of this Receiver instance.
        _service.RequestHello(&_ctx, &_request, &_responder, &_queue, &_queue, this);
      } else if (_status == PROCESS) {
        // Spawn a new Receiver instance to serve new clients while we process
        // the one for this Receiver. The instance will deallocate itself as
        // part of its FINISH state.
        (new Receiver(_service, _queue))->tick();

        // The actual processing.
        std::cout << "Greetings from " << _request.name() << std::endl;
        _response.set_message("Hello " + _request.name());

        // And we are done! Let the gRPC runtime know we've finished, using the
        // memory address of this instance as the uniquely identifying tag for
        // the event.
        _status = FINISH;
        _responder.Finish(_response, grpc::Status::OK, this);
      } else {
        if (_status != FINISH) {} // Do something here...
        // Once in the FINISH state, deallocate ourselves (Receiver).
        delete this;
      }
    }

   private:
    // The means of communication with the gRPC runtime for an asynchronous
    // server.
    GreeterService::AsyncService& _service;
    // The producer-consumer queue where for asynchronous server notifications.
    grpc::ServerCompletionQueue& _queue;
    // Context for the rpc, allowing to tweak aspects of it such as the use
    // of compression, authentication, as well as to send metadata back to the
    // client.
    grpc::ServerContext _ctx;

    // What we get from the client.
    HelloRequest _request;
    // What we send back to the client.
    HelloResponse _response;

    // The means to get back to the client.
    grpc::ServerAsyncResponseWriter<HelloResponse> _responder;

    // Let's implement a tiny state machine with the following states.
    enum CallStatus { CREATE, PROCESS, FINISH };
    CallStatus _status;  // The current serving state.
  };

  void build_server() {
    grpc::ServerBuilder builder;
    // Listen on the given address without any authentication mechanism.
    builder.AddListeningPort(std::string{ADDRESS}, grpc::InsecureServerCredentials());
    // Register "_service" as the instance through which we'll communicate with
    // clients. In this case it corresponds to an *asynchronous* service.
    builder.RegisterService(&_service);
    // Get hold of the completion queue used for the asynchronous communication
    // with the gRPC runtime.
    _queue = builder.AddCompletionQueue();
    _server = builder.BuildAndStart();
  }

  std::unique_ptr<grpc::ServerCompletionQueue> _queue;
  GreeterService::AsyncService _service;
  std::unique_ptr<grpc::Server> _server;
};

int main() {
  Server server;
  server.run();
  return 0;
}
