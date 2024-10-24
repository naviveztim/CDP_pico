#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "hardware/i2c.h"
#include "hardware/interp.h"
#include "../headers/Cdp.h"
#include "../headers/Utils.h"
#include <chrono>



TimeSeries generateRandomTimeSeries(int numSamples) {
    
    TimeSeries ts;
    ts.ClassIndex = -1;

    // Generate 100 random values for the Values vector in the range [0.0, 1.0)
    for (int i = 0; i < numSamples; ++i) {
        double randomValue = static_cast<double>(std::rand()) / RAND_MAX; 
        ts.Values.push_back(randomValue);
    }

    // NOTE: 
    // Apply compression, derivative, normalization- if applied during training

    return ts;
}

int main()
{
    stdio_init_all();  // Initialize USB serial

    // Seed the random number generator 
    std::srand(static_cast<unsigned int>(std::time(0)));

    //Load already trained classifiers
    Cdp cdp;
    cdp.LoadClassifiers();

    while (true) {
        
        // Obtain the time series to be classified 
        int numSamples = 60; 
        TimeSeries ts = generateRandomTimeSeries(numSamples);

        // Predict class index of the goven time series 
        int found_index = cdp.ClassifyTimeSeries(ts);

        // Show results 
        printf("Predicted class index: %d\n", found_index);

        sleep_ms(100);             
    }

    return 0;
}
