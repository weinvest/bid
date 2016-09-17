#ifndef _THREAD_CHECKER_H
#define _THREAD_CHECKER_H
#include <atomic>
#include <string>
struct ThreadCheckContext
{
    ThreadCheckContext(const std::string& name);

    std::atomic<int32_t> WriterCount;
    std::atomic<int32_t> ReaderCount;
    const std::string Name;
};

class ThreadChecker
{
public:
    ThreadChecker(ThreadCheckContext& context, bool isRead = false);

    ~ThreadChecker();

private:
    ThreadCheckContext& mContext;
    bool mIsRead;
};
#endif
