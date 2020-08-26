//
// Created by idoniss@wincs.cs.bgu.ac.il on 11/19/18.
//

#include "Action.h"

extern Restaurant* backup;

BaseAction::BaseAction() : status(PENDING){};

BaseAction::~BaseAction(){};

void BaseAction::complete(){
    this->status=COMPLETED;
}

void BaseAction::error(string errorMsg) {
    this->errorMsg=errorMsg;
    this->status=ERROR;
}

string BaseAction::getErrorMsg() const{
    return errorMsg;
}

ActionStatus BaseAction::getStatus() const{
    return status;
}

void BaseAction::setStatus(ActionStatus otherStatus){
    this->status = otherStatus;
}

BackupRestaurant::BackupRestaurant() : BaseAction() {};

BackupRestaurant::~BackupRestaurant(){};

void BackupRestaurant::act(Restaurant &restaurant){
    if(backup== nullptr) {
        backup = new Restaurant(restaurant);
    }
    else {
        *backup=restaurant;
    }
    complete();
}

string BackupRestaurant::toString() const{
    return "backup " + getStatusAsString();
}

BackupRestaurant* BackupRestaurant::clone() const{
    BackupRestaurant* backupRestaurant_ptr = new BackupRestaurant();
    backupRestaurant_ptr->setStatus(this->getStatus());
    return backupRestaurant_ptr;
}

string BackupRestaurant::getActionType() const{
    return "backup";
}

string BackupRestaurant::getStatusAsString() const{
    if(getStatus() == PENDING)
        return "Pending";
    if(getStatus() == COMPLETED)
        return "Completed";
    else {
        return "Error";
    }
}

Close::Close(int id): BaseAction(), tableId(id){};

Close::~Close(){};

void Close::act(Restaurant &restaurant){
    Table* table_ptr=restaurant.getTable(tableId);
    if(table_ptr==nullptr || !table_ptr->isOpen()) {
        error("Table does not exist or is not open");
        cout << "Error: " +getErrorMsg() << endl;
    }
    else {
        cout << "Table "<< tableId << " was closed. Bill " << table_ptr->getBill() << "NIS" << endl;
        table_ptr->closeTable();
        complete();
    }
}

string Close::toString() const {
    string output="close " + to_string(tableId) + " " + getStatusAsString();
    if(getStatus()==ERROR){
        output+= ": " + getErrorMsg();
    }
    return output;
}

Close* Close::clone() const{
    Close* close_ptr = new Close(tableId);
    close_ptr->setStatus(this->getStatus());
    if(getStatus()==ERROR)
        close_ptr->error("Table does not exist or is not open");
    return close_ptr;
}

string Close::getActionType() const{
    return "close";
}

string Close::getStatusAsString() const{
    if(getStatus() == PENDING)
        return "Pending";
    if(getStatus() == COMPLETED)
        return "Completed";
    else {
        return "Error";
    }
}

CloseAll::CloseAll(): BaseAction() {};

CloseAll::~CloseAll(){};


void CloseAll::act(Restaurant &restaurant) {
    for (unsigned int i=0; i<restaurant.getNumOfTables(); i++){
        Table* table_ptr=restaurant.getTable(i);
        if(table_ptr->isOpen()){
            cout<< "Table " << to_string(i) << " was closed. Bill " << to_string(table_ptr->getBill()) <<"NIS" << endl;
            table_ptr->closeTable();
        }
    }
    complete();
}

string CloseAll::toString() const{
    string output="closeall " + getStatusAsString();
    return output;
}

CloseAll* CloseAll::clone() const{
    CloseAll* closeAll_ptr = new CloseAll();
    closeAll_ptr->setStatus(this->getStatus());
    return closeAll_ptr;
}

string CloseAll::getActionType() const{
    return "closeall";
}

string CloseAll::getStatusAsString() const{
    if(getStatus() == PENDING)
        return "Pending";
    if(getStatus() == COMPLETED)
        return "Completed";
    else {
        return "Error";
    }
}

