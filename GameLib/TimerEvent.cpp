
#include <stdio.h>
#include "TimerEvent.h"
#include "log.h"
#include <time.h>

// �ص�����
void handle_timeout(void* ptr)
{
	TimerEvent* sh = (TimerEvent*)ptr;
#ifdef WIN32
	stop_timer(&sh->m_ev);			// ����OnTimerǰ�棬��Ϊ�˷�ֹ��OnTimer����StartTimer
#endif
	sh->OnTimer(sh->m_ev.time_id);
}

TimerEvent::TimerEvent(void)
:m_bHaveStart(false)
,m_timeout(0)
,m_nId(0)
,m_TimeEvent(0)
{
	m_ev.time_id	= 0;
	m_ev.timer		= 0;
	m_ev.ptr		= this;
	m_ev.callback	= handle_timeout;
}

TimerEvent::~TimerEvent(void)
{
	StopTimer();
}

// ������ʱ��
void TimerEvent::StartTimer(int sec, int usec)
{
	log_debug("TimerEvent::StartTimer delay:=%d.", sec);

	if(m_bHaveStart)
		StopTimer();
    if(sec <= 0)
    {
        log_error("the timer is error %d ",sec);
        return;
    }

	m_timeout = sec;
	if(start_timer(sec, usec, &m_ev) < 0)
    {
        log_error("start_timer fail \n");
        return;
    }
	m_bHaveStart = true;
}
// ֹͣ��ʱ��
void TimerEvent::StopTimer()
{
	if(m_bHaveStart)
		stop_timer(&m_ev);
}
// ���ö�ʱ��
void TimerEvent::ResetTimer()
{
	StopTimer();
	StartTimer(m_timeout);
}
// ���ö�ʱ��ID
void TimerEvent::SetTimerId(int timer_id)
{
	m_ev.time_id = timer_id;
    m_nId = timer_id;
}
// ���ö�ʱ���ص�����
void TimerEvent::SetTimeEventObj(TimerOutEvent * obj, int id)
{
	m_nId = id;
	m_ev.time_id = id;
	m_TimeEvent = obj;
}
// ��ʱ����Ӧ����(��������)
void TimerEvent::OnTimer(int timer_id)
{
	m_bHaveStart = false;
	log_debug("the timer id is %d\n", timer_id);
	if(m_TimeEvent != 0)	
		m_TimeEvent->ProcessOnTimerOut(timer_id);
    else
        log_debug("Timeobj is null \n");  
}
