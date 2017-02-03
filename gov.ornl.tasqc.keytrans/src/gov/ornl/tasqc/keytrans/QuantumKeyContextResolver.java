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
package gov.ornl.tasqc.keytrans;

import javax.ws.rs.Produces;
import javax.ws.rs.ext.ContextResolver;
import javax.ws.rs.ext.Provider;

/**
 * This is a simple ContextResolver to provide access to the Quantum Key
 * database service.
 * 
 * The QuantumKeyContext should be a singleton. It should be set early in the
 * life cycle of the server, so setQuantumKeyContext() must be called from a
 * higher function (main() perhaps) before Java starts injecting this Resolver
 * into Resources. This is required because the QuantumKeyContext reads keys
 * continuously from disk.
 * 
 * @author Jay Jay Billings
 *
 */
@Provider
@Produces("text/plain")
public class QuantumKeyContextResolver
		implements ContextResolver<QuantumKeyContext> {

	/**
	 * The QuantumKeyContext that will be returned by this resolver.
	 */
	private static QuantumKeyContext context;

	/**
	 * This operation sets the QuantumKeyContext service that should be used to
	 * serve quantum keys by the client. It is a singleton.
	 * 
	 * @param keyContext
	 *            The (
	 */
	public static void setQuantumKeyContext(QuantumKeyContext keyContext) {
		context = keyContext;
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see javax.ws.rs.ext.ContextResolver#getContext(java.lang.Class)
	 */
	@Override
	public QuantumKeyContext getContext(Class<?> clazz) {
		return context;
	}

}
