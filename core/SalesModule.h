#pragma once
#include "schema_model.h"
class SalesModule {
public:
    SalesModule(SchemaModel* schema) : schema_(schema) {}
    void registerModule() {}
    void addSale(const QVariantMap&) {}
private:
    SchemaModel* schema_;
};
