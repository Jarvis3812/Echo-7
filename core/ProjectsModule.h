#pragma once
#include "schema_model.h"
class ProjectsModule {
public:
    ProjectsModule(SchemaModel* schema) : schema_(schema) {}
    void registerModule() {}
    void addProject(const QVariantMap&) {}
private:
    SchemaModel* schema_;
};
