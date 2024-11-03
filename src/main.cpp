#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include "httplib.h"

// Book class with enhanced features
class Book {
public:
    std::string title, author, genre;
    int edition, quantity;
    double price;

    Book(std::string t, std::string a, int e, std::string g, int q, double p)
        : title(t), author(a), edition(e), genre(g), quantity(q), price(p) {}

    std::string toStringResponse() {
        return "Title: " + title + 
               ", Author: " + author + 
               ", Edition: " + std::to_string(edition) + 
               ", Genre: " + genre + 
               ", Quantity: " + std::to_string(quantity) + 
               ", Price: $" + std::to_string(price);
    }
};

// Transaction class for order tracking
class Transaction {
public:
    std::string userId;
    std::string bookTitle;
    int quantity;
    double amount;
    std::string status;

    Transaction(std::string uid, std::string title, int qty, double amt)
        : userId(uid), bookTitle(title), quantity(qty), amount(amt), status("Pending") {}
};

// Enhanced User class
class User {
protected:
    std::string username;
    std::string password;
    std::string role;
    std::vector<Transaction> transactions;

public:
    User(std::string u, std::string p, std::string r) : username(u), password(p), role(r) {}
    virtual bool login(std::string u, std::string p) = 0;
    std::string getRole() { return role; }
    std::string getUsername() { return username; }
    
    void addTransaction(const Transaction& t) {
        transactions.push_back(t);
    }

    std::string getTransactionHistory() {
        std::string history;
        for (const auto& t : transactions) {
            history += "Book: " + t.bookTitle + 
                      ", Quantity: " + std::to_string(t.quantity) + 
                      ", Amount: $" + std::to_string(t.amount) + 
                      ", Status: " + t.status + "\n";
        }
        return history;
    }
};

// Enhanced Customer class
class Customer : public User {
public:
    Customer(std::string u, std::string p) : User(u, p, "customer") {}

    bool login(std::string u, std::string p) override {
        return username == u && password == p;
    }

    bool placeOrder(std::vector<Book>& inventory, const std::string& searchTitle, int quantity) {
        for (Book& b : inventory) {
            if (b.title == searchTitle && b.quantity >= quantity) {
                b.quantity -= quantity;
                Transaction t(username, searchTitle, quantity, b.price * quantity);
                addTransaction(t);
                return true;
            }
        }
        return false;
    }
};

// Enhanced Employee class
class Employee : public User {
public:
    Employee(std::string u, std::string p) : User(u, p, "employee") {}

    bool login(std::string u, std::string p) override {
        return username == u && password == p;
    }

    void addBook(std::vector<Book>& inventory, const std::string& title, const std::string& author, 
                int edition, const std::string& genre, int quantity, double price) {
        Book newBook(title, author, edition, genre, quantity, price);
        inventory.push_back(newBook);
        std::cout << "Book added successfully!" << std::endl;
    }
};

// Inventory System
class InventorySystem {
public:
    std::vector<Book> inventory;

    std::string getInventoryString() {
        std::string inv;
        for (const auto& book : inventory) {
            inv += book.toStringResponse() + "\n";
        }
        return inv;
    }

    std::string searchBooks(const std::string& term) {
        std::string results;
        for (const auto& book : inventory) {
            if (book.title.find(term) != std::string::npos || 
                book.author.find(term) != std::string::npos) {
                results += book.toStringResponse() + "\n";
            }
        }
        return results.empty() ? "No books found." : results;
    }
};

// Main function
int main() {
    InventorySystem inventorySystem;

    // Sample inventory
    inventorySystem.inventory.push_back(
        Book("C++ Programming", "Bjarne Stroustrup", 4, "Programming", 10, 59.99)
    );

    httplib::Server svr;

    // Display Inventory
    svr.Get("/api/inventory", [&](const httplib::Request&, httplib::Response& res) {
        res.set_content(inventorySystem.getInventoryString(), "text/plain");
    });

    // Search for Books
    svr.Get("/api/search", [&](const httplib::Request& req, httplib::Response& res) {
        auto term = req.get_param_value("term");
        res.set_content(inventorySystem.searchBooks(term), "text/plain");
    });

    // Place Order
    svr.Post("/api/place_order", [&](const httplib::Request& req, httplib::Response& res) {
        std::string title = req.get_param_value("title");
        int quantity = std::stoi(req.get_param_value("quantity"));

        bool orderPlaced = false;
        for (auto& book : inventorySystem.inventory) {
            if (book.title == title && book.quantity >= quantity) {
                book.quantity -= quantity;
                orderPlaced = true;
                break;
            }
        }

        res.set_content(orderPlaced ? "Order placed successfully!" : "Failed to place order: Insufficient quantity or book not found.", 
                        "text/plain");
    });

    // Add Book
    svr.Post("/api/add_book", [&](const httplib::Request& req, httplib::Response& res) {
        std::string title = req.get_param_value("title");
        std::string author = req.get_param_value("author");
        int edition = std::stoi(req.get_param_value("edition"));
        std::string genre = req.get_param_value("genre");
        int quantity = std::stoi(req.get_param_value("quantity"));
        double price = std::stod(req.get_param_value("price"));

        inventorySystem.inventory.emplace_back(title, author, edition, genre, quantity, price);
        res.set_content("Book added successfully!", "text/plain");
    });

    svr.listen("localhost", 8080);
    return 0;
}
