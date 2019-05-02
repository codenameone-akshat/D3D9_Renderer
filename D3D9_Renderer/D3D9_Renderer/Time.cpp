#include "Time.h"

#include <sstream>
#include <cmath>

#include <windows.h>

//TODO: wrap into a logger class
#define LogInfo( s )            \
{                             \
   std::wostringstream os_;    \
   os_ << s << std::endl;                   \
   OutputDebugString(os_.str().c_str());  \
}

namespace renderer
{
    Time::Time()
        :m_tStart(),
        m_tEnd(),
        m_dt(0),
        m_systemTimer(0),
        m_numFrames(0),
        m_fps(0)
    {
    }
    Time::~Time()
    {
    }
    void Time::OnTimerEndCalled()
    {
        m_dt = abs(std::chrono::duration_cast<std::chrono::milliseconds>(m_tEnd - m_tStart).count());
        if (m_systemTimer >= 1000) //1 second = 1000 milliseconds
        {
            OnSystemTimeStep();
        }
            m_systemTimer += m_dt;
            ++m_numFrames;
    }
    void Time::OnSystemTimeStep()
    {
        m_fps = m_numFrames; //record number of frames passed in a second.
        
        m_systemTimer = 0;
        m_numFrames = 0;

        //LogInfo(std::to_string(m_fps).c_str());
    }
}