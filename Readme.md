Accurate time series classification on Raspberry Pi Pico 

1. Produce a model using [C++ binaries](https://github.com/naviveztim/CDP_C_plus_binaries). 
   Based on operating system one can download Windows or Linux version of them.
2. Build the project

cd cdp_c_plus_pico 
rm -rf ./build (only if previous build exist) 
mkdir build 
cd build 
cmake .. 
make 

3. Deploy uf2 binaries on Raspberry Pi Pico device
4. test the output on Arduino IDE

For more [info](https://medium.com/@ivan_mitzev/accurate-time-series-classification-algorithm-on-raspberry-pi-pico-113a27523eac)

Extra notes: 
Check what symbols are generated in the object file (Classifiers.bin): 
cd ./build
nm  Classifiers.o
0008fd68 D _binary__mnt_c_Users_PC_pico_cdp_c_plus_pico_models_Classifiers_bin_end
0008fd68 A _binary__mnt_c_Users_PC_pico_cdp_c_plus_pico_models_Classifiers_bin_size
00000000 D _binary__mnt_c_Users_PC_pico_cdp_c_plus_pico_models_Classifiers_bin_start

Use these symbols as extern in C++ code to access the binary object.