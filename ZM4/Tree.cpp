#include "Tree.h"

Tree::Tree() {
	this->errorCode = SUCCESS;
	this->root = nullptr;
}

Tree::~Tree() {
	if (this->root != nullptr) {
		delete this->root;
	}
}

Tree::Tree(Tree &otherTree) {
	this->errorCode = SUCCESS;
	if (otherTree.root != nullptr) {
		this->root = new Node(*otherTree.root, this);
		this->variables = otherTree.variables;
	}
	else {
		this->root = nullptr;
	}
}

void Tree::operator=(Tree &otherTree) {
	this->clear();
	if (otherTree.root != nullptr) {
		this->root = new Node(*otherTree.root, this);
		this->variables = otherTree.variables;
	}
	else {
		this->root = nullptr;
	}
}

Tree Tree::operator+(Tree &otherTree) {
	Tree newTree(*this);
	newTree.attachOtherTree(otherTree);
	newTree.updateVariables();
	return newTree;
}

void Tree::clear() {
	if (this->root != nullptr) {
		delete this->root;
	}
	this->root = nullptr;
	this->variables.clear();
	this->errorCode = SUCCESS;
}

int Tree::parseExpression(std::string &expression) {
	this->clear();
	this->root = new Node(this);
	if (expression == EMPTY_STRING){
		this->errorCode = ERROR_WHILE_PARSING_FORMULA;
	}
	int lastParsedIndex = root->parseString(expression, 0);
	if (lastParsedIndex < expression.size()) {
		this->errorCode = ERROR_WHILE_PARSING_FORMULA;
	}
	int currentErrorCode = this->errorCode;
	this->errorCode = SUCCESS;
	return currentErrorCode;
}

std::string Tree::printNodes() {
	if (this->root == nullptr) {
		return MSG_TREE_EMPTY;
	}
	else {
		return this->root->printNodes();
	}
}
std::string Tree::printONPFormula() {
	if (this->root == nullptr) {
		return MSG_TREE_EMPTY;
	}
	else {
		return this->root->printONPFormula();
	}
}
void Tree::addVariable(std::string variable) {
	if (std::find(variables.begin(), variables.end(), variable) == variables.end()) {
		variables.push_back(variable);
	}
}
void Tree::updateVariables() {
	if (this->root != nullptr) {
		this->variables.clear();
		this->root->updateVariables();
	}
}
std::string Tree::vars() {
	std::string result;
	if (this->root == nullptr) {
		return MSG_TREE_EMPTY;
	}
	else if (this->variables.size() == 0) {
		return MSG_NO_VARIABLES;
	}
	else {
		int numberOfVariables = variables.size();
		std::string result = MSG_VARIABLES;
		for (auto iterator = variables.begin(); iterator != variables.end(); ++iterator) {
			result.append(*iterator);
			if (iterator + 1 != variables.end()) {
				result.append(COMMA_STRING);
				result.append(SPACE_STRING);
			}
		}
		return result;
	}
}

int Tree::computeValue(std::vector<int> *environment, double &result) {
	if (environment->size() != this->variables.size()) {
		return ERROR_INCORRECT_NUMBER_OF_VARIABLES;
	}
	if (this->root == nullptr) {
		return ERROR_TREE_NOT_CREATED;
	}
	else {
		this->environment = environment;
		result = this->root->computeValue();
		return SUCCESS;
	}
	
}

int Tree::numberOfConstants()
{
	if (this->root != nullptr) {
		return this->root->numberOfConstants();
	}
	return 0;
}

int Tree::indexOfVariable(std::string &variableName) {
	bool found = false;
	int index = 0;
	for (auto iterator = variables.begin(); iterator != variables.end() && !found;) {
		if (*iterator == variableName) {
			found = true;
		}
		else {
			++iterator;
			++index;
		}
	}
	return index;
}

void Tree::attachOtherTree(Tree &otherTree) {
	if (this->root == nullptr) {
		if (otherTree.root != nullptr) {
			this->root = new Node(*otherTree.root, this);
			updateVariables();
		}
	}
	else {
		if (otherTree.root != nullptr) {
			if (this->root->nodeType == Variable || this->root->nodeType == Variable) {
				delete this->root;
				this->root = new Node(*otherTree.root, this);
				updateVariables();
			}
			else {
				this->root->attachAtLeaf(*otherTree.root);
				updateVariables();
			}
		}
	}
}

int Tree::valueOfVariable(std::string &variableName) {
	return this->environment->at(indexOfVariable(variableName));
}
