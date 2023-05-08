#include "opti_worker.h"

namespace pascal2c::code_generation::Optimizer {

bool OptimizerWorker::rotateProgram(NodePtr root) {
	auto program_ptr = std::dynamic_pointer_cast<ast::Program>(root);

	if (program_ptr == nullptr) {
		::fprintf(stderr , "Bad ast root type!\n");
		return false;
	}

	auto body_ptr = program_ptr->program_body();


	return true;
}

// void OptimizerWorker::pass(const NodePtr& cur) {
// }


} // End namespace 