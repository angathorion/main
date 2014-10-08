#include "stdafx.h"
#include "internal/internal_datastore.h"
#include "internal/operations/post_operation.h"
#include "internal/operations/put_operation.h"
#include "internal/operations/erase_operation.h"
#include "datastore.h"

namespace You {
namespace DataStore {

DataStore::DataStore()
: internalDataStore(new Internal::InternalDataStore) {
}

DataStore& DataStore::get() {
	static DataStore ds;
	return ds;
}

Transaction& DataStore::begin() {
	while (this->isServing) { }  // for thread-safety
	isServing = true;
	internalDataStore->loadData();
	transactionStack.push(std::shared_ptr<Transaction>(new Transaction()));
	return *(transactionStack.top());
}

void DataStore::post(TaskId taskId, const SerializedTask& task) {
	std::shared_ptr<Internal::IOperation> operation =
		std::make_shared<Internal::PostOperation>(taskId, task);
	transactionStack.top()->push(operation);
}

void DataStore::put(TaskId taskId, const SerializedTask& task) {
	std::shared_ptr<Internal::IOperation> operation =
		std::make_shared<Internal::PutOperation>(taskId, task);
	transactionStack.top()->push(operation);
}

void DataStore::erase(TaskId taskId) {
	std::shared_ptr<Internal::IOperation> operation =
		std::make_shared<Internal::EraseOperation>(taskId);
	transactionStack.top()->push(operation);
}

std::vector<SerializedTask> DataStore::getAllTask() {
	return internalDataStore->getAllTask();
}

void DataStore::notifyCommit() {
	bool isSaved = internalDataStore->saveData();
	if (isSaved) {
		isServing = false;
	}
	// TODO(digawp): else throw exception?
}

void DataStore::notifyRollback() {
	transactionStack.pop();
	isServing = false;
}

Internal::InternalDataStore& DataStore::getInternal() {
	return *internalDataStore.get();
}

}  // namespace DataStore
}  // namespace You