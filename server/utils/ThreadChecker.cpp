#include "DCallStack.h"
#include "ThreadChecker.h"
#include <iostream>
ThreadCheckContext::ThreadCheckContext(const std::string& name)
    :ReaderCount(0)
    ,WriterCount(0)
    ,Name(name)
{}

ThreadChecker::ThreadChecker(ThreadCheckContext& context, bool isRead)
    :mContext(context)
    ,mIsRead(isRead)
{
    int32_t writerCount = 0;
    if(isRead)
    {
        mContext.ReaderCount.fetch_add(1);
        writerCount = mContext.WriterCount.load();
    }
    else
    {
        writerCount = mContext.WriterCount.fetch_add(1);
    }

    if(0 != writerCount)
    {
        std::cerr << "MultiEnter: " << mContext.Name << '\n';
        OutputCallStack();
    }
}

ThreadChecker::~ThreadChecker()
{
    bool multiExit = false;
    if(mIsRead)
    {
        mContext.ReaderCount.fetch_sub(1);
        multiExit = mContext.WriterCount.load() > 0;
    }
    else
    {
        auto writerCount = mContext.WriterCount.fetch_sub(1) - 1;
        multiExit = writerCount > 0 || mContext.ReaderCount.load() != 0;
    }

    if(multiExit)
    {
        std::cerr << "MultiExit: " << mContext.Name << '\n';
        OutputCallStack();
    }
}
