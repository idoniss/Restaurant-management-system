//
// Created by idoniss on 11/3/18.
//

#include "Restaurant.h"

extern Restaurant* backup;

Restaurant::Restaurant(): open(true){}

Restaurant::Restaurant(const string &configFilePath) {
    numOfTables = (unsigned int)getInputNumOfTables(configFilePath);
    string NumOfSeatsInTablesAsString = getNumOfSeatsInTablesAsString(configFilePath);
    vector<int> numOfSeatsVector = getNumOfSeatsInTables(NumOfSeatsInTablesAsString);
    for(unsigned int i = 0; i<numOfTables; i++){
        if(!(NumOfSeatsInTablesAsString.empty()))
            tables.push_back(new Table(numOfSeatsVector[i]));
    }
    vector<vector<string>> dishes = getDishes(configFilePath);
    for(unsigned int i = 0; i<dishes.size(); i++){
        if(!((dishes[i][0]).empty() || (dishes[i][1]).empty() || (dishes[i][2]).empty()))
            menu.push_back(Dish(i,dishes[i][0],stoi(dishes[i][2]),getDishType((string)dishes[i][1])));
    }
}

Table* Restaurant::getTable(int id){
    return tables[id];
    }

void Restaurant::clean() {
    open = false;
    menu.clear();
    numOfTables = 0;
    for(unsigned int i = 0;i<tables.size();i++){
        delete tables[i];
    }
    tables.clear();
    for(unsigned int i = 0;i<actionsLog.size();i++){
        delete actionsLog[i];
    }
    actionsLog.clear();
}

void Restaurant::copy(const Restaurant &other){
    open = other.open;
    menu = other.menu;
    numOfTables = other.numOfTables;
    for(unsigned int i = 0;i<(other.tables).size();i++){
        Table* tabl_ptr = new Table(*((other.tables)[i]));
        tables.push_back(tabl_ptr);
    }
    for(unsigned int i = 0;i<(other.actionsLog).size();i++){
        BaseAction* action_ptr = ((other.actionsLog)[i])->clone();
        actionsLog.push_back(action_ptr);
    }
}

//destructor
Restaurant:: ~Restaurant(){
    clean();
}

//copy assignment
Restaurant& Restaurant::operator=(const Restaurant &other){
    clean();
    copy(other);
    return *this;
}

//copy constructor
Restaurant::Restaurant(const Restaurant &other){
copy(other);
}

//move constructor
Restaurant::Restaurant(Restaurant &&other){
    open = other.open;
    tables = other.tables;
    menu = other.menu;
    actionsLog = other.actionsLog;
    numOfTables = other.numOfTables;
    other.open = false;
    other.tables.clear();
    other.menu.clear();
    other.actionsLog.clear();
    other.numOfTables = 0;
}

// Move Assignment
Restaurant& Restaurant::operator=(Restaurant &&other){
    clean();
    open = other.open;
    tables = other.tables;
    menu = other.menu;
    actionsLog = other.actionsLog;
    numOfTables = other.numOfTables;
    other.open = false;
    other.tables.clear();
    other.menu.clear();
    other.actionsLog.clear();
    other.numOfTables = 0;
    return *this;
}

void Restaurant::start() {
    cout << "Restaurant is now open!" << endl;
    bool runRestaurant = true;
    while (runRestaurant) {
        string actionAsString;
        getline(cin, actionAsString);
        if(actionAsString.compare("restore")==0 && backup!=nullptr) {
            RestoreResturant().act(*this);
            RestoreResturant* restore_ptr = new RestoreResturant();
            restore_ptr->setStatus(COMPLETED);
            actionsLog.push_back(restore_ptr);
        }
        else {
            BaseAction *action_ptr = getActionType(actionAsString);
            if ((action_ptr->getActionType()).compare("closeall") == 0) {
                action_ptr->act(*this);
                runRestaurant = false;
                delete action_ptr;
            }
            else{
                action_ptr->act(*this);
                actionsLog.push_back(action_ptr);
            }
        }
    }
}

