//
// Created by idoniss@wincs.cs.bgu.ac.il on 11/6/18.
//

#include <Dish.h>

Dish::Dish(int d_id, string d_name, int d_price, DishType d_type): id(d_id), name(d_name), price(d_price), type(d_type) {};

//destructor
Dish::~Dish() {}

// Copy Constructor
Dish::Dish(const Dish &other): id(other.id), name(other.name), price(other.price), type(other.type) {};

//Copy Assignment
Dish& Dish::operator=(const Dish &other){
    return *this;
}

int Dish::getId() const {
    return id;
}

int Dish::getPrice() const {
    return price;
}
string Dish::getName() const {
    return name;
}

DishType Dish::getType() const {
    return type;
}

string Dish::typeToString() const {
    if(type == VEG)
        return "VEG";
    if(type == SPC)
        return "SPC";
    if(type == BVG)
        return "BVG";
    else
        return "ALC";
}

string Dish::toString() const {
    string typeAsString = typeToString();
    string output =  name + " " + typeAsString + " " + to_string(price) + "NIS";
    return output;
}