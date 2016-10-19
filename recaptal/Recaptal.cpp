#include "Stdafx.h"
#include "Recaptal.h"
#include "NetworkProtocol.h"
Recaptal* Recaptal::GetInstance()
{
	static Recaptal gInstance;
	return &gInstance;
}

void Recaptal::Start(const std::string& dev, const std::string& url, const std::string& port)
{

}

void Recaptal::GotPacket(uint8_t *args, const struct pcap_pkthdr *header, const uint8_t *packet)
{

}