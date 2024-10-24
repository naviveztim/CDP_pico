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


//const string CLASSIFIERS_FOLDER = ".\\_classifiers\\";
//const string CLASSIFIERS_PATH = CLASSIFIERS_FOLDER + "//" + "Classifiers.bin"; 

class Cdp {

public:
    API_EXPORT Cdp();
    //API_EXPORT void Fit(DataSet& trainDataSet);
    //API_EXPORT vector<int> ClassifyDataset(DataSet& testDataSet);
    API_EXPORT int ClassifyTimeSeries(TimeSeries& timeSeries);
    //API_EXPORT void SaveClassifiers();
    API_EXPORT void LoadClassifiers(); 

private: 
    int _patternLength;
    vector<shared_ptr<ShapeletClassifier>> _decisionTrees;
    vector<tuple<int, string>> _decisionPatterns;

private: 
    //shared_ptr<ShapeletClassifier> _trainDecisionTree(vector<int>& classesInDataSet, DataSet& dataSet);
    //void _trainDecisionTreeS(DataSet& dataSet);
    //void _prepareDecisionPatterns(DataSet& trainDataSet);
    vector<int> _getMostPopularIndexesFromDecisionPattern(const string& decisionPattern); 
    
};

#endif
