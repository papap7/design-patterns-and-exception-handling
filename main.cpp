// E-Commerce System - Applying Singleton pattern, Stategy Pattern for payment, Exception Handling, Four pillars of OOP while avoiding STLs(vectors, maps, algorithms, etc.)

#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <limits>
using namespace std;

bool isValidId(const string &input)
{
    if (input.length() != 3)
        return false;
    for (char c : input)
    {
        if (!isdigit(c))
            return false;
    }
    return true;
}

bool isValidYesOrNo()
{
    for (;;)
    {
        string tempChoice;
        getline(cin, tempChoice);
        cin.clear();
        if (tempChoice.empty())
        {
            cout << "Invalid input. Please input [(Y) YES | (N) NO]: ";
            continue;
        }
        if (tempChoice.length() != 1)
        {
            cout << "Invalid input. Please input [(Y) YES | (N) NO]: ";
            continue;
        }
        char choice = toupper(tempChoice[0]);
        if (choice == 'Y')
        {
            return true;
        }
        else if (choice == 'N')
        {
            return false;
        }
        else
        {
            cout << "Invalid input. Please input [(Y) YES | (N) NO]: ";
            continue;
        }
    }
}

class ECommerceException
{
private:
    string _message;

public:
    ECommerceException(const string &msg) : _message(msg) {}
    string what() const
    {
        return _message;
    }
};

class FindProductIdException : public ECommerceException
{
public:
    FindProductIdException(const string &msg) : ECommerceException(msg) {}
};

class EmptyCartException : public ECommerceException
{
public:
    EmptyCartException(const string &msg) : ECommerceException(msg) {}
};

class PaymentException : public ECommerceException
{
public:
    PaymentException(const string &msg) : ECommerceException(msg) {}
};

class Product
{
private:
    string id, name;
    double price;

public:
    Product() : id(""), name(""), price(0.0) {}
    Product(string id, string name, double price) : id(id), name(name), price(price) {}

    string getId() { return id; }
    string getName() { return name; }
    double getPrice() { return price; }

    void display()
    {
        cout << left
             << setw(15) << id
             << setw(25) << name
             << '$' << setw(14) << fixed << setprecision(2) << price << "\n";
    }
};

class CartItem
{
private:
    Product products;
    int quantity;

public:
    CartItem(const Product &products, int quantity) : products(products), quantity(quantity) {}

    Product getProduct() { return products; }
    int getQuantity() { return quantity; }
    void incrementQuantity() { quantity++; }

    void display()
    {
        cout << left
             << setw(15) << products.getId()
             << setw(25) << products.getName()
             << '$' << setw(14) << fixed << setprecision(2) << products.getPrice()
             << setw(10) << quantity << "\n";
    }

    double getTotalPrice()
    {
        return products.getPrice() * quantity;
    }
};

class PaymentStrategy
{
public:
    virtual ~PaymentStrategy() {}
    virtual void pay(double amount) = 0;
    virtual string getPaymentMethod() const = 0;
};

class CashPayment : public PaymentStrategy
{
public:
    void pay(double amount) override
    {
        cout << "Paid $" << fixed << setprecision(2) << amount << " using Cash" << endl;
    }

    string getPaymentMethod() const override
    {
        return "Cash";
    }
};

class CreditCardPayment : public PaymentStrategy
{
public:
    void pay(double amount) override
    {
        cout << "Paid $" << fixed << setprecision(2) << amount << " using Credit/Debit Card" << endl;
    }

    string getPaymentMethod() const override
    {
        return "Credit / Debit Card";
    }
};

class GCashPayment : public PaymentStrategy
{
public:
    void pay(double amount) override
    {
        cout << "Paid $" << fixed << setprecision(2) << amount << " using GCash" << endl;
    }

    string getPaymentMethod() const override
    {
        return "GCash";
    }
};

class Order
{
private:
    static int nextOrderId;
    int orderId;
    CartItem **cartItems;
    int itemCount;
    double totalAmount;
    string paymentMethod;

public:
    Order(CartItem **items, int count, string _paymentMethod)
        : itemCount(count), paymentMethod(_paymentMethod)
    {
        orderId = nextOrderId++;
        totalAmount = 0.0;

        if (itemCount > 0)
        {
            cartItems = new CartItem *[itemCount];
            for (int i = 0; i < itemCount; i++)
            {
                cartItems[i] = new CartItem(*(items[i]));
                totalAmount += cartItems[i]->getTotalPrice();
            }
        }
        else
        {
            cartItems = nullptr;
        }
    }

    ~Order()
    {
        if (cartItems != nullptr)
        {
            for (int i = 0; i < itemCount; i++)
            {
                delete cartItems[i];
            }
            delete[] cartItems;
        }
    }

    int getOrderId() const { return orderId; }
    double getTotalAmount() const { return totalAmount; }
    string getPaymentMethod() const { return paymentMethod; }

