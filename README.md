# rawtcp - library
 A Cross Platform library that reimplements the TCP protocol and exposes lower levels of the socket API using raw sockets.

## Building
### MacOS<br/>
#### Dependencies
To build this library in MacOS, it requires the pcap library to be installed on your device. You can easily install this with the brew command. The library uses pkg-config to find it's dependencies, and as a result requires pcap to be added to the `PKG_CONFIG_PATH`.
```
brew install libpcap
brew info libpcap              # Should show you a command that resembles the following command in a pkg-config section
export PKG_CONFIG_PATH="..."   # Path will be set to whatever you installation directory is
```
#### Setup
Setting up the library to be build can be done easily with the *.sh scripts I've written, all of which require cmake to be installed and in your path.
```
./cmake_init_shared.sh # To generate a dynamically linked library
./cmake_init_static.sh # To generate a statically linked library
```
The library can then be built using the normal cmake build command.
```
cmake --build build
```
Any projects using this library will require root access to open the actual raw socket.
### Windows<br/>
#### Dependencies
This library requires the npcap library, the windows version of pcap. This can be done first by going to the npcap website and running the installer, the default settings should* work fine. You then want to download the npcap SDK. If you intend to use my scripts to set up the project, I would recommend unzipping into the `C:\Program Files\Npcap` directory (which should be created by the npcap installer).
#### Setup
Setting up the library to be build can be done easily with the *.sh scripts I've written, all of which require cmake to be installed and in your path.
```
cmake_init_shared.bat # To generate a dynamically linked library
cmake_init_static.bat # To generate a statically linked library
```
### Linux<br/>
Support is not finished yet, likely will not require any dependencies.
#### Setup
Setting up the library to be build can be done easily with the *.sh scripts I've written, all of which require cmake to be installed and in your path.
```
./cmake_init_shared.sh # To generate a dynamically linked library
./cmake_init_static.sh # To generate a statically linked library
```
