#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <exception>
#include <cctype>
#include <windows.h>
using namespace std;

class MyException : public exception {
private:
    string text;
    int line;
    string datetime;
    string fullMessage;
public:
    MyException() {
        text = "Unknown exception";
        line = 0;
        datetime = __DATE__;

        fullMessage = "Error Message: " + text +
            "\nLine: " + to_string(line) +
            "\nDate: " + datetime;
    }
    MyException(string text, int line = 0) {
        this->text = text;
        this->line = line;
        datetime = __DATE__;

        fullMessage = "Error Message: " + this->text +
            "\nLine: " + to_string(this->line) +
            "\nDate: " + datetime;
    }
    const char* what() const noexcept override {
        return fullMessage.c_str();
    }
};
class ValidationException : public MyException {
public:
    ValidationException(string message, int line = 0)
        : MyException(message, line) {
    }
};
class StockException : public MyException {
public:
    StockException(string message, int line = 0)
        : MyException(message, line) {
    }
};
class PaymentException : public MyException {
public:
    PaymentException(string message, int line = 0)
        : MyException(message, line) {
    }
};


template <typename T>
class MyVector {
private:
    T* arr;
    int count;
    int capacity;
public:
    MyVector() {
        count = 0;
        capacity = 10;
        arr = new T[capacity]{};
    }
    MyVector(const MyVector& other) {
        count = other.count;
        capacity = other.capacity;
        arr = new T[capacity]{};

        for (int i = 0; i < count; i++) {
            arr[i] = other.arr[i];
        }
    }
    MyVector& operator=(const MyVector& other) {
        if (this != &other) {
            delete[] arr;
            count = other.count;
            capacity = other.capacity;
            arr = new T[capacity]{};
            for (int i = 0; i < count; i++) {
                arr[i] = other.arr[i];
            }
        }
        return *this;
    }
    void push_back(T data) {
        if (count == capacity) {
            capacity += 10;
            T* temp_arr = new T[capacity]{};
            for (int i = 0; i < count; i++) {
                temp_arr[i] = arr[i];
            }
            delete[] arr;
            arr = temp_arr;
        }
        arr[count] = data;
        count++;
    }
    void pop_back() {
        if (count > 0) {
            count--;
        }
    }
    void push_front(const T& data) {
        if (count == capacity) {
            int newCapacity = capacity + 10;
            T* temp_arr = new T[newCapacity]{};
            for (int i = 0; i < count; i++) {
                temp_arr[i + 1] = arr[i];
            }
            delete[] arr;
            arr = temp_arr;
            capacity = newCapacity;
        }
        else {
            for (int i = count; i > 0; i--) {
                arr[i] = arr[i - 1];
            }
        }
        arr[0] = data;
        count++;
    }
    void pop_front() {
        if (count == 0) {
            return;
        }
        for (int i = 0; i < count - 1; i++) {
            arr[i] = arr[i + 1];
        }
        count--;
    }
    void clear() {
        count = 0;
    }
    int size() const {
        return count;
    }
    int getSize() const {
        return count;
    }
    bool empty() const {
        return count == 0;
    }
    T& operator[](int index) {
        return arr[index];
    }
    const T& operator[](int index) const {
        return arr[index];
    }
    T* begin() {
        return arr;
    }
    T* end() {
        return arr + count;
    }
    const T* begin() const {
        return arr;
    }
    const T* end() const {
        return arr + count;
    }
    ~MyVector() {
        delete[] arr;
    }
};


string CurrentDate() {
    time_t now = time(0);
    tm localTime;
    localtime_s(&localTime, &now);
    stringstream ss;
    ss << put_time(&localTime, "%Y-%m-%d");
    return ss.str();
}
MyVector<string> Split(const string& line, char delimiter = '|') {
    MyVector<string> result;
    string item;
    stringstream ss(line);
    while (getline(ss, item, delimiter)) {
        result.push_back(item);
    }
    return result;
}
int ToInt(const string& value) {
    stringstream ss(value);
    int number;
    char extra;

    if (!(ss >> number) || (ss >> extra)) {
        throw MyException("Tam eded formatinda yanlis melumat: " + value);
    }
    return number;
}
double ToDouble(const string& value) {
    stringstream ss(value);
    double number;
    char extra;

    if (!(ss >> number) || (ss >> extra)) {
        throw MyException("Eded formatinda yanlis melumat: " + value);
    }
    return number;
}
string CleanField(string value) {
    for (int i = 0; i < (int)value.length(); i++) {
        if (value[i] == '|') {
            value[i] = '/';
        }
        if (value[i] == '\n' || value[i] == '\r') {
            value[i] = ' ';
        }
    }
    return value;
}
void Pause() {
    cout << "\nDavam etmek ucun ENTER-e bas...";
    cin.get();
}
int ReadInt(const string& prompt) {
    int value;
    while (true) {
        cout << prompt;
        if (cin >> value) {
            cin.ignore(10000, '\n');
            return value;
        }
        cout << "Yanlis reqem, yeniden cehd edin.\n";
        cin.clear();
        cin.ignore(10000, '\n');
    }
}
double ReadDouble(const string& prompt) {
    double value;
    while (true) {
        cout << prompt;
        if (cin >> value && value >= 0) {
            cin.ignore(10000, '\n');
            return value;
        }
        cout << "Yanlis mebleg. Menfi olmayan eded daxil edin.\n";
        cin.clear();
        cin.ignore(10000, '\n');
    }
}
string ReadText(const string& prompt) {
    string value;
    cout << prompt;
    getline(cin, value);
    return value;
}
void Header(const string& title) {
    cout << "\n";
    cout << "============================================================\n";
    cout << "                 RESTAURANT MANAGEMENT\n";
    cout << "============================================================\n";
    cout << "  " << title << "\n";
    cout << "------------------------------------------------------------\n";
}
void ShowMessage(const string& message) {
    cout << "\n[INFO] " << message << "\n";
}

class FileManager {
public:
    static MyVector<string> ReadFromFile(string path = "data.txt") {
        MyVector<string> data;
        ifstream fs(path);
        if (fs.is_open()) {
            string row;
            while (getline(fs, row)) {
                if (!row.empty()) {
                    data.push_back(row);
                }
            }
        }
        fs.close();
        return data;
    }

    static void WriteToFile(
        MyVector<string> data,
        string path = "data.txt"
    ) {
        ofstream fs(path, ios::out);
        if (fs.is_open()) {
            for (int i = 0; i < data.size(); i++) {
                fs << data[i] << "\n";
            }
        }
        else {
            throw MyException("File acmaq mumkun olmadi: " + path, __LINE__);
        }
        fs.close();
    }
};


class Person {
protected:
    int id;
    string username;
    string password;
public:
    Person(int id, string username, string password) {
        this->id = id;
        this->username = username;
        this->password = password;
    }
    virtual void Display() const = 0;
    int GetId() const {
        return id;
    }
    string GetUsername() const {
        return username;
    }
    bool CheckPassword(string value) const {
        return password == value;
    }
    virtual ~Person() {}
};


