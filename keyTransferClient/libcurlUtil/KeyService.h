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

#ifndef LIBCURLUTIL_KEYSERVICE_H_
#define LIBCURLUTIL_KEYSERVICE_H_

#include <LibcurlUtils.h>

namespace tasqc {

/**
 * This class provides 256-bit keys to clients upon request.
 *
 * This service will be default look for keys at localhost:8000/, but hostnames
 * and ports can be configured. Hostnames and ports are mapped as
 * hostname:port/ such that for a hostname of localhost and port of 8000 they
 * will be mapped as localhost:8000/.
 *
 * \author Jay Jay Billings
 */
class KeyService {

private:

	/**
	 * This utility is used to pull keys from the server.
	 */
	HTTPInterface * http;

	/**
	 * The hostname. Default value is localhost.
	 */
	std::string hostname;

	/**
	 * The port. Default value is 8000.
	 */
	std::string port;

	/**
	 * This is the connection prefix for the services. By default it maps to
	 * localhost:8000/tasqc/ and is constructed by combining the hostname and
	 * port if and when they are set.
	 */
	std::string prefix;

	/**
	 * This is the prefix used for retrieving keys from the server. By default
	 * it is localhost:8000/tasqc/api/keys?authKey= and the value of the last
	 * key retrieved from the server is appended to it to make the request.
	 */
	std::string getPrefix;

	/**
	 * The number of keys served.
	 */
	int keyCounter = 0;

	/**
	 * This operation regenerates the prefixes for pulling keys. It is called
	 * when the hostname or port changes and it regenerates the base prefix and
	 * sub-prefixes, such as the key retrieval prefix.
	 */
	void regeneratePrefixes();

public:

	/**
	 * The constructor
	 */
	KeyService();

	/**
	 * Constructor that takes pointer to HTTPInterface.
	 * Note that KeyService will delete this pointer upon
	 * going out of scope or being deleted.
	 */
	KeyService(HTTPInterface * interface);

	/**
	 * Virtual destructor
	 */
	virtual ~KeyService();

	/**
	 * This operation sets the hostname that the key service should use for
	 * obtaining keys.
	 * @param hostname the hostname
	 */
	void setHostname(const std::string & hostname);

	/**
	 * This operation gets the hostname that the key service is currently
	 * configured to use.
	 * @return the hostname
	 */
	std::string getHostname();

	/**
	 * This operation sets the port that the key service should use for
	 * obtaining keys on the host.
	 * @param port the port
	 */
	void setPort(const std::string & port);

	/**
	 * This operation gets the port that the key service is currently
	 * configured to use.
	 * @param the port
	 */
	std::string getPort();

	/**
	 * This operation returns a key. It requires the last key for
	 * authentication that the client is authorized to receive keys.
	 * @param lastKey the previous key, for authentication
	 * @return the next key
	 */
	std::string getKey(const std::string & lastKey);

	/**
	 * This operation returns a previously served key with an id equal to
	 * keyId. It requires the last key for authentication that the client
	 * is authorized to receive keys.
	 * @param lastKey the previous key, for authentication
	 * @param keyId the unique ID number of the key being requested
	 * @return the next key or an invalid key if the server has no key with
	 * an id of keyid
	 */
	std::string getKey(const std::string & lastKey, const std::string & keyId);

	/**
	 * Return the Id for the currently returned key.
	 */
	int getKeyId();
};

} // end namespace tasqc

#endif /* LIBCURLUTIL_KEYSERVICE_H_ */

