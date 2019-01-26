# Ne10_zynq_libs
It's a fork of the [Ne10 project](https://github.com/projectNe10/Ne10). 

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
1. Clone this github to your CentOS directory.
```
git clone URL
```
2. Download GNU Embedded Toolchain and unzip in ```/opt/``` and change the name by ```gcc-arm-none-eabi```. You have to sure that inside of this folder are the folder ```bin```, ```lib```, etc.
