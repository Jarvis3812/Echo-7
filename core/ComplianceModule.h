#pragma once
#include "schema_model.h"
class ComplianceModule {
public:
    ComplianceModule(SchemaModel* schema) : schema_(schema) {}
    void registerModule() {}
    void audit(const QVariantMap&) {}
private:
    SchemaModel* schema_;
};
