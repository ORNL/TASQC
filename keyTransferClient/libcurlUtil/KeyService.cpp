/**----------------------------------------------------------------------------
Copyright (c) 2015-, UT-Battelle LLC
All rights reserved.

Authors: Jay Jay Billings, Phil Evans, Alex McCaskey
Author Contact: Phil Evans, evanspg@ornl.gov

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

* Neither the name of fire nor the names of its
  contributors may be used to endorse or promote products derived from
  this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
-----------------------------------------------------------------------------*/
#include "KeyService.h"
#include <stdlib.h>

namespace tasqc {

KeyService::KeyService() : hostname("localhost"), port("8000"){
	// Use the prefix regeneration function to setup the prefixes.
	regeneratePrefixes();
	http = new LibcurlUtils();
}

KeyService::KeyService(HTTPInterface * interface) :
		http(interface), hostname("localhost"), port("8080") {
	regeneratePrefixes();
}

KeyService::~KeyService() {
	// TODO Auto-generated destructor stub
	delete http;
}

void KeyService::regeneratePrefixes() {
	prefix = hostname + ":" + port + "/";
	getPrefix = prefix + "api/keys?authKey=";
}

void KeyService::setHostname(const std::string & hostname) {
	this->hostname = hostname;
	// Hostname has changed. Regenerate the prefixes.
	regeneratePrefixes();
}

std::string KeyService::getHostname() {
	return hostname;
}

std::string KeyService::getPort() {
	return port;
}

void KeyService::setPort(const std::string & port) {
	this->port = port;
	// Port has changed. Regenerate the prefixes.
	regeneratePrefixes();
}

std::string KeyService::getKey(const std::string & lastKey) {
	// Just add the last key onto the prefix and send it.
	std::string url = getPrefix + lastKey;

	// Issue the GET request to the HTTPInteface object
	std::string response = http->get(url,"user",
			"password");

	// Check for GET errors, if none, increment the Id counter.
	if (response != "CURL could not be initialized." &&
			response.find("returned HTTP code") == std::string::npos) {
		keyCounter++;
	}

	// Return the response.
	return response;
}

int KeyService::getKeyId() {
	return keyCounter;
}

} // end namespace tasqc
