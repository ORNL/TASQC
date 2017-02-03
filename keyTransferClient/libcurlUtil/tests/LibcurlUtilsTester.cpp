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

* Neither the name of keytransclient nor the names of its
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
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE UpdaterTester_testSuite

#include <fstream>
#include <sstream>
#include <iostream>
#include <LibcurlUtils.h>
#include "TesterUtils.h"
#include <memory>
#include <boost/test/included/unit_test.hpp>

using namespace std;
using namespace tasqc;

/**
 * A LibcurlUtilsPtr is a shared pointer object referring to a LibcurlUtils instance.
 */
typedef std::shared_ptr<LibcurlUtils> LibcurlUtilsPtr;

struct TestStruct {

	/**
	 * An instance of LibcurlUtilsPtr used for testing.
	 */
	LibcurlUtilsPtr libcurlUtilsPtr;

	/**
	 * A standard template library map container keyed on TesterPropertyType.
	 */
	TesterPropertyMap testerPropertyMap;

	TestStruct() {

		//Construct a new LibcurlUtils instance.
		libcurlUtilsPtr = LibcurlUtilsPtr(new LibcurlUtils());

		//Get the contents of updatertests.properties as a string.
		stringstream ss;
		ss << "max_number_of_posts=100\n";
		ss << "max_post_time_interval=500\n";
		ss << "url_path=http://localhost:8000/api/keys\n";
		ss << "username=user\n";
		ss << "password=secret\n";

		string contents = ss.str();

		//Create the tester property map.
		testerPropertyMap = TesterUtils::getTesterPropertyMap(contents);

		return;
	}

	~TestStruct() {}

};


BOOST_FIXTURE_TEST_SUITE(LibcurlUtilsTester_testSuite,TestStruct)

/**
 * Checks the LibcurlUtils get() operation.
 */
BOOST_AUTO_TEST_CASE(checkGet) {

	//Create a string to hold the full url to the SampleText.txt file
	string urlFilePath = testerPropertyMap.at(URL_PATH);

	//Use libcurlUtils to get the contents of the file at urlPath.
	string urlContents = libcurlUtilsPtr->get(urlFilePath, testerPropertyMap.at(USER_NAME), testerPropertyMap.at(PASS_WORD));

	// Verify that get() returns the "Failure" since no authentication was passed.
	BOOST_REQUIRE_EQUAL(urlContents,"Failure");

	// Try to pull down a key using the proper URL.
	// FIXME! Temporary use of authKey=1.
	urlFilePath += "?authKey=1";
	urlContents = libcurlUtilsPtr->get(urlFilePath, testerPropertyMap.at(USER_NAME), testerPropertyMap.at(PASS_WORD));
	BOOST_REQUIRE(!urlContents.empty());

	return;
}

BOOST_AUTO_TEST_SUITE_END()