BaseAction* Restaurant::getActionType(string actionAsString) const{
    string firstWord = actionAsString.substr(0, actionAsString.find(' '));
    if(firstWord.compare("open")==0){
        return stringToOpenTable(actionAsString);
    }
    if(firstWord.compare("order")==0){
        return stringToOrder(actionAsString);
    }
    if(firstWord.compare("move")==0){
        return stringToMoveCustomer(actionAsString);
    }
    if(firstWord.compare("close")==0){
        return stringToClose(actionAsString);
    }
    if(firstWord.compare("closeall")==0){
        return stringToCloseAll(actionAsString);
    }
    if(firstWord.compare("menu")==0){
        return stringToPrintMenu(actionAsString);
    }
    if(firstWord.compare("status")==0){
        return stringToPrintTableStatus(actionAsString);
    }
    if(firstWord.compare("log")==0){
        return stringToPrintActionsLog(actionAsString);
    }
    if(firstWord.compare("backup")==0){
        return stringToBackupRestaurant(actionAsString);
    }
    else
        return stringToRestoreResturant(actionAsString);
}

OpenTable* Restaurant::stringToOpenTable(string actionAsString) const{
    int tableId = actionAsString.at(5) - '0';
    string customers = actionAsString.substr(7);
    vector<Customer*> openTableCustomersVector;
    vector<pair<string,string>> customersPairsVector = stringToCustomersVector(stringToCustomers(customers));
    for(unsigned int i = 0;i<customersPairsVector.size();i++){
        string name = customersPairsVector[i].first;
        string type = customersPairsVector[i].second;
        Customer* customer_ptr;
        if(type.compare("veg")==0){
            int id = Customer::getCustomerCounter();
            customer_ptr = new VegetarianCustomer(name,id);
        }
        else if(type.compare("spc")==0){
            int id = Customer::getCustomerCounter();
            customer_ptr = new SpicyCustomer(name,id);
        }
        else if(type.compare("alc")==0){
            int id = Customer::getCustomerCounter();
            customer_ptr = new AlchoholicCustomer(name,id);
        }
        else {
            int id = Customer::getCustomerCounter();
            customer_ptr = new CheapCustomer(name,id);
        }
        openTableCustomersVector.push_back(customer_ptr);
    }
    return new OpenTable(tableId,openTableCustomersVector);
}

vector<pair<string,string>> Restaurant::stringToCustomersVector(vector<string> vStrings) const{
    vector<pair<string,string>> customersVector;
    for(unsigned int i = 0;i<vStrings.size();i++){
        unsigned long commaIndex = vStrings[i].find(',');
        customersVector.push_back(pair<string,string>(vStrings[i].substr(0,commaIndex),vStrings[i].substr(commaIndex + 1)));
    }
    return customersVector;
};

vector<string> Restaurant::stringToCustomers(string customers) const{
    stringstream customerSeperated(customers);
    istream_iterator<string> begin(customerSeperated);
    std::istream_iterator<string> end;
    std::vector<std::string> vstrings(begin, end);
    return vstrings;
};

Order* Restaurant::stringToOrder(string actionAsString) const{
    int tableId = actionAsString.at(6) - '0';
    return new Order(tableId);
}

MoveCustomer* Restaurant::stringToMoveCustomer(string actionAsString) const{
    int srcTable = actionAsString.at(5) - '0';
    int dstTable = actionAsString.at(7) - '0';
    int id = actionAsString.at(9) - '0';
    return new MoveCustomer(srcTable, dstTable, id);
}

Close* Restaurant::stringToClose(string actionAsString) const{
    int tableId = actionAsString.at(6) - '0';
    return new Close(tableId);
}

CloseAll* Restaurant::stringToCloseAll(string actionAsString) const{
    return new CloseAll();
}

PrintMenu* Restaurant::stringToPrintMenu(string actionAsString) const{
    return new PrintMenu();
}

PrintTableStatus* Restaurant::stringToPrintTableStatus(string actionAsString) const{
    int tableId = actionAsString.at(7) - '0';
    return new PrintTableStatus(tableId);
}

PrintActionsLog* Restaurant::stringToPrintActionsLog(string actionAsString) const{
    return new PrintActionsLog();
}

BackupRestaurant* Restaurant::stringToBackupRestaurant(string actionAsString) const{
    return new BackupRestaurant();
}

