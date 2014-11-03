/// \author A0112054Y
#include "stdafx.h"

#include "../../../You-Utils/log.h"
#include "../../../You-DataStore/datastore.h"
#include "../../../You-DataStore/transaction.h"

#include "../model.h"
#include "../controller.h"
#include "add_task.h"
#include "batch_add_subtasks.h"
#include "delete_task.h"

namespace You {
namespace QueryEngine {
namespace Internal {
namespace Action {

namespace {
	using Transaction = You::DataStore::Transaction;
	using DataStore = You::DataStore::DataStore;
	using Log = You::Utils::Log;
}

const std::wstring DeleteTask::logCategory =
	Query::logCategory + L"[DeleteTask]";

std::unique_ptr<Query> DeleteTask::getReverse() {
	if (deletedTask.getSubtasks().empty()) {
		return std::unique_ptr<Query>(new AddTask(
			deletedTask.getID(),
			deletedTask.getDescription(),
			deletedTask.getDeadline(),
			deletedTask.getPriority(),
			deletedTask.getDependencies(),
			deletedTask.getSubtasks()));
	} else {
		std::vector<std::unique_ptr<Query>> subtaskAddQueries;
		// Construct add query for each subtask
		for (const auto& cid : deletedTask.getSubtasks()) {
			auto c = children[cid];
			subtaskAddQueries.push_back(
				std::unique_ptr<Query>(
					new AddTask(
						c.getDescription(),
						c.getDeadline(),
						c.getPriority(),
						c.getDependencies(),
						c.getSubtasks())));
		}
		return std::unique_ptr<Query>(new BatchAddSubTasks(
			deletedTask.getID(),
			deletedTask.getDescription(),
			deletedTask.getDeadline(),
			deletedTask.getPriority(),
			deletedTask.getDependencies(),
			std::move(subtaskAddQueries)));
	}
}

void DeleteTask::makeTransaction(const Task::ID id) {
	Log::debug << (boost::wformat(L"%1% : ERASE \"%2%\"\n")
		% logCategory % id).str();
	Transaction t(DataStore::get().begin());
	DataStore::get().erase(id);
	t.commit();
}

void DeleteTask::constructUndoTree(State& state, Task::ID id) {
	Task c = state.get().graph().getTask(id);
	for (auto cid : c.getSubtasks()) {
		constructUndoTree(state, cid);
	}
	children.insert({ id, c });
}

void DeleteTask::deleteFromStateAndDataStore(
	State& state, Task::ID id) {
	Controller::Graph::deleteTask(state.graph(), this->id);
	Controller::Graph::deleteTask(state.sgraph(), this->id);
	makeTransaction(id);
}

void DeleteTask::deleteTree(State& state, Task::ID id) {
	Task c = state.get().graph().getTask(id);
	for (auto cid : c.getSubtasks()) {
		deleteTree(state, cid);
	}
	deleteFromStateAndDataStore(state, id);
}

Response DeleteTask::execute(State& state) {
	deletedTask = state.get().graph().getTask(id);
	if (!deletedTask.getSubtasks().empty()) {
		deleteTree(state, id);
	} else {
		deleteFromStateAndDataStore(state, id);
	}
	return this->id;
}

}  // namespace Action
}  // namespace Internal
}  // namespace QueryEngine
}  // namespace You
