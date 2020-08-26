#ifndef RESTAURANT_H_
#define RESTAURANT_H_

#include "Table.h"
#include "Action.h"
#include <iostream>
#include <fstream>
#include <istream>
#include <sstream>
#include <iterator>

class BaseAction;
class Order;
class MoveCustomer;
class BackupRestaurant;
class Close;
class CloseAll;
class RestoreResturant;
class OpenTable;
class PrintActionsLog;
class PrintMenu;
class PrintTableStatus;

using namespace std;

class Restaurant{		
public:
	Restaurant();

	//destructor
	virtual ~Restaurant();

	//copy constructor
	Restaurant(const Restaurant &other);

	//move constructor
	Restaurant(Restaurant &&other);

	// Copy Assignment
	Restaurant& operator=(const Restaurant &other);

	// Move Assignment
	Restaurant& operator=(Restaurant &&other);

	void clean();
	void copy(const Restaurant &other);
    Restaurant(const string &configFilePath);
    void start();
    unsigned int getNumOfTables() const;
    Table* getTable(int id);
	const vector<BaseAction*>& getActionsLog() const; // Return a reference to the history of actions
    vector<Dish>& getMenu();

private:
    bool open;
    vector<Table*> tables;
    vector<Dish> menu;
    vector<BaseAction*> actionsLog;
    unsigned int numOfTables;
    unsigned int getInputNumOfTables (const string &configFilePath) const;
    string getNumOfSeatsInTablesAsString(const string &configFilePath) const;
    vector<int> getNumOfSeatsInTables(string numOfSeats) const;
    vector<string> getDishesToStrings(const string &configFilePath) const;
    vector<string> getDishDescription(string dishDescription) const;
    vector<vector<string>> getDishes(const string &configFilePath) const;
    enum DishType getDishType(string dishType) const;
	BaseAction* getActionType(string actionAsString) const;
	OpenTable* stringToOpenTable(string actionAsString) const;
	vector<pair<string,string>> stringToCustomersVector(vector<string> vStrings) const;
	vector<string> stringToCustomers(string customers) const;
	Order* stringToOrder(string actionAsString) const;
	MoveCustomer* stringToMoveCustomer(string actionAsString) const;
	Close* stringToClose(string actionAsString) const;
	CloseAll* stringToCloseAll(string actionAsString) const;
	PrintMenu* stringToPrintMenu(string actionAsString) const;
	PrintTableStatus* stringToPrintTableStatus(string actionAsString) const;
	PrintActionsLog* stringToPrintActionsLog(string actionAsString) const;
	BackupRestaurant* stringToBackupRestaurant(string actionAsString) const;
	RestoreResturant* stringToRestoreResturant(string actionAsString) const;
};

#endif