#pragma once
#include <string>
#include <cstdlib>
#include <vector>
#include <cmath>
#include <cctype>
#include "Tree.h"
#include "constants.h"
#include "stringOps.h"
const enum NodeType {
	OperationSum,
	OperationSub,
	OperationMult,
	OperationDiv,
	OperationSin,
	OperationCos,
	Constant,
	Variable,
	Undefined
};
const enum TypeGeneric {
	OPERATION,
	CONSTANT,
	VARIABLE
};
const int NUMBER_OF_OPERATIONS = 6;
const int NUMBER_OF_TYPES = 8;

const std::string OPERATION_SUM = "+";
const std::string OPERATION_SUB = "-";
const std::string OPERATION_MULT = "*";
const std::string OPERATION_DIV = "/";
const std::string OPERATION_SIN = "sin";
const std::string OPERATION_COS = "cos";

const int DEFAULT_CONSTANT = 1;

class Node {
private:
	friend class Tree;
	friend class GeneticAlgorithm;
	Node(Tree* parentTree);
	Node(Node &otherNode, Tree* parentTree);
	~Node();
	int parseString(std::string &stringToParse, int startIndex);
	std::string printNodes();
	std::string printONPFormula();
	void attachAtLeaf(Node &otherNode);
	void updateVariables();
	double computeValue();
	int nodeType;
	Tree *parentTree;
	std::vector<Node*> children;
	std::string variableName;
	int constantValue;
	bool removeForbiddenChars(std::string &expression);
	int numberOfConstants();
};