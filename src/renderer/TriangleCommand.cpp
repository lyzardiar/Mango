#include "TriangleCommand.h"
#include "PipeLine.h"



void RE::TriangleCommand::Commit() {
	PipeLine::instance.AddCommand(this);
}
