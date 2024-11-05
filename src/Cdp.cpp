#include "../headers/ShapeletClassifier.h"
#include "../headers/Utils.h"
#include "../headers/Cdp.h"
#include <iostream>
#include <cstdint>

// These symbols are provided by the linker for the binary file.
// They are automatically generated and may very when different environment is used. 
// To double check the names of associated with Classifiers.o, type: 
// nm  Classifiers.o
extern "C" {
    extern const uint8_t _binary__mnt_c_Users_PC_pico_cdp_c_plus_pico_models_Classifiers_bin_start;
    extern const uint8_t _binary__mnt_c_Users_PC_pico_cdp_c_plus_pico_models_Classifiers_bin_end;
}

uint32_t Cdp::_convertToLittleEndian(uint32_t value) {
    return ((value >> 24) & 0xFF) |
        ((value >> 8) & 0xFF00) |
        ((value << 8) & 0xFF0000) |
        ((value << 24) & 0xFF000000);
}


Cdp::Cdp() {}

void Cdp::LoadClassifiers() {

    uint32_t listSize = 0;
    uint32_t lit_end_listSize = 0;

    printf("Enter Cdp::LoadClassifiers.\n");

    // Access the start of the binary data and its size
    const uint8_t* modelData = &_binary__mnt_c_Users_PC_pico_cdp_c_plus_pico_models_Classifiers_bin_start;
    const uint32_t modelSize = &_binary__mnt_c_Users_PC_pico_cdp_c_plus_pico_models_Classifiers_bin_end - &_binary__mnt_c_Users_PC_pico_cdp_c_plus_pico_models_Classifiers_bin_start;
    printf("modelSize: %d\n", modelSize);

    // Create a stringstream object to simulate file reading from memory
    std::string binaryString(reinterpret_cast<const char*>(modelData), modelSize);
    std::stringstream memoryStream(binaryString, std::ios::in | std::ios::binary);
    //memoryStream.write(reinterpret_cast<const char*>(modelData), modelSize);
    printf("memoryStream created successfully.\n");

    // Read the number of classifiers in the list
    // TODO: DOEs not read number of 
    memoryStream.seekg(0, std::ios::beg);
    memoryStream.read(reinterpret_cast<char*>(&listSize), sizeof(listSize));
    printf("listSize: %d\n", listSize); 

    lit_end_listSize = _convertToLittleEndian(listSize);
    printf("lit_end_listSize: %d\n", lit_end_listSize);

    // Deserialize classifiers
    for (uint32_t i = 0; i < listSize; ++i) {
        auto classifier = std::make_shared<ShapeletClassifier>();
        if (classifier != nullptr) {
            classifier->shapelet = Utils::Deserialize(memoryStream);
            _decisionTrees.push_back(classifier);
        }
    }
    printf("Classifiers seserialized."); 

    // Load decision patterns 
    _decisionPatterns = Utils::LoadDecisionPattern(memoryStream);

    printf("Exit Cdp::LoadClassifiers.\n");
}

int Cdp::ClassifyTimeSeries(TimeSeries& timeSeries) {

    // Collect decision pattern from all classifiers 
    std::string stringDecisionPattern;
    for (const auto& classifier : _decisionTrees) {
        stringDecisionPattern += classifier->GetDecisionPath(timeSeries);
    }

    // Compare found decision pattern with training decision patterns 
    auto mostPopularIndexes = _getMostPopularIndexesFromDecisionPattern(stringDecisionPattern);

    return mostPopularIndexes[0];
}


vector<int> Cdp::_getMostPopularIndexesFromDecisionPattern(const string& decisionPattern) {
    // Calculate similarity distances and find the maximum similarity level

    double maxSimilarityLevel = 0.0;
    vector<int> indexes;
    for (const auto& [trainClassIndex, trainPattern] : _decisionPatterns) {
        double similarity = Utils::SimilarityCoefficient(decisionPattern, trainPattern);
        if (similarity > maxSimilarityLevel) {
            maxSimilarityLevel = similarity;
            indexes.clear();
            indexes.push_back(trainClassIndex);
        }
        else if (similarity == maxSimilarityLevel) {
            indexes.push_back(trainClassIndex);
        }
    }

    // Get the most popular index among filtered results
    return indexes;
}

