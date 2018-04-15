#pragma once
#if !defined(DEF_TTIMING)
#define DEF_TTIMING

#include <Windows.h>

class TTiming
{
protected:
	LARGE_INTEGER m_liStart;
	LARGE_INTEGER m_liStop;

	LONGLONG m_llFrequency;
	LONGLONG m_llCorrection;

public:
	TTiming(void);

	void Begin(void);
	double End(void);
};

inline TTiming::TTiming(void)
{
	LARGE_INTEGER liFrequency;

	QueryPerformanceFrequency(&liFrequency);
	m_llFrequency = liFrequency.QuadPart;

	// Calibration
	Sleep(0);
	QueryPerformanceCounter(&m_liStart);
	QueryPerformanceCounter(&m_liStop);

	m_llCorrection = m_liStop.QuadPart - m_liStart.QuadPart;
}

inline void TTiming::Begin(void)
{
	Sleep(0);
	QueryPerformanceCounter(&m_liStart);
}

inline double TTiming::End(void)
{
	QueryPerformanceCounter(&m_liStop);

	return (double)(m_liStop.QuadPart - m_liStart.QuadPart - m_llCorrection)*1000.0 / m_llFrequency;
}

#endif
