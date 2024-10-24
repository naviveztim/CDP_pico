#pragma once

#ifndef CDP_H
#define CDP_H

#if defined(_WIN32) || defined(_WIN64)
#ifdef BUILDING_DLL
#define API_EXPORT __declspec(dllexport)
#else
#define API_EXPORT __declspec(dllimport)
#endif
#else
#ifdef BUILDING_DLL
#define API_EXPORT __attribute__((visibility("default")))
#else
#define API_EXPORT
#endif
#endif

#include <algorithm>
#include <string>
#include <memory>
#include "ShapeletClassifier.h"

class Cdp {

public:
    API_EXPORT Cdp();
    API_EXPORT int ClassifyTimeSeries(TimeSeries& timeSeries);
    API_EXPORT void LoadClassifiers(); 

private: 
    int _patternLength;
    vector<shared_ptr<ShapeletClassifier>> _decisionTrees;
    vector<tuple<int, string>> _decisionPatterns;

private: 
    vector<int> _getMostPopularIndexesFromDecisionPattern(const string& decisionPattern); 
    
};

#endif
