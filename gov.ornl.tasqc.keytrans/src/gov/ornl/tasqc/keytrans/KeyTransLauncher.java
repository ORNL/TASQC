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

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.net.URI;

import org.glassfish.grizzly.http.server.HttpServer;
import org.glassfish.grizzly.http.server.StaticHttpHandler;
import org.glassfish.jersey.grizzly2.httpserver.GrizzlyHttpServerFactory;
import org.glassfish.jersey.server.ResourceConfig;

/**
 * This class is responsible for launching the Grizzly Webserver that runs the
 * Quantum Key Transfer Engine for TASQC.
 * 
 * @author Jay Jay Billings
 */
public class KeyTransLauncher {

	// Base URI the Grizzly HTTP server will listen on
	public static final String BASE_URI = "http://0.0.0.0:8000/api";

	// Package id that contains the key transfer classes.
	public static final String packageID = "gov.ornl.tasqc.keytrans";

	/**
	 * This operation starts the server and registers the JAX-RS resources.
	 * 
	 * @return The HTTP server, in this case Grizzly.
	 * @throws IOException
	 *             This exception is thrown if the server fails to start.
	 */
	public static HttpServer startServer() throws IOException {
		// Create the resource configuration that pulls JAX-RS classes from the
		// KeyTrans package.
		final ResourceConfig resConf = new ResourceConfig().packages(packageID);

		// Create the server at base URI and register the KeyTrans JAX-RS
		// classes with it. Do not start it yet so that the static content can
		// be correctly configured too.
		HttpServer server = GrizzlyHttpServerFactory
				.createHttpServer(URI.create(BASE_URI), resConf, false);

		// Create a handler to serve the static content and register it
		StaticHttpHandler staticContentHandler = new StaticHttpHandler(
				"./static/web");
		server.getServerConfiguration().addHttpHandler(staticContentHandler,
				"/");

		// Start the server
		server.start();

		// Return the server
		return server;
	}

	/**
	 * This operation loads the keys from the IDQ key file.
	 */
	public static void parseKeyFile() {
		try {
			// Open the IDQ file
			FileReader fileReader = new FileReader("SecretKey.log");
			BufferedReader reader = new BufferedReader(fileReader);
			String line;
			// Open the key database file for this program
			FileWriter fileWriter = new FileWriter("keyDB.txt");
			BufferedWriter writer = new BufferedWriter(fileWriter);

			// Read the key from the file.
			while ((line = reader.readLine()) != null) {
				// The key is the line after the line denoted "KEY"
				if (line.startsWith("KEY")) {
					// So load it
					line = reader.readLine();
					// Break the key into smaller 765-bit chunks
					String key;
					while (line.length() >= 768) {
						key = line.substring(0, 767);
						line = line.substring(767);
						// Write the key to the key database
						writer.write(key);
						writer.newLine();
					}
				}
			}

			// Clean up shop
			reader.close();
			writer.close();
		} catch (IOException e) {
			// TODO Complain and ADD LOGGING UTILITY!
			e.printStackTrace();
		}

	}

	/**
	 * This is main method that launches the Quantum Key Transfer engine.
	 * 
	 * @param args
	 *            the incoming arguments from the command line
	 * @throws Exception
	 *             This exception is thrown if the QuantumKeyContext cannot be
	 *             started.
	 * @throws InterruptedException
	 *             This exception is thrown if the server cannot be started or
	 *             stopped properly.
	 */
	public static void main(String[] args)
			throws InterruptedException, Exception {
		// Some instructions for the user
		String launchMsg = String
				.format("Quantum Key Transfer Engine launched at "
						+ "%s\nHit enter to stop it...", BASE_URI);

		parseKeyFile();

		// Start the key reading service
		QuantumKeyContext keyContext = new QuantumKeyContext();
		keyContext.start();
		// keyContext.setProperties(null); //FIXME! Add properties support.
		// Set the QuantumKeyContext on its Resolver so that Jersey can load it
		// into the server when asked.
		QuantumKeyContextResolver.setQuantumKeyContext(keyContext);

		// Start the server
		final HttpServer server = startServer();

		// Dump the launch message, wait on user input and then shutdown the
		// server.
		System.out.println(launchMsg);
		System.in.read();
		server.shutdown();

		// Stop the key reading service
		keyContext.stop();

		return;
	}
}
