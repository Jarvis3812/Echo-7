#pragma once
#include "schema_model.h"
class FinanceModule {
public:
    FinanceModule(SchemaModel* schema) : schema_(schema) {}
    void registerModule() {}
    void addExpense(const QVariantMap&) {}
private:
    SchemaModel* schema_;
};
