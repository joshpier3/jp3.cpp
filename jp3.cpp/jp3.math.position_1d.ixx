export module jp3.math:position_1d;

import :mathematical_unit;

namespace jp3::math
{
	export template<mathematical_unit unit_type>
	struct position_1d
	{
		unit_type x;
	};
}