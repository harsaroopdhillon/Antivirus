#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <openssl\sha.h>
#include <fstream>
std::string sha256(const std::string str) {


	unsigned char hash[SHA256_DIGEST_LENGTH];
	SHA256_CTX sha256;
	if(!SHA256_Init(&sha256)) {
		return 0;
	}
	if (!SHA256_Update(&sha256, str.c_str(), str.size())) {
		return 0;
	}
	if (!SHA256_Final(hash, &sha256)) {
		return 0;
	}
	std::stringstream ss;
	for (unsigned int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
		ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
	}
	return ss.str();
}
std::string gethash(const std::string & file) {
	if (auto ifs = std::ifstream(file, std::ios::binary)) {
		std::string s((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
		return sha256(s);
	}
	return 0;
	
}
