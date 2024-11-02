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

    void displayBook() {
        std::cout << "Title: " << title << "\nAuthor: " << author
            << "\nEdition: " << edition << "\nGenre: " << genre
            << "\nQuantity: " << quantity << "\nPrice: $" << price << std::endl;
    }

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

    bool updateBookQuantity(std::vector<Book>& inventory, const std::string& title, int newQuantity) {
        for (Book& b : inventory) {
            if (b.title == title) {
                b.quantity = newQuantity;
                return true;
            }
        }
        return false;
    }
};

// Enhanced InventorySystem
class InventorySystem {
public:
    std::vector<Book> inventory;
    int lowStockThreshold = 5;

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
        return results;
    }

    std::string getLowStockAlert() {
        std::string alerts;
        for (const auto& book : inventory) {
            if (book.quantity <= lowStockThreshold) {
                alerts += "Low Stock Alert: " + book.title + 
                         " (Quantity: " + std::to_string(book.quantity) + ")\n";
            }
        }
        return alerts;
    }
};

// Main function with enhanced endpoints
int main() {
    InventorySystem inventorySystem;
    AuthenticationSystem authSystem;

    // Sample inventory
    inventorySystem.inventory.push_back(
        Book("C++ Programming", "Bjarne Stroustrup", 4, "Programming", 10, 59.99)
    );

    // Add users
    Customer customer1("john_doe", "password123");
    Employee employee1("admin_user", "adminpass");

    authSystem.addUser(&customer1);
    authSystem.addUser(&employee1);

    httplib::Server svr;

    // Enhanced endpoints
    svr.Get("/api/inventory", [&](const httplib::Request&, httplib::Response& res) {
        res.set_content(inventorySystem.getInventoryString(), "text/plain");
    });

    svr.Get("/api/search", [&](const httplib::Request& req, httplib::Response& res) {
        auto term = req.get_param_value("term");
        res.set_content(inventorySystem.searchBooks(term), "text/plain");
    });

    svr.Get("/api/low-stock", [&](const httplib::Request&, httplib::Response& res) {
        res.set_content(inventorySystem.getLowStockAlert(), "text/plain");
    });

    // Original endpoints remain the same
    // ... (your existing endpoints)

    svr.listen("localhost", 8080);
    return 0;
}
