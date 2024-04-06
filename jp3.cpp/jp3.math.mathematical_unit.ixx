export module jp3.math:mathematical_unit;

import std;

namespace jp3::math
{
	template<typename type>
	concept mathematical_unit = std::is_arithmetic<type>::value;
}