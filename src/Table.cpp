//
// Created by idoniss on 11/3/18.
//

#include <iostream>
#include "Table.h"

Table::Table(int t_capacity): capacity(t_capacity), open(false) {};

//destructor
Table::~Table() {
    clear();
    }

void Table::copy(const Table &other) {
    capacity=other.capacity;
    open=other.open;
    orderList = other.orderList;
    vector<Customer*> otherCustomers = other.customersList;
    for(unsigned int i=0; i<otherCustomers.size(); i++){
        Customer* customer_ptr = otherCustomers[i]->clone();
        customersList.push_back(customer_ptr);
    }
}

void Table::clear(){
    for(unsigned int i=0; i<customersList.size(); i++) {
        delete customersList[i];
    }
    capacity = 0;
    open = false;
    orderList.clear();
}

//copy constructor
Table::Table(const Table &other){
    copy(other);
}


//move constructor
Table::Table(Table &&other){
    capacity=other.capacity;
    open=other.open;
    orderList = other.orderList;
    customersList = other.customersList;
    other.customersList.clear();
    other.capacity = 0;
    other.open = false;
    other.orderList.clear();
}

//assignment operator
Table& Table::operator=(const Table &other){
    clear();
    copy(other);
    return *this;
}

//move assignment
Table& Table::operator=(Table &&other){
    clear();
    capacity=other.capacity;
    open=other.open;
    orderList = other.orderList;
    customersList = other.customersList;
    other.customersList.clear();
    other.capacity = 0;
    other.open = false;
    other.orderList.clear();
    return *this;
}
string Table::getStatusAsString() const {
    if(isOpen())
        return "open";
    else
        return "closed";
}

unsigned int Table::getCapacity() const {
    return capacity;
}

bool Table::isOpen() const {
    return open;
}

void Table::addCustomer(Customer *customer) {
    Customer* customer_ptr = customer->clone();
    customersList.push_back(customer_ptr);
}

void Table::removeCustomer(int id) {
    int index = findById(id);
    if(index != -1){
        delete customersList[index];
        customersList.erase(customersList.begin() + index);
    }
}

int Table::findById(int id) const {
    for(unsigned int i = 0;i<customersList.size();i++){
        if(customersList[i]->getId() == id){
           return i;
        }
    }
    return -1;
}

Customer* Table::getCustomer(int id) {
    int customerIndex = findById(id);
    if(customerIndex != -1){
        return customersList[customerIndex];
    }
    return nullptr;
}

vector<Customer*>& Table::getCustomers() {
    return customersList;
}

vector<OrderPair>& Table::getOrders() {
    return orderList;
}

void Table:: order(const vector<Dish> &menu){
    for(unsigned int i = 0; i<customersList.size(); i++) {
        vector<int> orders = customersList[i]->order(menu);
        Customer* tmpCustomer_ptr = customersList[i];
        for (unsigned int j = 0; j < (unsigned int)orders.size(); j++) {
            orderList.push_back(pair<int, Dish>(customersList[i]->getId(), menu[orders[j]]));
            cout << tmpCustomer_ptr->getName()
                 << " ordered " << menu[orders[j]].getName() << endl;
        }
    }
}

void Table::addDish(OrderPair& dish){
    orderList.push_back(dish);
}

void Table::openTable() {
    open = true;
}

void Table::closeTable() {
    open = false;
    for(unsigned int i = 0;i<customersList.size();i++){
        delete customersList[i];
    }
    orderList.clear();
    customersList.erase(customersList.begin(), customersList.end());
}

int Table::getBill() {
    int sum=0;
    for(unsigned int i = 0;i<orderList.size();i++){
        sum+=orderList[i].second.getPrice();
    }
    return sum;
}