#include <iostream>
#include <string>

using namespace std;

/*
Ideas:

- According to excercise the queue follows FILO (First In Last Out) principle

- First defines a "Node" class for representing a website and a "Stack" class for managing the backward and forward navigation history
- Write functions:
	+ "navigateBack" and "navigateForward" functions simulate going back and forward in the web browser
*/

// Node class to represent a website
class Node {
public:
    string url;
    string title;
    Node* next;

    Node(string u, string t) : url(u), title(t), next(nullptr) {}
};

// Stack class for managing the navigation history
class Stack {
public:
    Node* top;

    Stack() : top(nullptr) {}

    bool isEmpty() {
        return top == nullptr;
    }

    void push(string url, string title) {
        Node* newNode = new Node(url, title);
        newNode->next = top;
        top = newNode;
    }

    Node* pop() {
        if (isEmpty()) {
            cout << "Stack is empty." << endl;
            return nullptr;
        }

        Node* poppedNode = top;
        top = top->next;
        return poppedNode;
    }

    void display() {
        Node* current = top;
        while (current != nullptr) {
            cout << "Title: " << current->title << ", URL: " << current->url << endl;
            current = current->next;
        }
    }
};

// Function to navigate backward
void navigateBack(Stack& stackBackward, Stack& stackForward) {
    if (stackBackward.isEmpty()) {
        cout << "Cannot go back. Backward stack is empty." << endl;
        return;
    }

    Node* currentPage = stackBackward.pop();
    stackForward.push(currentPage->url, currentPage->title);
    cout << "Going back to: " << currentPage->title << endl;
    delete currentPage; // Free the memory
}

// Function to navigate forward
void navigateForward(Stack& stackBackward, Stack& stackForward) {
    if (stackForward.isEmpty()) {
        cout << "Cannot go forward. Forward stack is empty." << endl;
        return;
    }

    Node* nextPage = stackForward.pop();
    stackBackward.push(nextPage->url, nextPage->title);
    cout << "Going forward to: " << nextPage->title << endl;
    delete nextPage; // Free the memory
}

int main() {
    Stack backwardStack;
    Stack forwardStack;

    // Navigation test push
    backwardStack.push("https://www.example1.com", "Example 1");
    backwardStack.push("https://www.example2.com", "Example 2");
    backwardStack.push("https://www.example3.com", "Example 3");

    cout << "Current Stack:" << endl;
    backwardStack.display();

    navigateBack(backwardStack, forwardStack);

    cout << "\nUpdated Stack after navigating back:" << endl;
    backwardStack.display();

    navigateForward(backwardStack, forwardStack);

    cout << "\nUpdated Stack after navigating forward:" << endl;
    backwardStack.display();

    return 0;
}