// SchemaModel header
#pragma once
#include <map>
#include <string>
class Database;
class SchemaModel {
public:
    SchemaModel(Database* db);
    std::map<std::string, double> getSalesData();
    std::map<std::string, double> getClientData();
private:
    Database* db_;
};
