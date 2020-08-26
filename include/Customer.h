#ifndef CUSTOMER_H_
#define CUSTOMER_H_

#include <vector>
#include "Dish.h"
#include <algorithm>

using namespace std;

class Customer{
public:
	virtual ~Customer();
    Customer(string c_name, int c_id);
    virtual vector<int> order(const vector<Dish> &menu)=0;
    virtual string toString() const = 0;
    string getName() const;
    int getId() const;
	virtual Customer* clone() const =0;
	virtual string getTypeClone() const=0;
	static int getCustomerCounter();
    virtual const vector<int> & getDishes() const=0;
    void incCustomerCounter();
private:
    const string name;
    const int id;
protected:
    unsigned int ordersCounter;
    int getMostExpensiveByType(const vector<Dish>& menu, DishType type) const;
	static int customerCounter;
};


class VegetarianCustomer : public Customer {
public:
    virtual ~VegetarianCustomer();
    VegetarianCustomer(string name, int id);
    vector<int> order(const vector<Dish> &menu);
    string toString() const;
	VegetarianCustomer* clone() const;
	string getTypeClone() const;
    const vector<int> & getDishes() const;

private:
    vector<int> dishes;

private:

    int getFirstVeg(const vector<Dish> &menu) const;

    int getBVG(const vector<Dish> &menu) const;
};

class CheapCustomer : public Customer {
public:
    virtual ~CheapCustomer();
	CheapCustomer(string name, int id);
    vector<int> order(const vector<Dish> &menu);
    string toString() const;
	CheapCustomer* clone() const override;
	string getTypeClone() const;
    const vector<int>& getDishes() const;
private:
    vector<int> dishes;
};


class SpicyCustomer : public Customer {
public:
	virtual ~SpicyCustomer();
	SpicyCustomer(string name, int id);
	vector<int> order(const vector<Dish> &menu);
	string toString() const;
	SpicyCustomer* clone() const override;
	string getTypeClone() const;
    const vector<int> & getDishes() const;
    void setThereIsSpicyDishInMenu(bool isThereSpicyDishInMenu);
    bool isThereSpicyDishInMenu() const;
private:
	vector<int> dishes;
	bool thereIsSpicyDishInMenu;

	int cheapestBVG(const vector<Dish> &menu) const;
};


class AlchoholicCustomer : public Customer {
public:
    virtual ~AlchoholicCustomer();
    AlchoholicCustomer(string name, int id);
    vector<int> order(const vector<Dish> &menu);
    string toString() const;
	AlchoholicCustomer* clone() const override;
	string getTypeClone() const;
    const vector<int> & getDishes() const;
private:
	vector<int> dishes;
	vector<int> lastOrder;
	vector<Dish*> alcoholicDishes;
    static bool compareByPriceLowToHigh(Dish* dish_1, Dish* dish_2);
    void getAlcoholicDishes(const vector<Dish> &menu);
};


#endif