MoveCustomer::MoveCustomer(int src, int dst, int customerId): BaseAction(), srcTable(src),dstTable(dst), id(customerId){}

MoveCustomer::~MoveCustomer(){};

void MoveCustomer::act(Restaurant &restaurant) {
    Table *srcTable_ptr = restaurant.getTable(srcTable);
    Table *dstTable_ptr = restaurant.getTable(dstTable);
    if (srcTable_ptr == nullptr || dstTable_ptr == nullptr || !srcTable_ptr->isOpen()
        || !dstTable_ptr->isOpen() || srcTable_ptr->getCustomer(id) == nullptr
        || dstTable_ptr->getCapacity() == (unsigned int)dstTable_ptr->getCustomers().size()) {
        error("Cannot move customer");
        cout << "Error: " +getErrorMsg() << endl;
    }
    else {
        vector<OrderPair> &srcTableOrders = srcTable_ptr->getOrders();
        vector<OrderPair> keep = srcTableOrders;
        vector<int> customerToMoveDishes = vector<int>();
        srcTableOrders.clear();
        for (unsigned int i = 0; i < keep.size(); i++) {
            if (srcTableOrders[i].first == id) {
                customerToMoveDishes.push_back(srcTableOrders[i].second.getId());
                }
            }
        for (unsigned int j = 0; j < keep.size(); j++) {
            if (!(keep[j].first == id)) {
                srcTableOrders.push_back(keep[j]);
            }
        }
        moveDishesToDstTable(restaurant, customerToMoveDishes, dstTable_ptr);
        dstTable_ptr->addCustomer(srcTable_ptr->getCustomer(id));
        srcTable_ptr->removeCustomer(id);
        if (srcTable_ptr->getCustomers().size() == 0)
            srcTable_ptr->closeTable();
        complete();
    }
}

void MoveCustomer::moveDishesToDstTable(Restaurant &restaurant, vector<int>& dishes,Table* dstTable) const{
    for(unsigned int i = 0;i<dishes.size();i++){
        Dish& tmpDish = restaurant.getMenu()[dishes[i]];
        OrderPair dish = OrderPair(id,tmpDish);
        dstTable->addDish(dish);
    }
}

string MoveCustomer::toString() const {
    string output  ="move " +to_string(srcTable) + " "+to_string(dstTable)
                    + " "+to_string(id) + " " +getStatusAsString();
    if(getStatus()==ERROR){
        output+= ": " + getErrorMsg();
    }
    return output;
}

MoveCustomer* MoveCustomer::clone() const{
    MoveCustomer* moveCustomer_ptr = new MoveCustomer(srcTable, dstTable, id);
    moveCustomer_ptr->setStatus(this->getStatus());
    if(getStatus()==ERROR)
        moveCustomer_ptr->error("Cannot move customer");
    return moveCustomer_ptr;
}

string MoveCustomer::getActionType() const{
    return "move";
}

string MoveCustomer::getStatusAsString() const{
    if(getStatus() == PENDING)
        return "Pending";
    if(getStatus() == COMPLETED)
        return "Completed";
    else {
        return "Error";
    }
}

OpenTable::OpenTable(int id, vector<Customer*> customersList): BaseAction(), tableId(id),customers(customersList){
}

OpenTable::~OpenTable() {
    for(unsigned int i = 0;i<customers.size();i++){
        delete customers[i];
    }
    customers.clear();
}

OpenTable::OpenTable(const OpenTable &other):tableId(other.tableId) {
    for(unsigned int i = 0;i<other.customers.size();i++){
        Customer* customer_ptr = (other.customers)[i]->clone();
        this->customers.push_back(customer_ptr);
    }
}

OpenTable::OpenTable(OpenTable &&other): tableId(other.tableId), customers(other.customers){}