class User : public Person {
private:
    string fullname;
    string phone;
    bool active;
public:
    User() : Person(0, "", "") {
        fullname = "";
        phone = "";
        active = true;
    }
    User(int id, string username, string password,
        string fullname, string phone, bool active = true)
        : Person(id, username, password) {
        this->fullname = fullname;
        this->phone = phone;
        this->active = active;
    }
    void Display() const override {
        cout << left << setw(6) << id
            << setw(18) << username
            << setw(25) << fullname
            << setw(16) << phone
            << (active ? "Aktiv" : "Bloklanmis") << '\n';
    }
    string GetFullname() const {
        return fullname;
    }
    string GetPhone() const {
        return phone;
    }
    bool IsActive() const {
        return active;
    }
    void SetActive(bool value) {
        active = value;
    }
    string ToFile() const {
        return to_string(id) + "|" +
            CleanField(username) + "|" +
            CleanField(password) + "|" +
            CleanField(fullname) + "|" +
            CleanField(phone) + "|" +
            (active ? "1" : "0");
    }
};


class Admin : public Person {
public:
    Admin(int id, string username, string password)
        : Person(id, username, password) {
    }
    void Display() const override {
        cout << "Administrator: " << username << '\n';
    }
};


class Ingredient {
private:
    int id;
    string name;
    double quantity;
    double pricePerKg;
public:
    Ingredient(int id, string name, double quantity, double pricePerKg) {
        this->id = id;
        this->name = name;
        this->quantity = quantity;
        this->pricePerKg = pricePerKg;
    }
    int GetId() const {
        return id;
    }
    string GetName() const {
        return name;
    }
    double GetQuantity() const {
        return quantity;
    }
    double GetPricePerKg() const {
        return pricePerKg;
    }
    void AddQuantity(double amount) {
        if (amount <= 0) {
            throw ValidationException("Miqdar musbet olmalidir...", __LINE__);
        }
        quantity += amount;
    }
    void RemoveQuantity(double amount) {
        if (amount <= 0) {
            throw ValidationException("Miqdar musbet olmalidir...", __LINE__);
        }
        if (quantity < amount) {
            throw StockException("Stokda yeterli qeder inqrident yoxdur...", __LINE__);
        }
        quantity -= amount;
    }
    void Display() const {
        cout << left << setw(6) << id
            << setw(22) << name
            << setw(15) << quantity
            << fixed << setprecision(2)
            << pricePerKg << " AZN/kg\n";
    }
    string ToFile() const {
        ostringstream ss;

        ss << id << '|'
            << CleanField(name) << '|'
            << setprecision(12) << quantity << '|'
            << pricePerKg;
        return ss.str();
    }
};


class RecipeIngredient {
private:
    int ingredientId;
    string ingredientName;
    double quantity;
public:
    RecipeIngredient() {
        ingredientId = 0;
        ingredientName = "";
        quantity = 0;
    }
    RecipeIngredient(int ingredientId, string ingredientName, double quantity) {
        this->ingredientId = ingredientId;
        this->ingredientName = ingredientName;
        this->quantity = quantity;
    }
    int GetIngredientId() const {
        return ingredientId;
    }
    string GetIngredientName() const {
        return ingredientName;
    }
    double GetQuantity() const {
        return quantity;
    }
    void SetQuantity(double amount) {
        if (amount < 0) {
            throw ValidationException(
                "Inqrident miqdari menfi ola bilmez...", __LINE__);
        }
        quantity = amount;
    }
    void Display() const {
        cout << ingredientName << " - " << quantity << " grams\n";
    }
};


class Food {
private:
    int id;
    string name;
    string description;
    double price;
    int preparationTime;
    MyVector<RecipeIngredient> recipe;
public:
    Food(int id, string name, string description,
        double price, int preparationTime) {
        if (price <= 0) {
            throw ValidationException(
                "Yemeyin qiymeti 0dan yuksek olmalidir...", __LINE__);
        }
        if (preparationTime < 0) {
            throw ValidationException(
                "Hazirlama muddeti menfi ola bilmez...", __LINE__);
        }
        this->id = id;
        this->name = name;
        this->description = description;
        this->price = price;
        this->preparationTime = preparationTime;
    }
    virtual ~Food() {}
    virtual string GetType() const = 0;
    int GetId() const {
        return id;
    }
    string GetName() const {
        return name;
    }
    string GetDescription() const {
        return description;
    }
    double GetPrice() const {
        return price;
    }
    int GetPreparationTime() const {
        return preparationTime;
    }
    void SetPrice(double value) {
        if (value <= 0) {
            throw ValidationException(
                "Qiymet sifirdan yuksek olmalidir...", __LINE__);
        }
        price = value;
    }
    const MyVector<RecipeIngredient>& GetRecipe() const {
        return recipe;
    }
    void AddRecipeIngredient(const RecipeIngredient& item) {
        for (const RecipeIngredient& current : recipe) {
            if (current.GetIngredientId() == item.GetIngredientId()) {
                throw ValidationException(
                    "Inqrident artiq reseptde movcuddur.", __LINE__);
            }
        }
        recipe.push_back(item);
    }
    void Display() const {
        cout << left << setw(6) << id
            << setw(22) << name
            << setw(12) << fixed << setprecision(2) << price
            << setw(12) << (to_string(preparationTime) + " min")
            << GetType() << '\n';
    }
    void DisplayInfo() const {
        cout << "\nYemeyin ID: " << id
            << "\nAd: " << name
            << "\nNov: " << GetType()
            << "\nTesvir: " << description
            << "\nQiymet: " << fixed << setprecision(2) << price << " AZN"
            << "\nHazirlanma muddeti: " << preparationTime << " minutes\n";
        cout << "\nInqridentler:\n";
        for (const RecipeIngredient& item : recipe) {
            item.Display();
        }
    }
    string ToFile() const {
        ostringstream ss;
        ss << id << '|'
            << GetType() << '|'
            << CleanField(name) << '|'
            << CleanField(description) << '|'
            << setprecision(12) << price << '|'
            << preparationTime;
        return ss.str();
    }
};

class Pizza : public Food {
public:
    Pizza(int id, string name, string description,
        double price, int time)
        : Food(id, name, description, price, time) {
    }
    string GetType() const override {
        return "Pizza";
    }
};
class Burger : public Food {
public:
    Burger(int id, string name, string description,
        double price, int time)
        : Food(id, name, description, price, time) {
    }
    string GetType() const override {
        return "Burger";
    }
};
class Pasta : public Food {
public:
    Pasta(int id, string name, string description,
        double price, int time)
        : Food(id, name, description, price, time) {
    }
    string GetType() const override {
        return "Pasta";
    }
};


