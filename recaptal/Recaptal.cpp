#include "Stdafx.h"
#include <memory>
#include "pcap.h"
#include "Recaptal.h"
#include "NetworkProtocol.h"
#include "http_parser.h"
#include "HttpResponse.h"
#include "Log.h"
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TTransportUtils.h>
#include "ReCaptcha.h"
#include "ReCaptcha_types.h"

Recaptal* Recaptal::GetInstance()
{
	static Recaptal gInstance;
	return &gInstance;
}
WindowArea NewWindowArea(int32_t left, int32_t right, int32_t top, int32_t bottom)
{
	WindowArea area;
	area.left = left;
	area.right = right;
	area.top = top;
	area.bottom = bottom;

	return area;
}

Recaptal::Recaptal()
	:mParser((http_parser*)malloc(sizeof(http_parser)))
{
	auto pArea = std::make_shared<std::vector<WindowArea>>();
	pArea->emplace_back(NewWindowArea( 25, 30, 10, 17 ));
	pArea->emplace_back(NewWindowArea( 41, 45, 10, 17 ));
	pArea->emplace_back(NewWindowArea( 57, 62, 10, 17 ));
	pArea->emplace_back(NewWindowArea( 73, 77, 10, 17 ));
	pArea->emplace_back(NewWindowArea(25, 30, 30, 36));
	pArea->emplace_back(NewWindowArea(41, 45, 30, 36));
	pArea->emplace_back(NewWindowArea(57, 62, 30, 36));
	pArea->emplace_back(NewWindowArea(73, 77, 30, 36));
	mWindowAreas = pArea;

	mSetting = (http_parser_settings*)malloc(sizeof(http_parser_settings));
	mSetting->on_message_begin = Recaptal::OnHttpMessageBegin;
	mSetting->on_message_complete = Recaptal::OnHttpMessageEnd;
	mSetting->on_status = Recaptal::OnHttpStatus;
	mSetting->on_chunk_header = Recaptal::OnHttpChunkHeader;
	mSetting->on_chunk_complete = Recaptal::OnHttpChunkComplete;
	mSetting->on_header_field = Recaptal::OnHttpHeaderField;
	mSetting->on_header_value = Recaptal::OnHttpHeaderValue;
	mSetting->on_body = Recaptal::OnHttpBody;
	mSetting->on_url = Recaptal::OnHttpURL;
	mSetting->on_headers_complete = Recaptal::OnHttpHeaderEnd;

	http_parser_init(mParser, HTTP_RESPONSE);
	mParser->data = this;
}

Recaptal::~Recaptal()
{
	free(mParser);
	free(mSetting);
}

bool Recaptal::Connect2Server()
{
	using namespace std;
	using namespace apache::thrift;
	using namespace apache::thrift::protocol;
	using namespace apache::thrift::transport;

	boost::shared_ptr<TTransport> socket(new TSocket("192.168.0.101", 9090));
	mTransport = boost::shared_ptr<TTransport>(new TBufferedTransport(socket));
	boost::shared_ptr<TProtocol> protocol(new TBinaryProtocol(mTransport));

	mClient = boost::shared_ptr<ReCaptchaClient>(new ReCaptchaClient(protocol));
	try
	{
		mTransport->open();
		return true;
	}
	catch (const std::exception&)
	{
		return false;
	}
}

