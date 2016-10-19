#ifndef _RECAPTAL_CLIENT_H
#define _RECAPTAL_CLIENT_H
#include <string>
class Recaptal
{
public:
	static Recaptal* GetInstance();

	void Start(const std::string& dev, const std::string& url, const std::string& port);

private:
	void GotPacket(uint8_t *args, const struct pcap_pkthdr *header, const uint8_t *packet);
};
#endif