class Stock {
private:
    MyVector<Ingredient*> ingredients;
public:
    Stock() {}
    Stock(const Stock&) = delete;
    Stock& operator=(const Stock&) = delete;
    ~Stock() {
        for (Ingredient* item : ingredients) {
            delete item;
        }
    }
    Ingredient* FindIngredient(int id) const {
        for (Ingredient* item : ingredients) {
            if (item->GetId() == id) {
                return item;
            }
        }
        return nullptr;
    }
    Ingredient* FindByName(string name) const {
        for (Ingredient* item : ingredients) {
            if (item->GetName() == name) {
                return item;
            }
        }
        return nullptr;
    }
    int NextId() const {
        int maxId = 0;
        for (Ingredient* item : ingredients) {
            if (item->GetId() > maxId) {
                maxId = item->GetId();
            }
        }
        return maxId + 1;
    }
    void AddIngredient(Ingredient* ingredient) {
        if (ingredient == nullptr) {
            throw ValidationException("Yanlis inqrident...", __LINE__);
        }
        if (FindByName(ingredient->GetName()) != nullptr) {
            throw ValidationException(
                "Inqrident adi artiq movcuddur...", __LINE__);
        }
        ingredients.push_back(ingredient);
    }
    void IncreaseIngredient(int id, double amount) {
        Ingredient* item = FindIngredient(id);
        if (item == nullptr) {
            throw ValidationException("Inqrident tapilmadi...", __LINE__);
        }
        item->AddQuantity(amount);
    }
    void RemoveIngredient(int id, double amount) {
        Ingredient* item = FindIngredient(id);
        if (item == nullptr) {
            throw ValidationException("Inqrident tapilmadi...", __LINE__);
        }
        item->RemoveQuantity(amount);
    }
    void ShowAll() const {
        cout << left << setw(6) << "ID"
            << setw(22) << "Inqredient"
            << right << setw(12) << "Miqdar"
            << setw(14) << "AZN/Kg" << '\n';
        cout << string(54, '-') << '\n';
        for (Ingredient* item : ingredients) {
            item->Display();
        }
    }
    bool HasEnough(int id, double amount) const {
        Ingredient* item = FindIngredient(id);
        return item != nullptr &&
            item->GetQuantity() + 0.000001 >= amount;
    }
    bool CanPrepare(
        int count,
        const MyVector<RecipeIngredient>& recipe
    ) const {
        for (const RecipeIngredient& item : recipe) {
            if (!HasEnough(
                item.GetIngredientId(),
                item.GetQuantity() * count)) {
                return false;
            }
        }
        return true;
    }
    void Consume(
        int count,
        const MyVector<RecipeIngredient>& recipe
    ) {
        if (!CanPrepare(count, recipe)) {
            throw StockException(
                "Sifaris ucun stock kifayet qeder deyil...", __LINE__);
        }
        for (const RecipeIngredient& item : recipe) {
            RemoveIngredient(
                item.GetIngredientId(),
                item.GetQuantity() * count
            );
        }
    }
    void ReadFromFile() {
        for (Ingredient* item : ingredients) {
            delete item;
        }
        ingredients.clear();
        MyVector<string> lines =
            FileManager::ReadFromFile("ingredients.txt");
        for (const string& line : lines) {
            try {
                MyVector<string> p = Split(line);
                if (p.size() != 4) {
                    continue;
                }
                Ingredient* item = new Ingredient(
                    ToInt(p[0]),
                    p[1],
                    ToDouble(p[2]),
                    ToDouble(p[3])
                );
                ingredients.push_back(item);
            }
            catch (const MyException& e) {
                cout << "Ingredient record skipped: " << e.what() << endl;
            }
        }
    }
    void WriteToFile() const {
        MyVector<string> lines;
        for (Ingredient* item : ingredients) {
            lines.push_back(item->ToFile());
        }
        FileManager::WriteToFile(lines, "ingredients.txt");
    }
    bool Empty() const {
        return ingredients.empty();
    }
};


class Menu {
private:
    MyVector<Food*> foods;
public:
    Menu() {}
    Menu(const Menu&) = delete;
    Menu& operator=(const Menu&) = delete;
    ~Menu() {
        for (Food* food : foods) {
            delete food;
        }
    }
    Food* FindFood(int id) const {
        for (Food* food : foods) {
            if (food->GetId() == id) {
                return food;
            }
        }
        return nullptr;
    }
    int NextId() const {
        int maxId = 0;
        for (Food* food : foods) {
            if (food->GetId() > maxId) {
                maxId = food->GetId();
            }
        }
        return maxId + 1;
    }
    void AddFood(Food* food) {
        if (food == nullptr) {
            throw ValidationException("Yanlis yemek.", __LINE__);
        }
        if (FindFood(food->GetId()) != nullptr) {
            throw ValidationException("Yemek ID'i movcuddur.", __LINE__);
        }
        if (food->GetRecipe().empty()) {
            throw ValidationException(
                "Yemek en az 1 inqridente malik olmalidir.", __LINE__);
        }
        foods.push_back(food);
    }
    void RemoveFood(int id) {
        for (int i = 0; i < foods.size(); i++) {
            if (foods[i]->GetId() == id) {
                delete foods[i];
                for (int j = i; j < foods.size() - 1; j++) {
                    foods[j] = foods[j + 1];
                }
                foods.pop_back();
                return;
            }
        }
        throw ValidationException("Yemek tapilmadi.", __LINE__);
    }
    void ShowMenu(const Stock& stock, bool availableOnly = true) const {
        cout << left << setw(6) << "ID"
            << setw(22) << "Yemek"
            << setw(12) << "Qiymet"
            << setw(12) << "Zaman"
            << "Type\n";
        cout << string(60, '-') << '\n';
        bool found = false;
        for (Food* food : foods) {
            bool available =
                stock.CanPrepare(1, food->GetRecipe());
            if (!availableOnly || available) {
                food->Display();
                found = true;
            }
        }
        if (!found) {
            cout << "Hal-hazirda yemek movcud deyil.\n";
        }
    }
    void ReadFromFile(Stock& stock) {
        for (Food* food : foods) {
            delete food;
        }
        foods.clear();
        MyVector<string> foodLines =
            FileManager::ReadFromFile("foods.txt");
        MyVector<string> recipeLines =
            FileManager::ReadFromFile("recipes.txt");
        for (const string& line : foodLines) {
            try {
                MyVector<string> p = Split(line);
                if (p.size() != 6) {
                    continue;
                }
                int id = ToInt(p[0]);
                double price = ToDouble(p[4]);
                int time = ToInt(p[5]);
                Food* food = nullptr;
                if (p[1] == "Pizza") {
                    food = new Pizza(id, p[2], p[3], price, time);
                }
                else if (p[1] == "Burger") {
                    food = new Burger(id, p[2], p[3], price, time);
                }
                else if (p[1] == "Pasta") {
                    food = new Pasta(id, p[2], p[3], price, time);
                }
                if (food != nullptr) {
                    foods.push_back(food);
                }
            }
            catch (const MyException& e) {
                cout << "Qida qeydi buraxildi: "
                    << e.what() << '\n';
            }
        }
        for (const string& line : recipeLines) {
            try {
                MyVector<string> p = Split(line);
                if (p.size() != 3) {
                    continue;
                }
                int foodId = ToInt(p[0]);
                int ingredientId = ToInt(p[1]);
                double quantity = ToDouble(p[2]);
                Food* food = FindFood(foodId);
                Ingredient* ingredient =
                    stock.FindIngredient(ingredientId);
                if (food != nullptr && ingredient != nullptr) {
                    food->AddRecipeIngredient(
                        RecipeIngredient(
                            ingredientId,
                            ingredient->GetName(),
                            quantity
                        )
                    );
                }
            }
            catch (const MyException& e) {
                cout << "Resept qeydi buraxildi: "
                    << e.what() << '\n';
            }
        }
    }
    void WriteToFile() const {
        MyVector<string> foodLines;
        MyVector<string> recipeLines;
        for (Food* food : foods) {
            foodLines.push_back(food->ToFile());
            const MyVector<RecipeIngredient>& recipe =
                food->GetRecipe();
            for (const RecipeIngredient& item : recipe) {
                ostringstream ss;
                ss << food->GetId() << '|'
                    << item.GetIngredientId() << '|'
                    << setprecision(12) << item.GetQuantity();
                recipeLines.push_back(ss.str());
            }
        }
        FileManager::WriteToFile(foodLines, "foods.txt");
        FileManager::WriteToFile(recipeLines, "recipes.txt");
    }
    bool Empty() const {
        return foods.empty();
    }
};


