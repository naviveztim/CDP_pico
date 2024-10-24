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

Cdp::Cdp() {}

void Cdp::LoadClassifiers() {

    // Access the start of the binary data and its size
    const uint8_t* modelData = &_binary__mnt_c_Users_PC_pico_cdp_c_plus_pico_models_Classifiers_bin_start;
    const size_t modelSize = &_binary__mnt_c_Users_PC_pico_cdp_c_plus_pico_models_Classifiers_bin_end - &_binary__mnt_c_Users_PC_pico_cdp_c_plus_pico_models_Classifiers_bin_start;

    // Create a stringstream object to simulate file reading from memory
    std::stringstream memoryStream(std::ios::in | std::ios::binary);
    memoryStream.write(reinterpret_cast<const char*>(modelData), modelSize);
    memoryStream.seekg(0, std::ios::beg);  

    // Read the number of classifiers in the list
    size_t listSize;
    memoryStream.read(reinterpret_cast<char*>(&listSize), sizeof(listSize));

    // Deserialize classifiers
    for (size_t i = 0; i < listSize; ++i) {
        auto classifier = std::make_shared<ShapeletClassifier>();
        if (classifier != nullptr) {
            classifier->shapelet = Utils::Deserialize(memoryStream);
            _decisionTrees.push_back(classifier);
        }
    }

    // Load decision patterns 
    _decisionPatterns = Utils::LoadDecisionPattern(memoryStream);

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

