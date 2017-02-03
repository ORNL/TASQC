#This is the key monitoring backend for the TASQC Key Transfer Engine.

## Prerequisites:
Java 7 JDK

Maven

CMake

cURL

Make sure that the Java JDK is installed, not just the JRE. Fedora will install it by default, but on Ubuntu you need to explicitly install the JDK on top of the JRE.

## Build instructions for the server:

Move into gov.ornl.tasqc.keytrans

Run 

>mvn site

>mvn package

>mvn package shade:shade

Then execute with

>java -jar target/keytrans-server-&lt;version>.jar

## Build instructions for the client:

*More information is available in keyTransferClient/README.md.*

From the base directory where you checked out the repository:

>mkdir keyTransferClient-build

>cd keyTransferClient-build

>cmake ../keyTransferClient-build

You may pick any build directory you want, actually. Just replace "../" with the directory.
