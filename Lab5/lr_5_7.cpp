#include <iostream>
#include <chrono>

class Product {
public:

    Product() {
        title = "";
        id = 0;
        weight = 0;
        price = 0;
        storage_life = 0;
    }
    Product(std::string Title, unsigned int ID, float Weight, float Price, unsigned int Time) {
        title = Title;
        id = ID;
        weight = Weight;
        price = Price;
        storage_life = Time;
    }
    Product(Product& old) {
        title = old.title;
        id = old.id;
        weight = old.weight;
        price = old.price;
        storage_life = old.storage_life;
    }
    ~Product () {
        title = "";
        id = 0;
        weight = 0;
        price = 0;
        storage_life = 0;
    }

    virtual float calculateStorageFree() {
        float cost = weight;
        return cost;
    }

    virtual void displayInfo() {
        std::cout << "Info about product " << title << ":\n";
        std::cout << "ID: " << id << '\n';
        std::cout << "Weight: " << weight << '\n';
        std::cout << "Price: " << price << '\n';
        std::cout << "Storage life: " << storage_life << '\n';
    }

private:
    std::string title;
    unsigned int id;
    float weight;
    float price;
    unsigned int storage_life;
};

class PerishableProduct : Product {
public:

    PerishableProduct() {
        expirationDate = 0;
    };
//    PerishableProduct(std::string Title, unsigned int ID, float Weight, float Price, unsigned int Time, time_t end) : Product(std::string Title, unsigned int ID, float Weight, float Price, unsigned int Time) {
//        expirationDate = end;
//    };
    PerishableProduct(PerishableProduct& old)  : Product(old) {
        expirationDate = old.expirationDate;
    }
    ~PerishableProduct () {
        expirationDate = 0;
    }

    float calculateStorageFree() override {
        float cost = Product::calculateStorageFree();
        time_t now = time(nullptr);
        float kef = (float)(expirationDate - now) / 3600 / 24;
        if (kef > 0) {
            cost /= kef;
        } else {
            cost *= kef;
        }
        std::cout << (float)(expirationDate - now) / 3600 / 24;
        return cost;
    }


private:
    time_t expirationDate;
};

class ElectronicProduct : Product {

};

class BuildingMaterial : Product {

};


int main() {
    time_t now = time(nullptr);
    PerishableProduct a = PerishableProduct{"hui", 2, 1.2, 120, 5, now};

    std::cout << now;
}