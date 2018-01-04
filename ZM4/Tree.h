#pragma once
#include "Node.h"

const std::string MSG_TREE_EMPTY = "The tree is empty.";
const std::string MSG_NO_VARIABLES = "The tree contains no variables.";
const std::string MSG_VARIABLES = "Variables: ";

const enum ERROR_CODES {
	ERROR_TREE_NOT_CREATED,
	ERROR_INCORRECT_NUMBER_OF_VARIABLES,
	ERROR_WHILE_PARSING_FORMULA,
	SUCCESS
};
class Tree {
public:
	friend class Node;
	friend class TreeManager;
	friend class GeneticAlgorithm;
	Tree();
	Tree(Tree &otherTree);
	~Tree();
	void operator=(Tree &otherTree);
	Tree operator+(Tree &otherTree);
	Tree randomizeTree();
	int parseExpression(std::string &expression);
	std::string printNodes();
	std::string printONPFormula();
	void clear();
	std::string vars();
	int computeValue(std::vector<int> *environment, double &result);
	int numberOfConstants();
private:
	void attachOtherTree(Tree &otherTree);
	int valueOfVariable(std::string &variableName);
	int indexOfVariable(std::string &variableName);
	std::vector<int> *environment;
	void addVariable(std::string);
	void updateVariables();
	std::vector<std::string> variables;
	int errorCode;
	Node *root;
	double fitness;
};