#include "../headers/Utils.h"
#include "../headers/ShapeletClassifier.h"

string ShapeletClassifier::GetDecisionPath(TimeSeries& timeSeries) {

	// Fills path generated from shapelet classifier-> Could be 'L' or 'R'

	std::string pathString;
	double distance = Utils::SubsequenceDist(timeSeries, shapelet.ShapeletsValues);

	// Left leave
	if (distance <= shapelet.OptimalSplitDistance) {
		if (shapelet.LeftClassIndex != -1)
			pathString.append("L");
		else
			pathString.append("0");
	}
	// Right leave 
	else {
		if (shapelet.RightClassIndex != -1)
			pathString.append("R");
		else
			pathString.append("0");
	}

	return pathString;
}

