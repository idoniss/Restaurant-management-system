#ifndef DISH_H_
#define DISH_H_

#include <string>

using namespace std;

enum DishType{
    VEG, SPC, BVG, ALC
};

class Dish{
public:
	// Destructor
	virtual ~Dish();

	// Copy Constructor
	Dish(const Dish &other);

	//Copy Assignment
	Dish& operator=(const Dish &other);

    Dish(int d_id, std::string d_name, int d_price, DishType d_type);
    int getId() const;
    string getName() const;
    int getPrice() const;
    DishType getType() const;
	string toString() const;
	string typeToString() const;
private:
	const int id;
    const string name;
    const int price;
    const DishType type;
};


#endif