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

## Build instructions for the client:

*More information is available in keyTransferClient/README.md.*

From the base directory where you checked out the repository:

```sh
mkdir keyTransferClient-build
cd keyTransferClient-build
cmake ../keyTransferClient-build
```

You may pick any build directory you want, actually. Just replace `../` with the directory.
