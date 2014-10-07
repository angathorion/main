/// \author A0112054Y
#include "stdafx.h"

#include "internal/action/add_task.h"
#include "internal/action/update_task.h"
#include "internal/action/delete_task.h"
#include "internal/action/filter_task.h"
#include "internal/action/delete_task.h"
#include "internal/action/update_task.h"
#include "internal/state.h"
#include "api.h"

namespace You {
namespace QueryEngine {

std::unique_ptr<Query>
AddTask(Task::Description description, Task::Time deadline,
	Task::Priority priority, Task::Dependencies dependencies) {
	using AddTask = Internal::Action::AddTask;
	return std::unique_ptr<Query>(new AddTask(description, deadline,
		priority, dependencies));
}

std::unique_ptr<Query>
FilterTask(const Filter& filter) {
	using FilterTask = Internal::Action::FilterTask;
	return std::unique_ptr<Query>(new FilterTask(filter));
}

std::unique_ptr<Query>
DeleteTask(Task::ID id) {
	using DeleteTask = Internal::Action::DeleteTask;
	return std::unique_ptr<Query>(new DeleteTask(id));
}

std::unique_ptr<Query>
UpdateTask(Task::ID id, Task::Description description,
Task::Time deadline, Task::Priority priority, Task::Dependencies dependencies) {
	using UpdateTask = Internal::Action::UpdateTask;
	return std::unique_ptr<Query>(new UpdateTask(id,
		description, deadline, priority, dependencies));
}

Response executeQuery(std::unique_ptr<Query> query) {
	return query->execute(Internal::State::get());
}

}  // namespace QueryEngine
}  // namespace You

