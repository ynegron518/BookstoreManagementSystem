#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <iomanip>     // For std::setprecision
#include <sstream>     // For std::ostringstream
#include "httplib.h"
#include "json.hpp"    // Include the JSON library

using json = nlohmann::json;

// Book class with enhanced features
class Book {
public:
    std::string title, author, genre;
    int edition, quantity;
    double price;

    Book(std::string t, std::string a, int e, std::string g, int q, double p)
        : title(t), author(a), edition(e), genre(g), quantity(q), price(p) {}

<<<<<<< HEAD
    // Convert book data to JSON format
    json toJson() const {
        return json{
            {"title", title},
            {"author", author},
            {"edition", edition},
            {"genre", genre},
            {"quantity", quantity},
            {"price", price}
        };
=======
    std::string toStringResponse() const {
        std::ostringstream oss;
        oss << "Title: " << title
            << ", Author: " << author
            << ", Edition: " << edition
            << ", Genre: " << genre
            << ", Quantity: " << quantity
            << ", Price: $" << std::fixed << std::setprecision(2) << price;
        return oss.str();
>>>>>>> 2e9067971253a89345b7d2578894ef429c5a608d
    }
};

// Inventory System class
class InventorySystem {
public:
    std::vector<Book> inventory;

<<<<<<< HEAD
    // Get inventory as JSON
    json getInventoryJson() const {
        json invJson = json::array();
=======
    std::string getInventoryString() const {
        std::string inv;
>>>>>>> 2e9067971253a89345b7d2578894ef429c5a608d
        for (const auto& book : inventory) {
            invJson.push_back(book.toJson());
        }
        return invJson;
    }

<<<<<<< HEAD
    // Search books and return as JSON
    json searchBooks(const std::string& term) const {
        json results = json::array();
        for (const auto& book : inventory) {
            if (book.title.find(term) != std::string::npos || book.author.find(term) != std::string::npos) {
                results.push_back(book.toJson());
=======
    std::string searchBooks(const std::string& term) const {
        std::string results;
        for (const auto& book : inventory) {
            if (book.title.find(term) != std::string::npos || book.author.find(term) != std::string::npos) {
                results += book.toStringResponse() + "\n";
>>>>>>> 2e9067971253a89345b7d2578894ef429c5a608d
            }
        }
        return results.empty() ? json::array() : results;
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

<<<<<<< HEAD
    // Endpoint to display the full inventory as JSON
    svr.Get("/inventory", [&](const httplib::Request&, httplib::Response& res) {
        json invJson = inventorySystem.getInventoryJson();
        res.set_content(invJson.dump(), "application/json");
    });

    // Endpoint to search for books by title or author and return as JSON
=======
    // Endpoint to display the full inventory (now accessible at /inventory)
    svr.Get("/inventory", [&](const httplib::Request&, httplib::Response& res) {
        res.set_content(inventorySystem.getInventoryString(), "text/plain");
    });

    // Endpoint to search for books by title or author (now accessible at /search)
>>>>>>> 2e9067971253a89345b7d2578894ef429c5a608d
    svr.Get("/search", [&](const httplib::Request& req, httplib::Response& res) {
        auto term = req.get_param_value("term");
        json searchResults = inventorySystem.searchBooks(term);
        res.set_content(searchResults.dump(), "application/json");
    });

<<<<<<< HEAD
    // Place an order for a book, returning a JSON response
    svr.Post("/place_order", [&](const httplib::Request& req, httplib::Response& res) {
        auto orderData = json::parse(req.body);
        std::string title = orderData["title"];
        int quantity = orderData["quantity"];
=======
    // Place an order for a book (now accessible at /place_order)
    svr.Post("/place_order", [&](const httplib::Request& req, httplib::Response& res) {
        std::string title = req.get_param_value("title");
        int quantity = std::stoi(req.get_param_value("quantity"));
>>>>>>> 2e9067971253a89345b7d2578894ef429c5a608d

        bool orderPlaced = false;
        for (auto& book : inventorySystem.inventory) {
            if (book.title == title && book.quantity >= quantity) {
                book.quantity -= quantity;
                orderPlaced = true;
                break;
            }
        }

        json response = {
            {"status", orderPlaced ? "success" : "error"},
            {"message", orderPlaced ? "Order placed successfully!" : "Failed to place order: Insufficient quantity or book not found."}
        };
        res.set_content(response.dump(), "application/json");
    });

<<<<<<< HEAD
    // Add a new book to the inventory from JSON data
    svr.Post("/add_book", [&](const httplib::Request& req, httplib::Response& res) {
        auto bookData = json::parse(req.body);
        std::string title = bookData["title"];
        std::string author = bookData["author"];
        int edition = bookData["edition"];
        std::string genre = bookData["genre"];
        int quantity = bookData["quantity"];
        double price = bookData["price"];
=======
    // Add a new book to the inventory (now accessible at /add_book)
    svr.Post("/add_book", [&](const httplib::Request& req, httplib::Response& res) {
        std::string title = req.get_param_value("title");
        std::string author = req.get_param_value("author");
        int edition = std::stoi(req.get_param_value("edition"));
        std::string genre = req.get_param_value("genre");
        int quantity = std::stoi(req.get_param_value("quantity"));
        double price = std::stod(req.get_param_value("price"));
>>>>>>> 2e9067971253a89345b7d2578894ef429c5a608d

        inventorySystem.inventory.emplace_back(title, author, edition, genre, quantity, price);

        json response = {{"status", "success"}, {"message", "Book added successfully!"}};
        res.set_content(response.dump(), "application/json");
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


