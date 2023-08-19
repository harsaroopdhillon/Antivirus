#pragma once
#include "SyntaxTree.h"

namespace compiler
{
	class Data : public SyntaxTree
	{
	public:
		Data(const std::string &data)
			: data(data)
		{}

		virtual ~Data() = default;

		virtual std::string toCode()
		{
			if (data[0] == '\"' && data[data.length() - 1] == '\"')
			{
				data.erase(0, 1); data.erase(data.length() - 1);
			}
			return data;
		}

	private:
		std::string data;
	};
}
