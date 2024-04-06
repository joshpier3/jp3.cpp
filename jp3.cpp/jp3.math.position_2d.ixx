export module jp3.math:position_2d;

import :mathematical_unit;

namespace jp3::math
{
	export template<mathematical_unit unit_type>
	struct position_2d
	{
		unit_type x;
		unit_type y;
	};
}