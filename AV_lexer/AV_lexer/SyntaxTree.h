#include <vector>
#include <string>
#include <memory>

namespace compiler
{
	class SyntaxTree
	{
	public:
		SyntaxTree() = default;

		virtual ~SyntaxTree() = default;

		virtual std::string toCode() = 0;

	private:
		std::vector<std::unique_ptr<SyntaxTree>> children;

	};
}
