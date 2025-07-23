#pragma once
#include "schema_model.h"
class HRModule {
public:
    HRModule(SchemaModel* schema) : schema_(schema) {}
    void registerModule() {}
    void addEmployee(const QVariantMap&) {}
private:
    SchemaModel* schema_;
};
