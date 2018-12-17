#include <iostream>
#include <string>

using namespace std;

class Stack {
	public:
		Stack *next;
		Stack *head;
		int value;
		char sign;
		bool isNum;
		int size;

		Stack() {
			isNum = false;
			next = NULL;
			head = NULL;
		}

		void push(int value, char sign, bool isNum) {
			Stack *newElement = new Stack();
			if (isNum) {
				newElement->value = value;
				newElement->isNum = true;
				newElement->sign = NULL;
			}
			else {
				newElement->sign = sign;
				newElement->isNum = false;
				newElement->value = NULL;
			}
			if (head == NULL)
				head = newElement;
			else {
				newElement->next = head;
				head = newElement;
			}
		}
		
		void print() {
			Stack *temp = head;
			while (temp != NULL) {
				if (temp->isNum)
					cout << temp->value << ends;
				else cout << temp->sign << ends;
				temp = temp->next;
			}
		}

		void setSize() {
			Stack *temp = this->head;
			int length = 0;

			while (this->head != NULL) {
				length++;
				this->head = this->head->next;
			}
			this->head = temp;
			this->size = length;
		}
		
		~Stack() {
			while (head != NULL) {
				Stack *temp = head;
				head = head->next;
				delete temp;
			}
		}
};

void DivInto2Stacks(string line, Stack *mainStack, Stack *charStack) {
	for (int i = 0; i < line.size(); i++) {
		if ('0' <= line[i] && line[i] <= '9')
			mainStack->push(line[i] - '0', line[i], true);
		else {
			if (charStack->head == NULL)
				charStack->push(line[i], line[i], false);
			else {
				if (line[i] == '+') {
					if (charStack->head != NULL && charStack->head->sign == '+')
						mainStack->push(NULL, line[i], false);
					else if (charStack->head != NULL && charStack->head->next == NULL || charStack->head->sign != '(') {
						mainStack->push(NULL, charStack->head->sign, false);
						charStack->head->sign = line[i];
					}
					else if (charStack->head != NULL) {
						while (charStack->head != NULL && charStack->head->sign != '(') {
							Stack *cSHead = charStack->head;
							mainStack->push(NULL, cSHead->sign, false);
							charStack->head = cSHead->next;
							delete cSHead;
						}
						charStack->push(NULL, line[i], false);
					}
				}
				else if (line[i] == '-') {
					if (charStack->head != NULL && charStack->head->sign == '+' || charStack->head->sign == '(')
						charStack->push(NULL, line[i], false);
					else if (charStack->head != NULL && charStack->head->sign != '*') {
						while (charStack->head != NULL && (charStack->head->sign != '(' || charStack->head->sign != '+')) {
							Stack *cSHead = charStack->head;
							mainStack->push(NULL, cSHead->sign, false);
							charStack->head = cSHead->next;
							delete cSHead;
						}
						charStack->push(NULL, line[i], false);
					}
					else {
						mainStack->push(NULL, charStack->head->sign, false);
						charStack->head->sign = line[i];
					}
				}
				else if (line[i] == '*') {
					if (charStack->head != NULL && charStack->head->sign == '*')
						mainStack->push(NULL, line[i], false);
					else charStack->push(NULL, line[i], false);
				}
				else {
					if (line[i] == '(')
						charStack->push(NULL, line[i], false);
					else {
						if (charStack->head != NULL) {
							while (charStack->head->sign != '(') {
								Stack *cSHead = charStack->head;
								mainStack->push(NULL, cSHead->sign, false);
								charStack->head = cSHead->next;
								delete cSHead;
							}
							Stack *cSHead = charStack->head;
							charStack->head = cSHead->next;
							delete cSHead;
						}
					}
				}
			}
		}
	}
}

Stack *Unite(Stack *mainStack, Stack *charStack) {
	Stack *result = new Stack();

	for (int i = 0; i < charStack->size; i++) {
		mainStack->push(NULL, charStack->head->sign, false);
		charStack->head = charStack->head->next;
	}
	mainStack->setSize();

	while (mainStack->head != NULL) {
		if (mainStack->head->isNum)
			result->push(mainStack->head->value, NULL, true);
		else result->push(NULL, mainStack->head->sign, false);
		mainStack->head = mainStack->head->next;
	}
	delete charStack;
	delete mainStack;

	return result;
}

int Calculate(Stack *mainStack) {
	Stack *mSHead = mainStack->head;
	while (mainStack->head->next != NULL) {
		if (mainStack->head->next->next != NULL) {
			if (mainStack->head->next->next->sign == '+') {
				mainStack->head->value += mainStack->head->next->value;
				if (mainStack->head->next->next->next != NULL)
					mainStack->head->next = mainStack->head->next->next->next;
				else mainStack->head->next = NULL;
				mainStack->head = mSHead;
			}
			else if (mainStack->head->next->next->sign == '-') {
				mainStack->head->value -= mainStack->head->next->value;
				if (mainStack->head->next->next->next != NULL)
					mainStack->head->next = mainStack->head->next->next->next;
				else mainStack->head->next = NULL;
				mainStack->head = mSHead;
			}
			else if (mainStack->head->next->next->sign == '*') {
				mainStack->head->value *= mainStack->head->next->value;
				if (mainStack->head->next->next->next != NULL)
					mainStack->head->next = mainStack->head->next->next->next;
				else mainStack->head->next = NULL;
				mainStack->head = mSHead;
			}
			else mainStack->head = mainStack->head->next;
		}
	}
	mainStack->head = mSHead;
	
	return mSHead->value;
}

int main() {
	Stack *mainStack = new Stack();
	Stack *charStack = new Stack();
	string line;

	cin >> line;
	
	DivInto2Stacks(line, mainStack, charStack);
	
	charStack->setSize();

	Stack *unRes = Unite(mainStack, charStack);

	unRes->setSize();

	cout << endl << "Result: " << Calculate(unRes) << endl;

	delete unRes;

	return 0;
}