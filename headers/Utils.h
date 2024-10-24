#ifndef UTILS_H
#define UTILS_H

#include <cmath>
#include <algorithm>
#include <vector>
#include <sstream>
#include <iostream>
#include "Shapelet.h"

using namespace std; 

class TimeSeries {
public:
	int ClassIndex;
	std::vector<double> Values;
};

class Utils {
public:
	static double EuclideanDistance(const vector<double>& array1
									, const vector<double>& array2
									, double currentMinDistance);
	static double SubsequenceDist(const TimeSeries& timeSeries, const std::vector<double>& candidateValues);
	static double AverageValue(const vector<double>& array
							   , int startIndex
							   , int numElements);
	static double StdDev(const vector<double>& values);
	static void Derivative(vector<double>& signal);
	static void Normalize(vector<double>& signal);
	static void CompressATimeSeries(TimeSeries& timeSeries, int compressionIndex);
	static double SimilarityCoefficient(const string& s1, const std::string& s2); 
	static std::vector<std::tuple<int, std::string>> LoadDecisionPattern(std::stringstream& ifs);
	static Shapelet Deserialize(std::stringstream& ifs);
};

#endif 