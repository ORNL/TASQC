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
import java.io.FileReader;
import java.io.IOException;
import java.util.HashMap;
import java.util.Map;
import java.util.Properties;
import java.util.concurrent.ArrayBlockingQueue;
import java.util.concurrent.atomic.AtomicBoolean;
import java.util.concurrent.atomic.AtomicReference;
import java.util.function.BinaryOperator;

/**
 * This class reads quantum keys from the key database and makes them available
 * to clients. Keys are loaded from the file system as they become available and
 * loaded into a blocking queue that will wait when it is full and when it is
 * empty. Reading the keys is performed on a separate thread. This class is, in
 * general, thread safe.
 * 
 * This class must be started and stopped with the start() and stop() operations
 * respectively because it needs to properly stop any on-going file I/O.
 * 
 * This class can be configured by passing Properties to the setProperties()
 * operation. This operation must be called after the start() operation, but it
 * is not required that it be called. This class has a full range of default
 * values for all of its parameters.
 * 
 * @author Jay Jay Billings
 *
 */
public class QuantumKeyContext implements Runnable {

	/**
	 * The full set of quantum keys available.
	 */
	private ArrayBlockingQueue<String> keyQueue;

	/**
	 * The maximum number of keys that can be stored in the queue.
	 */
	private int maxQueueSize = 1024;

	/**
	 * The name of the file where quantum keys are stored.
	 */
	private String keyFileName = "keys.txt";

	/**
	 * True if the quantum key context is loading keys and operating normally,
	 * false otherwise.
	 */
	private AtomicBoolean running;

	/**
	 * The next key.
	 */
	private AtomicReference<String> nextKey;

	/**
	 * The thread that governs I/O events for loading keys and loading the key
	 * queue.
	 */
	private Thread eventLoop;

	/**
	 * TODO Replace this with an encrypted file that is read from and regularly
	 * pruned as keys age to the point they are not likely to be retrieved. A
	 * map of previously served keys, indexed by their ID numbers.
	 */
	private Map<Integer, QuantumKey> keyMap;

	/**
	 * This is the minimum fraction of the queue's keys that should be left
	 * before it is reloaded.
	 */
	private float minQueueLoadFraction = 0.1f;

	/**
	 * The unique identifier that will be sent with the next key
	 */
	private AtomicReference<Integer> nextID;

	/**
	 * Constructor
	 */
	public QuantumKeyContext() {
		// The service is not running yet.
		running = new AtomicBoolean(false);

		// No keys are available yet
		nextKey = new AtomicReference<String>("");

		// Initialize the ID counter
		nextID = new AtomicReference<Integer>(0);

		// Initialize the old key map
		keyMap = new HashMap<Integer, QuantumKey>();
	}

	/**
	 * This operation sets the properties of the Context. It must be called
	 * before the start operation and if it is called after start() it will fail
	 * with an exception.
	 * 
	 * @param props
	 *            Properties (HashTable<String,String>) that control the way the
	 *            Context is configured. Valid keys are:
	 *            <ul>
	 *            <li><b>keyFileName</b></li> - the name of the file where keys
	 *            are stored. (Default = keyDB.txt)
	 *            <li><b>queueSize</b></li> - the size of the queue that holds
	 *            the keys. (Default = 1024)
	 *            </ul>
	 * @throws Exception
	 *             This exception is thrown is start() was previously called or
	 *             properties is null.
	 */
	public void setProperties(Properties props) throws Exception {

		// This operation should not run if the thread is running!
		if (!running.get()) {
			// Reset each property from the default value in turn. Start with
			// the key file name.
			String tmpKeyFileName = (String) props.get("keyFileName");
			if (tmpKeyFileName != null) {
				keyFileName = tmpKeyFileName;
			}
			// Set the max queue size
			int tmpMaxQueueSize = (int) props.get("queueSize");
			if (tmpMaxQueueSize > 0) {
				maxQueueSize = tmpMaxQueueSize;
			}
		} else {
			// Set an error message and throw the exception. Note a
			// NullPointerException will automatically be thrown if props is
			// null.
			String msg = "Properties cannot be set after the Context is started!\n";
			msg += " (Cannot setProperties() before start!())";
			throw new Exception(msg);
		}
	}

