/**----------------------------------------------------------------------------
Copyright (c) 2017-, UT-Battelle LLC
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

import javax.xml.bind.annotation.XmlRootElement;

/**
 * This class is the representation of a Quantum Key provided by a
 * QuantumKeyContext. It contains the raw bits of the key and a unique
 * identifier for the key.
 * 
 * @author Robert Smith
 *
 */
@XmlRootElement
public class QuantumKey {

	/**
	 * The ID number which uniquely identifies this key. Id is to be a
	 * nonnegative integer. Negative IDs are to be set only if a key is invalid.
	 */
	private int Id;

	/**
	 * The binary representation of the key.
	 */
	private String key;

	/**
	 * The default constructor.
	 */
	public QuantumKey() {
		Id = 0;
		key = "";
	}

	/**
	 * Getter method for the key's Id.
	 * 
	 * @return The key's Id.
	 */
	public int getId() {
		return Id;
	}

	/**
	 * Getter method for the set of bits defining the key.
	 * 
	 * @return A BitSet containing the key's binary representation.
	 */
	public String getKey() {
		return key;
	}

	/**
	 * Setter method for the key's Id.
	 * 
	 * @param newId
	 */
	public void setId(int newId) {
		Id = newId;
	}

	/**
	 * Setter method for the key.
	 * 
	 * @param newKey
	 */
	public void setKey(String newKey) {
		key = newKey;
	}
}
