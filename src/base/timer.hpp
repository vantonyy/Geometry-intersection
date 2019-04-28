#pragma once

#include <ctime>
#include <sstream>
#include <string>
#include <chrono>

namespace tools {

typedef std::chrono::nanoseconds nanoseconds;
typedef std::chrono::microseconds microseconds;
typedef std::chrono::milliseconds milliseconds;
typedef std::chrono::seconds seconds;
typedef std::chrono::minutes minutes;
typedef std::chrono::hours hours;

/*
 * @class timer
 * @brief this class provides some interfaces to use timing
 *       (e.g. how long it takes to run parts of a program).
*/
class Timer
{
private:
	typedef std::chrono::high_resolution_clock high_resolution_clock;
        typedef high_resolution_clock::time_point time_point;
public:
        //Constructor
        Timer()
                : m_started_at(high_resolution_clock::now())
                , m_paused_at(getNullTimePoint())
        {
        }

        //@brief function returns true if the timer is started, false otherwise
        bool isStarted() const
        {
                return m_started_at != getNullTimePoint();
        }

        //@brief function returns true if the timer is paused, false otherwise
        bool isPaused() const
        {
                return m_paused_at != getNullTimePoint();
        }

        //@brief function returns true if the timer is running, false otherwise
        bool isRunning() const
        {
                return !isPaused() && isStarted();
        }

        //@brief function starts or restarts the timer
        void restart()
        {
                m_paused_at = getNullTimePoint();
                m_started_at = high_resolution_clock::now();
        }

        //@brief function stops the timer
        void stop()
        {
                m_started_at = getNullTimePoint();
                m_paused_at = m_started_at;
        }

        //@brief function pouse the timer
        void pause()
        {
                if (isPaused() || !isStarted()) {
                        return;
                }
                m_paused_at = high_resolution_clock::now();
        }

        //@brief function resume the timer
        void resume()
        {
                if (!isPaused()) {
                        return;
                }
                m_started_at += high_resolution_clock::now() - m_paused_at;
                m_paused_at = getNullTimePoint();
        }

        /*
         * @brief function to get the number of elapsed time since the the last time restart() was called.
         * If the timer is still running (if stop/pause has not been called),
         * this number will be continually growing as time passes. Returns 0 if the timer was never started.
         */
	template<typename DurationType>
	typename DurationType::rep elapsed() const
	{
		if (!isStarted()) {
			return typename DurationType::rep(0);
		}
		time_point::duration elapsed = isPaused()
			? m_paused_at - m_started_at
			: high_resolution_clock::now() - m_started_at;
		return std::chrono::duration_cast<DurationType>(elapsed).count();
	}
public:
        //@brief function to get the local time, second level resolution (HH:MM:SS),
        //      based on the time zone settings of the computer.
        static std::string getCurrentDateTime()
        {
		std::time_t now = std::time(0);
		char buf[32];
		ctime_s(buf, sizeof(buf), &now);
		return buf;
        }
private:
        static const time_point& getNullTimePoint()
        {
                static time_point null_time_point;
                return null_time_point;
        }
private:
        time_point m_started_at;
        time_point m_paused_at;
}; // class timer

}
