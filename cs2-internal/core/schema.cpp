#include "schema.h"

#include <vector>
#include <algorithm>

#include "interfaces.h"

#include "../sdk/interfaces/ischemasystem.h"

struct SchemaData_t
{
	FNV1A_t uHashedFieldName = 0x0ULL;
	std::uint32_t uOffset = 0x0U;
};

static std::vector<SchemaData_t> vecSchemaData;

bool SCHEMA::Setup()
{
	CSchemaSystemTypeScope* pTypeScope = I::SchemaSystem->FindTypeScopeForModule(CXOR("client.dll"));
	if (pTypeScope == nullptr)
		return false;

	const int nTableSize = pTypeScope->hashClasses.Count();
	printf(CXOR("found %d schema classes in module\n"), nTableSize);

	UtlTSHashHandle_t* pElements = new UtlTSHashHandle_t[nTableSize + 1U];
	const auto nElements = pTypeScope->hashClasses.GetElements(0, nTableSize, pElements);

	for (int i = 0; i < nElements; i++)
	{
		const UtlTSHashHandle_t hElement = pElements[i];

		if (hElement == 0)
			continue;

		CSchemaClassBinding* pClassBinding = pTypeScope->hashClasses[hElement];
		if (pClassBinding == nullptr)
			continue;

		SchemaClassInfoData_t* pDeclaredClassInfo;
		pTypeScope->FindDeclaredClass(&pDeclaredClassInfo, pClassBinding->szBinaryName);

		if (pDeclaredClassInfo == nullptr)
			continue;

		if (pDeclaredClassInfo->nFieldSize == 0)
			continue;

		for (auto j = 0; j < pDeclaredClassInfo->nFieldSize; j++)
		{
			SchemaClassFieldData_t* pFields = pDeclaredClassInfo->pFields;

			CRT::String_t<MAX_PATH> szFieldClassBuffer(CXOR("%s->%s"), pClassBinding->szBinaryName, pFields[j].szName);
			vecSchemaData.emplace_back(FNV1A::Hash(szFieldClassBuffer.Data()), pFields[j].nSingleInheritanceOffset);
		}
	}

	delete[] pElements;

	return true;
}

std::uint32_t SCHEMA::GetOffset(const FNV1A_t uHashedFieldName)
{
	if (const auto it = std::ranges::find_if(vecSchemaData, [uHashedFieldName](const SchemaData_t& data) { return data.uHashedFieldName == uHashedFieldName; }); it != vecSchemaData.end())
		return it->uOffset;

	printf(CXOR("failed to find offset for field with hash: 0x%llX\n"), static_cast<std::uint64_t>(uHashedFieldName));

	CS_ASSERT(false);
	
	return 0U;
}

std::uint32_t SCHEMA::GetForeignOffset(const char* szModulenName, const FNV1A_t uHashedClassName, const FNV1A_t uHashedFieldName)
{
	CSchemaSystemTypeScope* pTypeScope = I::SchemaSystem->FindTypeScopeForModule(szModulenName);
	if (pTypeScope == nullptr)
		return false;

	const int nTableSize = pTypeScope->hashClasses.Count();

	UtlTSHashHandle_t* pElements = new UtlTSHashHandle_t[nTableSize + 1U];

	const auto nElements = pTypeScope->hashClasses.GetElements(0, nTableSize, pElements);

	std::uint32_t uOffset = 0x0;

	for (int i = 0; i < nElements; i++)
	{
		const UtlTSHashHandle_t hElement = pElements[i];

		if (hElement == 0)
			continue;

		CSchemaClassBinding* pClassBinding = pTypeScope->hashClasses[hElement];
		if (pClassBinding == nullptr)
			continue;

		SchemaClassInfoData_t* pDeclaredClassInfo;
		pTypeScope->FindDeclaredClass(&pDeclaredClassInfo, pClassBinding->szBinaryName);

		if (pDeclaredClassInfo == nullptr)
			continue;

		if (pDeclaredClassInfo->nFieldSize == 0)
			continue;

		for (auto j = 0; j < pDeclaredClassInfo->nFieldSize; j++)
		{
			SchemaClassFieldData_t* pFields = pDeclaredClassInfo->pFields;
			if (pFields == nullptr)
				continue;

			SchemaClassFieldData_t field = pFields[j];
			if (FNV1A::Hash(pClassBinding->szBinaryName) == uHashedClassName && FNV1A::Hash(field.szName) == uHashedFieldName)
				uOffset = field.nSingleInheritanceOffset;
		}
	}

	if (uOffset == 0x0)
		printf(CXOR("failed to find offset for field with hash: 0x%llX\n"), static_cast<std::uint64_t>(uHashedFieldName));

	return uOffset;
}