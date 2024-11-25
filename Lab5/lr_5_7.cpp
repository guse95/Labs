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

    Product& operator= (const Product& a) {
        title = a.title;
        id = a.id;
        weight = a.weight;
        price = a.price;
        storage_life = a.storage_life;
        return *this;
    }

private:
    std::string title;
    unsigned int id;
    float weight;
    float price;
    unsigned int storage_life;
};

class PerishableProduct : public Product {
public:

    PerishableProduct() {
        expirationDate = 0;
    };
    PerishableProduct(std::string Title, unsigned int ID, float Weight, float Price, unsigned int Time, time_t end) : Product(Title, ID, Weight, Price, Time) {
        expirationDate = end;
    };
    PerishableProduct(PerishableProduct& old)  : Product(old) {
        expirationDate = old.expirationDate;
    }
    ~PerishableProduct () = default;

    PerishableProduct& operator= (const PerishableProduct& a) {
        Product::operator= (a);
        expirationDate = a.expirationDate;
        return *this;
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

class ElectronicProduct : public Product {
public:

    ElectronicProduct() {
        warrantyPeriod = 0;
        powerRating = 0;
    };
    ElectronicProduct(std::string Title, unsigned int ID, float Weight, float Price, unsigned int Time,
                      unsigned int end, float power) : Product(Title, ID, Weight, Price, Time) {
        warrantyPeriod = end;
        powerRating = power;
    };
    ElectronicProduct(ElectronicProduct& old)  : Product(old) {
        warrantyPeriod = old.warrantyPeriod;
        powerRating = old.powerRating;
    }
    ~ElectronicProduct () = default;

    ElectronicProduct& operator= (const ElectronicProduct& a) {
        Product::operator= (a);
        warrantyPeriod = a.warrantyPeriod;
        powerRating = a.powerRating;
        return *this;
    }

    void displayInfo() override {
//        Product::displayInfo();
        std::cout << "Warranty period: " << warrantyPeriod << '\n';
        std::cout << "Power rating: " << powerRating << '\n';
    }

private:
    unsigned int warrantyPeriod;
    float powerRating;
};

class BuildingMaterial : public Product {
public:

    BuildingMaterial() {
        flammability = false;
    };
    BuildingMaterial(std::string Title, unsigned int ID, float Weight, float Price, unsigned int Time, bool fireDanger) : Product(Title, ID, Weight, Price, Time) {
        flammability = fireDanger;
    };
    BuildingMaterial(BuildingMaterial& old)  : Product(old) {
        flammability = old.flammability;
    }
    ~BuildingMaterial () = default;

    BuildingMaterial& operator= (const BuildingMaterial& a) {
        Product::operator= (a);
        flammability = a.flammability;
        return *this;
    }

    float calculateStorageFree() override {
        float cost = Product::calculateStorageFree();
        if (flammability) {
            cost *= 2;
        }
        return cost;
    }

private:
    bool flammability;
};

union Prod {
    PerishableProduct p;
    ElectronicProduct e;
    BuildingMaterial b;
};

struct node {
    int type;
    Prod data
};


class Warehouse : public PerishableProduct, public ElectronicProduct, public BuildingMaterial {
public:
    Warehouse() {
        array = {};
        capacity = 1;
        cur_len = 0;
    }


    Warehouse& operator+= (const PerishableProduct& a) {
        array.push_back(node{1, a})
        return *this;
    }

private:
    std::vector<node> array;
    int capacity;
    int cur_len;
};


int main() {
    time_t now = time(nullptr);
    PerishableProduct a = PerishableProduct{"hui", 2, 1.2, 120, 5, now};

    std::cout << now;
}