void OpenTable::act(Restaurant &restaurant){
    Table* table_ptr = restaurant.getTable(tableId);
    if(table_ptr==nullptr) {
        error("Table does not exist or is already open");
        cout << "Error: " + getErrorMsg() << endl;
    }

    if(table_ptr->isOpen()){
        error("Table does not exist or is already open");
        cout << "Error: " +getErrorMsg() << endl;
        for(unsigned int i = 0;i<customers.size();i++){
            customers[i]->incCustomerCounter();
        }
    }
    else {
        table_ptr->openTable();
        for(unsigned int i=0; i<customers.size(); i++){
            table_ptr->addCustomer(customers[i]);
        }
        complete();
    }
}

string OpenTable::toString() const {
    string customersString;
    for (unsigned int i=0; i<customers.size(); i++){
        customersString+=customers[i]->toString()+" ";
    }
    string output="open " + to_string(tableId) + " " +customersString + getStatusAsString();
    if(getStatus()==ERROR){
        output+= ": " + getErrorMsg();
    }
    return output;
}

OpenTable* OpenTable::clone() const{
    vector<Customer *> newOpenTableCustomers;
    for(unsigned int i = 0;i<customers.size();i++){
        Customer* customer_ptr = customers[i]->clone();
        newOpenTableCustomers.push_back(customer_ptr);
    }
    OpenTable* openTable_ptr = new OpenTable(tableId,newOpenTableCustomers);
    openTable_ptr->setStatus(this->getStatus());
    if(getStatus()==ERROR)
        openTable_ptr->error("Table does not exist or is already open");
    return openTable_ptr;
}

string OpenTable::getActionType() const{
    return "open";
}

string OpenTable::getStatusAsString() const{
    if(getStatus() == PENDING)
        return "Pending";
    if(getStatus() == COMPLETED)
        return "Completed";
    else {
        return "Error";
    }
}

Order::Order(int id): BaseAction(), tableId(id) {}

Order::~Order(){};

void Order::act(Restaurant &restaurant) {
    Table* table_ptr=restaurant.getTable(tableId);
    if(table_ptr==nullptr || !table_ptr->isOpen()){
        error("Table does not exist or is not open");
        cout << "Error: " +getErrorMsg() << endl;
    }
    else {
        table_ptr->order(restaurant.getMenu());
        complete();
    }
}

string Order::toString() const {
    string output = "order " + to_string(tableId) + " " + getStatusAsString();
    if(getStatus()==ERROR){
        output += ": " + getErrorMsg();
    }
    return output;
}

Order* Order::clone() const{
    Order* order_ptr = new Order(tableId);
    order_ptr->setStatus(this->getStatus());
    if(getStatus()==ERROR)
        order_ptr->error("Table does not exist or is not open");
    return order_ptr;
}

string Order::getActionType() const{
    return "order";
}

string Order::getStatusAsString() const{
    if(getStatus() == PENDING)
        return "Pending";
    if(getStatus() == COMPLETED)
        return "Completed";
    else {
        return "Error";
    }
}

PrintActionsLog::PrintActionsLog(): BaseAction() {};

PrintActionsLog::~PrintActionsLog(){};

string PrintActionsLog::toString() const {
    string output= "log " + getStatusAsString();
    if(getStatus()==ERROR){
        output+= ": " + getErrorMsg();
    }
    return output;
}

void PrintActionsLog::act(Restaurant &restaurant){
    const vector<BaseAction*>& actionsLog_ref=restaurant.getActionsLog();
    for(unsigned int i=0; i<actionsLog_ref.size(); i++){
        cout << actionsLog_ref[i]->toString() << endl;
    }
    complete();
}

PrintActionsLog* PrintActionsLog::clone() const{
    PrintActionsLog* printActionsLog_ptr = new PrintActionsLog();
    printActionsLog_ptr->setStatus(this->getStatus());
    return printActionsLog_ptr;
}

string PrintActionsLog::getActionType() const{
    return "log";
}

string PrintActionsLog::getStatusAsString() const{
    if(getStatus() == PENDING)
        return "Pending";
    if(getStatus() == COMPLETED)
        return "Completed";
    else {
        return "Error";
    }
}

PrintMenu::PrintMenu(): BaseAction(){}

PrintMenu::~PrintMenu(){};


