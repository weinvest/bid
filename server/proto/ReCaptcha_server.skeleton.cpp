// This autogenerated skeleton file illustrates how to build a server.
// You should copy it to another filename to avoid overwriting it.

#include "ReCaptcha.h"
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;

using boost::shared_ptr;

class ReCaptchaHandler : virtual public ReCaptchaIf {
 public:
  ReCaptchaHandler() {
    // Your initialization goes here
  }

  void doCenterScan(FontCenterScanResponse& _return, const FontCenterScanRequest& req) {
    // Your implementation goes here
    printf("doCenterScan\n");
  }

  void doCenterScanByIndex(FontCenterScanIndexResponse& _return, const FontCenterScanIndexRequest& req) {
    // Your implementation goes here
    printf("doCenterScanByIndex\n");
  }

  void giveCenterScanFeedback(FontCenterScanIndexFeedbackResponse& _return, const FontCenterScanIndexFeedbackRequest& req) {
    // Your implementation goes here
    printf("giveCenterScanFeedback\n");
  }

};

int main(int argc, char **argv) {
  int port = 9090;
  shared_ptr<ReCaptchaHandler> handler(new ReCaptchaHandler());
  shared_ptr<TProcessor> processor(new ReCaptchaProcessor(handler));
  shared_ptr<TServerTransport> serverTransport(new TServerSocket(port));
  shared_ptr<TTransportFactory> transportFactory(new TBufferedTransportFactory());
  shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());

  TSimpleServer server(processor, serverTransport, transportFactory, protocolFactory);
  server.serve();
  return 0;
}

