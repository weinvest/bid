#ifndef _RECAPTAL_CLIENT_H
#define _RECAPTAL_CLIENT_H
#include <string>
#include <thread>
#include <memory>
class Recaptal
{
public:
	static Recaptal* GetInstance();

	bool Start(const std::string& dev, const std::string& url, const std::string& port);

private:
	static void GotPacket(uint8_t *args, const struct pcap_pkthdr *header, const uint8_t *packet);

	std::shared_ptr<std::thread> mThread;
};
#endif