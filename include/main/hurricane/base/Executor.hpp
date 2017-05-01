#pragma once

#include "../message/MessageLoop.h"
#include <thread>

namespace hurricane {
	namespace base {

		template<typename TaskType>
		class Executor {
		public:
			enum class Status {
				Stopping,
				Running,
			};

			Executor() : m_status(Status::Stopping) {}
			virtual ~Executor() {}

			/// @param name 任务名
			/// @param task 用户传递的任务
			void StartTask(std::string const& name, TaskType* task) {
				m_taskName = name;
				m_task = std::shared_ptr<TaskType>(task);
				m_thread = std::thread(std::bind(&Executor::StartThread, this));
			}

			virtual void StopTask() {
				m_messageLoop.stop();
			}

			Status status()const { return m_status; }
			std::string const& GetTaskName()const { return m_taskName; }
		protected:
			virtual void OnCreate() = 0;
			virtual void OnStop() = 0;
			std::shared_ptr<TaskType>	m_task;
			message::MessageLoop		m_messageLoop;

		private:
			void StartThread() {
				m_status = Status::Running;
				OnCreate();
				m_messageLoop.run();
				OnStop();
			}

			std::thread	m_thread;
			Status		m_status;
			std::string m_taskName;
		};
	}
}
