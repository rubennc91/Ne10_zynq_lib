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

## Build the libraries Ne10 to work in ARM-standalone

1. Download GNU Embedded Toolchain from [here](https://developer.arm.com/open-source/gnu-toolchain/gnu-rm/downloads) and unzip in ```/opt/``` and change the name by ```gcc-arm-none-eabi```. You have to sure that inside of this folder are the folder ```bin```, ```lib```, etc.
2. A cmake file needs to be changed slightly. You can install ```cmake``` if you didn't do it yet with [this](https://cmake.org/install/).   
   
   You need to locate the cmake installation folder, generally is located in ```/usr/share/cmake```. So, when you locate it, go inside of the cmake folder, open the directory ```Modules/Platform``` and in this folder find the file ```Linux-GNU.cmake``` and change the line: ```set(CMAKE_SHARED_LIBRARY_LINK_${lang}_FLAGS "-rdynamic")``` by this one: ``` set(CMAKE_SHARED_LIBRARY_LINK_${lang}_FLAGS)# "-rdynamic")```.   
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
## Add the libraries to Xilinx SDK project
Once the libraries have been compiled, it is possible to include them in the desired project. In the case of using Xilinx SDK, it is necessary to follow the following steps:
1. Add the library to the gcc linker:
	```
	-l Ne10 m -L ../Ne10-standalone/build/modules/
	```
    Inside of this folder you can see a .a file. 
2. Add the library included in Path and Symbols (C/C++ General):
	```
	$NE10_PATH/Ne10-standalone/inc/
	```
3. If you have any problem with compilation, change the sentence of the Miscellaneous:
	```
	-mcpu=cortex-a9 -mfpu=neon -mfloat-abi=hard
	```
4. You have to make sure that in Directories, you can view the directory: 
```
/"Ne10-standalone/inc".
```


## Some advice for Zynq uses
If you want to make an FFT less than 256 points, maybe you can run the program perfectly, depends on the platform you are implementing.
But if you have a problem, you may not have enought "heap" memory.
To avoid this problem it is necessart to change the properties of the linker script. On the  project, right-click and select "Generate linker script". 
In this part, you can change the reserved memory for the program. Change the "Heap Size", default is 1KB, then change the size in bits that you need (4096000 = 4MB). 
Arround 4MB is anought to make an FFT of 8192 points. 

With all this, you can run the sample program. 

The sample program has some definitions to test all possibilities that the library has. 

```
#define NFFT 128
```
This define is used to select the number of points for the FFT function.

The following defines are used to select between different FFT configurations. 
	- ```C2C:``` is used to select between differents input types, it is possible real (0) or complex (1). The output is always complex. 
	- ```NEON```: You can select between using the NEON module (1) or not using it (0).
	- ```FFT```: If you need to perform the inverse FFT (Complex inputs, and real outputs), you can modify it with this define. 
	Obiously if you select C2C=1 the inputs are complex and the output complex too, therefore the iFFT is implemented, but the input is complex and the output too. 
```
#define C2C 	1	// COMPLEX INPUT AND COMPLEX OUTPUT
#define NEON	0	// USING NEON BLOCK
#define FFT	0	// FFT = 0; iFFT = 1;
```
The final definition is as follows:
	- ```SHOW_RESULTS``` it is used to display the input and output of the FFT test running. 
	- ```TEST_SAMPLES``` it is used to select the number of repetitions that the FFT algorithm must execute to obtain the time. 
```
#define SHOW_RESULTS 	0
#define TEST_SAMPLES	1000
```
