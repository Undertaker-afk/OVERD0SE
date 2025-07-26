#include "schemav2.hpp"

#include "../sdk/interfaces/ischemasystem.h"

#include "../utilities/fnv1a.h"

std::optional<int32_t> CSchemaManager::GetSchemaOffsetInternal(const char* moduleName, const char* bindingName, const char* fieldName) 
{
    CSchemaSystemTypeScope* typeScope = I::SchemaSystem->FindTypeScopeForModule(moduleName);
    if (!typeScope) 
    {
        printf(CXOR("schemamgr No type scope found for %s\n"), moduleName);
        return {};
    }

    SchemaClassInfoData_t* classInfo;

    typeScope->FindDeclaredClass(&classInfo, bindingName);
    if (!classInfo)
        return {};

    uint32_t fieldHash = FNV1A::Hash(fieldName);
    for (int i = 0; classInfo->pFields && i < classInfo->nFieldSize; ++i) 
    {
        auto& field = classInfo->pFields[i];
        if (FNV1A::Hash(field.szName) == fieldHash) 
        {
            printf(CXOR("schemamgr offset %s, field: %s\n"), bindingName, fieldName);
            return field.nSingleInheritanceOffset;
        }
    }

    printf(CXOR("schemamgr no field named: %s, in binding: %s\n"), fieldName, bindingName);

    return {};
}