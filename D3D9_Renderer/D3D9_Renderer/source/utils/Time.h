#pragma once

#include <chrono>

namespace renderer 
{
	class Time
	{
    public:
		Time();
		~Time();
        
        inline void BeginTimer() { m_tStart = std::chrono::high_resolution_clock::now(); };
        inline void EndTimer() 
        {
            m_tEnd = std::chrono::high_resolution_clock::now(); 
            OnTimerEndCalled();
        };
        inline int64_t GetDT() const { return m_dt; };
        inline int16_t GetFPS() const { return m_fps; }
        
    private:
        void OnTimerEndCalled();
        void OnSystemTimeStep();
        
        std::chrono::high_resolution_clock::time_point m_tStart;
        std::chrono::high_resolution_clock::time_point m_tEnd;
		int64_t m_dt;
		int64_t m_systemTimer;
        int16_t m_numFrames;
        int16_t m_fps;
	};
}