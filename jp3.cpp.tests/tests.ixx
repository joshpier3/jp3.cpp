export module tests;

import std;
import jp3.tdd;
import id_test;
import unique_resource_test;

export class all_tests
{
public:
	void run()
	{
		std::ofstream test_results{ "test_results.txt", std::ios::ate };

		if (!test_results)
			throw std::runtime_error{ "Failed to open test_results.txt for writing." };

		using clock_t = std::chrono::high_resolution_clock;
		using timepoint_t = decltype(clock_t::now());

		timepoint_t start_time{};

		const auto test_start {
			[&](std::string_view test_name)
			{
				std::println(test_results, "Running test \"{}\".", test_name);
				start_time = clock_t::now();
			}
		};

		const auto test_finish{
			[&](std::string_view test_name, int total_cases, int num_cases_passed)
			{
				const auto running_time = clock_t::now() - start_time;
				const bool passed = num_cases_passed == total_cases;
				const std::string_view test_result = passed ? "passed" : "failed";

				std::println(test_results, "Test \"{}\" {} ({}/{} passed) time : {}.\n\n", test_name, test_result, num_cases_passed, total_cases, running_time);
			}
		};

		const auto test_case_finish{
			[&](std::string_view case_name, bool passed)
			{
				const std::string_view result = passed ? "passed" : "failed";

				std::println(test_results, "{} : {}.", case_name, result);
			}
		};

		const auto run_test{
			[&](auto& test) -> bool
			{
				const auto test_start_id = test.on_test_started.subscribe(test_start);
				const auto test_finish_id = test.on_test_finished.subscribe(test_finish);
				const auto test_case_finish_id = test.on_test_case_finished.subscribe(test_case_finish);

				const bool test_result = jp3::tdd::run_test(test);

				test.on_test_started.unsubscribe(test_start_id);
				test.on_test_finished.unsubscribe(test_finish_id);
				test.on_test_case_finished.unsubscribe(test_case_finish_id);

				return test_result;
			}
		};

		run_test(id);
		run_test(unique_resource);
	}

private:
	id_test id{};
	unique_resource_test unique_resource{};
};