class CartItem {
private:
    int foodId;
    int count;
    double unitPrice;
    string foodName;
    MyVector<RecipeIngredient> ingredients;
public:
    CartItem() {
        this->foodId = foodId;
        this->count = count;
        this->unitPrice = unitPrice;
        this->foodName = foodName;
    }
    CartItem(const Food& food) {
        foodId = food.GetId();
        count = 1;
        unitPrice = food.GetPrice();
        foodName = food.GetName();
        ingredients = food.GetRecipe();
    }
    int GetFoodId() const {
        return foodId;
    }
    int GetCount() const {
        return count;
    }
    double GetUnitPrice() const {
        return unitPrice;
    }
    string GetFoodName() const {
        return foodName;
    }
    const MyVector<RecipeIngredient>& GetIngredients() const {
        return ingredients;
    }
    void AddCount() {
        count++;
    }
    void SetIngredientQuantity(int ingredientId, double amount) {
        for (RecipeIngredient& item : ingredients) {
            if (item.GetIngredientId() == ingredientId) {
                item.SetQuantity(amount);
                return;
            }
        }
        throw ValidationException(
            "Inqrident yemeyin reseptinde movcud deyil.", __LINE__);
    }
    void AddExtraIngredient(
        int ingredientId,
        string ingredientName,
        double amount
    ) {
        if (amount <= 0) {
            throw ValidationException(
                "Elave miqdar musbet olmalidir.", __LINE__);
        }
        for (RecipeIngredient& item : ingredients) {
            if (item.GetIngredientId() == ingredientId) {
                item.SetQuantity(item.GetQuantity() + amount);
                return;
            }
        }
        ingredients.push_back(
            RecipeIngredient(ingredientId, ingredientName, amount)
        );
    }
    double GetTotal() const {
        return unitPrice * count;
    }
};

class Cart {
private:
    MyVector<CartItem> items;
public:
    void AddToCart(const Food& food) {
        for (CartItem& item : items) {
            if (item.GetFoodId() == food.GetId()) {
                item.AddCount();
                return;
            }
        }
        items.push_back(CartItem(food));
    }
    void RemoveFromCart(int foodId) {
        for (int i = 0; i < items.size(); i++) {
            if (items[i].GetFoodId() == foodId) {
                for (int j = i; j < items.size() - 1; j++) {
                    items[j] = items[j + 1];
                }
                items.pop_back();
                return;
            }
        }
        throw ValidationException("Yemek sebetde movcud deyil.", __LINE__);
    }
    CartItem* FindItem(int foodId) {
        for (CartItem& item : items) {
            if (item.GetFoodId() == foodId) {
                return &item;
            }
        }
        return nullptr;
    }
    const MyVector<CartItem>& GetItems() const {
        return items;
    }
    bool Empty() const {
        return items.empty();
    }
    double CalculateTotal() const {
        double total = 0;
        for (const CartItem& item : items) {
            total += item.GetTotal();
        }
        return total;
    }
    void ShowCart() const {
        if (items.empty()) {
            cout << "Sizin sebetiniz bosdur.\n";
            return;
        }
        cout << left << setw(6) << "ID"
            << setw(22) << "Yemek"
            << setw(10) << "Miqdar"
            << setw(12) << "Vahid"
            << "Umumi\n";
        cout << string(58, '-') << '\n';
        for (const CartItem& item : items) {
            cout << left << setw(6) << item.GetFoodId()
                << setw(22) << item.GetFoodName()
                << setw(10) << item.GetCount()
                << setw(12) << fixed << setprecision(2)
                << item.GetUnitPrice()
                << item.GetTotal() << '\n';
        }
        cout << "\nUMUMI: " << fixed << setprecision(2)
            << CalculateTotal() << " AZN\n";
    }
    void Clear() {
        items.clear();
    }
};


class Transaction {
private:
    int id;
    string type;
    double amount;
    string description;
    string date;
public:
    Transaction() {
        id = 0;
        type = "";
        amount = 0;
        description = "";
        date = "";
    }
    Transaction(int id, string type, double amount,
        string description, string date) {
        this->id = id;
        this->type = type;
        this->amount = amount;
        this->description = description;
        this->date = date;
    }
    string GetType() const {
        return type;
    }
    double GetAmount() const {
        return amount;
    }
    string GetDate() const {
        return date;
    }
    string ToFile() const {
        ostringstream ss;
        ss << id << '|'
            << type << '|'
            << setprecision(12) << amount << '|'
            << CleanField(description) << '|'
            << date;
        return ss.str();
    }
    void Display() const {
        cout << left << setw(6) << id
            << setw(12) << type
            << setw(12) << fixed << setprecision(2) << amount
            << setw(30) << description
            << date << endl;
    }
};


class Statistics {
private:
    MyVector<Transaction> transactions;
public:
    void ReadFromFile() {
        transactions.clear();
        MyVector<string> lines = FileManager::ReadFromFile("transactions.txt");
        for (const string& line : lines) {
            try {
                MyVector<string> p = Split(line);
                if (p.size() != 5) {
                    continue;
                }
                transactions.push_back(Transaction(ToInt(p[0]), p[1], ToDouble(p[2]), p[3], p[4]));
            }
            catch (const MyException& e) {
                cout << "Emeliyyat qeydi buraxildi: " << e.what() << endl;
            }
        }
    }

    int NextId() const {
        int maxId = 0;
        for (const Transaction& t : transactions) {
            MyVector<string> p = Split(t.ToFile());
            if (p.size() > 0) {
                int currentId = ToInt(p[0]);
                if (currentId > maxId) {
                    maxId = currentId;
                }
            }
        }
        return maxId + 1;
    }

    void AddTransaction(
        string type,
        double amount,
        string description
    ) {
        if (amount <= 0) {
            throw MyException("Emeliyyat miqdari musbet olmalidir.", __LINE__);
        }
        transactions.push_back(Transaction(NextId(), type, amount, description, CurrentDate()));
        WriteToFile();
    }
    void WriteToFile() const {
        MyVector<string> lines;
        for (const Transaction& t : transactions) {
            lines.push_back(t.ToFile());
        }
        FileManager::WriteToFile(lines, "transactions.txt");
    }
    double GetIncomeForDate(string date) const {
        double total = 0;
        for (const Transaction& t : transactions) {
            if (t.GetType() == "GELIR" &&
                t.GetDate() == date) {
                total += t.GetAmount();
            }
        }
        return total;
    }
    double GetExpenseForDate(string date) const {
        double total = 0;
        for (const Transaction& t : transactions) {
            if (t.GetType() == "XERC" &&
                t.GetDate() == date) {
                total += t.GetAmount();
            }
        }
        return total;
    }
    double GetMonthTotal(string month, string type) const {
        double total = 0;
        for (const Transaction& t : transactions) {
            if (t.GetType() == type &&
                t.GetDate().substr(0, 7) == month) {
                total += t.GetAmount();
            }
        }
        return total;
    }
    void ShowAll() const {
        cout << left << setw(6) << "ID"
            << setw(12) << "Nov"
            << setw(12) << "Miqdar"
            << setw(30) << "Tesvir"
            << "Tarix" << endl;
        cout << string(75, '-') << endl;
        for (const Transaction& t : transactions) {
            t.Display();
        }
    }
};