    void display() const
    {
        cout << "\nOrder ID: " << orderId << endl;
        cout << "Total Amount: $" << fixed << setprecision(2) << totalAmount << endl;
        cout << "Payment Method: " << paymentMethod << endl;
        cout << "Order Details:" << endl;
        cout << left << setw(15) << "PRODUCT ID" << setw(25) << "NAME"
             << setw(14) << "PRICE" << setw(10) << "QUANTITY" << endl;

        for (int i = 0; i < itemCount; i++)
        {
            cartItems[i]->display();
        }
        cout << endl;
    }
};

int Order::nextOrderId = 1;

class ECommerceStore
{
private:
    Product *products;
    int productCount;

    CartItem **cart;
    int cartSize;

    Order **orders;
    int orderCount;
    int orderSize;
    int orderCapacity;

private:
    ECommerceStore()
    {
        initializeProducts();

        cartSize = 0;
        cart = nullptr;

        orderCount = 0;
        orderCapacity = 2; // Initial capacity
        orders = new Order *[orderCapacity];
    }
    void initializeProducts()
    {
        productCount = 5;
        products = new Product[productCount];

        products[0] = Product("001", "Socks Set", 10.90);
        products[1] = Product("002", "Basketball", 20.95);
        products[2] = Product("003", "T-Shirt 2 Pack", 7.99);
        products[3] = Product("004", "Toaster", 18.99);
        products[4] = Product("005", "Sunglasses", 15.60);
    }

    Product &findProductById(const string &id)
    {
        for (int i = 0; i < productCount; i++)
        {
            if (products[i].getId() == id)
            {
                return products[i];
            }
        }
        throw FindProductIdException("Product ID not found: " + id);
    }

    int findProductInCart(const string &productId)
    {
        for (int i = 0; i < cartSize; i++)
        {
            if (cart[i]->getProduct().getId() == productId)
            {
                return i;
            }
        }
        return -1;
    }

public:
    ~ECommerceStore()
    {
        delete[] products;

        if (cart != nullptr)
        {
            for (int i = 0; i < cartSize; i++)
            {
                delete cart[i];
            }
            delete[] cart;
        }

        if (orders != nullptr)
        {
            for (int i = 0; i < orderCount; i++)
            {
                delete orders[i];
            }
            delete[] orders;
        }
    }

    static ECommerceStore &get_instance()
    {
        static ECommerceStore instance;
        return instance;
    }

    ECommerceStore(const ECommerceStore &) = delete;
    void displayProducts()
    {
        cout << "\n"
             << left
             << string(23, ' ') << "PRODUCTS\n"
             << string(54, '-') << "\n"
             << setw(15) << "PRODUCT ID" << setw(25) << "NAME" << setw(14) << "PRICE" << "\n"
             << string(54, '-') << "\n";

        for (int i = 0; i < productCount; i++)
        {
            products[i].display();
        }
        cout << string(54, '-') << "\n";
    }

    void addToCart(const string &productId)
    {
        try
        {
            Product &product = findProductById(productId);
            int index = findProductInCart(productId);
            if (index != -1)
            {
                cart[index]->incrementQuantity();
            }
            else
            {
                CartItem **newCart = new CartItem *[cartSize + 1];
                for (int i = 0; i < cartSize; i++)
                {
                    newCart[i] = cart[i];
                }
                newCart[cartSize] = new CartItem(product, 1);
                delete[] cart;
                cart = newCart;
                cartSize++;
            }
            cout << "Product added successfully!\n";
        }
        catch (const ECommerceException &e)
        {
            cout << "Error: " << e.what() << "\n";
        }
    }

    void displayCart()
    {
        if (cartSize == 0)
        {
            return;
        }
        cout << string(30, ' ') << "CART\n";
        cout << left
             << string(64, '-') << "\n"
             << setw(15) << "PRODUCT ID" << setw(25) << "NAME" << setw(14) << "PRICE" << setw(10) << "QUANTITY" << "\n"
             << string(64, '-') << "\n";

        for (int i = 0; i < cartSize; i++)
        {
            cart[i]->display();
        }
        cout << string(64, '-') << "\n";

        double total = calculateCartTotal();
        cout << "Total Amount: $" << fixed << setprecision(2) << total << "\n\n";
    }

    double calculateCartTotal()
    {
        double total = 0.0;
        for (int i = 0; i < cartSize; i++)
        {
            total += cart[i]->getTotalPrice();
        }
        return total;
    }

    void checkout(PaymentStrategy *paymentStrategy)
    {
        try
        {
            if (paymentStrategy == nullptr)
            {
                throw PaymentException("Payment strategy is not set.");
            }

            if (cartSize == 0)
            {
                throw EmptyCartException("Cannot checkout an empty cart");
            }

            double total = calculateCartTotal();
            try
            {
                paymentStrategy->pay(total);
            }
            catch (const exception &e)
            {
                cout << "Payment processing failed: " << e.what() << endl;
                throw PaymentException("Payment processing failed: " + string(e.what()));
            }
            catch (...)
            {
                cout << "Payment processing failed due to an unknown error." << endl;
                throw PaymentException("Payment processing failed due to an unknown error.");
            }
            orders[orderCount] = new Order(cart, cartSize, paymentStrategy->getPaymentMethod());
            logOrder(orders[orderCount]);

            cout << "You have successfully checked out the product/s!\n";

            orderCount++;
            clearCart();
        }
        catch (const ECommerceException &e)
        {
            cout << "Error: " << e.what() << endl;
        }
    }

