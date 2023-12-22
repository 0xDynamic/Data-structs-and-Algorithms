#include <iostream>
#include <queue>
#include <string>

using namespace std;

/*
Ideas:

- Initializes three items with their respective quantities and prices
- Then enqueues three customer orders with different items and quantities

- The main function processes the orders using a queue and displays whether the order was successful or not
-> Finally, it displays the updated stock information after processing the orders
*/

struct Item { // Structure to represent an item in stock
    string name;
    int quantity;
    double price;
};

struct Order { // Structure to represent a customer order
    string customerName;
    int itemsToBuy; // Number of items a customer can buy
    Item item;
};

Item initItem(string name, int quantity, double price) { // Function to initialize an item with given details
    Item newItem;
    newItem.name = name;
    newItem.quantity = quantity;
    newItem.price = price;
    return newItem;
}

Order initOrder(string customerName, int itemsToBuy, Item item) { // Function to initialize an order with given details
    Order newOrder;
    newOrder.customerName = customerName;
    newOrder.itemsToBuy = itemsToBuy;
    newOrder.item = item;
    return newOrder;
}

void displayItem(Item item) { // Function to display information about an item
    cout << "Item: " << item.name << "\tQuantity: " << item.quantity << "\tPrice: $" << item.price << endl;
}

void displayOrder(Order order) { // Function to display information about an order
    cout << "Customer: " << order.customerName << "\tItem: " << order.item.name << "\tItems to buy: " << order.itemsToBuy << endl;
}

int main() {
    // Initialize items
    Item item1 = initItem("Product1", 10, 29.99);
    Item item2 = initItem("Product2", 5, 49.99);
    Item item3 = initItem("Product3", 8, 19.99);

    // Initialize a queue of orders
    queue<Order> orderQueue;

    // Enqueue orders
    orderQueue.push(initOrder("Customer1", 2, item1));
    orderQueue.push(initOrder("Customer2", 1, item2));
    orderQueue.push(initOrder("Customer3", 3, item3));

    // Process orders using queue follows the FIFO (First In First Out) principle
    while (!orderQueue.empty()) {
        Order currentOrder = orderQueue.front();
        displayOrder(currentOrder);

        // Check if there is enough quantity in stock
        if (currentOrder.item.quantity >= currentOrder.itemsToBuy) {
            cout << "Order successful! Total cost: $" << currentOrder.itemsToBuy * currentOrder.item.price << endl;

            // Reduce the number of products in stock
            currentOrder.item.quantity -= currentOrder.itemsToBuy;
        } else {
            cout << "Warning: Insufficient quantity for " << currentOrder.item.name << ". Order canceled." << endl;
        }

        // Dequeue the processed order
        orderQueue.pop();
    }

    // Display updated stock information
    cout << "\nUpdated Stock Information:\n";
    displayItem(item1);
    displayItem(item2);
    displayItem(item3);

    return 0;
}
