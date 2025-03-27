#include "../include/traffic-orchestrator.hpp"

/*
int getRandomNumber(int max){
	static std::random_device rd;	// Génére un seed aléatoire
	static std::mt19937 gen(rd());	// Générateur Mersenne Twister
	static std::uniform_int_distribution<int> dist(0, max);	// Distribution uniforme entre 0 et max
	return dist(gen);
}
*/

int	getRandomNumberMinMax(int min, int max) {
	static std::random_device rd;	// Générateur de seed
	static std::mt19937 gen(rd());	// Générateur Mersenne Twister
	std::uniform_int_distribution<int> distrib(min, max);
	return distrib(gen);
}

int	roundToHundred(int value) {
	if (value < 0)
		return -roundToHundred(-value);
	return (value + 50) / 100 * 100;	// Ajoute 50 avant de diviser pour arrondir
}