	/**
	 * This operation starts the event loop that loads the keys from the disk.
	 * This operation should always be called once after construction and before
	 * any calls to get a key.
	 * 
	 * This operation blocks for one second while it waits for the first key to
	 * be loaded so that key information is always available. If a key is not
	 * loaded after one second, it throws an exception and the Context should
	 * not be used.
	 * 
	 * @throws InterruptedException
	 *             This exception is thrown if the thread cannot sleep.
	 * @throws Exception
	 *             This exception is thrown if keys are not loaded after one
	 *             second and it indicates that the service is not working
	 *             properly.
	 */
	public void start() throws InterruptedException, Exception {

		// Create the queue. 1024 is a good default size because, if I
		// remember correctly, it is the maximum size of this structure
		// where Java still performs admirably. That may be incorrect
		// though. I think I read that three years ago (...in 2012...).
		keyQueue = new ArrayBlockingQueue<String>(maxQueueSize);

		// Start the service
		eventLoop = new Thread(this);
		running.set(true);
		eventLoop.start();

		// Block for a second
		Thread.currentThread();
		Thread.sleep(1000);
		// Throw the exception if the keys can't be loaded.
		if (!"".equals(nextKey.get())) {
			throw new Exception(
					"Unable to load keys! Do not use this Context!");
		}
		// Set the initial key value since keys have been loaded.
		nextKey.set(keyQueue.take());

		return;
	}

	/**
	 * This operation stops the event loop. This job should always be called to
	 * shutdown the service. This operation will block the calling thread until
	 * the event loop is shutdown.
	 * 
	 * @throws InterruptedException
	 *             This exception is thrown if the calling thread cannot be made
	 *             to join for some unspecified reason.
	 */
	public void stop() throws InterruptedException {
		// Shut down the service
		running.set(false);
		eventLoop.join(10);
	}

	/**
	 * This operation acts as the event loop for the service. It reads and
	 * retrieves keys from the file, updating both the queue and the nextKey
	 * value as required.
	 */
	@Override
	public void run() {
		try {
			// Open the file
			FileReader fileReader = new FileReader(keyFileName);
			BufferedReader reader = new BufferedReader(fileReader);
			String line;

			// Load the keys continuously until the service is shutdown.
			while (running.get()) {
				// Read the next key from the file
				if ((line = reader.readLine()) != null) {
					// Load the key into the queue
					keyQueue.put(line);
				}
				// FIXME! This could be optimized to sleep for some amount of
				// time and then to read until the file is empty or the queue is
				// full.
			}

			// Clean up shop
			reader.close();
		} catch (IOException | InterruptedException e) {
			// TODO Complain and ADD LOGGING UTILITY!
			running.set(false);
			e.printStackTrace();
		}

		return;
	}

	/**
	 * The operation retrieves a previously obtained key, which had been created
	 * with the getNextKey() method, based on the specified ID.
	 * 
	 * @param Id
	 *            The ID number of the key which is to be retrieved. This must
	 *            be non-negative.
	 * @return The previously generated key with the given Id number, or null if
	 *         no such key exists.
	 */
	public QuantumKey getKeyById(int Id) {
		return keyMap.get(Id);
	}

	/**
	 * This operation retrieves the next available quantum key. It will
	 * automatically discard the key unless there are no other keys left in the
	 * queue.
	 * 
	 * @return the next quantum key
	 * @throws InterruptedException
	 *             This exception is thrown if the next key cannot be properly
	 *             retrieved.
	 */
	public QuantumKey getNextKey() throws InterruptedException {

		// In practice this strategy could fail if the queue is being quickly
		// depleted because multiple values of the same key will return while
		// the key queue is being repopulated. Let's see how it works for now,
		// but it may require some better logic to handle that situation.

		String keyString;
		if (keyQueue.size() >= 1) {
			// Load the key if there are extra keys available and replace it
			// with the next key in the queue.
			keyString = nextKey.getAndSet(keyQueue.take());
		} else {
			// Just return the current key since it is the last key.
			keyString = nextKey.get();
		}

		// The key under construction
		QuantumKey key = new QuantumKey();

		// Assign the next available id to this key and increment the next id
		key.setId(nextID.getAndAccumulate(1, new BinaryOperator<Integer>() {
			@Override
			public Integer apply(Integer m, Integer n) {
				return m + n;
			}
		}));

		// Set the converted data to the key
		key.setKey(keyString);

		// Add the key to the map
		keyMap.put(key.getId(), key);

		return key;
	}

}
