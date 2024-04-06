export module unique_resource_test;

import jp3.tdd;
import jp3.core;

struct test_deleter
{
	inline static bool called{ false };

	void operator()([[maybe_unused]] int i) noexcept
	{
		called = true;
	}
};

export class unique_resource_test : public jp3::tdd::test
{
public:
	unique_resource_test() : jp3::tdd::test("unique_resource")
	{}

	void run()
	{
		default_construct_creates_an_invalid_resource();
		construct_from_valid_resource_creates_a_valid_unique_resource();
		reset_actually_resets_the_resource();
		release_ownership_invalidates_its_resource_and_returns_the_valid_resource();
	}

private:
	using test_resource = jp3::unique_resource<int, 0, test_deleter>;

	void default_construct_creates_an_invalid_resource()
	{
		test_resource resource{};
		require(!resource.is_valid() && !resource, "Default construction creates an invalid resource.");
	}

	void construct_from_valid_resource_creates_a_valid_unique_resource()
	{
		test_resource resource{ 5 };
		require(resource.is_valid() && static_cast<bool>(resource), "Constructing from a valid resource creates a valid unique resource object.");
	}

	void reset_actually_resets_the_resource()
	{
		test_resource resource{ 5 };
		resource.reset();

		require(!resource.is_valid() && test_deleter::called && resource.get() == 0, "A call to unique_resource::reset actually resets the resource.");
	}

	void release_ownership_invalidates_its_resource_and_returns_the_valid_resource()
	{
		test_resource resource{ 5 };
		auto value = resource.release_ownership();

		require(!resource.is_valid() && value == 5, "A call to unique_resource::release_ownership invalidates it's resource and returns the held resource.");
	}
};