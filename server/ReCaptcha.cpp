// This autogenerated skeleton file illustrates how to build a server.
// You should copy it to another filename to avoid overwriting it.
#include <thread>
#include "ReCaptcha.h"
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>
#include <boost/program_options.hpp>
#include "FontScanner.h"
#include "IndexScanner.h"
#include "FontScanContext.h"
#include "utils/FileHandler.h"
#include "Spliter.h"
using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;

using boost::shared_ptr;

class ReCaptchaHandler : virtual public ReCaptchaIf {
 public:
  ReCaptchaHandler(const std::string& fontRoot, const std::string& saveRoot, int32_t nThreads)
  :mContext(fontRoot)
  ,mScanner(nThreads)
  ,mIndexScanner(mContext)
  ,mSaveRoot(saveRoot)
  {
  }

  void saveImage(CImg<uint8_t>& image, std::string& name, bool ispng)
  {
      auto realPath = mSaveRoot / name;
      std::string filePath = realPath.string();
      std::string ext(".png");
      if(!ispng)
      {
          ext = ".jpg";
      }

      if(fs::exists(filePath + ext))
      {
          char n('0');
          while(fs::exists(filePath + n + ext))
          {
              n++;
          }

          filePath += n;
      }
      image.save((filePath + ext).c_str());
  }

  void loadImage(CImg<uint8_t>& image, const std::string& buffer, bool ispng )
  {
      if(ispng)
      {
          image.load_png_buffer(buffer.c_str(), buffer.length());
      }
      else
      {
          image.load_jpeg_buffer((const uint8_t*)buffer.c_str(), buffer.length());
      }
  }
  void doCenterScan(FontCenterScanResponse& _return, const FontCenterScanRequest& req) {
      CImg<uint8_t> image;
      loadImage(image, req.image, req.isPNG);

      mScanner.Scan(_return
          , req.centerWindows.begin()
          , req.centerWindows.end()
          , image
          , mContext);

      std::string repResult;
      for(auto& result : _return.results)
      {
          repResult += result;
      }
      saveImage(image, repResult, req.isPNG);

      std::cout << repResult << std::endl;
  }

  void doCenterScanByIndex(FontCenterScanIndexResponse& _return, const FontCenterScanIndexRequest& req) {
      CImg<uint8_t> image;
      image.load_jpeg_buffer((const uint8_t*)req.image.c_str(), req.image.length());

      auto& areas = mIndexScanner.Scan(req, image, _return.guessId);


      mScanner.Scan(_return
         , areas.cbegin()
         , areas.cend()
         , image
         , mContext);
  }

  void giveCenterScanFeedback(FontCenterScanIndexFeedbackResponse& _return, const FontCenterScanIndexFeedbackRequest& req) {
    // Your implementation goes here
    mIndexScanner.AcceptFeedback(req);
    _return.status = 0;
  }

private:
    FontScanContext mContext;
    FontScanner mScanner;
    IndexScanner mIndexScanner;
    fs::path mSaveRoot;

    Spliter mSpliter;
};

int main(int argc, char **argv) {

    using namespace boost::program_options;
    options_description opts("Reca options");
    opts.add_options()
            ("help,h","print this help information.")
            ("port,p", value<int32_t>()->default_value(9090), "listen port")
            ("fontRoot,f",value<std::string>(),"root directory 4 font")
            ("saveRoot,s", value<std::string>(), "root directory 4 save image")
            ("threads,t",value<int32_t>()->default_value(std::thread::hardware_concurrency()),"password");

    variables_map vm;
    store(parse_command_line(argc,argv,opts),vm);

    if(vm.count("help") || 0 == vm.count("fontRoot") || 0 == vm.count("saveRoot"))
    {
        std::cout<<opts<<std::endl;
        return 0;
    }

    std::string fontRoot = vm["fontRoot"].as<std::string>();
    std::string saveRoot = vm["saveRoot"].as<std::string>();
    if(!fs::is_directory(fontRoot))
    {
        std::cout << fontRoot << " not a directory" << std::endl;
        return -1;
    }

    if(!fs::is_directory(saveRoot))
    {
        std::cout << saveRoot << " not a directory" << std::endl;
        return -1;
    }

    auto nThreads = vm["threads"].as<int32_t>();
    auto port = vm["port"].as<int32_t>();

    shared_ptr<ReCaptchaHandler> handler(new ReCaptchaHandler(fontRoot, saveRoot, nThreads));
    shared_ptr<TProcessor> processor(new ReCaptchaProcessor(handler));
    shared_ptr<TServerTransport> serverTransport(new TServerSocket(port));
    shared_ptr<TTransportFactory> transportFactory(new TBufferedTransportFactory());
    shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());

    TSimpleServer server(processor, serverTransport, transportFactory, protocolFactory);
    server.serve();
    return 0;
}
