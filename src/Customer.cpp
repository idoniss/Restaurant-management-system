//
// Created by idoniss@wincs.cs.bgu.ac.il on 11/11/18.
//

#include "Customer.h"

int Customer::customerCounter;

//***Customer class***

Customer::Customer(string c_name, int c_id) : name(c_name), id(c_id) {
    ordersCounter = 0;
};

Customer::~Customer(){};

string Customer::getName() const {
    return name;
};

int Customer::getId() const {
    return id;
};

int Customer::getCustomerCounter() {
    int customerID = customerCounter;
    customerCounter++;
    return customerID;
}

void Customer::incCustomerCounter(){
    this->customerCounter++;
}

//get the most expensive dish in the menu
int Customer::getMostExpensiveByType(const vector<Dish>& menu, DishType type) const {
    Dish const * tempDish=&(menu[0]);
    bool thereIsType=false;
    if(!(tempDish->getType()==type)) {
        for(unsigned int i=0; i<menu.size() && !thereIsType; i++) {
            if(menu[i].getType()==type) {
                tempDish =& menu.at(i);
                thereIsType = true;
            }
        }
    }
    if(thereIsType) {
        for (unsigned int i = 0; i < menu.size(); i++) {
            if (menu[i].getType() == type && tempDish->getPrice() < menu[i].getPrice()) {
                tempDish = &menu[i];
            }
        }
        int id = tempDish->getId();
        return id;
    }
    return -1;
};

//VegeterianCustomer

VegetarianCustomer::VegetarianCustomer(string name, int id): Customer(name, id), dishes(vector<int>()) {};

VegetarianCustomer::~VegetarianCustomer() {
    dishes.clear();
};

string VegetarianCustomer::toString() const {
    return getName() + "," + getTypeClone();
}

vector<int> VegetarianCustomer::order(const vector<Dish> &menu) {
    if (ordersCounter == 0) {
        int vegDishID = getFirstVeg(menu);
        if (!(vegDishID == -1)) {
            int vegBVG = getMostExpensiveByType(menu, BVG);
            if (!(vegBVG == -1)) {
                dishes.push_back(vegDishID);
                dishes.push_back(vegBVG);
            }
            ordersCounter++;
            return dishes;
        }
        else
            return dishes;
    }
    else
        return dishes;
}

// returns the id of the first VEG dish in the menu
int VegetarianCustomer::getFirstVeg(const vector<Dish>& menu) const{
    for(unsigned int i=0; i<menu.size(); i++) {
        if(menu[i].getType()==VEG){
            return menu[i].getId();
        }
    }
    return -1;
}

VegetarianCustomer* VegetarianCustomer::clone() const {
    return new VegetarianCustomer(this->getName(),this->getId());
}

string VegetarianCustomer::getTypeClone() const{
    return "veg";
}

const vector<int> &VegetarianCustomer::getDishes() const {
    return dishes;
}



//CheapCustomer

CheapCustomer::CheapCustomer(string name, int id): Customer(name, id), dishes(vector<int>()){};

CheapCustomer::~CheapCustomer() {
    dishes.clear();
};

const vector<int> &CheapCustomer::getDishes() const {
    return dishes;
}

vector<int> CheapCustomer::order(const vector<Dish> &menu){
    if(ordersCounter==0){
        Dish const * tempDish=&menu[0];
        for(unsigned int i=0; i<menu.size(); i++){
            if(menu[i].getPrice()<tempDish->getPrice())
                tempDish=&menu[i];
        }
        dishes.push_back(tempDish->getId());
        ordersCounter++;
    }
    else if(ordersCounter==1) {
        dishes.erase(dishes.begin());
        ordersCounter++;
    }
    return dishes;
}

string CheapCustomer::toString() const {
    return getName() + "," + getTypeClone();
}

CheapCustomer* CheapCustomer::clone() const {
    CheapCustomer* customer=new CheapCustomer(getName(),getId());
    customer->ordersCounter=ordersCounter;
    customer->dishes=this->dishes;
    return customer;
}

string CheapCustomer::getTypeClone() const{
    return "chp";
}
//SpicyCustomer

SpicyCustomer::SpicyCustomer(string name, int id) : Customer(name,id), dishes(vector<int>()), thereIsSpicyDishInMenu(false){};

SpicyCustomer::~SpicyCustomer() {
    dishes.clear();
};

const vector<int> &SpicyCustomer::getDishes() const {
    return dishes;
}

bool SpicyCustomer::isThereSpicyDishInMenu() const {
    return this->thereIsSpicyDishInMenu;
}

