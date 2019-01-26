# Ne10_zynq_libs
It's a copy/fork of the [Ne10 project](https://github.com/projectNe10/Ne10). 

This is the Ne10 libraries to use it with Xilinx Zynq in baremetal. 

The libraries could be used in any ARM in baremetal, but it was not tested.

In order to compile the libraries it is necessary:
* CentOS 7
* cmake version 2.8.12.2
* gcc-arm-none-eabi-linux (ver. 2018 or upper)
* Vivado Design Suite (optional)

The GNU Embedded Toolchain (gcc-arm-none-eabi-linux) is be able to download from [here](https://developer.arm.com/open-source/gnu-toolchain/gnu-rm/downloads)

If you wish install Vivado Design Suite, you can download from [here](https://www.xilinx.com/support/download.html)

# Build the libraries Ne10 to work in ARM-standalone

1. Download GNU Embedded Toolchain from [here](https://developer.arm.com/open-source/gnu-toolchain/gnu-rm/downloads) and unzip in ```/opt/``` and change the name by ```gcc-arm-none-eabi```. You have to sure that inside of this folder are the folder ```bin```, ```lib```, etc.
2. A cmake file needs to be changed slightly. You can install ```cmake``` if you didn't do it yet with [this](https://cmake.org/install/).

  1.You need to locate the cmake installation folder, generally is located in ```/usr/share/cmake```. So, when you locate it, go inside of the cmake folder, open the directory ```Modules/Platform``` and in this folder find the file ```Linux-GNU.cmake``` and change the line: ```set(CMAKE_SHARED_LIBRARY_LINK_${lang}_FLAGS "-rdynamic")``` by this one: ``` set(CMAKE_SHARED_LIBRARY_LINK_${lang}_FLAGS)# "-rdynamic")```

   The ```-rdynamic``` flag allows to the system make dynamic memory reservations, but in our case it does not usefull, because the libraries are intended to be used in standalone, where dynamic reservations have no relevance. This step can be reversed when the library is compiled.

3. Clone this github to your CentOS directory.
```
git clone URL
```
4. Create the build folder and open it.
```
mkdir build && cd build
```
5. Build the libraries.
```
cmake -DCMAKE_TOOLCHAIN_FILE=../GNUlinux_config.cmake ..
make
```
