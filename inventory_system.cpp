#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <iomanip>
#include <algorithm>
#include <limits>

using namespace std;

// Product Data Structure
class Product {
public:
    string sku;
    string name;
    string description;
    string category;
    int currentStock;
    int reservedStock;
    double buyingCost;
    double retailPrice;
    int reorderPoint;

    Product(string s, string n, string d, string c, int cs, int rs, double bc, double rp, int rOP)
        : sku(s), name(n), description(d), category(c), currentStock(cs), reservedStock(rs),
          buyingCost(bc), retailPrice(rp), reorderPoint(rOP) {}

    double getPhysicalStock() const { return currentStock - reservedStock; }
    double getTotalStock() const { return currentStock; }
    double getInventoryValue() const { return getTotalStock() * buyingCost; }
    double getPotentialValue() const { return getTotalStock() * retailPrice; }
    bool isLowStock() const { return getTotalStock() <= reorderPoint; }
};

// Inventory Manager
class InventoryManager {
private:
    map<string, Product> products;

public:
    void addProduct(const Product& product) {
        products[product.sku] = product;
        cout << "Product added: " << product.name << " (SKU: " << product.sku << ")" << endl;
    }

    bool intakeStock(const string& sku, int quantity) {
        if (products.find(sku) == products.end()) {
            cout << "Error: Product with SKU " << sku << " not found." << endl;
            return false;
        }
        products[sku].currentStock += quantity;
        cout << "Stock increased: " << quantity << " units added for SKU " << sku << endl;
        cout << "New stock level: " << products[sku].currentStock << endl;
        return true;
    }

    bool deductStock(const string& sku, int quantity) {
        if (products.find(sku) == products.end()) {
            cout << "Error: Product with SKU " << sku << " not found." << endl;
            return false;
        }
        if (products[sku].currentStock < quantity) {
            cout << "Error: Insufficient stock. Available: " << products[sku].currentStock << endl;
            return false;
        }
        products[sku].currentStock -= quantity;
        cout << "Stock decreased: " << quantity << " units sold/deducted for SKU " << sku << endl;
        cout << "New stock level: " << products[sku].currentStock << endl;
        return true;
    }

    bool adjustStock(const string& sku, int adjustment, string reason) {
        if (products.find(sku) == products.end()) {
            cout << "Error: Product with SKU " << sku << " not found." << endl;
            return false;
        }
        int newStock = products[sku].currentStock + adjustment;
        if (newStock < 0) {
            cout << "Error: Adjustment would result in negative stock." << endl;
            return false;
        }
        products[sku].currentStock = newStock;
        cout << "Stock adjusted: " << adjustment << " units (" << reason << ") for SKU " << sku << endl;
        cout << "New stock level: " << products[sku].currentStock << endl;
        return true;
    }

    void searchBySKU(const string& sku) {
        auto it = products.find(sku);
        if (it == products.end()) {
            cout << "Product not found with SKU: " << sku << endl;
        } else {
            displayProduct(it->second);
        }
    }

    void searchByName(const string& name) {
        bool found = false;
        for (const auto& pair : products) {
            if (pair.second.name.find(name) != string::npos) {
                displayProduct(pair.second);
                found = true;
            }
        }
        if (!found) cout << "No products found with name: " << name << endl;
    }

    void searchByCategory(const string& category) {
        bool found = false;
        for (const auto& pair : products) {
            if (pair.second.category == category) {
                displayProduct(pair.second);
                found = true;
            }
        }
        if (!found) cout << "No products found in category: " << category << endl;
    }

    void displayAllProducts() {
        if (products.empty()) {
            cout << "Inventory is empty." << endl;
            return;
        }
        cout << "\n=== ALL PRODUCTS ===" << endl;
        for (const auto& pair : products) {
            displayProduct(pair.second);
        }
    }

    void checkLowStockAlerts() {
        cout << "\n=== LOW STOCK ALERTS ===" << endl;
        bool hasAlerts = false;
        for (const auto& pair : products) {
            if (pair.second.isLowStock()) {
                displayProduct(pair.second);
                cout << "  ALERT: Stock below reorder point!" << endl;
                hasAlerts = true;
            }
        }
        if (!hasAlerts) cout << "No low stock alerts." << endl;
    }

