#pragma once

#include <vector>
#include <deque>
#include <future>
#include <memory>
#include <condition_variable>
#include <thread>
#include <functional>

namespace Space
{

	template<class ResultType>
	class AsyncTask
	{
	public:
		typedef typename std::function<ResultType(void)> InvokeFuncType;
		typedef typename std::function<void(ResultType)> ThenFuncType;
		AsyncTask(InvokeFuncType&& func)
			:_invoke_func(std::forward<InvokeFuncType>(func))
		{
		}

		void then(ThenFuncType func)
		{
			_then_func = func;
		}

		void invoke() {
			auto result = _invoke_func();
			_promise.set_value(result);
		}

		void then_invoke()
		{
			if (_then_func)
				_then_func(_promise.get_future().get());
		}

	private:
		std::promise<ResultType> _promise;
		ThenFuncType _then_func = nullptr;
		InvokeFuncType _invoke_func = nullptr;
	};

	template <typename TaskType>
	class FIFOSchedulePolicy
	{
	public:
		bool push(std::shared_ptr<TaskType> task)
		{
			_container.push_back(task);
			return true;
		}

		size_t size() const
		{
			return _container.size();
		}

		bool empty() const
		{
			return _container.empty();
		}
		std::shared_ptr<TaskType> pop()
		{
			auto ret = _container.front();
			_container.pop_front();
			return ret;
		}
	private:
		std::deque<std::shared_ptr<TaskType>> _container;
	};

	template<
		typename TaskType,
		typename SchedulePolicy = FIFOSchedulePolicy<TaskType>>
		class ThreadPool : std::enable_shared_from_this<ThreadPool<TaskType, SchedulePolicy>>
	{
	public:
		typedef typename TaskType TaskType;
		void Schedule(std::shared_ptr<TaskType> task)
		{
			std::lock_guard<std::recursive_mutex> lock(_mutex);
			if (_scheduler.push(task))
			{
				_task_event.notify_one();
			}
		}

		bool Poll()
		{
			std::shared_ptr<TaskType> task;
			{
				std::unique_lock<std::recursive_mutex> wait_lock(_finish_mutex);
				if (_finished_tasks.empty())
				{
					_task_finished.wait(wait_lock);
				}
				if (!_finished_tasks.empty())
				{
					task = _finished_tasks.front();
					_finished_tasks.pop_front();
				}
			}
			if (task)
			{
				task->then_invoke();
			}
			if (_alive_workers <= 0)
				return false;
			else
				return true;
		}

		bool Execute()
		{
			std::shared_ptr<TaskType> task;
			{
				std::unique_lock<std::recursive_mutex> wait_lock(_mutex);
				while (_scheduler.empty())
				{
					if (_stopped)
						return false;
					_active_workers--;
					_task_event.wait(wait_lock);
					_active_workers++;
				}
				task = _scheduler.pop();
			}
			if (task)
			{
				task->invoke();
				std::lock_guard<std::recursive_mutex> lock(_finish_mutex);
				_finished_tasks.push_back(task);
				_task_finished.notify_all();
			}
			return true;
		}

		ThreadPool()
		{
			workers.resize(4);
			for (int i = 0; i < 4; i++)
			{
				workers[i] = std::make_shared<std::thread>(
					[this]() {
					_alive_workers++;
					while (this->Execute()) {}
					_alive_workers--;
				});
				_active_workers++;
			}
		}

		void terminate()
		{
			_stopped = true;
			_task_event.notify_all();
			_task_finished.notify_all();
		}

		void join()
		{
			for (auto worker : workers)
				if (worker)
					worker->join();
		}
		~ThreadPool()
		{
		}
	private:
		SchedulePolicy _scheduler;
		std::deque<std::shared_ptr<TaskType>> _finished_tasks;
		std::recursive_mutex _mutex;
		std::recursive_mutex _finish_mutex;

		std::vector<std::shared_ptr<std::thread>> workers;

		std::condition_variable_any _task_event;
		std::condition_variable_any _task_finished;
		std::atomic<int> _alive_workers = 0;
		std::atomic<int> _active_workers = 0;
		std::atomic<bool> _stopped = false;
	};


	template <typename Pool, typename Func, typename ...Args>
	typename std::shared_ptr<typename AsyncTask<typename std::result_of<Func(Args...)>::type>>
		Schedule(const std::shared_ptr<Pool> pool, Func&& func, Args&&... args)
	{
		auto task = std::make_shared<AsyncTask<typename std::result_of<Func(Args...)>::type>>(
			std::bind(std::forward<Func>(func), std::forward<Args>(args)...));
		pool->Schedule(task);
		return task;
	}

	template< typename Pool>
	void Schedule(const std::shared_ptr<Pool> pool, const std::shared_ptr<typename Pool::TaskType> task)
	{
		pool->Schedule(task);
	}
}