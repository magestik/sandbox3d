/*
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3.0 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library.
 */

#pragma once

#if defined(__gnu_linux__) || defined(ANDROID)
#include <time.h>
#endif

#ifdef WIN32
#include <windows.h>
#endif

class Timer
{

public:

	/**
	 * Constructor
	 */
	inline explicit Timer(void)
	{
		#ifdef WIN32
			QueryPerformanceFrequency(&m_frequency);
		#endif
	}

	/**
	 * Destructor
	 */
	inline virtual ~Timer(void)
	{
		// ...
	}

	/**
	 * Start the timer
	 */
	inline void Start(void)
	{
		SetTime(&m_startTime);
	}

	/**
	 * Stop the Timer
	 */
	inline void Stop(void)
	{
		SetTime(&m_endTime);
	}

	/**
	 * Get the time ellapsed in micro-seconds
	 */
	inline double getElapsedTimeInMs(void)
	{
#ifdef WIN32
		return((m_endTime.QuadPart - m_startTime.QuadPart)*1000 / (double)m_frequency.QuadPart);
#else
		return((m_endTime.tv_sec - m_startTime.tv_sec)*1000.0)+((m_endTime.tv_nsec - m_startTime.tv_nsec)*1e-6);
#endif
	}

	/**
	 * Get the time ellapsed in seconds
	 */
	inline double getElapsedTime(void)
	{
#ifdef WIN32
		return((m_endTime.QuadPart - m_startTime.QuadPart) / (double)m_frequency.QuadPart);
#else
		return((m_endTime.tv_sec - m_startTime.tv_sec))+((m_endTime.tv_nsec - m_startTime.tv_nsec)*1e-9);
#endif
	}

private:

#ifdef WIN32

	inline void SetTime(LARGE_INTEGER * pTime)
	{
		QueryPerformanceCounter(pTime);
	}

    LARGE_INTEGER m_frequency;
    LARGE_INTEGER m_startTime;
    LARGE_INTEGER m_endTime;

#else

	inline void SetTime(struct timespec * pTime)
	{
		clock_gettime(CLOCK_MONOTONIC, pTime);
	}

	struct timespec		m_startTime;
	struct timespec		m_endTime;

#endif

};
