#ifndef ACTION_H_
#define ACTION_H_


#include <iostream>
#include "Customer.h"
#include "Restaurant.h"

using namespace std;

enum ActionStatus{
    PENDING, COMPLETED, ERROR
};

//Forward declaration
class Restaurant;

class BaseAction{
public:
    BaseAction();
    virtual ~BaseAction();
    ActionStatus getStatus() const;
    virtual void act(Restaurant& restaurant)=0;
    virtual string toString() const=0;
    virtual BaseAction* clone() const=0;
    virtual string getActionType() const=0;
    virtual string getStatusAsString() const=0;
    void setStatus(ActionStatus otherStatus);
protected:
    void complete();
    void error(string errorMsg);
    string getErrorMsg() const;
private:
    string errorMsg;
    ActionStatus status;
};


class OpenTable : public BaseAction {
public:
    // Destructor
    virtual ~OpenTable();
    // Copy Constructor
    OpenTable(const OpenTable &other);
    // Move Constructor
    OpenTable(OpenTable &&other);
    OpenTable(int id, vector<Customer *> customersList);
    void act(Restaurant &restaurant);
    string toString() const;
    OpenTable* clone() const;
    virtual string getActionType() const;
    virtual string getStatusAsString() const;
private:
    const int tableId;
    vector<Customer *> customers;
};


class Order : public BaseAction {
public:
    Order(int id);
    virtual ~Order();
    void act(Restaurant &restaurant);
    string toString() const;
    Order* clone() const;
    virtual string getActionType() const;
    virtual string getStatusAsString() const;
private:
    const int tableId;
};


class MoveCustomer : public BaseAction {
public:
    MoveCustomer(int src, int dst, int customerId);
    virtual ~MoveCustomer();
    void act(Restaurant &restaurant);
    string toString() const;
    MoveCustomer* clone() const;
    virtual string getActionType() const;
    virtual string getStatusAsString() const;
    void moveDishesToDstTable(Restaurant &restaurant, vector<int>& dishes,Table* dstTable) const;
private:
    const int srcTable;
    const int dstTable;
    const int id;
};


class Close : public BaseAction {
public:
    Close(int id);
    virtual ~Close();
    void act(Restaurant &restaurant);
    string toString() const;
    Close* clone() const;
    virtual string getActionType() const;
    virtual string getStatusAsString() const;
private:
    const int tableId;
};


class CloseAll : public BaseAction {
public:
    CloseAll();
    virtual ~CloseAll();
    void act(Restaurant &restaurant);
    string toString() const;
    CloseAll* clone() const;
    virtual string getActionType() const;
    virtual string getStatusAsString() const;
private:
};


class PrintMenu : public BaseAction {
public:
    PrintMenu();
    virtual ~PrintMenu();
    void act(Restaurant &restaurant);
    string toString() const;
    PrintMenu* clone() const;
    virtual string getActionType() const;
    virtual string getStatusAsString() const;
private:
};


class PrintTableStatus : public BaseAction {
public:
    PrintTableStatus(int id);
    virtual ~PrintTableStatus();
    void act(Restaurant &restaurant);
    string toString() const;
    PrintTableStatus* clone() const;
    virtual string getActionType() const;
    virtual string getStatusAsString() const;
private:
    const int tableId;
};


class PrintActionsLog : public BaseAction {
public:
    PrintActionsLog();
    virtual ~PrintActionsLog();
    void act(Restaurant &restaurant);
    string toString() const;
    PrintActionsLog* clone() const;
    virtual string getActionType() const;
    virtual string getStatusAsString() const;
private:
};


class BackupRestaurant : public BaseAction {
public:
    BackupRestaurant();
    virtual ~BackupRestaurant();

    void act(Restaurant &restaurant);
    string toString() const;
    BackupRestaurant* clone() const;
    virtual string getActionType() const;
    virtual string getStatusAsString() const;
private:
};


class RestoreResturant : public BaseAction {
public:
    RestoreResturant();
    virtual ~RestoreResturant();

    void act(Restaurant &restaurant);
    string toString() const;
    RestoreResturant* clone() const;
    virtual string getActionType() const;
    virtual string getStatusAsString() const;
};


#endif