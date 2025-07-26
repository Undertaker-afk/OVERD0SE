#pragma once

#include "../utilities/common.h"
#include "../utilities/fnv1a.h"

#define SCHEMA_ADD_OFFSET(TYPE, NAME, OFFSET) \
    [[nodiscard]] CS_INLINE std::add_lvalue_reference_t<TYPE> NAME() \
    { \
        static const std::uint32_t uOffset = OFFSET; \
        return *reinterpret_cast<std::add_pointer_t<TYPE>>(reinterpret_cast<std::uint8_t*>(this) + (uOffset)); \
    }

#define SCHEMA_ADD_POFFSET(TYPE, NAME, OFFSET) \
    [[nodiscard]] CS_INLINE std::add_pointer_t<TYPE> NAME() \
    { \
        const static std::uint32_t uOffset = OFFSET; \
        return reinterpret_cast<std::add_pointer_t<TYPE>>(reinterpret_cast<std::uint8_t*>(this) + (uOffset)); \
    }

#define SCHEMA_ADD_FOREIGN_OFFSET(TYPE, NAME, MODULE, CLASS, FIELD) \
    [[nodiscard]] CS_INLINE std::add_lvalue_reference_t<TYPE> NAME() \
    { \
        const static std::uint32_t uOffset = SCHEMA::GetForeignOffset(MODULE, FNV1A::Hash(CLASS), FNV1A::Hash(FIELD)); \
        return *reinterpret_cast<std::add_pointer_t<TYPE>>(reinterpret_cast<std::uint8_t*>(this) + (uOffset)); \
    }

#define SCHEMA_ADD_FIELD_OFFSET(TYPE, NAME, FIELD, ADDITIONAL) \
    SCHEMA_ADD_OFFSET(TYPE, NAME, SCHEMA::GetOffset(FNV1A::Hash(FIELD)) + ADDITIONAL)

#define SCHEMA_ADD_FIELD(TYPE, NAME, FIELD) \
    SCHEMA_ADD_FIELD_OFFSET(TYPE, NAME, FIELD, 0U)

#define SCHEMA_ADD_FIELD_OFFSET_FOREIGN(TYPE, NAME, MODULE, CLASS, FIELD, ADDITIONAL) \
    SCHEMA_ADD_FOREIGN_OFFSET(TYPE, NAME, MODULE, CLASS, FIELD)

#define SCHEMA_ADD_FIELD_FOREIGN(TYPE, NAME, MODULE, CLASS, FIELD) \
    SCHEMA_ADD_FIELD_OFFSET_FOREIGN(TYPE, NAME, MODULE, CLASS, FIELD, 0U)

#define SCHEMA_ADD_PFIELD_OFFSET(TYPE, NAME, FIELD, ADDITIONAL) SCHEMA_ADD_POFFSET(TYPE, NAME, SCHEMA::GetOffset(FNV1A::Hash(FIELD)) + ADDITIONAL)

#define SCHEMA_ADD_PFIELD(TYPE, NAME, FIELD) SCHEMA_ADD_PFIELD_OFFSET(TYPE, NAME, FIELD, 0U)

namespace SCHEMA
{
	bool Setup();

	[[nodiscard]] std::uint32_t GetOffset(const FNV1A_t uHashedFieldName);
	[[nodiscard]] std::uint32_t GetForeignOffset(const char* szModulenName, const FNV1A_t uHashedClassName, const FNV1A_t uHashedFieldName);
}