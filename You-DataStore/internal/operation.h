#pragma once
#ifndef YOU_DATASTORE_INTERNAL_OPERATION_H_
#define YOU_DATASTORE_INTERNAL_OPERATION_H_

#include "../task_typedefs.h"

namespace You {
namespace DataStore {
namespace Internal {

/// A pure virtual class of operations to be put into transaction stack
class IOperation {
public:
	/// Executes the operation
	virtual bool run() = 0;
protected:
	TaskId taskId;
	SerializedTask task;
};

}  // namespace Internal
}  // namespace DataStore
}  // namespace You

#endif  // YOU_DATASTORE_INTERNAL_OPERATION_H_
