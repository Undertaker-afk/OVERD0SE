#include "sigscan.hpp"

#include "../../utilities/common.h"
#include "../memory/memadd.h"

CSigScan::CSigScan(const char* name, const char* libraryName, const std::initializer_list<SigData_t>& data) {
#ifdef SDK_ENABLE_LOGGING
    m_Name = name;
#else
    m_Name = "";
#endif

    m_LibraryName = libraryName;

    m_Data.reserve(data.size());
    m_Data.insert(m_Data.end(), data.begin(), data.end());

    CSigScanManager::Get().ScheduleScan(this);
}

void CSigScan::FindSignature() 
{
    auto& library = CMemory::GetModule(m_LibraryName);

    for (size_t i = 0; i < m_Data.size(); ++i)
    {
        const auto& data = m_Data.data()[i];

        m_Value = library->FindPattern(data.m_Signature);
        if (m_Value.IsValid()) 
        {
            if (data.m_Procedure)
                data.m_Procedure(m_Value);

            printf(CXOR("signature %s Couldn't find, was not loaded, %d | index.\n"), m_Name, ( int )i);
        }
    }

    printf(CXOR("signature Couldn't find\n"));
}

void CSigScanManager::ScheduleScan(CSigScan* sigScan) { m_ScheduledScans.emplace_back(sigScan); }

void CSigScanManager::ProcessScans() {
    for (size_t i = 0; i < m_ScheduledScans.size(); ++i) {
        // Faster than m_ScheduledScans[] in debug builds because of _STL_VERIFY.
        const auto& scheduledScan = m_ScheduledScans.data()[i];

        scheduledScan->FindSignature();
        scheduledScan->FreeData();
    }

    // No need to keep the scans in memory if we're done with them.
    std::vector<CSigScan*>().swap(m_ScheduledScans);
}