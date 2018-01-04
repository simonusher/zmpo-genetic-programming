#pragma once
#include <ctime>
#include "Tree.h"
class GeneticAlgorithm {
public:
	const std::vector<std::string> varNames = { "x", "y" };
	//distrib
	const int DISTRIB_OPERATION = 40;
	const int DISTRIB_CONSTANT = 70;
	const int DISTRIB_VARIABLE = 100;
	const int DISTRIB_SUM = DISTRIB_VARIABLE;
	const int DEFAULT_SIZE_OF_POPULATION = 100;
	const int DEFAULT_NUMBER_OF_ITERATIONS = 100000;
	const double DEFAULT_HYBRIDIZATION_PROBABILITY = 0.3;
	const double DEFAULT_MUTATION_PROBABILITY = 0.3;
	GeneticAlgorithm();
	GeneticAlgorithm(int sizeOfPopulation, int numberOfiterations, double hybridizationProbability, double mutationProbability);
	~GeneticAlgorithm();
	void initializePopulation();
	void run();
	void printPopulation();
	
	
//private:
	void select();
	void hybridize();
	void mutate();
	void evaluate();
	Tree *randomizeTree();
	Node *randomizeNode(Tree *parentTree);
	int randomType();
	int randomOperation();
	int randomConstant();
	std::string randomVariable();
	int sizeOfPopulation;
	int numberOfIterations;
	double hybridizationProbability;
	double mutationProbability;
	std::vector<Tree*> population;
};