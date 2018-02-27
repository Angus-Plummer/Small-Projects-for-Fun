// Program to investigate the effectiveness of a roullete strategy that is guaranteed* to work.
// *If you have an infinite pool of money to bet with.

#include "stdlib.h"
#include <iostream>
#include <time.h>
#include <vector>


//function to generate a random number between an upper and lower limit in divisions of range/n
double randomNumber(double upper, double lower, int n) {
	double r;
	r = lower + (rand() % (n + 1) * (1. / n) * (upper - lower));
	return r;
}

//spins the roulette wheel, returns 1 for a win (assuming bet on red each time)
bool spin(){
	int result = randomNumber(0, 36, 37);
	//0 is green so
	if (result == 0){
return 0;
	}
	//assumes red/black alternates with number so even = black
	if (result % 2 == 0){
		return 0;
	}
	//if odd then it was red so return 1 for win
	else{
		return 1;
	}
}
int targetWinnings = 10000;
int minBet = 50;
int numRuns = 10000;



int main() {
	srand(time(NULL));

	for (int minBet = 50; minBet < 500; minBet += 50){

		std::vector<int> maxLossesData;
		double averageMaxLoss = 0;
		int greatestMaxLoss = 0;

		for (int testNum = 0; testNum < numRuns; testNum++){

			int totalWinnings = 0;
			int currentLoss = 0;
			int maxLoss = 0;
			int totalSpins = 0;

			for (int numSpins = 0; totalWinnings < targetWinnings; numSpins++){
				//update maximum loss if current loss is greatest
				if (currentLoss > maxLoss){
					maxLoss = currentLoss;
				}
				//if not in debt
				if (currentLoss == 0){
					//bet minBet
					if (spin()){
						//if win spin then add to winnings
						totalWinnings += minBet;
					}
					else{
						//if lose then add to currentLoss to earn back
						currentLoss += minBet;
					}
				}
				//if in debt
				else{
					//bet the currentLoss
					if (spin()){
						//if win spin reset current loss
						currentLoss = 0;
					}
					else{
						//add to current loss
						currentLoss += currentLoss;
					}
				}
				totalSpins = numSpins;
			}
			maxLossesData.push_back(maxLoss);
			averageMaxLoss += maxLoss / numRuns;
			if (greatestMaxLoss < maxLoss){
				greatestMaxLoss = maxLoss;
			}
		}
		//vector to hold the bins for the max losses (like histogram but only possible powers of 2)
		std::vector<int> bins;
		//first bin is if there are no losses
		bins.push_back(0);
		//put minBet * 2^n size bins up to maximum loss
		for (int binsize = minBet; binsize <= greatestMaxLoss; binsize *= 2){
			bins.push_back(binsize);
		}

		//create vector to hold frequency of losses in relevent bins
		std::vector<int> binFreq;
		for (int length = 0; length < bins.size(); length++){
			binFreq.push_back(0);
		}

		//iterate over results and fill in binFreq correspondingly
		for (int i = 0; i < maxLossesData.size(); i++){
			//find which element in the bins this loss amount is and add 1 to corresponding element of frequency vector
			for (int j = 0; j < bins.size(); j++){
				if (maxLossesData[i] == bins[j]){
					binFreq[j] += 1;
				}
			}
		}
		std::cout << "minimum bet = " << minBet << "\n";
		std::cout << "greatest maximum loss = " << greatestMaxLoss << "\n";
		std::cout << "mean maximum loss = " << averageMaxLoss << " to win " << targetWinnings << "\n\n";

		//print out frequency distribution

		for (int j = 0; j < binFreq.size(); j++){
			for (int dots = 0; dots < (double)binFreq[j] / (double)numRuns * 100; dots++){
				std::cout << "-";
			}
			std::cout << " (" << bins[j] << " " << (double)binFreq[j] / (double)numRuns * 100 << " %) \n";
		}
	}
	//std::cout << "total spins = " << totalSpins << "\";

	int waitInt;

	std::cin >> waitInt;
}
