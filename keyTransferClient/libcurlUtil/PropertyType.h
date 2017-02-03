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
#ifndef PROPERTYTYPE_H
#define PROPERTYTYPE_H

namespace tasqc {

/**
 * The PropertyType enumeration consists of literals representing each type of
 * configuration property used by Updater.
 */
enum PropertyType {
	/**
	 * This literal indicates a url.
	 */
	URL = 0,

	/**
	 * This literal indicates a simulation item id.
	 */
	ITEM_ID,

	/**
	 * This literal indicates a unique client key.
	 */
	CLIENT_KEY,

	/**
	 * This literal represents the username that should be used for
	 * authenticating with the server.
	 */
	USERNAME,

	/**
	 * This literal represents the username that should be used for
	 * authenticating with the server.
	 */
	PASSWORD,

	NETWORKING_TOOL
};

} // end namespace tasqc

#endif

