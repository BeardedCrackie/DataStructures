#pragma once

#include <iostream>
//#include <fstream>
#include <bitset>
#include <string>
#include <sstream>
#include <libds/heap_monitor.h>

//using namespace std;

class NetworkRoute
{
public:
	NetworkRoute();
	//todo NetworkRoute(NetworkRoute& other);
	void setNetworkAddress(std::string address);
	//todo void setNetworkMask(std::string mask);
	void setNetworkPrefix(int prefix);
	void setNextHop(std::string nextHopAddress);
	void setTtl(std::string ttl);
	void printRoute();
	std::string toString();

	std::bitset<32> getNetworkAddress() const { return this->networkAddress; };
	std::bitset<32> getNetworkMask() const { return this->networkMask; };
	int getPrefix() const { return this->prefix; }
	std::bitset<32> getNextHop() const { return this->nextHop; };
	long getTtl() const { return this->ttl; };
	size_t getOctet(size_t octetNumber);

	static std::string bitsetToIp(std::bitset<32> addressInBitset);
	static std::bitset<32> ipToBitset(std::string ipAddress);

private:
	std::bitset<32> networkAddress;
	std::bitset<32> networkMask;
	int prefix;
	std::bitset<32> nextHop;
	long ttl;
};

NetworkRoute::NetworkRoute()
{
	this->networkAddress = std::bitset<32>();
	this->prefix = 0;
	this->networkMask = std::bitset<32>();
	this->nextHop = std::bitset<32>();
	this->ttl = 0;
}

void NetworkRoute::setNetworkAddress(std::string address)
{
	this->networkAddress = this->ipToBitset(address);
}

void NetworkRoute::setNetworkPrefix(int prefix)
{
	this->prefix = prefix;
	this->networkMask.reset();
	for (size_t i = 0; i < prefix; i++)
	{
		this->networkMask.set(31-i);
	}
}

void NetworkRoute::setNextHop(std::string nextHopAddress)
{
	this->nextHop = this->ipToBitset(nextHopAddress);
}

inline void NetworkRoute::setTtl(std::string ttl)
{
	long time = 0;
	long tmp = 0;
	for (int i = 0; i < ttl.length(); i++)
	{
		switch (ttl[i]) {
		case 'w':
			tmp *= 7;
			[[fallthrough]];
		case 'd':
			tmp *= 24;
			[[fallthrough]];
		case 'h':
			tmp *= 60;
			[[fallthrough]];
		case 'm':
			time += tmp;
			tmp = 0;
			break;
		case ':':
			tmp *= 60;
			time += tmp;
			tmp = 0;
			break;
		default:
			tmp *= 10;
			tmp += (long)ttl[i]-48;
		}
	}
	this->ttl = time;
}

void NetworkRoute::printRoute()
{
	std::cout << this->toString() << std::endl;
}

inline std::string NetworkRoute::toString()
{
	return "route: " 
		+ this->bitsetToIp(this->networkAddress)
		+ "/" 
		+ std::to_string(this->prefix)
		+ ", mask: "
		+ this->bitsetToIp(this->networkMask)
		+ ", next-hop: "
		+ this->bitsetToIp(this->nextHop)
		+ ", ttl: "
		+ std::to_string(this->ttl);
}

std::string NetworkRoute::bitsetToIp(std::bitset<32> addressInBitset)
{
	std::string resultIp = "";
	long tmp = 0;
	std::string delimeter = "";
	for (int i = 31; i >= 0; i--)
	{
		tmp *= 2;
		tmp += addressInBitset[i];
		if (i % 8 == 0) {
			resultIp += delimeter;
			resultIp += std::to_string(tmp);
			tmp = 0;
			delimeter = ".";
		}
	}
	return resultIp;

	//todo optimalization
	//std::string bitString = addressInBitset.to_string();
	//std::string resultIp = "";
	//std::istringstream bit_stream(bitString);
	//std::bitset<8> tmpSet;
	//std::string delimeter = "";
	//for (size_t i = 0; i < 4; i++)
	//{
	//	bit_stream >> tmpSet;
	//	resultIp += delimeter + std::to_string(tmpSet.to_ulong());
	//	delimeter = ".";
	//}
	//return resultIp;
}

std::bitset<32> NetworkRoute::ipToBitset(std::string ipAddress)
{
	std::istringstream ipStream(ipAddress);
	std::string bitString = "";

	std::string octet;
	for (size_t i = 0; i < 4; i++)
	{
		std::getline(ipStream, octet, '.');
		bitString += std::bitset<8>(stoi(octet)).to_string();
	}
	return std::bitset<32>(bitString);
}

size_t NetworkRoute::getOctet(size_t octetNumber)
{
	size_t octetValue = 0;
	for (int i = 7; i >= 0; i--)
	{
		octetValue *= 2;
		octetValue += this->networkAddress[(3 - octetNumber) * 8 + i];
	}
	return octetValue;
}


struct NetworkBlock
{
	bool operator==(const NetworkBlock& other) const
	{
		return route == other.route;
	}

	bool operator!=(const NetworkBlock& other) const
	{
		return !(*this == other);
	}

	NetworkRoute* route = nullptr;
};


struct NetworkHierarchyBlock : NetworkBlock
{
	bool operator==(const NetworkHierarchyBlock& other) const
	{
		return octetValue == other.octetValue && route == other.route;
	}

	size_t octetValue = 0 - 1;
};