void SpicyCustomer::setThereIsSpicyDishInMenu(bool isThereSpicyDishInMenu){
    this->thereIsSpicyDishInMenu = isThereSpicyDishInMenu;
}

vector<int> SpicyCustomer::order(const vector<Dish> &menu) {
    if(ordersCounter==0){
        int mostExpensiveSpicy = getMostExpensiveByType(menu, SPC);
        if (!(mostExpensiveSpicy == -1)) {
            setThereIsSpicyDishInMenu(true);
            dishes.push_back(mostExpensiveSpicy);
            ordersCounter++;
        }
    }
    else if (ordersCounter==1 && isThereSpicyDishInMenu()) {
        dishes.erase(dishes.begin());
        int cheapestBVGID=cheapestBVG(menu);
        if(cheapestBVGID!=-1) {
            dishes.push_back(cheapestBVGID);
            ordersCounter++;
        }
        else
            ordersCounter++;
    }
    else if(ordersCounter>1)
        ordersCounter++;
    return dishes;
}

SpicyCustomer *SpicyCustomer::clone() const {
    SpicyCustomer* customer=new SpicyCustomer(getName(),getId());
    customer->ordersCounter=this->ordersCounter;
    customer->dishes=this->dishes;
    return customer;
}

//find the cheapest BVG in the menu
int SpicyCustomer::cheapestBVG(const vector<Dish>& menu) const{
    Dish const * tempDish=&menu[0];
    bool thereIsBVG=false;
    if(!(tempDish->getType()==BVG)) {
        for(unsigned int i=0; i<menu.size() && !(thereIsBVG); i++) {
            if(menu[i].getType()==BVG) {
                tempDish = &menu[i];
                thereIsBVG = true;
            }
        }
    }
    if(thereIsBVG) {
        for (unsigned int i = 0; i < menu.size(); i++) {
            if (menu[i].getType() == BVG && tempDish->getPrice() > menu[i].getPrice()) {
                tempDish =&menu[i];
            }
        }
        return tempDish->getId();
    }
    return -1;
}

string SpicyCustomer::toString() const {
    return getName() + "," + getTypeClone();
}

string SpicyCustomer::getTypeClone() const{
    return "spc";
}
//AlcoholicCustomer

AlchoholicCustomer::AlchoholicCustomer(string name, int id) : Customer(name,id) , dishes(vector<int>()), lastOrder(vector<int>()), alcoholicDishes(vector<Dish*>()) {};

AlchoholicCustomer::~AlchoholicCustomer() {
    dishes.clear();
    lastOrder.clear();
    alcoholicDishes.clear();
};

const vector<int> &AlchoholicCustomer::getDishes() const {
    return dishes;
}

AlchoholicCustomer *AlchoholicCustomer::clone() const {
    AlchoholicCustomer* customer=new AlchoholicCustomer(this->getName(),this->getId());
    customer->ordersCounter=this->ordersCounter;
    customer->dishes=this->dishes;
    customer->lastOrder = this->lastOrder;
    customer->alcoholicDishes = this->alcoholicDishes;
    return customer;
}

bool AlchoholicCustomer::compareByPriceLowToHigh(Dish* dish_1, Dish* dish_2){
    return ((dish_1)->getPrice() < (dish_2)->getPrice());
}

vector<int> AlchoholicCustomer::order(const vector<Dish> &menu) {
    if(ordersCounter==0) {
        getAlcoholicDishes(menu);
        sort(alcoholicDishes.begin(), alcoholicDishes.end(),compareByPriceLowToHigh);
        if(alcoholicDishes.size()){
            dishes.push_back((alcoholicDishes[0])->getId());
            lastOrder.push_back((alcoholicDishes[0])->getId());
            ordersCounter++;
        }
    }
    else{
        if(lastOrder.size()){
            lastOrder.erase(lastOrder.begin());
             if ((unsigned  int)alcoholicDishes.size()>ordersCounter) {
                 dishes.push_back(alcoholicDishes[ordersCounter]->getId());
                 lastOrder.push_back(alcoholicDishes[ordersCounter]->getId());
                  ordersCounter++;
            }
        }
        else{
            return lastOrder;
        }
    }
    return lastOrder;
}

//gets the ALC dishes from the menu
void AlchoholicCustomer::getAlcoholicDishes(const vector<Dish> &menu) {
    for (unsigned int i = 0; i < menu.size(); i++) {
        if (menu[i].getType() == ALC) {
            alcoholicDishes.push_back((Dish*) &menu[i]);
        }
    }
}

string AlchoholicCustomer::toString() const {
    return getName() + "," + getTypeClone();
}

string AlchoholicCustomer::getTypeClone() const{
    return "alc";
}