    void calculateInventoryValue() {
        double totalCostValue = 0.0;
        double totalRetailValue = 0.0;
        for (const auto& pair : products) {
            totalCostValue += pair.second.getInventoryValue();
            totalRetailValue += pair.second.getPotentialValue();
        }
        cout << "\n=== INVENTORY VALUE ===" << endl;
        cout << fixed << setprecision(2);
        cout << "Total Cost Value: $" << totalCostValue << endl;
        cout << "Total Retail Value: $" << totalRetailValue << endl;
    }

private:
    void displayProduct(const Product& product) {
        cout << "\n--- Product Details ---" << endl;
        cout << "SKU: " << product.sku << endl;
        cout << "Name: " << product.name << endl;
        cout << "Description: " << product.description << endl;
        cout << "Category: " << product.category << endl;
        cout << "Current Stock: " << product.currentStock << endl;
        cout << "Reserved Stock: " << product.reservedStock << endl;
        cout << "Physical Stock: " << product.getPhysicalStock() << endl;
        cout << "Buying Cost: $" << fixed << setprecision(2) << product.buyingCost << endl;
        cout << "Retail Price: $" << product.retailPrice << endl;
        cout << "Reorder Point: " << product.reorderPoint << endl;
        cout << "Inventory Value (cost): $" << product.getInventoryValue() << endl;
    }
};

// Main Program
int main() {
    InventoryManager manager;

    // Add sample products
    manager.addProduct(Product("SKU001", "Wireless Mouse", "Ergonomic wireless mouse", "Electronics", 50, 5, 15.00, 29.99, 10));
    manager.addProduct(Product("SKU002", "USB-C Cable", "6ft USB-C charging cable", "Accessories", 200, 20, 3.50, 9.99, 50));
    manager.addProduct(Product("SKU003", "Laptop Stand", "Adjustable aluminum stand", "Accessories", 30, 0, 25.00, 49.99, 8));
    manager.addProduct(Product("SKU004", "Mechanical Keyboard", "RGB mechanical keyboard", "Electronics", 45, 10, 45.00, 89.99, 15));
    manager.addProduct(Product("SKU005", "Monitor Arm", "Single monitor arm", "Furniture", 15, 3, 30.00, 59.99, 5));

    int choice;
    do {
        cout << "\n=== INVENTORY MANAGEMENT SYSTEM ===" << endl;
        cout << "1. Add Product" << endl;
        cout << "2. Intake Stock (increase)" << endl;
        cout << "3. Deduct Stock (decrease)" << endl;
        cout << "4. Adjust Stock" << endl;
        cout << "5. Search by SKU" << endl;
        cout << "6. Search by Name" << endl;
        cout << "7. Search by Category" << endl;
        cout << "8. Display All Products" << endl;
        cout << "9. Check Low Stock Alerts" << endl;
        cout << "10. Calculate Inventory Value" << endl;
        cout << "0. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice) {
            case 1: {
                string sku, name, desc, category;
                int cs, rs, rOP;
                double bc, rp;
                cout << "Enter SKU: "; getline(cin, sku);
                cout << "Enter Name: "; getline(cin, name);
                cout << "Enter Description: "; getline(cin, desc);
                cout << "Enter Category: "; getline(cin, category);
                cout << "Enter Current Stock: "; cin >> cs;
                cout << "Enter Reserved Stock: "; cin >> rs;
                cout << "Enter Buying Cost: "; cin >> bc;
                cout << "Enter Retail Price: "; cin >> rp;
                cout << "Enter Reorder Point: "; cin >> rOP;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                manager.addProduct(Product(sku, name, desc, category, cs, rs, bc, rp, rOP));
                break;
            }
            case 2: {
                string sku;
                int qty;
                cout << "Enter SKU: "; getline(cin, sku);
                cout << "Enter quantity to add: "; cin >> qty;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                manager.intakeStock(sku, qty);
                break;
            }
            case 3: {
                string sku;
                int qty;
                cout << "Enter SKU: "; getline(cin, sku);
                cout << "Enter quantity to deduct: "; cin >> qty;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                manager.deductStock(sku, qty);
                break;
            }
            case 4: {
                string sku, reason;
                int adj;
                cout << "Enter SKU: "; getline(cin, sku);
                cout << "Enter adjustment amount (positive or negative): "; cin >> adj;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Enter reason (e.g., damaged, returned): "; getline(cin, reason);
                manager.adjustStock(sku, adj, reason);
                break;
            }
            case 5: {
                string sku;
                cout << "Enter SKU to search: "; getline(cin, sku);
                manager.searchBySKU(sku);
                break;
            }
            case 6: {
                string name;
                cout << "Enter product name to search: "; getline(cin, name);
                manager.searchByName(name);
                break;
            }
            case 7: {
                string category;
                cout << "Enter category to search: "; getline(cin, category);
                manager.searchByCategory(category);
                break;
            }
            case 8:
                manager.displayAllProducts();
                break;
            case 9:
                manager.checkLowStockAlerts();
                break;
            case 10:
                manager.calculateInventoryValue();
                break;
            case 0:
                cout << "Exiting..." << endl;
                break;
            default:
                cout << "Invalid choice!" << endl;
        }
    } while (choice != 0);

    return 0;
}
