#pragma once
#include "schema_model.h"
class SupportModule {
public:
    SupportModule(SchemaModel* schema) : schema_(schema) {}
    void registerModule() {}
    void resolveTicket(const QVariantMap&) {}
    void updateInsights(void*) {}
private:
    SchemaModel* schema_;
};
