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
    Product(const Product& old) {
        title = old.title;
        id = old.id;
        weight = old.weight;
        price = old.price;
        storage_life = old.storage_life;
    }
    virtual ~Product () = default;

    virtual float calculateStorageFree() {
        float cost = weight;
        return cost;
    }

    unsigned int getId() const {
        return id;
    }

    virtual void displayInfo() {
        std::cout << "Info about product " << title << ":\n";
        std::cout << "ID: " << id << '\n';
        std::cout << "Weight: " << weight << '\n';
        std::cout << "Price: " << price << '\n';
        std::cout << "Storage life: " << storage_life << '\n';
    }

    Product& operator= (const Product& a) = default;

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
    ~PerishableProduct () override = default;

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

    unsigned int deadline() const {
        time_t now = time(nullptr);
        unsigned int ans = (unsigned int)(expirationDate - now) / 3600 / 24;
        return ans;
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
    ~ElectronicProduct () override = default;

    ElectronicProduct& operator= (const ElectronicProduct& a) {
        Product::operator= (a);
        warrantyPeriod = a.warrantyPeriod;
        powerRating = a.powerRating;
        return *this;
    }

    void displayInfo() override {
        Product::displayInfo();
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
    ~BuildingMaterial () override = default;

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

class Warehouse  {
public:

    Warehouse& operator+= (Product& a) {
        array.push_back(std::make_shared<Product>(a));
        return *this;
    }

    Warehouse& operator-= (unsigned int id) {
        int size = (int)array.size();
        for (int i = 0; i < size; ++i) {
            if (array[i]->getId() == id) {
                array.erase(array.begin() + id);
                break;
            }
        }
        return *this;
    }

    std::vector<std::shared_ptr<Product>> getExpiringProducts(unsigned int days) {
        std::vector<std::shared_ptr<Product>> dead;
        for (const std::shared_ptr<Product>& el : array) {
            std::shared_ptr<PerishableProduct> product = std::dynamic_pointer_cast<PerishableProduct>(el);
            if (product && (product->deadline() <= days)) {
                dead.push_back(product);
            }
        }
        return dead;
    }

    void displayInventory() {
        for (const auto& el : array) {
            if (auto perishable = std::dynamic_pointer_cast<PerishableProduct>(el)) {
                perishable->displayInfo();
            } else if (auto electronic = std::dynamic_pointer_cast<ElectronicProduct>(el)) {
                electronic->displayInfo();
            } else if (auto buildingMaterial = std::dynamic_pointer_cast<BuildingMaterial>(el)) {
                buildingMaterial->displayInfo();
            } else {
                el->displayInfo();
            }
            std::cout << '\n';
        }
    }

private:
    std::vector<std::shared_ptr<Product>> array;
};


int main() {
    time_t now = time(nullptr);
    PerishableProduct a = PerishableProduct{"hui", 2, 1.2, 120, 5, now};
    ElectronicProduct b = ElectronicProduct{"rozetka", 3, 0.2, 50, 5, 30,220};
//    a.displayInfo();
    Warehouse sklad;
    sklad += a;
    sklad += b;
    sklad.displayInventory();
    std::cout << "////////////////";
    sklad -= 0;
//    std::cout << now;
}