#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "hardware/i2c.h"
#include "hardware/interp.h"
#include "../headers/Cdp.h"
#include "../headers/Utils.h"
#include <chrono>
#include <random>


TimeSeries generateRandomTimeSeries(bool useSignal, bool normalize, int compressionFactor, int numSamples) {
    
    TimeSeries ts;
    
    // Resize the vector to hold the given number of elements
    ts.Values.resize(numSamples);

    // Create a random number generator
    std::random_device rd;  
    std::mt19937 gen(rd()); 

    // Define the distribution in the range [-2, 2]
    std::uniform_real_distribution<float> dis(-2.0f, 2.0f);

    // Fill 'ts.Values' with 576 random values
    for (auto i = 0; i < ts.Values.size(); ++i) {
        ts.Values[i] = dis(gen);
    }

    // Take derivative, if specified
    if (!useSignal)
    {
        Utils::Derivative(ts.Values);
    }

    // Normalize the signal, if specified
    if (normalize)
    {
        Utils::Normalize(ts.Values);
    }

    // Compress the signal, if specified 
    if (compressionFactor > 1)
    {
        Utils::CompressATimeSeries(ts, compressionFactor);
    }

    
    return ts;
}

int main()
{
    int found_index = 1;
    int numSamples = 500; 
    int testRepetitions = 100; 
    std::chrono::high_resolution_clock::time_point start, end;
    std::chrono::duration<double> elapsed;
    float elapsedTime = 0.0; 
    Cdp cdp;
    
    stdio_init_all();  // Initialize USB serial

    // Wait a bit to ensure the USB serial is ready
    sleep_ms(2000);

    try {
        // Load already trained classifiers
        cdp.LoadClassifiers();

        while (true) {
            try {

                start = std::chrono::high_resolution_clock::now();

                // Obtain the time series to be classified
                TimeSeries ts = generateRandomTimeSeries(true, true, 4, numSamples);

                for (auto i = 0; i < testRepetitions; i++) {
                    
                    // Classify the time series 
                    found_index = cdp.ClassifyTimeSeries(ts);
                }

                end = std::chrono::high_resolution_clock::now();
                elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
                elapsedTime = elapsed.count() / testRepetitions; 

                // Show timiing
                printf("Average Inference Time: %.2f\n\n", elapsedTime);
                 
                // Show results
                printf("Predicted class index: %d\n\n", found_index);
                
                sleep_ms(1000);
            }
            catch (const std::exception& e) {
                printf("Error during classification: %s\n", e.what());
            }
        }
    }
    catch (const std::exception& e) {
        printf("Initialization error: %s\n", e.what());
    }
    catch (...) {
        printf("An unknown error occurred during initialization.\n");
    }

    return 0;
}
