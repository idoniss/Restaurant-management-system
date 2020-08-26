#ifndef TABLE_H_
#define TABLE_H_

#include "Customer.h"

typedef std::pair<int, Dish> OrderPair;

class Table{
public:
    Table(int t_capacity);

    //destructor
    virtual ~Table();

    //copy constructor
    Table(const Table &other);

    //move constructor
    Table(Table &&other);

    // Copy Assignment
    Table& operator=(const Table &other);

    // Move Assignment
    Table& operator=(Table &&other);

    void copy(const Table &other);
    void clear();
    unsigned int getCapacity() const;
    void addCustomer(Customer* customer);
    void removeCustomer(int id);
    Customer* getCustomer(int id);
    vector<Customer*>& getCustomers();
    vector<OrderPair>& getOrders();
    void order(const vector<Dish> &menu);
    void openTable();
    void closeTable();
    int getBill();
    bool isOpen() const;
    void addDish(OrderPair& dish);
    string getStatusAsString() const;
private:
    int findById(int id) const;
    int capacity;
    bool open;
    vector<Customer*> customersList;
    vector<OrderPair> orderList; //A list of pairs for each order in a table - (customer_id, Dish)
};


#endif