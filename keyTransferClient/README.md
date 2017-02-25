# Quantum Key Transfer Client

The Quantum Key Transfer Client is a thin wrapper around the popular 
curl utility for retrieving available quantum-generated keys from a 
remote key generation server. This utility can be used by GNU Radio 
clients to retrieve generated keys secured over an encrypted classical 
channel. 

The thin curl wrapper is called LibcurlUtil and provides a 
get method to perform an HTTP Get operation on the server 
providing quantum-generated keys. 

## Prerequisites

You will need git and cmake to build Key Transfer Client.

## Checkout and build

From a shell, execute the following commands to compile the code:

```sh
git clone https://code.ornl.gov/aqw/tasqc
cd tasqc
mkdir keyTransferClient-build
cd keyTransferClient-build
cmake ../keyTransferClient -DCMAKE_BUILD_TYPE=Debug -G"Eclipse CDT4 - Unix Makefiles" -DCMAKE_ECLIPSE_VERSION=4.5
make
```

The above will get the code running, but it will not run the tests. 
Issue the following command to do that:

```
bash
make test
```

Build flags, such as `-Wal`l, can be set by prepending the `CXX_FLAGS` variable to 
the cmake command as such

If you want to install the compiled product to your system you can run the `make install` command to finish it.
If you want to install the client to a particularly directory, you can do that by setting the install prefix:

```
cmake ../keyTransferClient -DCMAKE_INSTALL_PREFIX=<directory_you_want> <other cmake arguments>
make
make install
```

## Questions

Questions can be directed to me at evanspg <at> ornl <dot> gov, mccaskeyaj <at> ornl <dot> gov, or 
billingsjj <at> ornl <dot> gov.
