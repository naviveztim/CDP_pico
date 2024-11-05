#include <numeric>
#include <string>
#include <vector>
#include <stdexcept>
#include <tuple>
#include <cstring>  
#include "../headers/Utils.h"
#include "../headers/Shapelet.h"
#include "../headers/ShapeletClassifier.h"

double Utils::EuclideanDistance(const std::vector<double>& array1
								, const std::vector<double>& array2
								, double currentMinDistance) 
{
	if (array1.empty() || array2.empty()) {
		throw std::invalid_argument("Euclidean.NullArray!");
	}

	if (array1.size() != array2.size()) {
		throw std::invalid_argument("Euclidean.IncorrectArrayLength");
	}

	double euclidean = 0.0;

	for (uint32_t i = 0; i < array1.size(); i++) {
		euclidean += (array1[i] - array2[i]) * (array1[i] - array2[i]);
		if (euclidean > currentMinDistance) {
			break;
		}
	}

	return euclidean;
}

double Utils::SimilarityCoefficient(const string& s1, const std::string& s2) {
	
	double result = 0.0;
	const char* array1 = s1.c_str();
	const char* array2 = s2.c_str();

	if (s1.empty() || s2.empty()) {
		throw std::invalid_argument("input string");
	}

	uint32_t length = s1.length();

	if (length != s2.length()) {
		throw std::invalid_argument("input string");
	}

	for (uint32_t i = 0; i < length; i++) {
		if (array1[i] == array2[i]) {
			result += 1.0;
		}
	}

	result /= length;

	return result;
}

double Utils::SubsequenceDist(const TimeSeries& timeSeries
								, const std::vector<double>& candidateValues) 
{
	uint32_t timeSeriesLen = timeSeries.Values.size();
	uint32_t candidateLen = candidateValues.size();

	double minDistance = std::numeric_limits<double>::max();
	std::vector<double> currentArray(candidateLen);

	for (uint32_t k = 0; k < timeSeriesLen - candidateLen + 1; k++) {
		
		std::copy(timeSeries.Values.begin() + k, timeSeries.Values.begin() + k + candidateLen, currentArray.begin());
		
		//Calculate the distance and update minDistance
		minDistance = std::min(minDistance, EuclideanDistance(candidateValues, currentArray, minDistance));
	}

	return minDistance;
}


double Utils::AverageValue(const std::vector<double>& array, int startIndex, int numElements) {
	if (array.size() < static_cast<uint32_t>(startIndex + numElements)) {
		throw std::invalid_argument("averageValueArguments");
	}

	double sumValue = std::accumulate(array.begin() + startIndex, array.begin() + startIndex + numElements, 0.0);
	return sumValue / numElements;
}

double Utils::StdDev(const std::vector<double>& values)
{
	double ret = 0;
	if (!values.empty()) {
		double avg = std::accumulate(values.begin(), values.end(), 0.0) / values.size();
		double sum = std::accumulate(values.begin(), values.end(), 0.0, [avg](double acc, double val) {
			return acc + std::pow(val - avg, 2);
		});
		ret = std::sqrt(sum / (values.size() - 1));
	}
	return ret;
}

void Utils::Derivative(std::vector<double>& signal) {
	if (signal.size() < 2) {
		return;
	}
	std::adjacent_difference(signal.begin(), signal.end(), signal.begin());
	signal[0] = 0;
}

void Utils::Normalize(std::vector<double>& signal) {
	if (signal.empty()) {
		return;
	}
	double avg = std::accumulate(signal.begin(), signal.end(), 0.0) / signal.size();
	double std = StdDev(signal);
	if (std == 0.0) {
		return; 
	}
	std::transform(signal.begin(), signal.end(), signal.begin(), [avg, std](double val) {
		return (val - avg) / std;
		});
}
void Utils::CompressATimeSeries(TimeSeries& timeSeries
								, int compressionIndex) 
{
	uint32_t newTimeSeriesLength = timeSeries.Values.size() / compressionIndex;
	std::vector<double> newValues(newTimeSeriesLength);
	uint32_t startIndex = 0;
	for (uint32_t i = 0; i < newTimeSeriesLength; i++) {
		newValues[i] = AverageValue(timeSeries.Values, (int)startIndex, compressionIndex);
		startIndex += compressionIndex;
	}
	timeSeries.Values = newValues;
}

std::vector<std::tuple<int, std::string>> Utils::LoadDecisionPattern(std::stringstream& ifs) {
	
	// Read the size of the entire vector 
	uint32_t vector_size=0;
	ifs.read(reinterpret_cast<char*>(&vector_size), sizeof(vector_size));

	std::vector<std::tuple<int, std::string>> data;
	data.reserve(vector_size);

	for (uint32_t i = 0; i < vector_size; ++i) {
		
		// Read the class index 
		int int_value=-1;
		ifs.read(reinterpret_cast<char*>(&int_value), sizeof(int_value));

		// Read the decision pattern 
		uint32_t string_size=0;
		ifs.read(reinterpret_cast<char*>(&string_size), sizeof(string_size));

		// Push the pair to the decision pattern container 
		std::string str_value(string_size, '\0');
		ifs.read(&str_value[0], string_size);

		data.emplace_back(int_value, str_value);
	}

	return data; 
}

Shapelet Utils::Deserialize(std::stringstream& ifs) {
	
	Shapelet deserializedObject;
	
	// Deserialize primitive data
	ifs.read(reinterpret_cast<char*>(&deserializedObject.OptimalSplitDistance), sizeof(deserializedObject.OptimalSplitDistance));
	ifs.read(reinterpret_cast<char*>(&deserializedObject.BestInformationGain), sizeof(deserializedObject.BestInformationGain));
	ifs.read(reinterpret_cast<char*>(&deserializedObject.LeftClassIndex), sizeof(deserializedObject.LeftClassIndex));
	ifs.read(reinterpret_cast<char*>(&deserializedObject.RightClassIndex), sizeof(deserializedObject.RightClassIndex));

	// Deserialize vector 
	uint32_t vectorSize;
	ifs.read(reinterpret_cast<char*>(&vectorSize), sizeof(vectorSize));
	deserializedObject.ShapeletsValues.resize(vectorSize);
	if (vectorSize > 0) {
		ifs.read(reinterpret_cast<char*>(deserializedObject.ShapeletsValues.data()), vectorSize * sizeof(double));
	}

	return deserializedObject;
}

