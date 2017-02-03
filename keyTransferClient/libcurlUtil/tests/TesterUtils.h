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

* Neither the name of kettransclient nor the names of its
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
#ifndef TESTERUTILS_H
#define TESTERUTILS_H

#include <string>
#include <vector>
#include <memory>
#include <map>
#include <boost/shared_ptr.hpp>
#include <PropertyType.h>
#include "TesterPropertyType.h"

/**
 * A TesterPropertyMap is a map template associating with TesterPropertyType keys and string values.
 */
typedef std::map<TesterPropertyType, std::string> TesterPropertyMap;


std::vector<std::string> &split(const std::string &s, char delim,
                                std::vector<std::string> &elems)
{
  std::stringstream ss(s);
  std::string item;
  while (std::getline(ss, item, delim))
    elems.push_back(item);
  return elems;
}

std::vector<std::string> split(const std::string &s, char delim)
{
  std::vector<std::string> elems;
  split(s, delim, elems);
  return elems;
}


/**
 * TesterUtils is a utility class containing static operations used by the Updater tester classes.
 */
class TesterUtils {

public:

    /**
     * Creates and returns a TesterPropertyMap type associating a TesterPropertyType with a string value
     * from a string formatted as a Java properties file.
     *
     * @param testerPropertyString A updatertests.properties formatted string.
     * @return A TesterPropertyMap comprised of the name/value pairs in the testerPropertyString.
     */
    static TesterPropertyMap getTesterPropertyMap(std::string testerPropertyString) {
        // Create an empty map associating a TesterPropertyType with a std::string value
        TesterPropertyMap testerPropertyMap;

        // Split propertyString into lines and store in a vector
        std::vector<std::string> lines = split(testerPropertyString, '\n');

        // Loop over all tokens
        for(std::string line : lines) {

            // Create a new vector to hold the name and value
            std::vector<std::string> pair;

            //If the "=" character is found in this line
            if(line.find("=") != std::string::npos) {

                //Split the line into a property/value pair
                pair = split(line, '=');

                //Get the property name
                std::string property = pair[0];

                //Get the property value
                std::string value = pair[1];

                if(property=="max_number_of_posts") {

                    //Insert TesterPropertyMap MAX_NUMBER_OF_POSTS and the value
                    testerPropertyMap.insert(TesterPropertyMap::value_type(MAX_NUMBER_OF_POSTS, value));

                } else if(property=="max_post_time_interval") {

                    //Insert TesterPropertyMap MAX_POST_TIME_INTERVAL and the value
                    testerPropertyMap.insert(TesterPropertyMap::value_type(MAX_POST_TIME_INTERVAL, value));

                } else if(property=="ignore_ssl_peer_verification") {

                    //Insert TesterPropertyMap IGNORE_SSL_PEER_VERIFICATION and the value
                    testerPropertyMap.insert(TesterPropertyMap::value_type(IGNORE_SSL_PEER_VERIFICATION, value));

                } else if(property=="url_path") {

                    //If the url_path value does not end in "/"
                    if(value[value.length()-1] != '/') {

                        //Then add a "/" to the end of the value.
                        value += "/";
                    }

                    //Insert TesterPropertyMap URL_PATH and the value
                    testerPropertyMap.insert(TesterPropertyMap::value_type(URL_PATH, value));

                } else if (property =="username") {
                	// Get the username
                    testerPropertyMap.insert(TesterPropertyMap::value_type(USER_NAME, value));
                } else if (property =="password") {
                	// Get the username
                    testerPropertyMap.insert(TesterPropertyMap::value_type(PASS_WORD, value));
                }

            }

        }

        //Return the property map
        return testerPropertyMap;
    }

};

#endif
