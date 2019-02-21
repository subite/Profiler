//
// Created by danta on 12/11/18.
//

#include "Profiler.h"

thread_local Profiler::PTimelineBase *Profiler::PTimelineBase::m_instance = nullptr;