#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

using namespace std;

// Define Order structure
struct Order {
    int orderId;
    int traderId;
    int stockId;
    int quantity;
    string type; // "BUY" or "SELL"
};

// Define Trader structure
class Trader {
private:
    int traderId;
    int balance;
    string username;
    string password;
    vector<Order> orders;

public:
    vector<Order>& getOrders() {
        return orders;
    }
    const vector<Order>& getOrders() const {
        return orders;
    }

public:
    // Constructor
    Trader(int id, int bal, const string& uname, const string& pass) : traderId(id), balance(bal), username(uname), password(pass) {}

    // Getter for trader ID
    int getId() const {
        return traderId;
    }

    // Getter for balance
    int getBalance() const {
        return balance;
    }

    // Getter for username
    const string& getUsername() const {
        return username;
    }

    // Getter for password
    const string& getPassword() const {
        return password;
    }

    // Create a new order
    void createOrder(const Order& order) {
        orders.push_back(order);
    }

    // Withdraw amount from balance
    void withdraw(int amount) {
        balance -= amount;
    }

    // Deposit amount to balance
    void deposit(int amount) {
        balance += amount;
    }
};

// Define Stock structure
class Stock {
private:
    int stockId;
    string name;
    double price;
    int availableQuantity;

public:
    // Default constructor
    Stock() : stockId(0), name(""), price(0.0), availableQuantity(0) {}

    // Constructor
    Stock(int id, const string& n, double p, int qty) : stockId(id), name(n), price(p), availableQuantity(qty) {}

    // Getter for stock ID
    int getStockId() const {
        return stockId;
    }

    // Getter for name
    const string& getName() const {
        return name;
    }

    // Getter for price
    double getPrice() const {
        return price;
    }

    // Getter for available quantity
    int getAvailableQuantity() const {
        return availableQuantity;
    }

    // Function to decrease quantity
    void decreaseQuantity(int quantity) {
        availableQuantity -= quantity;
    }

    // Function to increase quantity
    void increaseQuantity(int quantity) {
        availableQuantity += quantity;
    }
};

// Trading Application class
class TradingApp {
private:
    vector<Trader> traders;
    unordered_map<int, Stock> stocks;
    int nextOrderId;

public:
    // Constructor
    TradingApp() : nextOrderId(1) {
        // Register admin on startup
        traders.push_back(Trader(0, 0, "admin", "admin123"));
    }

    // Admin login
    bool adminLogin(const string& username, const string& password) const {
        return (username == "admin" && password == "admin123");
    }

    // Trader login
    bool traderLogin(const string& username, const string& password) const {
        for (const auto& trader : traders) {
            if (trader.getUsername() == username && trader.getPassword() == password) {
                return true;
            }
        }
        return false;
    }

    // Register a new trader
    void registerTrader(int traderId, int balance, const string& username, const string& password) {
        traders.push_back(Trader(traderId, balance, username, password));
        cout << "Trader Registered Successfully.\n";
    }

    // Add a new stock
    void addStock(int stockId, const string& name, double price, int quantity) {
        if (stocks.find(stockId) == stocks.end()) {
            stocks[stockId] = Stock(stockId, name, price, quantity);
            cout << "Stock added successfully.\n";
        } else {
            cout << "Stock ID " << stockId << " already exists.\n";
        }
    }

    // Display stock information
    void displayStocks() const {
        cout << "Stocks Available:\n";
        cout << "Stock ID | Name | Price | Available Quantity\n";
        for (const auto& pair : stocks) {
            const Stock& stock = pair.second;
            cout << stock.getStockId() << " | " << stock.getName() << " | " << stock.getPrice() << " | " << stock.getAvailableQuantity() << "\n";
        }
    }

    // Create a new order
    void createOrder(int traderId, int stockId, int quantity, const string& type) {
        if (stocks.find(stockId) != stocks.end()) {
            Stock& stock = stocks[stockId];
            if (stock.getAvailableQuantity() >= quantity) {
                // Retrieve trader's balance using traderId
                int traderIndex = -1;
                for (size_t i = 0; i < traders.size(); ++i) {
                    if (traders[i].getId() == traderId) {
                        traderIndex = i;
                        break;
                    }
                }
                if (traderIndex != -1) {
                    double totalCost = quantity * stock.getPrice();
                    if (traders[traderIndex].getBalance() >= totalCost) {
                        traders[traderIndex].withdraw(totalCost);
                        stock.decreaseQuantity(quantity);
                        traders[traderIndex].createOrder({nextOrderId++, traderId, stockId, quantity, type});
                        cout << "Order executed successfully.\n";
                    } else {
                        cout << "Insufficient balance to execute order.\n";
                    }
                } else {
                    cout << "Trader ID " << traderId << " not found.\n";
                }
            } else {
                cout << "Insufficient quantity available for stock ID " << stockId << ".\n";
            }
        } else {
            cout << "Stock ID " << stockId << " not found.\n";
        }
    }

// Display trader's orders
void displayTraderOrders(const string& username) const {
    bool foundTrader = false;
    for (const auto& trader : traders) {
        if (trader.getUsername() == username) {
            const vector<Order>& orders = trader.getOrders();
            cout << "Trader " << username << "'s Orders:\n";
            cout << "Order ID | Stock ID | Quantity | Type\n";
            if (orders.empty()) {
                cout << "No orders placed.\n";
            } else {
                for (const auto& order : orders) {
                    cout << order.orderId << " | " << order.stockId << " | " << order.quantity << " | " << order.type << "\n";
                }
            }
            foundTrader = true;
            break;
        }
    }
    if (!foundTrader) {
        cout << "Trader with username " << username << " not found.\n";
    }
}


