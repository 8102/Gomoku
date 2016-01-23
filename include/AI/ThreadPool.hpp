#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <vector>
#include <queue>
#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <functional>
#include <stdexcept>

class ThreadPool
{
public:

    ThreadPool(size_t);
    template<class F, class... Args>
    auto enqueue(F&& f, Args&&... args) -> std::future<typename std::result_of<F(Args...)>::type>;
    ~ThreadPool();

private:
    std::vector<std::thread> _workers;
    std::queue<std::function<void()> > _tasks;

private:
    std::mutex _mutex;
    std::condition_variable _cond;
    bool _stop;

};

inline ThreadPool::ThreadPool(size_t threads)
    :   _stop(false)
{
    for(size_t i = 0; i < threads; ++i)
        _workers.emplace_back(
            [this]
            {
                for(;;)
                {
                    std::function<void()> task;

                    std::unique_lock<std::mutex> lock(this->_mutex);
                    this->_cond.wait(lock,
                        [this]{ return this->_stop || !this->_tasks.empty(); });
                    if(this->_stop && this->_tasks.empty())
                        return;
                    task = std::move(this->_tasks.front());
                    this->_tasks.pop();

                    task();
                }
            }
        );
}

template<class F, class... Args>
auto ThreadPool::enqueue(F&& f, Args&&... args) -> std::future<typename std::result_of<F(Args...)>::type>
{
    using return_type = typename std::result_of<F(Args...)>::type;

    auto task = std::make_shared< std::packaged_task<return_type()> >(
            std::bind(std::forward<F>(f), std::forward<Args>(args)...)
        );

    std::future<return_type> res = task->get_future();
    std::unique_lock<std::mutex> lock(_mutex);
    if(_stop)
        throw std::runtime_error("enqueue on stopped ThreadPool");

    _tasks.emplace([task](){ (*task)(); });
    _cond.notify_one();
    return res;
}

inline ThreadPool::~ThreadPool()
{
    std::unique_lock<std::mutex> lock(_mutex);
    _stop = true;
    _cond.notify_all();
    for(std::thread &worker: _workers)
        worker.join();
}

#endif