    void clearCart()
    {
        for (int i = 0; i < cartSize; i++)
        {
            delete cart[i];
        }
        delete[] cart;
        cart = nullptr;
        cartSize = 0;
    }
    void logOrder(const Order *order)
    {
        try
        {
            ofstream logFile("order_log.txt", ios::app);
            if (!logFile)
            {
                throw ECommerceException("Cannot open log file");
            }

            time_t now = time(0);
            tm *ltm = localtime(&now);

            logFile << "[LOG] -> Order ID: " << order->getOrderId()
                    << " has been successfully checked out and paid using "
                    << order->getPaymentMethod() << ". "
                    << endl;

            logFile.close();
        }
        catch (const exception &e)
        {
            cout << "Warning: Failed to log order: " << e.what() << endl;
        }
    }
    void displayOrders()
    {
        if (orderCount == 0)
        {
            cout << "No orders found.\n\n";
            return;
        }
        cout << string(29, ' ') << "ORDERS\n"
             << string(64, '-') << "\n";
        for (int i = 0; i < orderCount; i++)
        {
            orders[i]->display();
        }
    }
};

int main()
{
    ECommerceStore &store = ECommerceStore::get_instance();

    int choice;
    string productId;

    for (;;)
    {
        cout << string(13, ' ') << "E-COMMERCE SYSTEM\n";
        cout << string(42, '-') << "\n";
        cout << "1. View Products\n";
        cout << "2. View Shopping Cart\n";
        cout << "3. View Orders\n";
        cout << "4. Exit\n\n";
        for (;;)
        {
            cout << "Input number of choice (1 - 4): ";
            string tempChoice;
            getline(cin, tempChoice);
            if (!(tempChoice.length() == 1 && tempChoice[0] >= '1' && tempChoice[0] <= '4'))
            {
                cout << "Invalid input. Please enter a valid choice.\n";
                continue;
            }
            choice = stoi(tempChoice);
            break;
        }
        cout << "\n";
        switch (choice)
        {
        case 1:

            cout << "View Products";
            store.displayProducts();
            for (;;)
            {
                cout << "Enter the ID of the product you want to add in the shopping cart [000]: ";
                string id;
                getline(cin, id);
                if (!isValidId(id))
                {
                    cout << "Invalid input. Product ID must be a 3-digit number.\n";
                    continue;
                }
                store.addToCart(id);
                cout << "Do you want to add another product [(Y) YES | (N) NO]? ";
                if (isValidYesOrNo())
                {
                    continue;
                }
                else
                {
                    cout << "Going back to main menu...\n\n";
                    break;
                }
            }
            break;
        case 2:
            cout << "View Shopping Cart\n";
            store.displayCart();
            if (store.calculateCartTotal() == 0.0)
            {
                cout << "No items in the cart.\n\n";
                break;
            }
            char checkout;
            cout << "Do you want to check out all the products[(Y) YES | (N) NO]? ";
            if (isValidYesOrNo())
            {
                store.displayCart();
                int paymentChoice;
                PaymentStrategy *paymentStrategy = nullptr;

                cout << "Select Payment Method:" << endl;
                cout << "1. Cash" << endl;
                cout << "2. Credit/Debit Card" << endl;
                cout << "3. GCash" << endl;
                for (;;)
                {
                    cout << "Input number of choice (1 - 4): ";
                    string tempChoice;
                    getline(cin, tempChoice);

                    if (tempChoice.length() == 1 && tempChoice[0] >= '1' && tempChoice[0] <= '3')
                    {
                        paymentChoice = stoi(tempChoice);
                        break;
                    }
                    else
                    {
                        cout << "Invalid input. Please enter a valid choice (1 - 4).\n";
                        continue;
                    }
                }

                switch (paymentChoice)
                {
                case 1:
                    paymentStrategy = new CashPayment();
                    break;
                case 2:
                    paymentStrategy = new CreditCardPayment();
                    break;
                case 3:
                    paymentStrategy = new GCashPayment();
                    break;
                default:
                    cout << "Invalid payment method" << endl;
                    continue;
                }

                store.checkout(paymentStrategy);

                delete paymentStrategy;
            }
            cout << "\n";
            break;
        case 3:
            cout << "View Orders\n";
            store.displayOrders();
            break;
        case 4:
            cout << "Exiting the program.\n";
            return 0;
        default:
            cout << "Invalid choice. Please enter a number between 1 and 4.\n";
            break;
        }
    }
}
