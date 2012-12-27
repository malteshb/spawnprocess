spawnprocess
============

Developer notes:
-----------------


SPAWNPROCESS is a linux process to invoke the any binary like openvpn, udhcpd etc., in cloudnode for Clearpath Networks Business Cloud solutions.
It is designed to run any binary like openvpn, udhcpd etc., for cloudnode. Currently the spawniprocess image is added to cloudnode package.

User gets a spawnprocess image and copied to debian package building directroy which needs to be build.


Requirements for Development Environment:
-----------------------------------------
Development system should run with gcc(GNU)




Steps to Configure and Build spawnprocess:
------------------------------------------
Needs to get spawnprocess check out from github as below
git clone https://username:password@github.com/clearpath-networks/spawnprocess.git
move to the spawnprocess directory
make distclean
./configure
make
we will get the spawnprocess image in the path spawnprocess/src/spawnprocess


spawnprocess Image:
--------------------
The spawnprocess image can build on different architecture 32bit as well 64bit.
currently we are building with amd64(64bit) and i386(32bit) on different flavours of Ubuntu
spawnprocess Image must be replaced based on target architecture.




TODO:
----
1) Need to add spawnprocess with cloudnode on the firmware build process

