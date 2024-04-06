export module jp3.tdd;

/* 
Use template below this line to create a test.
class [test_name] : public jp3::tdd::test
{
public:
	[test_name]() : jp3::tdd::test("[YOUR HUMAN READABLE TEST NAME HERE]")
	{
		//You can subscribe to events here to add needed functionality to your tests.
		//Look below for event definitions.
		//on_test_started.subscribe([](const std::string_view test_name){});
		//on_test_finished.subscribe([](const std::string_view test_name, const int num_total_cases, const int num_passed_cases){});
		//on_case_started.subscribe([](const std::string_view case_name){});
		//on_case_finished.subscribe([](const std::string_view case_name, const bool case_passed));
	}

	//The code in this function executes on a call to jp3::tdd::run_test.
	void run()
	{
		//Call your test methods here.
		test_method_with_a_good_understandable_name_goes_here();
	}

private:
	//Your test methods go below.

	void test_method_with_a_good_understandable_name_goes_here()
	{
		//Call the "require" function to state your case.
		require(1 == 1, "Test case with a good understandable statement / name goes here.");
	}
};
*/

import std;
import jp3.core;

namespace jp3::tdd
{
	export class test
	{
	public:
		explicit test(const std::string_view name) :
			m_name{ name }
		{
			if (m_name.empty())
				throw std::invalid_argument{ "A test must have a name." };

			m_results.reserve(25);
		}

		std::string_view get_name() const noexcept
		{
			return m_name;
		}

		event<std::string_view> on_test_started { 1 };
		event<std::string_view> on_test_case_started { 1 };
		event<std::string_view, bool> on_test_case_finished { 1 };
		event<std::string_view, int, int> on_test_finished { 2 };

	protected:
		bool require(const bool condition, const std::string_view case_name)
		{
			if (case_name.empty())
				throw std::invalid_argument{ "The test::require method must have case name." };

			on_test_case_started.fire(case_name);

			std::println("{} : {}", case_name, condition ? "passed" : "failed");

			m_results.push_back(condition);

			on_test_case_finished.fire(case_name, condition);

			return condition;
		}

	private:
		std::string m_name;
		std::vector<bool> m_results{};
		decltype(std::chrono::high_resolution_clock::now()) m_start_time{};

		template<typename test_type> 
		requires(std::is_base_of_v<test, test_type>)
		friend inline bool run_test(test_type&);

		void start_test()
		{
			on_test_started.fire(m_name);

			std::println("Running test \"{}\".", m_name);

			m_start_time = std::chrono::high_resolution_clock::now();
		}

		void end_test()
		{
			const auto total_time = std::chrono::high_resolution_clock::now() - m_start_time;

			const auto num_passed = std::ranges::count_if(m_results, [](const bool case_result) { return case_result; });
			const auto total_cases = static_cast<decltype(num_passed)>(m_results.size());
			const auto num_failed = total_cases - num_passed;
			const bool test_passed = num_failed == 0;

			std::println("Test {} {} ({}/{} passed) time : {}.\n", m_name, test_passed ? "passed" : "failed", num_passed, total_cases, total_time);

			on_test_finished.fire(m_name, static_cast<int>(total_cases), static_cast<int>(num_passed));
		}
	};

	export template<typename test_type>
	requires(std::is_base_of_v<test, test_type>)
	inline bool run_test(test_type& test)
	{
		bool test_passed{ false };
		const auto id = test.on_test_finished.subscribe(
			[&test_passed](
				[[maybe_unused]] std::string_view test_name,
				int total_cases,
				int num_cases_passed
			) noexcept
			{
				test_passed = total_cases == num_cases_passed;
			}
		);

		test.start_test();
		test.run();
		test.end_test();

		test.on_test_finished.unsubscribe(id);

		return test_passed;
	}
}