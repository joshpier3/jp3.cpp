export module id_test;

import jp3.tdd;

export class id_test : public jp3::tdd::test 
{
public:
	id_test() : jp3::tdd::test("id")
	{

	}

	void run()
	{
		default_construct_creates_an_invalid_id();
		id_construct_with_a_valid_value_creates_a_valid_id();
		generate_unique_id_for_the_same_type_actually_generates_a_unique_id();
		generate_unique_id_for_different_types_generates_the_same_id_value();
		generate_unique_id_on_invalid_being_zero_first_id_is_one();
	}

private:
	void default_construct_creates_an_invalid_id()
	{
		jp3::id<> id{};
		require(!id.is_valid(), "Default construct creates an invalid id.");
	}

	void id_construct_with_a_valid_value_creates_a_valid_id()
	{
		jp3::id<> id{5};
		require(id.is_valid(), "Construct with a valid value creates a valid id.");
	}

	void generate_unique_id_for_the_same_type_actually_generates_a_unique_id()
	{
		auto id1 = jp3::generate_unique_id_for<double>();
		auto id2 = jp3::generate_unique_id_for<double>();
		auto id3 = jp3::generate_unique_id_for<double>();

		require(id1 != id2 && id1 != id3 && id2 != id3, "Generate unique id for the same type actually generates a unique id.");
	}

	void generate_unique_id_for_different_types_generates_the_same_id_value()
	{
		auto id1 = jp3::generate_unique_id_for<int>();
		auto id2 = jp3::generate_unique_id_for<short>();

		require(id1.get_value() == id2.get_value(), "Generate unique id for different types generates the same id value.");
	}

	void generate_unique_id_on_invalid_being_zero_first_id_is_one()
	{
		auto id = jp3::generate_unique_id_for<long, std::size_t, 0>();

		require(id.get_value() == 1, "Generate unique id on invalid being zero. First id has a value of one.");
	}
};