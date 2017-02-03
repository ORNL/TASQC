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
#define BOOST_TEST_MODULE example
#include <boost/test/included/unit_test.hpp>
#include "KeyService.h"
#include <stdlib.h>
#include "FakeLibcurlUtils.hpp"

using namespace tasqc;

BOOST_AUTO_TEST_SUITE(KeyServiceTestSuite)

/**
 * Checks the KeyService.getKey() operation.
 */
BOOST_AUTO_TEST_CASE(checkGet) {

	//Create a key service and get a key
	KeyService keyService;
	std::string lastKey = "1";
	std::string key = keyService.getKey(lastKey);

	// Try to pull down a key using the proper URL.
	// FIXME! Need predictable keys for the test!

	BOOST_REQUIRE(!key.empty());

	return;
}

/**
 * Makes sure that the setHostname and setPort operations work by setting them
 * to something else, which means they should fail assuming there is no server
 * there.
 */
BOOST_AUTO_TEST_CASE(checkHostAndPort) {

	//Create a key service and get a key
	KeyService keyService;
	std::string key = keyService.getKey("1");

	// Try to pull down a key using the proper URL.
	// FIXME! Need predictable keys for the test!
	BOOST_REQUIRE(!key.empty() && key.find("HTTP code 404") == key.npos);

	// Now store the original hostname and port before changing them.
	std::string hostname = keyService.getHostname();
	std::string port = keyService.getPort();

	// Change the hostname and make sure it fails
	keyService.setHostname("lillywood");
	key = keyService.getKey(key);
	BOOST_REQUIRE_EQUAL(key,"Could not resolve host: lillywood");

	// Reset the hostname, change the port and make sure it fails
	keyService.setHostname(hostname);
	keyService.setPort("1540");
	key = keyService.getKey(key);
	std::string err = "Failed to connect to localhost port 1540:";
	err += " Connection refused";
	BOOST_REQUIRE_EQUAL(key,err);

	// Reset both, change both and make sure it fails
	keyService.setHostname(hostname);
	keyService.setPort(port);
	keyService.setHostname("lillywood");
	keyService.setPort("1540");
	key = keyService.getKey(key);
	BOOST_REQUIRE_EQUAL(key,"Could not resolve host: lillywood");

	// Reset both, change both and make sure it fails
	keyService.setHostname(hostname);
	keyService.setPort(port);
	key = keyService.getKey(key);

	// Try to pull down a key using the proper URL.
	// FIXME! Need predictable keys for the test!

	BOOST_REQUIRE(!key.empty());

	return;
}

/**
 * Tests that for each key we get, we can also get its id.
 */
BOOST_AUTO_TEST_CASE(checkGetKeyId) {
	// Create a fake lib curl utils object that
	// returns good keys.
	bool returnGoodKeys = true;
	HTTPInterface * http = new FakeLibcurlUtils(returnGoodKeys);

	// Create the KeyService with the fake utils
	KeyService service(http);

	// At first the id should be 0
	BOOST_VERIFY(service.getKeyId() == 0);

	// Get a key and check that it has a valid id
	service.getKey("1");
	BOOST_VERIFY(service.getKeyId() == 1);

	// Again...
	service.getKey("2");
	BOOST_VERIFY(service.getKeyId() == 2);

	// Again...
	service.getKey("3");
	BOOST_VERIFY(service.getKeyId() == 3);

	// Now create a fake lib curl utils that returns
	// invalid keys - ie errors
	http = new FakeLibcurlUtils(false);
	KeyService badService(http);

	// Get an error response! make sure id wasn't incremented
	badService.getKey("4");
	BOOST_VERIFY(badService.getKeyId() == 0);

	badService.getKey("4");
	BOOST_VERIFY(badService.getKeyId() == 0);
}

BOOST_AUTO_TEST_SUITE_END()
