#ifndef SHAPELET_H
#define SHAPELET_H

#include <vector>

using namespace std; 

class Shapelet {
public:
	vector<double> ShapeletsValues;
	double OptimalSplitDistance;
	double BestInformationGain;
	int LeftClassIndex;
	int RightClassIndex;

	Shapelet() {
		OptimalSplitDistance = -1.0;
		BestInformationGain = 0.0;
		LeftClassIndex = -1; 
		RightClassIndex = -1;
	}
	
};

#endif 