class RestaurantPayment {
private:
    double balance;
    Statistics* statistics;
public:
    RestaurantPayment(Statistics* statistics) {
        balance = 0;
        this->statistics = statistics;
    }
    void LoadBalance() {
        MyVector<string> lines =
            FileManager::ReadFromFile("restaurant.txt");
        if (!lines.empty()) {
            try {
                balance = ToDouble(lines[0]);
            }
            catch (...) {
                balance = 0;
            }
        }
    }
    void SaveBalance() const {
        MyVector<string> lines;
        ostringstream ss;
        ss << setprecision(12) << balance;
        lines.push_back(ss.str());
        FileManager::WriteToFile(lines, "restaurant.txt");
    }
    void AddIncome(double amount, string description) {
        if (amount <= 0) {
            throw ValidationException(
                "Gelir musbet olmalidir.", __LINE__);
        }
        balance += amount;
        SaveBalance();
        statistics->AddTransaction(
            "GELIR", amount, description);
    }
    void AddExpense(double amount, string description) {
        if (amount <= 0) {
            throw ValidationException(
                "Xercler musbet olmalidir.", __LINE__);
        }
        if (amount > balance) {
            throw MyException(
                "Restoran budcesi kifayet deyil.", __LINE__);
        }
        balance -= amount;
        SaveBalance();
        statistics->AddTransaction(
            "XERC", amount, description);
    }
    double GetBalance() const {
        return balance;
    }
};


class Payment {
public:
    virtual bool Pay(double amount) = 0;
    virtual ~Payment() {}
};
class CashPayment : public Payment {
public:
    bool Pay(double amount) override {
        cout << "\nNagd odenis: "
            << fixed << setprecision(2)
            << amount << " AZN" << endl;
        int choice = ReadInt(
            "Naqd odenisi tesdiqleyirsiniz? (1-Yes, 0-No): ");
        return choice == 1;
    }
};
class CardPayment : public Payment {
public:
    bool Pay(double amount) override {
        cout << "\nKart odenisi: "
            << fixed << setprecision(2)
            << amount << " AZN" << endl;
        string card = ReadText(
            "Kartin son 4 reqemini daxil edin: ");
        bool valid = true;
        if (card.size() != 4) {
            valid = false;
        }
        else {
            for (int i = 0; i < card.size(); i++) {
                if (!isdigit(card[i])) {
                    valid = false;
                    break;
                }
            }
        }
        if (!valid) {
            cout << "Etibarsiz kart girisi. 4 reqem daxil edin." << endl;
            return false;
        }
        cout << "Kart odenisi tesdiqlendi." << endl;
        return true;
    }
};


class Order {
private:
    int id;
    int userId;
    string username;
    string date;
    double total;
    string details;

public:
    Order() {
        id = 0;
        userId = 0;
        username = "";
        date = "";
        total = 0;
        details = "";
    }
    Order(int id, int userId, string username,
        string date, double total, string details) {

        this->id = id;
        this->userId = userId;
        this->username = username;
        this->date = date;
        this->total = total;
        this->details = details;
    }
    int GetUserId() const {
        return userId;
    }
    double GetTotal() const {
        return total;
    }
    string ToFile() const {
        ostringstream ss;
        ss << id << '|'
            << userId << '|'
            << CleanField(username) << '|'
            << date << '|'
            << setprecision(12) << total << '|'
            << CleanField(details);
        return ss.str();
    }
    void Display() const {
        cout << "Order #" << id
            << " | User: " << username
            << " | Date: " << date
            << " | Total: " << fixed << setprecision(2)
            << total << " AZN" << endl;
        cout << "Items: " << details << endl;
    }
};

class OrderHistory {
private:
    MyVector<Order> orders;
public:
    void ReadFromFile() {
        orders.clear();
        MyVector<string> lines =
            FileManager::ReadFromFile("orders.txt");
        for (const string& line : lines) {
            try {
                MyVector<string> p = Split(line);
                if (p.size() != 6) {
                    continue;
                }
                orders.push_back(
                    Order(ToInt(p[0]), ToInt(p[1]), p[2], p[3], ToDouble(p[4]), p[5])
                );
            }
            catch (const MyException& e) {
                cout << "Order record skipped: "
                    << e.what() << endl;
            }
        }
    }
    int NextId() const {
        int maxId = 0;
        for (const Order& order : orders) {
            MyVector<string> p = Split(order.ToFile());
            if (p.size() > 0) {
                int currentId = ToInt(p[0]);
                if (currentId > maxId) {
                    maxId = currentId;
                }
            }
        }
        return maxId + 1;
    }
    void AddOrder(const Order& order) {
        orders.push_back(order);
        WriteToFile();
    }
    void WriteToFile() const {
        MyVector<string> lines;
        for (const Order& order : orders) {
            lines.push_back(order.ToFile());
        }
        FileManager::WriteToFile(lines, "orders.txt");
    }
    void ShowUserHistory(int userId) const {
        bool found = false;
        for (const Order& order : orders) {
            if (order.GetUserId() == userId) {
                order.Display();
                cout << "--------------------------------------------------" << endl;
                found = true;
            }
        }
        if (!found) {
            cout << "No orders found." << endl;
        }
    }
    void ShowAllHistory() const {
        if (orders.empty()) {
            cout << "No orders found." << endl;
            return;
        }
        for (const Order& order : orders) {
            order.Display();
            cout << "--------------------------------------------------" << endl;
        }
    }
};

class Authentication {
private:
    MyVector<User> users;

public:
    void ReadFromFile() {
        users.clear();
        MyVector<string> lines =
            FileManager::ReadFromFile("users.txt");
        for (const string& line : lines) {
            try {
                MyVector<string> p = Split(line);
                if (p.size() != 6) {
                    continue;
                }
                users.push_back(
                    User(ToInt(p[0]), p[1], p[2], p[3], p[4], p[5] == "1")
                );
            }
            catch (const MyException& e) {
                cout << "User record skipped: "
                    << e.what() << endl;
            }
        }
    }
    void WriteToFile() const {
        MyVector<string> lines;
        for (const User& user : users) {
            lines.push_back(user.ToFile());
        }
        FileManager::WriteToFile(lines, "users.txt");
    }
    int NextId() const {
        int maxId = 0;
        for (const User& user : users) {
            if (user.GetId() > maxId) {
                maxId = user.GetId();
            }
        }
        return maxId + 1;
    }
    bool UsernameExists(string username) const {
        for (const User& user : users) {
            if (user.GetUsername() == username) {
                return true;
            }
        }
        return false;
    }
    void RegisterUser() {
        Header("USER REGISTRATION");
        string username = ReadText("Username: ");
        if (username.empty()) {
            throw ValidationException(
                "Username cannot be empty.", __LINE__);
        }
        if (username.find('|') != string::npos) {
            throw ValidationException(
                "Username cannot contain | character.", __LINE__);
        }
        if (UsernameExists(username)) {
            throw ValidationException(
                "Username already exists.", __LINE__);
        }
        string password = ReadText("Password: ");
        if (password.size() < 4) {
            throw ValidationException(
                "Password must contain at least 4 characters.", __LINE__);
        }
        string fullname = ReadText("Full name: ");
        string phone = ReadText("Phone: ");
        if (fullname.empty() || phone.empty()) {
            throw ValidationException(
                "Full name and phone are required.", __LINE__);
        }
        users.push_back(
            User(NextId(), username, password, fullname, phone, true)
        );
        WriteToFile();
        ShowMessage("Registration completed successfully.");
    }
    User* LoginUser() {
        Header("USER LOGIN");
        string username = ReadText("Username: ");
        string password = ReadText("Password: ");
        for (User& user : users) {
            if (user.GetUsername() == username &&
                user.CheckPassword(password)) {
                if (!user.IsActive()) {
                    throw ValidationException(
                        "This account is blocked.", __LINE__);
                }
                return &user;
            }
        }
        throw ValidationException(
            "Incorrect username or password.", __LINE__);
    }
    void ShowUsers() const {
        cout << left << setw(6) << "ID" << setw(18) << "Username" << setw(25) << "Full name" << setw(16) << "Phone" << "Status" << endl;
        cout << string(75, '-') << endl;
        for (const User& user : users) {
            user.Display();
        }
    }
    void ToggleUserStatus(int id) {
        for (User& user : users) {
            if (user.GetId() == id) {
                user.SetActive(!user.IsActive());
                WriteToFile();
                return;
            }
        }
        throw ValidationException("User not found.", __LINE__);
    }
};

