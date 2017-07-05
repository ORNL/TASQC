# Timing Authentication Secured by Quantum Correlations

## Prerequisites:

* Java 7 JDK
* Maven
* CMake
* cURL

Make sure that the Java JDK is installed, not just the JRE. Fedora will install it by default, but on Ubuntu you need to explicitly install the JDK on top of the JRE.

## Build instructions for the server:

Move into `gov.ornl.tasqc.keytrans`

```sh
cd gov.ornl.tasqc.keytrans
```

Run 

```sh
mvn site
mvn package
mvn package shade:shade
```

Then execute with

```sh
java -jar target/keytrans-server-<version>.jar
```
## Properties

Properties are defined in the config.properties file. Currently supported properties are:

* secretKeyFile: The secret key source file. Defaults to "SecretKey.log".

* secretKeyFormat: The format for the secretKeyFile. The only available value is BitPerLine, which reads one "0" or "1" character from each line. All other settings will attempt to read the format used in SecretKey.log, where all keys are written in a single line of characters on the line after "KEY" appears. 

* keyFileName: The name of file where the key database will be written and subsequently read by the server. Defaults to "keyDB.txt". 

* queueSize: Sets the maximum number of keys for the queue of keys held in memory by the server.

## Build instructions for the client:

*More information is available in keyTransferClient/README.md.*

From the base directory where you checked out the repository:

```sh
mkdir keyTransferClient-build
cd keyTransferClient-build
cmake ../keyTransferClient-build
```

You may pick any build directory you want, actually. Just replace `../` with the directory.
