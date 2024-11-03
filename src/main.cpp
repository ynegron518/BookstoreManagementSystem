#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <iomanip>     // For std::setprecision
#include <sstream>     // For std::ostringstream
#include "httplib.h"
#include "json.hpp"    // JSON library

using json = nlohmann::json;

// Book class with JSON conversion
class Book {
public:
    std::string title, author, genre;
    int edition, quantity;
    double price;

    Book(std::string t, std::string a, int e, std::string g, int q, double p)
        : title(t), author(a), edition(e), genre(g), quantity(q), price(p) {}

    json toJson() const {
        return json{
            {"title", title},
            {"author", author},
            {"edition", edition},
            {"genre", genre},
            {"quantity", quantity},
            {"price", price}
        };
    }
};

// Inventory System class
class InventorySystem {
public:
    std::vector<Book> inventory;

    json getInventoryJson() const {
        json invJson = json::array();
        for (const auto& book : inventory) {
            invJson.push_back(book.toJson());
        }
        return invJson;
    }

    json searchBooks(const std::string& term) const {
        json results = json::array();
        for (const auto& book : inventory) {
            if (book.title.find(term) != std::string::npos || book.author.find(term) != std::string::npos) {
                results.push_back(book.toJson());
            }
        }
        return results.empty() ? json::array() : results;
    }
};

int main() {
    InventorySystem inventorySystem;

    // Sample inventory
    inventorySystem.inventory.push_back(Book("C++ Programming", "Bjarne Stroustrup", 4, "Programming", 10, 59.99));
    inventorySystem.inventory.push_back(Book("Data Structures", "Mark Allen Weiss", 3, "Computer Science", 5, 49.99));

    httplib::Server svr;

    // Inventory endpoint
    svr.Get("/api/inventory", [&](const httplib::Request&, httplib::Response& res) {
        json invJson = inventorySystem.getInventoryJson();
        res.set_content(invJson.dump(), "application/json");
    });

    // Search endpoint
    svr.Get("/api/search", [&](const httplib::Request& req, httplib::Response& res) {
        auto term = req.get_param_value("term");
        json searchResults = inventorySystem.searchBooks(term);
        res.set_content(searchResults.dump(), "application/json");
    });

    // Wishlist placeholder
    svr.Post("/api/add_to_wishlist", [&](const httplib::Request& req, httplib::Response& res) {
        json response = {{"status", "success"}, {"message", "Book added to wishlist!"}};
        res.set_content(response.dump(), "application/json");
    });

    // Order History placeholder
    svr.Get("/api/order_history", [&](const httplib::Request&, httplib::Response& res) {
        json orderHistory = json::array({
            {{"title", "C++ Programming"}, {"quantity", 2}, {"total_price", 119.98}},
            {{"title", "Data Structures"}, {"quantity", 1}, {"total_price", 49.99}}
        });
        res.set_content(orderHistory.dump(), "application/json");
    });

    // Checkout placeholder
    svr.Post("/api/checkout", [&](const httplib::Request&, httplib::Response& res) {
        json response = {{"status", "success"}, {"message", "Checkout successful. Thank you for your purchase!"}};
        res.set_content(response.dump(), "application/json");
    });

    // Start server on port 8081
    std::cout << "Starting server on http://localhost:8081..." << std::endl;
    if (!svr.listen("localhost", 8081)) {
    std::cerr << "Error: Server failed to start on port 8081!" << std::endl;
    return -1;
}


    return 0;
}