class RestaurantSystem {
private:
    Authentication authentication;
    Stock stock;
    Menu menu;
    Statistics statistics;
    RestaurantPayment payment;
    OrderHistory history;
    Admin admin;

    int currentUserId;
    string currentUsername;

public:
    RestaurantSystem()
        : payment(&statistics),
        admin(1, "admin", "admin123") {

        currentUserId = 0;
        currentUsername = "";

        LoadAll();
        SeedDemoData();
    }

    void LoadAll() {
        authentication.ReadFromFile();
        stock.ReadFromFile();
        menu.ReadFromFile(stock);
        statistics.ReadFromFile();
        payment.LoadBalance();
        history.ReadFromFile();
    }

    void SaveAll() {
        authentication.WriteToFile();
        stock.WriteToFile();
        menu.WriteToFile();
        statistics.WriteToFile();
        history.WriteToFile();
        payment.SaveBalance();
    }

    void SeedDemoData() {
        if (stock.Empty()) {
            stock.AddIngredient(
                new Ingredient(stock.NextId(), "Flour", 20000, 2.0));

            stock.AddIngredient(
                new Ingredient(stock.NextId(), "Cheese", 10000, 12.0));

            stock.AddIngredient(
                new Ingredient(stock.NextId(), "Tomato", 10000, 4.0));

            stock.AddIngredient(
                new Ingredient(stock.NextId(), "Sausage", 5000, 15.0));

            stock.AddIngredient(
                new Ingredient(stock.NextId(), "Beef", 8000, 18.0));

            stock.AddIngredient(
                new Ingredient(stock.NextId(), "Lettuce", 5000, 5.0));

            stock.AddIngredient(
                new Ingredient(stock.NextId(), "Pasta", 10000, 5.0));

            stock.AddIngredient(
                new Ingredient(stock.NextId(), "Cream", 5000, 8.0));

            stock.AddIngredient(
                new Ingredient(stock.NextId(), "Bun", 5000, 3.0));

            stock.AddIngredient(
                new Ingredient(stock.NextId(), "Sauce", 5000, 6.0));
        }

        if (menu.Empty()) {
            Food* pizza = new Pizza(
                menu.NextId(),
                "Margherita Pizza",
                "Classic pizza with cheese and tomato.",
                12.50,
                20
            );

            pizza->AddRecipeIngredient(
                RecipeIngredient(1, "Flour", 200));

            pizza->AddRecipeIngredient(
                RecipeIngredient(2, "Cheese", 50));

            pizza->AddRecipeIngredient(
                RecipeIngredient(3, "Tomato", 100));

            menu.AddFood(pizza);


            Food* burger = new Burger(
                menu.NextId(),
                "Beef Burger",
                "Beef burger with fresh lettuce and sauce.",
                9.50,
                15
            );

            burger->AddRecipeIngredient(
                RecipeIngredient(5, "Beef", 150));

            burger->AddRecipeIngredient(
                RecipeIngredient(6, "Lettuce", 30));

            burger->AddRecipeIngredient(
                RecipeIngredient(9, "Bun", 1));

            burger->AddRecipeIngredient(
                RecipeIngredient(10, "Sauce", 25));

            menu.AddFood(burger);


            Food* pasta = new Pasta(
                menu.NextId(),
                "Cream Pasta",
                "Pasta with cream sauce.",
                10.00,
                18
            );

            pasta->AddRecipeIngredient(
                RecipeIngredient(7, "Pasta", 180));

            pasta->AddRecipeIngredient(
                RecipeIngredient(8, "Cream", 70));

            pasta->AddRecipeIngredient(
                RecipeIngredient(2, "Cheese", 30));

            menu.AddFood(pasta);
        }

        stock.WriteToFile();
        menu.WriteToFile();
    }

    void AdminLogin() {
        Header("ADMIN LOGIN");

        string username = ReadText("Username: ");
        string password = ReadText("Password: ");

        if (username != admin.GetUsername() ||
            !admin.CheckPassword(password)) {

            throw ValidationException(
                "Invalid admin credentials.", __LINE__);
        }

        admin.Display();
        AdminPanel();
    }

    void UserAccess() {
        while (true) {
            Header("USER ACCESS");

            cout << "1. Register\n";
            cout << "2. Login\n";
            cout << "0. Back\n";

            int choice = ReadInt("Select: ");

            if (choice == 1) {
                try {
                    authentication.RegisterUser();
                }
                catch (const MyException& e) {
                    ShowMessage(e.what());
                }

                Pause();
            }
            else if (choice == 2) {
                try {
                    User* user = authentication.LoginUser();
                    OpenUserPanel(*user);
                }
                catch (const MyException& e) {
                    ShowMessage(e.what());
                    Pause();
                }
            }
            else if (choice == 0) {
                return;
            }
            else {
                ShowMessage("Invalid selection.");
            }
        }
    }

    void AdminPanel() {
        while (true) {
            Header("ADMIN PANEL");

            cout << "1. Manage Menu\n";
            cout << "2. Manage Stock\n";
            cout << "3. View Orders\n";
            cout << "4. Statistics\n";
            cout << "5. Manage Users\n";
            cout << "6. Restaurant Budget\n";
            cout << "7. Transaction History\n";
            cout << "0. Logout\n";

            int choice = ReadInt("\nSelect: ");

            try {
                if (choice == 1) {
                    MenuManagement();
                }
                else if (choice == 2) {
                    StockManagement();
                }
                else if (choice == 3) {
                    Header("ALL ORDERS");
                    history.ShowAllHistory();
                    Pause();
                }
                else if (choice == 4) {
                    ShowStatistics();
                }
                else if (choice == 5) {
                    UserManagement();
                }
                else if (choice == 6) {
                    Header("RESTAURANT BUDGET");

                    cout << "Current budget: "
                        << fixed << setprecision(2)
                        << payment.GetBalance() << " AZN\n";

                    Pause();
                }
                else if (choice == 7) {
                    Header("TRANSACTION HISTORY");
                    statistics.ShowAll();
                    Pause();
                }
                else if (choice == 0) {
                    return;
                }
                else {
                    ShowMessage("Invalid selection.");
                }
            }
            catch (const MyException& e) {
                ShowMessage(e.what());
                Pause();
            }
        }
    }

