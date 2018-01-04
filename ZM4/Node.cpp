#include "Node.h"
Node::Node(Tree *parentTree) {
	this->nodeType = NodeType::Undefined;
	this->parentTree = parentTree;
}

Node::Node(Node &otherNode, Tree *parentTree) {
	this->parentTree = parentTree;
	this->nodeType = otherNode.nodeType;
	if (this->nodeType == NodeType::Constant) {
		this->constantValue = otherNode.constantValue;
	}
	else if (this->nodeType == NodeType::Variable) {
		this->variableName = otherNode.variableName;
	}
	for (int i = 0; i < otherNode.children.size(); i++) {
		this->children.push_back(new Node(*(otherNode.children.at(i)), parentTree));
	}
}


Node::~Node() {
	for each (Node* childPtr in children) {
		delete childPtr;
	}
}

int Node::parseString(std::string &stringToParse, int startIndex) {

	int stringLength = stringToParse.length();
	//omitLeadingSpaces
	while (startIndex < stringLength && stringToParse.at(startIndex) == SPACE) {
		startIndex++;
	}

	std::string expression;
	
	if (startIndex >= stringLength) {
		this->nodeType = NodeType::Constant;
		this->constantValue = DEFAULT_CONSTANT;
		this->parentTree->errorCode = ERROR_WHILE_PARSING_FORMULA;
		return startIndex;
	}
	else {
		int currentIndex = startIndex;
		while (currentIndex < stringLength && stringToParse.at(currentIndex) != SPACE) {
			currentIndex++;
		}
		expression = stringToParse.substr(startIndex, currentIndex - startIndex);

		int numberOfChildren;

		if (expression == OPERATION_SUM) {
			numberOfChildren = 2;
			this->nodeType = NodeType::OperationSum;
		}
		else if (expression == OPERATION_SUB) {
			numberOfChildren = 2;
			this->nodeType = NodeType::OperationSub;
		}
		else if (expression == OPERATION_MULT) {
			numberOfChildren = 2;
			this->nodeType = NodeType::OperationMult;
		}
		else if (expression == OPERATION_DIV) {
			numberOfChildren = 2;
			this->nodeType = NodeType::OperationDiv;
		}
		else if (expression == OPERATION_SIN) {
			numberOfChildren = 1;
			this->nodeType = NodeType::OperationSin;
		}
		else if (expression == OPERATION_COS) {
			numberOfChildren = 1;
			this->nodeType = NodeType::OperationCos;
		}
		else if (stringOps::isNumber(expression)) {
			numberOfChildren = 0;
			this->nodeType = NodeType::Constant;
			this->constantValue = std::atoi(expression.c_str());
		}
		else {
			if (removeForbiddenChars(expression)) {
				this->parentTree->errorCode = ERROR_WHILE_PARSING_FORMULA;
			}
			if (expression.length() == 0) {
				return this->parseString(stringToParse, currentIndex); //TODO: ASK WHICH OPTION IS BETTER: REPLACING TRASH WITH 1 OR SKIPPING IT COMPLETELY
				//numberOfChildren = 0;
				//this->nodeType = NodeType::Constant;
				//this->constantValue = DEFAULT_CONSTANT;
			}
			else {
				numberOfChildren = 0;
				this->nodeType = NodeType::Variable;
				this->variableName = expression;
				this->parentTree->addVariable(expression);
			}
			
		}

		Node *child;
		switch (numberOfChildren) {
		case 0:
			return currentIndex;
			break;
		case 1:
			child = new Node(this->parentTree);
			children.push_back(child);
			return child->parseString(stringToParse, currentIndex);
			break;
		case 2:
			child = new Node(this->parentTree);
			Node *secondChild = new Node(this->parentTree);
			children.push_back(child);
			children.push_back(secondChild);
			return secondChild->parseString(stringToParse, child->parseString(stringToParse, currentIndex));
			break;
		}
	}
	
}

