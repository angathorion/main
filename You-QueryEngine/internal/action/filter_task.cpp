/// \author A0112054Y
#include "stdafx.h"

#include  "../task_serializer.h"
#include  "../state.h"
#include "filter_task.h"

namespace You {
namespace QueryEngine {
namespace Internal {
namespace Action {

Response FilterTask::execute(State& tasks) {
	std::vector<Task> result;
	auto filter = this->filter;
	std::vector<Task> all = tasks.graph().getTaskList();
	std::for_each(all.begin(), all.end(),
		[filter, &result] (const Task task) {
			if (filter(task)) {
				result.push_back(task);
			}
		}
	);
	return result;
}

}  // namespace Action
}  // namespace Internal
}  // namespace QueryEngine
}  // namespace You
