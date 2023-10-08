#include "include/sync.h"

namespace mw_tools {
namespace sync {

CriticalSection::CriticalSection() {
    critical_section_init(&m_crit_sec);
};

void CriticalSection::Enter() {
    critical_section_enter_blocking(&m_crit_sec);
};

void CriticalSection::Exit() {
     critical_section_exit(&m_crit_sec);
};

CriticalSectionGuard::CriticalSectionGuard(CriticalSection& cs) :
    m_cs(cs),
    m_locked(false)
{
    Enter();
};

CriticalSectionGuard::~CriticalSectionGuard() {
    if (m_locked) { 
        m_cs.Exit();
    }
};

void CriticalSectionGuard::Enter() {
    m_cs.Enter();
    m_locked = true;
};

void CriticalSectionGuard::Exit() {
    m_cs.Exit();
    m_locked = false;
};

}}