std::string Node::printNodes() {
	std::string result;
	switch (nodeType) {
	case Variable:
		result = variableName;
		break;
	case Constant:
		result = std::to_string(this->constantValue);
		break;
	case OperationCos:
		result = OPERATION_COS + SPACE + OPENING_BRACKET + children.at(0)->printNodes() + CLOSING_BRACKET;
		break;
	case OperationSin:
		result = OPERATION_SIN + SPACE + OPENING_BRACKET + children.at(0)->printNodes() + CLOSING_BRACKET;
		break;
	case OperationSum:
		result = OPENING_BRACKET + children.at(0)->printNodes() + SPACE + OPERATION_SUM + SPACE + children.at(1)->printNodes() + CLOSING_BRACKET;
		break;
	case OperationSub:
		result = OPENING_BRACKET + children.at(0)->printNodes() + SPACE + OPERATION_SUB + SPACE + children.at(1)->printNodes() + CLOSING_BRACKET;
		break;
	case OperationMult:
		result = OPENING_BRACKET + children.at(0)->printNodes() + SPACE + OPERATION_MULT + SPACE + children.at(1)->printNodes() + CLOSING_BRACKET;
		break;
	case OperationDiv:
		result = OPENING_BRACKET + children.at(0)->printNodes() + SPACE + OPERATION_DIV + SPACE + children.at(1)->printNodes() + CLOSING_BRACKET;
		break;
	}
	return result;
}

std::string Node::printONPFormula() {
	std::string result;
	switch (nodeType) {
	case Variable:
		result = variableName;
		break;
	case Constant:
		result = std::to_string(this->constantValue);
		break;
	case OperationCos:
		result = OPERATION_COS + SPACE + children.at(0)->printONPFormula();
		break;
	case OperationSin:
		result = OPERATION_SIN + SPACE + children.at(0)->printONPFormula();
		break;
	case OperationSum:
		result = OPERATION_SUM + SPACE + children.at(0)->printONPFormula() + SPACE + children.at(1)->printONPFormula();
		break;
	case OperationSub:
		result = OPERATION_SUB + SPACE + children.at(0)->printONPFormula() + SPACE + children.at(1)->printONPFormula();
		break;
	case OperationMult:
		result = OPERATION_MULT + SPACE + children.at(0)->printONPFormula() + SPACE + children.at(1)->printONPFormula();
		break;
	case OperationDiv:
		result = OPERATION_DIV + SPACE + children.at(0)->printONPFormula() + SPACE + children.at(1)->printONPFormula();
		break;
	}
	return result;
}


void Node::attachAtLeaf(Node &otherNode) {
	if (this == this->parentTree->root && this->children.size() == 0) {
		this->parentTree->root = new Node(otherNode, this->parentTree);
		delete this;
	}
	else {
		int childIndex = std::rand() % this->children.size();
		Node *child = this->children.at(childIndex);
		if (child->nodeType == Constant || child->nodeType == Variable) {
			delete child;
			this->children[childIndex] = new Node(otherNode, this->parentTree);
		}
		else {
			child->attachAtLeaf(otherNode);
		}
	}
}

void Node::updateVariables() {
	if (this->nodeType == NodeType::Variable) {
		this->parentTree->addVariable(this->variableName);
	}
	for each (Node* child in children) {
		child->updateVariables();
	}
}

double Node::computeValue() {
	switch (this->nodeType) {
	case OperationSum:
		return this->children.at(0)->computeValue() + this->children.at(1)->computeValue();
		break;
	case OperationSub:
		return this->children.at(0)->computeValue() - this->children.at(1)->computeValue();
		break;
	case OperationMult:
		return this->children.at(0)->computeValue() * this->children.at(1)->computeValue();
		break;
	case OperationDiv:
		return this->children.at(0)->computeValue() / this->children.at(1)->computeValue();
		break;
	case OperationSin:
		return std::sin(this->children.at(0)->computeValue());
		break;
	case OperationCos:
		return std::cos(this->children.at(0)->computeValue());
		break;
	case Constant:
		return this->constantValue;
		break;
	case Variable:
		return this->parentTree->valueOfVariable(this->variableName);
		break;
	}
	return 0;
}

bool Node::removeForbiddenChars(std::string &expression) {
	bool containsForbiddenChars = false;
	int currentPos = 0;
	int currentExpressionLength = expression.length();
	while (currentPos < currentExpressionLength) {
		if (!isalnum(expression.at(currentPos))) {
			expression.erase(currentPos, 1);
			currentExpressionLength = expression.length();
			containsForbiddenChars = true;
		}
		else {
			currentPos++;
		}
	}
	return containsForbiddenChars;
}

int Node::numberOfConstants()
{
	int result = 0;
	if (this->nodeType == Constant) {
		result++;
	}
	for (Node *child : children) {
		result += child->numberOfConstants();
	}
	return result;
}
