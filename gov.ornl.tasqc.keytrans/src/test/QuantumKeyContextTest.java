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
package test;

import static org.junit.Assert.assertEquals;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;

import gov.ornl.tasqc.keytrans.QuantumKeyContext;

/**
 * This class is responsible for testing
 * {@link gov.ornl.tasqc.keytrans.QuantumKeyContext}.
 * 
 * @author Jay Jay Billings
 *
 */
public class QuantumKeyContextTest {

	/**
	 * The context that will be tested.
	 */
	QuantumKeyContext context;

	/**
	 * This operation starts the key service before the tests are run.
	 * 
	 * @throws java.lang.Exception
	 */
	@Before
	public void setUp() throws Exception {
		context = new QuantumKeyContext();
		context.start();
	}

	/**
	 * @throws java.lang.Exception
	 */
	@After
	public void tearDown() throws Exception {
		context.stop();
	}

	/**
	 * Test method for
	 * {@link gov.ornl.tasqc.keytrans.QuantumKeyContext#getNextKey()}.
	 * 
	 * @throws IOException
	 *             This exception is thrown if the file can't be read.
	 * @throws InterruptedException
	 *             This exception is thrown if the key cannot be retrieved.
	 */
	@Test
	public void testGetNextKey() throws IOException, InterruptedException {

		// Open the reference key file
		FileReader fileReader = new FileReader("keys.txt");
		BufferedReader reader = new BufferedReader(fileReader);
		String line;
		ArrayList<String> lines = new ArrayList<String>();
		// Load the keys continuously until the service is shutdown.
		while ((line = reader.readLine()) != null) {
			lines.add(line);
		}
		// Clean up shop
		reader.close();
		// Start checking keys
		for (int i = 0; i < lines.size(); i++) {
			assertEquals(lines.get(i), context.getNextKey().getKey());
		}
		return;
	}

	/**
	 * This operation makes sure that the context will throw an exception if a
	 * client tries to set properties after it starts.
	 * 
	 * @throws Exception
	 *             an exception that was thrown if properties were set after the
	 *             service was started.
	 */
	@Test(expected = Exception.class)
	public void testNoLateProperties() throws Exception {
		// Null properties are as good as any other when they aren't allowed.
		context.setProperties(null);
	}

	/**
	 * This operation makes sure that the context will throw an exception if a
	 * client tries to set null properties.
	 * 
	 * @throws Exception
	 *             an exception that was thrown if properties were null.
	 */
	@Test(expected = Exception.class)
	public void testNoNullProperties() throws Exception {
		// We need to stop the server first so the properties will be used.
		context.stop();
		context.setProperties(null);
	}

}
