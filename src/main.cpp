#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <iomanip>     // For std::setprecision
#include <sstream>     // For std::ostringstream
#include "httplib.h"

// Book class with enhanced features
class Book {
public:
    std::string title, author, genre;
    int edition, quantity;
    double price;

    Book(std::string t, std::string a, int e, std::string g, int q, double p)
        : title(t), author(a), edition(e), genre(g), quantity(q), price(p) {}

    std::string toStringResponse() const {
        std::ostringstream oss;
        oss << "Title: " << title
            << ", Author: " << author
            << ", Edition: " << edition
            << ", Genre: " << genre
            << ", Quantity: " << quantity
            << ", Price: $" << std::fixed << std::setprecision(2) << price;
        return oss.str();
    }
};

// Inventory System class
class InventorySystem {
public:
    std::vector<Book> inventory;

    std::string getInventoryString() const {
        std::string inv;
        for (const auto& book : inventory) {
            inv += book.toStringResponse() + "\n";
        }
        return inv;
    }

    std::string searchBooks(const std::string& term) const {
        std::string results;
        for (const auto& book : inventory) {
            if (book.title.find(term) != std::string::npos || book.author.find(term) != std::string::npos) {
                results += book.toStringResponse() + "\n";
            }
        }
        return results.empty() ? "No books found." : results;
    }
};

// Main function with server setup
int main() {
    InventorySystem inventorySystem;

    // Sample inventory for demonstration
    inventorySystem.inventory.push_back(
        Book("C++ Programming", "Bjarne Stroustrup", 4, "Programming", 10, 59.99)
    );
    inventorySystem.inventory.push_back(
        Book("Data Structures", "Mark Allen Weiss", 3, "Computer Science", 5, 49.99)
    );

    httplib::Server svr;

    // Endpoint to display the full inventory (now accessible at /inventory)
    svr.Get("/inventory", [&](const httplib::Request&, httplib::Response& res) {
        res.set_content(inventorySystem.getInventoryString(), "text/plain");
    });

    // Endpoint to search for books by title or author (now accessible at /search)
    svr.Get("/search", [&](const httplib::Request& req, httplib::Response& res) {
        auto term = req.get_param_value("term");
        res.set_content(inventorySystem.searchBooks(term), "text/plain");
    });

    // Place an order for a book (now accessible at /place_order)
    svr.Post("/place_order", [&](const httplib::Request& req, httplib::Response& res) {
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

    // Add a new book to the inventory (now accessible at /add_book)
    svr.Post("/add_book", [&](const httplib::Request& req, httplib::Response& res) {
        std::string title = req.get_param_value("title");
        std::string author = req.get_param_value("author");
        int edition = std::stoi(req.get_param_value("edition"));
        std::string genre = req.get_param_value("genre");
        int quantity = std::stoi(req.get_param_value("quantity"));
        double price = std::stod(req.get_param_value("price"));

        inventorySystem.inventory.emplace_back(title, author, edition, genre, quantity, price);
        res.set_content("Book added successfully!", "text/plain");
    });

    // Debug message to confirm server is about to start
    std::cout << "Starting server on http://127.0.0.1:3000..." << std::endl;

    // Start the server and check for errors
    if (!svr.listen("127.0.0.1", 3000)) {
        std::cerr << "Error: Server failed to start!" << std::endl;
        return -1;
    }

    return 0;
}


