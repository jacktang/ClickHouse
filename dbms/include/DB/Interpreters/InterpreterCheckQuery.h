#pragma once

#include <DB/Interpreters/Context.h>
#include <DB/Interpreters/IInterpreter.h>

namespace DB
{

class InterpreterCheckQuery : public IInterpreter
{
public:
	InterpreterCheckQuery(ASTPtr query_ptr_, Context & context_);
	BlockIO execute() override;

private:
	Block getSampleBlock() const;

private:
	ASTPtr query_ptr;
	Context context;
	Block result;
};

}
