
#include "utils/status_code.hpp"

#include <vector>
#include <stack>
#include <cstddef>


namespace openck 
{

namespace simulator
{
    struct Charecter;
    struct Title;
    struct Province;
    struct War;
    struct Flank;
    struct UnitType;
    struct Trait;
    struct Religion;
    struct Culture;
    struct Society;
    struct Artifact;
    struct ReligionGroup;
	struct BloodLine;
};


struct Scope : ConditionBlock
{

};


struct CharecterScope : Scope
{
	enum struct OpCode
	{
		RETURN,
		CONTROLS_RELIGION,
	};

	simulator::Charecter& charecter; ///< The charecter to be evaluated.

	CharecterScope(simulator::Charecter& charecter, std::vector<int8_t>::iterator ip);

	bool execute();
	
	bool controls_religion() const;
};

struct ConditionBlock
{
	enum struct OpCode : int8_t
	{
		RETURN,
		SCOPE_CHARECTER,
	};

	enum struct CharecterOpCode : int8_t
	{
		CONTROLS_RELIGION,
		RELIGION,
		SCOPE_ANY_OWNED_BLOODLINE
	};

	enum struct BlockType : int8_t 
	{
		OR,
		AND,
		NOR,
		NAND
	};

	std::vector<int8_t> instructions; ///< List of instructions to execute when evaluating this block.
	std::vector<int8_t>::iterator ip; ///< Instruction pointer.
	std::vector<void*> pointers; ///< List of pointers
	std::stack<BlockType> stack;
	
	void* root;

	bool is_or;
	bool is_and;

	bool execute();

	bool execute_charecter();

	bool controls_religion(simulator::Charecter* charecter);

	bool religion(simulator::Charecter* charecter);

	void* load_pointer();

	bool advance();

	StatusCode evaluate_result(bool result);
};

}