bool Recaptal::Start(const std::string& url, const std::string& port)
{
	if (!Connect2Server())
	{
		return false;
	}

	pcap_if_t *alldevs, *d;
	char errbuf1[PCAP_ERRBUF_SIZE];
	if (pcap_findalldevs_ex(PCAP_SRC_IF_STRING, NULL, &alldevs, errbuf1) == -1)
	{
		fprintf(stderr, "Error in pcap_findalldevs_ex: %s\n", errbuf1);
		return -1;
	}
	std::string sDev = alldevs->name;
	std::string filter = "tcp and src host ";
	filter += url;
	filter += " and port ";
	filter += port;

	char errbuf[PCAP_ERRBUF_SIZE];		/* error buffer */
	pcap_t *handle;				/* packet capture handle */

	struct bpf_program fp;			/* compiled filter program (expression) */
	bpf_u_int32 mask;			/* subnet mask */
	bpf_u_int32 net;			/* ip */
	int num_packets = -1;			/* number of packets to capture */
	const char* filter_exp = filter.c_str();
	const char* dev = sDev.c_str();
	/* get network number and mask associated with capture device */
	if (pcap_lookupnet(dev, &net, &mask, errbuf) == -1)
	{
		fprintf(stderr, "Couldn't get netmask for device %s: %s\n",
			dev, errbuf);
		net = 0;
		mask = 0;
	}

	/* print capture info */
	printf("Device: %s\n", dev);
	printf("Number of packets: %d\n", num_packets);
	printf("Filter expression: %s\n", filter.c_str());

	/* open capture device */
	handle = pcap_open_live(dev, SNAP_LEN, 0, 0, errbuf);
	if (handle == NULL)
	{
		fprintf(stderr, "Couldn't open device %s: %s\n", dev, errbuf);
		return false;
	}

	/* make sure we're capturing on an Ethernet device [2] */
	if (pcap_datalink(handle) != DLT_EN10MB)
	{
		fprintf(stderr, "%s is not an Ethernet\n", dev);
		return false;
	}

	/* compile the filter expression */
	if (pcap_compile(handle, &fp, filter_exp, 0, net) == -1)
	{
		fprintf(stderr, "Couldn't parse filter %s: %s\n",
			filter_exp, pcap_geterr(handle));
		return false;
	}

	/* apply the compiled filter */
	if (pcap_setfilter(handle, &fp) == -1)
	{
		fprintf(stderr, "Couldn't install filter %s: %s\n",
			filter_exp, pcap_geterr(handle));
		return false;
	}

	mThread = std::make_shared<std::thread>([this, handle, num_packets, &fp]()
	{
		/* now we can set our callback function */
		pcap_loop(handle, num_packets, &Recaptal::GotPacket, (u_char*)this);

		/* cleanup */
		pcap_freecode(&fp);
		pcap_close(handle);
	});
	return true;
}

void Recaptal::GotPacket(u_char *args, const struct pcap_pkthdr *header, const u_char *packet)
{
	static int count = 1;                   /* packet counter */

											/* declare pointers to packet headers */
	const struct sniff_ethernet *ethernet;  /* The ethernet header [1] */
	const struct sniff_ip *ip;              /* The IP header */
	const struct sniff_tcp *tcp;            /* The TCP header */
	const char *payload;                    /* Packet payload */

	int size_ip;
	int size_tcp;
	int size_payload;

	count++;

	/* define ethernet header */
	ethernet = (struct sniff_ethernet*)(packet);

	/* define/compute ip header offset */
	ip = (struct sniff_ip*)(packet + SIZE_ETHERNET);
	size_ip = IP_HL(ip) * 4;
	if (size_ip < 20)
	{
		printf("   * Invalid IP header length: %u bytes\n", size_ip);
		return;
	}

	/* print source and destination IP addresses */
	printf("       From: %s\n", inet_ntoa(ip->ip_src));
	printf("         To: %s\n", inet_ntoa(ip->ip_dst));

	/* determine protocol */
	switch (ip->ip_p)
	{
	case IPPROTO_TCP:
		printf("   Protocol: TCP\n");
		break;
	case IPPROTO_UDP:
		printf("   Protocol: UDP\n");
		return;
	case IPPROTO_ICMP:
		printf("   Protocol: ICMP\n");
		return;
	case IPPROTO_IP:
		printf("   Protocol: IP\n");
		return;
	default:
		printf("   Protocol: unknown\n");
		return;
	}

	/*
	*  OK, this packet is TCP.
	*/

	/* define/compute tcp header offset */
	tcp = (struct sniff_tcp*)(packet + SIZE_ETHERNET + size_ip);
	size_tcp = TH_OFF(tcp) * 4;
	if (size_tcp < 20)
	{
		printf("   * Invalid TCP header length: %u bytes\n", size_tcp);
		return;
	}

	printf("   Src port: %d\n", ntohs(tcp->th_sport));
	printf("   Dst port: %d\n", ntohs(tcp->th_dport));

	/* define/compute tcp payload (segment) offset */
	payload = (const char *)(packet + SIZE_ETHERNET + size_ip + size_tcp);

	/* compute tcp payload (segment) size */
	size_payload = ntohs(ip->ip_len) - (size_ip + size_tcp);
	if (0 != size_payload)
	{
		Recaptal* pThis = reinterpret_cast<Recaptal*>(args);
		
		auto nparsed = http_parser_execute(pThis->mParser, pThis->mSetting, payload, size_payload);
	}
}