RestoreResturant* Restaurant::stringToRestoreResturant(string actionAsString) const{
    return new RestoreResturant();
}

unsigned int Restaurant::getNumOfTables() const {
    return numOfTables;
}

// gets the number of tables in the restaurant from the input file.
unsigned int Restaurant::getInputNumOfTables(const string &configFilePath) const {
    fstream file;
    file.open(configFilePath);
    if (file.is_open()) {
        string line;
        while (getline(file,line)) {
            if (line.empty()==false) {
                if (line[0] == '#' || line[0] == ' ') {
                    continue;
                }
            }
            else
                continue;
            file.close();
            return (unsigned int)(stoi(line));
        }
        return 0;
    }
    else
        return 0;
}

//return the line of tables capacities from the input file.
string Restaurant::getNumOfSeatsInTablesAsString(const string &configFilePath) const{
    ifstream file;
    file.open(configFilePath);
    if (file.is_open()) {
        int counter = 0;
        string line;
        while (getline(file, line)) {
            if (line.empty()==false) {
                if (line[0] == '#' || line[0] == ' ')
                    continue;
            }
            else
                continue;
            if (counter == 0) {
                counter++;
                continue;
            }
            file.close();
            return line;
        }
    }
    return "";
}

//turns the capacities line from string to a vector of ints.
vector<int> Restaurant::getNumOfSeatsInTables(string numOfSeats) const {
    vector<int> seatsVector;
    int counter=0;
    string str;
   for (unsigned int i=0; i<numOfSeats.length(); i++){
       if(numOfSeats[i]==',') {
           str = numOfSeats.substr(counter, i);
           seatsVector.push_back(stoi(str));
           counter=i+1;
       }
   }
    str = numOfSeats.substr(counter,numOfSeats.length());
    seatsVector.push_back(stoi(str));
    return seatsVector;
}


//returns a vector of the dishes description as strings.
vector<string> Restaurant::getDishesToStrings(const string &configFilePath) const{
    ifstream file;
    file.open(configFilePath);
    vector<string> dishesVector;
    if (file.is_open()) {
        int counter = 0;
        string line;
        while (getline(file, line)) {
            if (line.empty()==false) {
                if (line[0] == '#' || line[0] == ' ')
                    continue;
            }
            else
                continue;
            if(counter==0) {
                counter++;
                continue;
            }
            if(counter==1){
                counter++;
                continue;
            }

            dishesVector.push_back(line);
        }
    }
    return dishesVector;
}

//gets a string of dish description and turns it to a vector of size three: dishName, dishType, dishPrice.
vector<string> Restaurant::getDishDescription(string dishDescription) const {
    vector<string> dishes;
    size_t position1 = dishDescription.find(',');
    string dishName = dishDescription.substr(0,position1);
    dishes.push_back(dishName);
    size_t position2 = dishDescription.find(',',position1+1);
    string dishType = dishDescription.substr(position1 + 1, position2-(position1+1));
    dishes.push_back(dishType);
    string dishPriceAsString = dishDescription.substr(position2+1);
    dishes.push_back(dishPriceAsString);
    return dishes;
}

//gets the input file and returns a 2D vector of vector of strings so that each vector contains the
//description of the dish.
vector<vector<string>> Restaurant::getDishes(const string &configFilePath) const {
    vector<string> dishesAsLines = getDishesToStrings(configFilePath);
    vector<vector<string>> dishesDescriptionVector;
    for(unsigned int i = 0; i<dishesAsLines.size(); i++){
        dishesDescriptionVector.push_back(getDishDescription(dishesAsLines[i]));
    }
    return dishesDescriptionVector;
}

//gets the dishType as a string and returns its type.
enum DishType Restaurant::getDishType(string dishType) const {
    if(dishType.compare("VEG")==0){
        return VEG;
    }
    else if(dishType.compare("SPC")==0){
        return SPC;
    }
    else if(dishType.compare("BVG")==0){
        return BVG;
    }
    return ALC;
}

const vector<BaseAction*>& Restaurant::getActionsLog() const{
    return actionsLog;
}

vector<Dish>& Restaurant::getMenu(){
    return menu;
}