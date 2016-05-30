#ifndef _W_THREAD_H
#define _W_THREAD_H
#include <functional>
class Thread
{
public:
	typedef std::function<void ()> MainFunc;
	Thread(MainFunc mainFunc);

	void Start( void );
private:
	static UINT ThreadEntry( LPVOID pThis); 

	MainFunc mFunc;
};
#endif
