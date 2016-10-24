#ifndef _RECAPTAL_CLIENT_H
#define _RECAPTAL_CLIENT_H
#include <string>
#include <thread>
#include <memory>
#include "boost/smart_ptr/shared_ptr.hpp"
struct http_parser;
struct http_parser_settings;
struct HttpResponse;
class ReCaptchaClient;
namespace apache {
	namespace thrift {
		namespace transport {
			class TTransport;
		}
	}
}
class Recaptal
{
public:
	static Recaptal* GetInstance();

	bool Start(const std::string& url, const std::string& port);

private:
	bool Connect2Server();

	Recaptal();
	~Recaptal();

	static void GotPacket(uint8_t *args, const struct pcap_pkthdr *header, const uint8_t *packet);
	static int OnHttpMessageBegin(http_parser*);
	static int OnHttpMessageEnd(http_parser*);
	
	static int OnHttpStatus(http_parser*, const char *at, size_t length);
	static int OnHttpURL(http_parser*, const char *at, size_t length);

	static int OnHttpHeaderField(http_parser*, const char *at, size_t length);
	static int OnHttpHeaderValue(http_parser*, const char *at, size_t length);
	static int OnHttpHeaderEnd(http_parser*);
	static int OnHttpBody(http_parser*, const char *at, size_t length);

	static int OnHttpChunkHeader(http_parser*);
	static int OnHttpChunkComplete(http_parser*);

	std::shared_ptr<std::thread> mThread;
	HttpResponse* mHttpResponse;
	http_parser* mParser;
	http_parser_settings* mSetting;
	const char* mLastHeaderField;
	size_t mLastHeaderFieldLength;

	boost::shared_ptr<ReCaptchaClient> mClient;
	boost::shared_ptr<apache::thrift::transport::TTransport> mTransport;
	std::shared_ptr<void> mWindowAreas;
};
#endif