    void MenuManagement() {
        while (true) {
            Header("MENU MANAGEMENT");
            cout << "1. Show all foods\n";
            cout << "2. Add food\n";
            cout << "3. Remove food\n";
            cout << "4. Update food price\n";
            cout << "0. Back\n";
            int choice = ReadInt("Select: ");
            if (choice == 1) {
                menu.ShowMenu(stock, false);
                Pause();
            }
            else if (choice == 2) {
                cout << "Food type: 1-Pizza  2-Burger  3-Pasta\n";
                int type = ReadInt("Type: ");
                string name = ReadText("Food name: ");
                string description = ReadText("Description: ");
                double priceValue = ReadDouble("Price (AZN): ");
                int time = ReadInt("Preparation time (minutes): ");
                Food* food = nullptr;
                int id = menu.NextId();
                if (type == 1) {
                    food = new Pizza(id, name, description, priceValue, time);
                }
                else if (type == 2) {
                    food = new Burger(id, name, description, priceValue, time);
                }
                else if (type == 3) {
                    food = new Pasta(id, name, description, priceValue, time);
                }
                else {
                    throw ValidationException(
                        "Invalid food type.", __LINE__);
                }
                bool added = false;
                try {
                    cout << "\nAvailable ingredients:\n";
                    stock.ShowAll();
                    int count = ReadInt(
                        "How many ingredients in recipe? ");
                    if (count <= 0) {
                        throw ValidationException(
                            "Food must have at least one ingredient.", __LINE__);
                    }
                    for (int i = 0; i < count; i++) {
                        int ingredientId = ReadInt("Ingredient ID: ");
                        Ingredient* ingredient =
                            stock.FindIngredient(ingredientId);
                        if (ingredient == nullptr) {
                            throw ValidationException(
                                "Ingredient ID does not exist.", __LINE__);
                        }
                        double amount =
                            ReadDouble("Quantity in grams per portion: ");
                        food->AddRecipeIngredient(
                            RecipeIngredient(
                                ingredientId,
                                ingredient->GetName(),
                                amount
                            )
                        );
                    }
                    menu.AddFood(food);
                    added = true;
                    menu.WriteToFile();
                    ShowMessage("Food added successfully.");
                }
                catch (...) {
                    if (!added) {
                        delete food;
                    }
                    throw;
                }
            }
            else if (choice == 3) {
                menu.ShowMenu(stock, false);
                int id = ReadInt("Food ID to remove: ");
                menu.RemoveFood(id);
                menu.WriteToFile();
                ShowMessage("Food removed.");
            }
            else if (choice == 4) {
                menu.ShowMenu(stock, false);
                int id = ReadInt("Food ID: ");
                Food* food = menu.FindFood(id);
                if (food == nullptr) {
                    throw ValidationException(
                        "Food not found.", __LINE__);
                }
                double newPrice = ReadDouble("New price: ");
                food->SetPrice(newPrice);
                menu.WriteToFile();
                ShowMessage("Price updated.");
            }
            else if (choice == 0) {
                return;
            }
            else {
                ShowMessage("Invalid selection.");
            }
        }
    }
    void StockManagement() {
        while (true) {
            Header("STOCK MANAGEMENT");
            cout << "1. Show stock\n";
            cout << "2. Add new ingredient\n";
            cout << "3. Increase existing ingredient\n";
            cout << "4. Decrease ingredient\n";
            cout << "0. Back\n";
            int choice = ReadInt("Select: ");
            if (choice == 1) {
                stock.ShowAll();
                Pause();
            }
            else if (choice == 2) {
                string name = ReadText("Ingredient name: ");
                double quantity = ReadDouble("Initial quantity (grams): ");
                double pricePerKg = ReadDouble("Price per kg (AZN): ");
                if (quantity <= 0 || pricePerKg <= 0) {
                    throw ValidationException(
                        "Quantity and price must be positive.", __LINE__);
                }
                double cost = quantity / 1000.0 * pricePerKg;
                if (cost > payment.GetBalance()) {
                    throw MyException(
                        "Not enough restaurant budget.", __LINE__);
                }
                Ingredient* ingredient = new Ingredient(
                    stock.NextId(), name, 0, pricePerKg
                );
                try {
                    stock.AddIngredient(ingredient);
                }
                catch (...) {
                    delete ingredient;
                    throw;
                }
                stock.IncreaseIngredient(
                    ingredient->GetId(), quantity);
                payment.AddExpense(
                    cost, "Stock purchase: " + name);
                stock.WriteToFile();
                ShowMessage(
                    "Ingredient added. Expense: " +
                    to_string(cost) + " AZN"
                );
            }
            else if (choice == 3) {
                stock.ShowAll();
                int id = ReadInt("Ingredient ID: ");
                double quantity = ReadDouble("Additional quantity (grams): ");
                Ingredient* ingredient = stock.FindIngredient(id);
                if (ingredient == nullptr) {
                    throw ValidationException(
                        "Ingredient not found.", __LINE__);
                }
                double cost = quantity / 1000.0 * ingredient->GetPricePerKg();
                if (cost > payment.GetBalance()) {
                    throw MyException(
                        "Not enough restaurant budget.", __LINE__);
                }
                payment.AddExpense(
                    cost,
                    "Stock replenishment: " + ingredient->GetName()
                );
                stock.IncreaseIngredient(id, quantity);
                stock.WriteToFile();
                ShowMessage("Stock increased. Expense: " +
                    to_string(cost) + " AZN"
                );
            }
            else if (choice == 4) {
                stock.ShowAll();
                int id = ReadInt("Ingredient ID: ");
                double quantity = ReadDouble("Quantity to remove (grams): ");
                stock.RemoveIngredient(id, quantity);
                stock.WriteToFile();
                ShowMessage("Stock quantity decreased.");
            }
            else if (choice == 0) {
                return;
            }
            else {
                ShowMessage("Invalid selection.");
            }
        }
    }
    void ShowStatistics() {
        Header("RESTAURANT STATISTICS");
        string today = CurrentDate();
        string month = today.substr(0, 7);
        double dailyIncome =
            statistics.GetIncomeForDate(today);
        double dailyExpense =
            statistics.GetExpenseForDate(today);
        double monthlyIncome =
            statistics.GetMonthTotal(month, "INCOME");
        double monthlyExpense =
            statistics.GetMonthTotal(month, "EXPENSE");
        cout << "Date: " << today << "\n";
        cout << "\nDAILY\n";
        cout << "Income:  " << fixed << setprecision(2)
            << dailyIncome << " AZN\n";
        cout << "Expense: " << dailyExpense << " AZN\n";
        cout << "Profit:  "
            << dailyIncome - dailyExpense << " AZN\n";
        cout << "\nMONTHLY (" << month << ")\n";
        cout << "Income:  " << monthlyIncome << " AZN\n";
        cout << "Expense: " << monthlyExpense << " AZN\n";
        cout << "Profit:  "
            << monthlyIncome - monthlyExpense << " AZN\n";
        cout << "\nCURRENT BUDGET: "
            << payment.GetBalance() << " AZN\n";
        Pause();
    }
    void UserManagement() {
        Header("USER MANAGEMENT");
        authentication.ShowUsers();
        cout << "\n1. Block/Unblock user\n";
        cout << "0. Back\n";
        int choice = ReadInt("Select: ");
        if (choice == 1) {
            int id = ReadInt("User ID: ");
            authentication.ToggleUserStatus(id);
            ShowMessage("User status changed.");
        }
        else if (choice != 0) {
            ShowMessage("Invalid selection.");
        }
        Pause();
    }
    void UserPanel(User& user) {
        Cart cart;
        while (true) {
            Header("USER PANEL - Welcome " + user.GetUsername());
            cout << "1. Show menu\n";
            cout << "2. Food information\n";
            cout << "3. Add food to cart\n";
            cout << "4. Shopping cart\n";
            cout << "5. Order history\n";
            cout << "6. My profile\n";
            cout << "0. Logout\n";
            int choice = ReadInt("Select: ");
            try {
                if (choice == 1) {
                    menu.ShowMenu(stock, true);
                    Pause();
                }
                else if (choice == 2) {
                    menu.ShowMenu(stock, true);
                    int id = ReadInt("Food ID: ");
                    Food* food = menu.FindFood(id);
                    if (food == nullptr) {
                        throw ValidationException(
                            "Food not found.", __LINE__);
                    }
                    food->DisplayInfo();
                    Pause();
                }
                else if (choice == 3) {
                    menu.ShowMenu(stock, true);
                    int id = ReadInt("Food ID: ");
                    Food* food = menu.FindFood(id);
                    if (food == nullptr) {
                        throw ValidationException(
                            "Food not found.", __LINE__);
                    }
                    if (!stock.CanPrepare(1, food->GetRecipe())) {
                        throw StockException(
                            "This food is currently unavailable.", __LINE__);
                    }
                    cart.AddToCart(*food);
                    ShowMessage("Food added to cart.");
                    Pause();
                }
                else if (choice == 4) {
                    CartManagement(cart);
                }
                else if (choice == 5) {
                    Header("MY ORDER HISTORY");
                    history.ShowUserHistory(user.GetId());
                    Pause();
                }
                else if (choice == 6) {
                    Header("MY PROFILE");
                    cout << "ID: " << user.GetId()
                        << "\nUsername: " << user.GetUsername()
                        << "\nFull name: " << user.GetFullname()
                        << "\nPhone: " << user.GetPhone() << '\n';
                    Pause();
                }
                else if (choice == 0) {
                    cart.Clear();
                    return;
                }
                else {
                    ShowMessage("Invalid selection.");
                }
            }
            catch (const MyException& e) {
                ShowMessage(e.what());
                Pause();
            }
        }
    }
    void CartManagement(Cart& cart) {
        while (true) {
            Header("SHOPPING CART");
            cart.ShowCart();
            cout << "\n1. Remove food\n";
            cout << "2. Change ingredient quantity\n";
            cout << "3. Add extra ingredient\n";
            cout << "4. Checkout\n";
            cout << "0. Back\n";
            int choice = ReadInt("Select: ");
            if (choice == 1) {
                int id = ReadInt("Food ID to remove: ");
                cart.RemoveFromCart(id);
                ShowMessage("Item removed.");
            }
            else if (choice == 2) {
                int id = ReadInt("Food ID: ");
                CartItem* item = cart.FindItem(id);
                if (item == nullptr) {
                    throw ValidationException(
                        "Cart item not found.", __LINE__);
                }
                cout << "Recipe ingredients:\n";
                for (const RecipeIngredient& recipe :
                    item->GetIngredients()) {
                    cout << recipe.GetIngredientId()
                        << " - " << recipe.GetIngredientName()
                        << ": " << recipe.GetQuantity() << " grams\n";
                }
                int ingredientId = ReadInt("Ingredient ID: ");
                double amount =
                    ReadDouble("New amount in grams per portion: ");
                item->SetIngredientQuantity(ingredientId, amount);
                ShowMessage("Ingredient quantity updated.");
            }
            else if (choice == 3) {
                int id = ReadInt("Food ID: ");
                CartItem* item = cart.FindItem(id);
                if (item == nullptr) {
                    throw ValidationException(
                        "Cart item not found.", __LINE__);
                }
                stock.ShowAll();
                int ingredientId = ReadInt("Extra ingredient ID: ");
                Ingredient* ingredient =
                    stock.FindIngredient(ingredientId);
                if (ingredient == nullptr) {
                    throw ValidationException(
                        "Ingredient not found.", __LINE__);
                }
                double amount = ReadDouble("Extra amount in grams per portion: ");
                item->AddExtraIngredient(
                    ingredientId,
                    ingredient->GetName(),
                    amount
                );
                ShowMessage("Extra ingredient added.");
            }
            else if (choice == 4) {
                Checkout(cart);
                return;
            }
            else if (choice == 0) {
                return;
            }
            else {
                ShowMessage("Invalid selection.");
            }
        }
    }
    void Checkout(Cart& cart) {
        if (cart.Empty()) {
            throw ValidationException("Your cart is empty.", __LINE__);
        }
        Header("ORDER CHECKOUT");
        cart.ShowCart();
        cout << "\n1. Cash\n";
        cout << "2. Card (demo)\n";
        cout << "0. Cancel\n";
        int choice = ReadInt("Payment method: ");
        if (choice == 0) {
            return;
        }
        CashPayment cash;
        CardPayment card;
        Payment* method = nullptr;
        if (choice == 1) {
            method = &cash;
        }
        else if (choice == 2) {
            method = &card;
        }
        else {
            throw ValidationException(
                "Invalid payment method.", __LINE__);
        }
        for (const CartItem& item : cart.GetItems()) {
            if (!stock.CanPrepare(
                item.GetCount(), item.GetIngredients())) {
                throw StockException("Insufficient stock for " + item.GetFoodName(), __LINE__);
            }
        }
        double total = cart.CalculateTotal();
        if (!method->Pay(total)) {
            throw PaymentException(
                "Payment was cancelled or declined.", __LINE__);
        }
        for (const CartItem& item : cart.GetItems()) {
            stock.Consume(
                item.GetCount(),
                item.GetIngredients()
            );
        }
        payment.AddIncome(total, "Customer order payment");
        ostringstream details;
        for (const CartItem& item : cart.GetItems()) {
            details << item.GetFoodName()
                << " x" << item.GetCount() << "; ";
        }
        Order order(
            history.NextId(),
            currentUserId,
            currentUsername,
            CurrentDate(),
            total,
            details.str()
        );
        history.AddOrder(order);
        stock.WriteToFile();
        cart.Clear();
        ShowMessage(
            "Payment successful. Your order has been confirmed.");
        cout << "Paid: " << fixed << setprecision(2)
            << total << " AZN\n";
        Pause();
    }
    void OpenUserPanel(User& user) {
        currentUserId = user.GetId();
        currentUsername = user.GetUsername();
        UserPanel(user);
    }
    void Run() {
        while (true) {
            Header("MAIN MENU");
            cout << "1. Admin login\n";
            cout << "2. User panel\n";
            cout << "0. Exit\n";
            int choice = ReadInt("\nSelect: ");
            try {
                if (choice == 1) {
                    AdminLogin();
                }
                else if (choice == 2) {
                    UserAccess();
                }
                else if (choice == 0) {
                    SaveAll();
                    ShowMessage("All data saved. Goodbye!");
                    return;
                }
                else {
                    ShowMessage("Invalid selection.");
                }
            }
            catch (const MyException& e) {
                ShowMessage(e.what());
                Pause();
            }
        }
    }
};

int main() {
    try {
        RestaurantSystem restaurant;
        restaurant.Run();
    }
    catch (const MyException& e) {
        cout << "\nFATAL ERROR: " << e.what() << '\n';
        return 1;
    }
    catch (...) {
        cout << "\nUNKNOWN FATAL ERROR.\n";
        return 1;
    }
    return 0;
}