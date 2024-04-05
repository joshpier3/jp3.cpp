export module tests;

import std;
import jp3.tdd;
import id_test;

export class all_tests
{
public:
	all_tests()
	{
		m_tests.reserve(25);

		m_tests.emplace_back(id_test{});

	}

	void run()
	{
		for (test& test : m_tests)
		{
			std::visit(
				[](auto& test)
				{
					jp3::tdd::run_test(test);
				},
				test
			);
		}
	}

private:
	using test = std::variant<id_test>;
	std::vector<test> m_tests{};
};