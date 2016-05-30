#include "stdafx.h"
#include "Thread.h"

Thread::Thread(MainFunc mainFunc)
	:mFunc(mainFunc)
{
}


void Thread::Start( void )
{
	AfxBeginThread(ThreadEntry, this); 
}

UINT Thread::ThreadEntry( LPVOID pData)
{
    Thread* pThis = static_cast<Thread*>(pData);
	pThis->mFunc();

	return 0;
}