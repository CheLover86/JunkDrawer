#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <limits>
#include <cmath>

double calculateMean(const std::vector<double>& nums) {
	double mean;
	double total_sum = std::accumulate(nums.begin(), nums.end(), 0.0);

	if(nums.empty()) {
		std::cout << "Cannot divide by zero. \n";
		return 0.0;
	}
	else {
		mean = total_sum / nums.size();
		return mean;
	}
       	
}

double calculateMin(const std::vector<double>& nums) {
	if (nums.empty()) return 0.0;
	double minVal = *std::min_element(nums.begin(), nums.end());

	return minVal;
}

double calculateMax(const std::vector<double>& nums) {
	if (nums.empty()) return 0.0;
	double maxVal = *std::max_element(nums.begin(), nums.end());

	return maxVal;
}

double stdDev(const std::vector<double>& nums, bool isSample = true) {
	double mean = calculateMean(nums);

	double squaredSum = std::accumulate(nums.begin(), nums.end(), 0.0, [mean](double accum, double val) {
			return accum + std::pow(val - mean, 2);
			});
	
	double divisor = isSample ? (nums.size() - 1) : nums.size();
	if (divisor <= 0) return 0.0;
	
	return std::sqrt(squaredSum / divisor);	
}

int main() {
	std::vector<double> prices;
	double nums;
	int choice;

	std::cout << "Enter some numbers. Enter a character to quit: ";
	while (std::cin >> nums) {
		prices.push_back(nums);
	}

	std::cin.clear();
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	
	std::cout << "Options: 1: Mean, 2: Min, 3: Max, 4: Standard Deviation.  ";
	std::cin >> choice;


	switch(choice) {
		case 1:
			if (prices.empty()) {
				std::cout << "Vector is empty. \n";
			}
			else {
				double mean = calculateMean(prices);
				std::cout << mean << "\n";
			}
			break;
		case 2:
			if(prices.empty()) {
				std::cout << "Vector is empty. \n";
			}
			else {
				double min = calculateMin(prices);
				std::cout << min << "\n";
			}
			break;
		case 3:
			if(prices.empty()) {
				std::cout << "Vector is empty. \n";
			}
			else {
				double max = calculateMax(prices);
				std::cout << max << "\n"; 
			}
			break;
		case 4:
			if (prices.empty()) {
				std::cout << "Vector is empty. \n";
			}
			else {
				double sdev = stdDev(prices, true);
				std::cout << sdev << "\n";
			}
			break;
		default:
			std::cout << "Invalid option. \n";
			break;
	
	}
	
	return 0;
}
