#include "GeneticAlgorithm.h"
#include <iostream>

GeneticAlgorithm::GeneticAlgorithm() {
	this->sizeOfPopulation = DEFAULT_SIZE_OF_POPULATION;
	this->numberOfIterations = DEFAULT_NUMBER_OF_ITERATIONS;
	this->hybridizationProbability = DEFAULT_HYBRIDIZATION_PROBABILITY;
	this->mutationProbability = DEFAULT_MUTATION_PROBABILITY;
}

GeneticAlgorithm::GeneticAlgorithm(int sizeOfPopulation, int numberOfIterations, double hybridizationProbability, double mutationProbability) {
	this->sizeOfPopulation = sizeOfPopulation;
	this->numberOfIterations = numberOfIterations;
	this->hybridizationProbability = hybridizationProbability;
	this->mutationProbability = mutationProbability;
}

GeneticAlgorithm::~GeneticAlgorithm() {
	for (int i = 0; i < population.size(); i++) {
		delete population.at(i);
	}
	std::cout << "destroy";
}

void GeneticAlgorithm::initializePopulation() {
	for (int i = 0; i < sizeOfPopulation; i++) {
		this->population.push_back(randomizeTree());
	}
}

void GeneticAlgorithm::printPopulation() {
	for (int i = 0; i < population.size(); i++) {
		std::cout << population.at(i)->printNodes() << std::endl;
	}
}

Tree *GeneticAlgorithm::randomizeTree()
{
	Tree *randomTree = new Tree();
	randomTree->root = randomizeNode(randomTree);
	return randomTree;
}
Node* GeneticAlgorithm::randomizeNode(Tree *parentTree) {
	Node *randomNode = new Node(parentTree);
	int type = randomType();

	if (type == TypeGeneric::CONSTANT) {
		randomNode->nodeType = NodeType::Constant;
		randomNode->constantValue = randomConstant();
	}
	else if (type == TypeGeneric::VARIABLE) {
		randomNode->nodeType = NodeType::Variable;
		randomNode->variableName = randomVariable();
		parentTree->addVariable(randomNode->variableName);
	}
	else {
		randomNode->nodeType = randomOperation();
		switch (randomNode->nodeType) {
		case OperationSin:
		case OperationCos:
			randomNode->children.push_back(randomizeNode(parentTree));
			break;
		default:
			randomNode->children.push_back(randomizeNode(parentTree));
			randomNode->children.push_back(randomizeNode(parentTree));
		}

	}
	return randomNode;
}

int GeneticAlgorithm::randomType() {
	int typeDeterminer = std::rand() % DISTRIB_SUM;
	if (typeDeterminer <= DISTRIB_OPERATION) {
		return TypeGeneric::OPERATION;
	}
	else if (typeDeterminer <= DISTRIB_CONSTANT) {
		return TypeGeneric::CONSTANT;
	}
	else {
		return TypeGeneric::VARIABLE;
	}
}

int GeneticAlgorithm::randomOperation() {
	return std::rand() % NUMBER_OF_OPERATIONS;
}

int GeneticAlgorithm::randomConstant() {
	return std::rand();
}

std::string GeneticAlgorithm::randomVariable() {
	int index = std::rand() % varNames.size();
	return varNames.at(index);
}