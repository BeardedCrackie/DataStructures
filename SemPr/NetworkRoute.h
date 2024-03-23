#pragma once

#include <iostream>
#include <fstream>
#include <bitset>
#include <string>

using namespace std;

class NetworkRoute
{
public:
	NetworkRoute();
	//todo NetworkRoute(NetworkRoute& other);
	void setNetworkAddress(string address);
	//todo void setNetworkMask(string mask);
	void setNetworkPrefix(int prefix);
	void setNextHop(string nextHopAddress);
	void setTtl(double ttl) { this->ttl = ttl; };
	void printRoute();
private:
	bitset<32> networkAddress;
	bitset<32> networkMask;
	int prefix;
	bitset<32> nextHop;
	double ttl;

	string bitsetToIp(bitset<32> addressInBitset);
	bitset<32> ipToBitset(string ipAddress);

};

inline NetworkRoute::NetworkRoute()
{
	this->networkAddress = bitset<32>();
	this->prefix = 0;
	this->networkMask = bitset<32>();
	this->nextHop = bitset<32>();
	this->ttl = 0;
}

void NetworkRoute::setNetworkAddress(string address)
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

inline void NetworkRoute::setNextHop(string nextHopAddress)
{
	this->nextHop = this->ipToBitset(nextHopAddress);
}

inline void NetworkRoute::printRoute()
{
	cout << "route: " << this->bitsetToIp(this->networkAddress) << "/" << this->prefix 
		<< ", mask: " << this->bitsetToIp(this->networkMask)
		<< ", next-hop: " << this->bitsetToIp(this->nextHop)
		<< ", ttl: " << this->ttl
		<< endl;
}

inline string NetworkRoute::bitsetToIp(bitset<32> addressInBitset)
{
	string bit_string = addressInBitset.to_string();
	string resultIp = "";
	istringstream bit_stream(bit_string);
	bitset<8> tmpSet;
	string delimeter = "";
	for (size_t i = 0; i < 4; i++)
	{
		bit_stream >> tmpSet;
		resultIp += delimeter + to_string(tmpSet.to_ulong());
		delimeter = ".";
	}
	return resultIp;
}

inline bitset<32> NetworkRoute::ipToBitset(string ipAddress)
{
	stringstream ip_stream(ipAddress);
	string bit_string = "";

	string octet;
	for (size_t i = 0; i < 4; i++)
	{
		getline(ip_stream, octet, '.');
		bit_string += bitset<8>(stoi(octet)).to_string();
	}
	return bitset<32>(bit_string);
}