    // Display list of all registered traders
    void displayRegisteredTraders() const {
        cout << "Registered Traders:\n";
        for (const auto& trader : traders) {
            cout << "Trader ID: " << trader.getId() << ", Balance: " << trader.getBalance() << "\n";
        }
    }

    // Get trader ID from username
    int getTraderId(const string& username) const {
    for (size_t i = 0; i < traders.size(); ++i) {
        if (traders[i].getUsername() == username) {
            return traders[i].getId();
        }
    }
    return -1; // Return -1 if username is not found
}

    // Get trader balance by ID
    int getTraderBalance(int traderId) const {
        for (const auto& trader : traders) {
            if (trader.getId() == traderId) {
                return trader.getBalance();
            }
        }
        cout << "Trader ID " << traderId << " not found.\n";
        return -1; // Return -1 if trader ID is not found
    }
};


int main() {
    TradingApp app;

    int mainChoice;
    string username, password;
    int traderId; // Variable to store trader ID
    do {
        cout << "\nMenu:\n";
        cout << "1. Trader Login\n";
        cout << "2. Admin Login\n";
        cout << "3. Display Stocks\n";
        cout << "4. Register Trader\n";
        cout << "5. Exit\n";
        cout << "Enter your choice: ";
        cin >> mainChoice;

        switch (mainChoice) {
            case 1: {
                cout << "Enter Trader Username: ";
                cin >> username;
                cout << "Enter Trader Password: ";
                cin >> password;
                if (app.traderLogin(username, password)) {
    cout << "Trader Logged in successfully.\n";
    // Store trader ID after successful login
    traderId = app.getTraderId(username);
                    int traderChoice;
                    do {
                        cout << "\nTrader Menu:\n";
                        cout << "1. Display Stocks\n";
                        cout << "2. Create Order\n";
                        cout << "3. Display Orders\n";
                        cout << "4. Check Available Balance\n"; // New option
                        cout << "5. Logout\n";
                        cout << "Enter your choice: ";
                        cin >> traderChoice;

                        switch (traderChoice) {
                            case 1:
                                app.displayStocks();
                                break;
                            case 2: {
                                int stockId, quantity;
                                string type;
                                cout << "Enter Stock ID: ";
                                cin >> stockId;
                                cout << "Enter Quantity: ";
                                cin >> quantity;
                                cout << "Enter Order Type (BUY/SELL): ";
                                cin >> type;
                                app.createOrder(traderId, stockId, quantity, type);
                                break;
                            }
                            case 3:
    cout << "Enter Trader Username: ";
    cin >> username;
    app.displayTraderOrders(username); // Pass the username to the displayTraderOrders function
    break;
                            case 4:
                                cout << "Available Balance: " << app.getTraderBalance(traderId) << endl;
                                break;
                            case 5:
                                cout << "Logging out...\n";
                                break;
                            default:
                                cout << "Invalid choice. Please try again.\n";
                                break;
                        }
                    } while (traderChoice != 5);
                } else {
                    cout << "Invalid Username or Password.\n";
                }
                break;
            }
            case 2: {
                cout << "Enter Admin Username: ";
                cin >> username;
                cout << "Enter Admin Password: ";
                cin >> password;
                if (app.adminLogin(username, password)) {
                    cout << "Admin Logged in successfully.\n";
                    int choice;
                    do {
                        cout << "\nAdmin Menu:\n";
                        cout << "1. Add Stock\n";
                        cout << "2. Display Registered Traders\n";
                        cout << "3. Logout\n";
                        cout << "Enter your choice: ";
                        cin >> choice;

                        switch (choice) {
                            case 1: {
                                int stockId, quantity;
                                string name;
                                double price;
                                cout << "Enter Stock ID: ";
                                cin >> stockId;
                                cout << "Enter Stock Name: ";
                                cin >> name;
                                cout << "Enter Stock Price: ";
                                cin >> price;
                                cout << "Enter Available Quantity: ";
                                cin >> quantity;
                                app.addStock(stockId, name, price, quantity);
                                break;
                            }
                            case 2:
                                app.displayRegisteredTraders();
                                break;
                            case 3:
                                cout << "Logging out...\n";
                                break;
                            default:
                                cout << "Invalid choice. Please try again.\n";
                                break;
                        }
                    } while (choice != 3);
                } else {
                    cout << "Invalid Username or Password.\n";
                }
                break;
            }
            case 3:
                app.displayStocks();
                break;
            case 4: {
                int traderId, balance;
                cout << "Enter Trader ID: ";
                cin >> traderId;
                cout << "Enter Trader Balance: ";
                cin >> balance;
                cout << "Enter Trader Username: ";
                cin >> username;
                cout << "Enter Trader Password: ";
                cin >> password;
                app.registerTrader(traderId, balance, username, password);
                break;
            }
            case 5:
                cout << "Exiting...\n";
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
                break;
        }
    } while (mainChoice != 5);

    return 0;
}
