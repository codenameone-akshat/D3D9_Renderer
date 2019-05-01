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
        inline long long GetDT() const { return m_dt; };
        inline short GetFPS() const { return m_fps; }
        
    private:
        void OnTimerEndCalled();
        void OnSystemTimeStep();
        
        std::chrono::high_resolution_clock::time_point m_tStart;
        std::chrono::high_resolution_clock::time_point m_tEnd;
        long long m_dt;
        long long m_systemTimer;
        short m_numFrames;
        short m_fps;
	};
}