void PrintMenu::act(Restaurant &restaurant){
    vector<Dish> & dishes = restaurant.getMenu();
    for(unsigned int i = 0;i<dishes.size();i++){
        cout << dishes[i].toString() << endl;
    }
    complete();
}

string PrintMenu::toString() const{
    string output ="menu " + getStatusAsString();
    if(getStatus()==ERROR){
        output+= ": " + getErrorMsg();
    }
    return output;
}

PrintMenu* PrintMenu::clone() const{
    PrintMenu* printMenu_ptr = new PrintMenu();
    printMenu_ptr->setStatus(this->getStatus());
    return printMenu_ptr;
}

string PrintMenu::getActionType() const{
    return "menu";
}

string PrintMenu::getStatusAsString() const{
    if(getStatus() == PENDING)
        return "Pending";
    if(getStatus() == COMPLETED)
        return "Completed";
    else {
        return "Error";
    }
}

PrintTableStatus::PrintTableStatus(int id): BaseAction(), tableId(id){};

PrintTableStatus::~PrintTableStatus(){};


string PrintTableStatus::toString() const{
    string output="status " + to_string(tableId) + " " + getStatusAsString();
    if(getStatus()==ERROR){
        output+= ": " + getErrorMsg();
    }
    return output;
}

void PrintTableStatus::act(Restaurant &restaurant){
    string line;
    cout << "Table " << to_string(tableId) << " status: " << restaurant.getTable(tableId)->getStatusAsString() << endl;
    if(restaurant.getTable(tableId)->isOpen()){
        cout << "Customers:" << endl;
        vector<Customer*> &customers_ref=restaurant.getTable(tableId)->getCustomers();
        for(unsigned int i=0; i<customers_ref.size(); i++) {
            cout << customers_ref[i]->getId() << " " << customers_ref[i]->getName() << endl;
        }
        cout << "Orders:" << endl;
        vector<OrderPair>& orders_ref=restaurant.getTable(tableId)->getOrders();
        for (unsigned int i=0; i<orders_ref.size(); i++){
            cout << orders_ref[i].second.getName() << " " << orders_ref[i].second.getPrice()
                 << "NIS " << orders_ref[i].first << endl;
        }
        cout << "Current Bill: " << restaurant.getTable(tableId)->getBill() << "NIS" << endl;
    }
    complete();
}

PrintTableStatus* PrintTableStatus::clone() const{
    PrintTableStatus* printTableStatus_ptr = new PrintTableStatus(tableId);
    printTableStatus_ptr->setStatus(this->getStatus());
    return printTableStatus_ptr;
}

string PrintTableStatus::getActionType() const{
    return "status";
}

string PrintTableStatus::getStatusAsString() const{
    if(getStatus() == PENDING)
        return "Pending";
    if(getStatus() == COMPLETED)
        return "Completed";
    else {
        return "Error";
    }
}

RestoreResturant::RestoreResturant() : BaseAction() {};

RestoreResturant::~RestoreResturant(){};


void RestoreResturant::act(Restaurant &restaurant) {
    if (backup != nullptr) {
        restaurant=*backup;
        complete();
    }
    else {
        error("No backup available");
        cout << "Error: " +getErrorMsg() << endl;
    }
}

string RestoreResturant::toString() const{
    string output = "restore " + getStatusAsString();
    if(getStatus()==ERROR){
        output+= ": " + getErrorMsg();
    }
    return output;
}

RestoreResturant* RestoreResturant::clone() const{
    RestoreResturant* restoreResturant_ptr = new RestoreResturant();
    restoreResturant_ptr->setStatus(this->getStatus());
    if(getStatus()==ERROR)
        restoreResturant_ptr->error("No backup available");
    return restoreResturant_ptr;
}

string RestoreResturant::getActionType() const{
    return "restore";
}

string RestoreResturant::getStatusAsString() const{
    if(getStatus() == PENDING)
        return "Pending";
    if(getStatus() == COMPLETED)
        return "Completed";
    else {
        return "Error";
    }
}