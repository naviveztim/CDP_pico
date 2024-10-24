#ifndef SHAPELETCLASSIFIER_H
#define SHAPELETCLASSIFIER_H

#include <vector>
#include <unordered_map>
#include "Utils.h" 
#include "Shapelet.h"

class ShapeletClassifier {

public:
	Shapelet shapelet; 
	
public:
	ShapeletClassifier() {}
	std::string GetDecisionPath(TimeSeries& timeSeries);
};


#endif 
