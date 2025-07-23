#pragma once
#include "schema_model.h"
class InventoryModule {
public:
    InventoryModule(SchemaModel* schema) : schema_(schema) {}
    void registerModule() {}
    void addItem(const QVariantMap&) {}
private:
    SchemaModel* schema_;
};
