#pragma once

#include <pico/critical_section.h>

namespace mw_tools {
namespace sync {
    class CriticalSection {
        public:
        CriticalSection();
        void Enter();
        void Exit();
        private:
            critical_section_t m_crit_sec;
    };

    class CriticalSectionGuard {
        public:
        CriticalSectionGuard(CriticalSection& cs);
        ~CriticalSectionGuard();
        void Enter();
        void Exit();
        private:
        CriticalSection& m_cs;
        bool m_locked;
    };
}
}