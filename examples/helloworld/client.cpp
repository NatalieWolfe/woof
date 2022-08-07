#include <iostream>
#include <memory>

#include "examples/helloworld/hello.grpc.pb.h"
#include "grpcpp/grpcpp.h"

class Client {
 public:
  explicit Client(std::shared_ptr<grpc::Channel> channel) :
    _stub(GreeterService::NewStub(channel)) {}

  // Assembles the client's payload, sends it and presents the response back
  // from the server.
  HelloResponse hello(const HelloRequest& request) {
    // Container for the data we expect from the server.
    HelloResponse response;

    // Context for the client. It could be used to convey extra information to
    // the server and/or tweak certain RPC behaviors.
    grpc::ClientContext context;

    // The producer-consumer queue we use to communicate asynchronously with the
    // gRPC runtime.
    grpc::CompletionQueue cq;

    // Storage for the status of the RPC upon completion.
    grpc::Status status;

    // _stub->PrepareAsyncSayHello() creates an RPC object, returning
    // an instance to store in "call" but does not actually start the RPC
    // Because we are using the asynchronous API, we need to hold on to
    // the "call" instance in order to get updates on the ongoing RPC.
    std::unique_ptr<grpc::ClientAsyncResponseReader<HelloResponse>> rpc(
      _stub->PrepareAsyncHello(&context, request, &cq)
    );

    // StartCall initiates the RPC call
    rpc->StartCall();

    // Request that, upon completion of the RPC, "response" be updated with the
    // server's response; "status" with the indication of whether the operation
    // was successful. Tag the request with the integer 1.
    rpc->Finish(&response, &status, (void*)1);
    void* got_tag;
    bool ok = false;
    // Block until the next result is available in the completion queue "cq".
    // The return value of Next should always be checked. This return value
    // tells us whether there is any kind of event or the cq_ is shutting down.
    if (!cq.Next(&got_tag, &ok)) {} // Do something here...

    // Verify that the result from "cq" corresponds, by its tag, our previous
    // request.
    if (got_tag != (void*)1) {} // Do something here...
    // ... and that the request was completed successfully. Note that "ok"
    // corresponds solely to the request for updates introduced by Finish().
    if (!ok) {} // Do something here...

    // Act upon the status of the actual RPC.
    if (status.ok()) {
      return response;
    } else {
      return response; // Do something else here...
    }
  }

 private:
  // Out of the passed in Channel comes the stub, stored here, our view of the
  // server's exposed services.
  std::unique_ptr<GreeterService::Stub> _stub;
};

int main(int argc, char** argv) {
  HelloRequest request;
  if (argc >= 2) {
    *request.mutable_name() = argv[1];
  } else {
    *request.mutable_name() = "World";
  }

  Client client(
    grpc::CreateChannel("localhost:54321", grpc::InsecureChannelCredentials())
  );

  HelloResponse response = client.hello(request);
  std::cout << response.message() << std::endl;

  return 0;
}