int Recaptal::OnHttpMessageBegin(http_parser* pParser)
{
	Recaptal* pThis = (Recaptal*)pParser->data;
	pThis->mHttpResponse = new HttpResponse();
	pThis->mHttpResponse->content = new char[8192];
	pThis->mHttpResponse->content_length = 0;
	return 0;
}

int Recaptal::OnHttpMessageEnd(http_parser* pParser)
{
	Recaptal* pThis = (Recaptal*)pParser->data;
	auto pHttpResponse = pThis->mHttpResponse;
	//for (auto head : pHttpResponse->headers)
	//{
	//	LOG_INFO("%s:%s\n", head.first.c_str(), head.second.c_str());
	//}
	if (0 != pHttpResponse->headers.count("Content-Type"))
	{
		std::string contentType = pHttpResponse->headers["Content-Type"];
		if (contentType == "image/jpeg" || contentType == "image/png")
		{
			try
			{
				auto pAreas = std::static_pointer_cast<std::vector<WindowArea>>(pThis->mWindowAreas);
				FontCenterScanRequest request;
				request.image.assign(pHttpResponse->content, pHttpResponse->content_length);
				request.centerWindows = *pAreas;

				FontCenterScanIndexResponse response;
				pThis->mClient->doCenterScan(response, request);
			}
			catch (const std::exception& ex)
			{
				LOG_INFO("recaptcha failed:", ex.what());
			}
		}
	}
	return 0;
}

int Recaptal::OnHttpStatus(http_parser* pParser, const char *at, size_t length)
{
	Recaptal* pThis = (Recaptal*)pParser->data;
	auto pHttpResponse = pThis->mHttpResponse;
	pHttpResponse->status = std::string(at, length);
	return 0;
}

int Recaptal::OnHttpURL(http_parser* pParser, const char *at, size_t length)
{
	Recaptal* pThis = (Recaptal*)pParser->data;
	auto pHttpResponse = pThis->mHttpResponse;
	pHttpResponse->uri = std::string(at, length);
	return 0;
}

int Recaptal::OnHttpHeaderField(http_parser* pParser, const char *at, size_t length)
{
	Recaptal* pThis = (Recaptal*)pParser->data;
	pThis->mLastHeaderField = at;
	pThis->mLastHeaderFieldLength = length;
	return 0;
}

int Recaptal::OnHttpHeaderValue(http_parser* pParser, const char *at, size_t length)
{
	Recaptal* pThis = (Recaptal*)pParser->data;
	auto pHttpResponse = pThis->mHttpResponse;
	pHttpResponse->headers[std::string(pThis->mLastHeaderField, pThis->mLastHeaderFieldLength)] =
		std::string(at, length);

	return 0;
}

int Recaptal::OnHttpHeaderEnd(http_parser*)
{
	return 0;
}

int Recaptal::OnHttpBody(http_parser* pParser, const char *at, size_t length)
{
	Recaptal* pThis = (Recaptal*)pParser->data;
	auto pHttpResponse = pThis->mHttpResponse;
	memcpy((void*)(pHttpResponse->content + pHttpResponse->content_length), at, length);
	pHttpResponse->content_length += length;
	return 0;
}

int Recaptal::OnHttpChunkHeader(http_parser*)
{
	return 0;
}

int Recaptal::OnHttpChunkComplete(http_parser*)
{
	return 0;
}
