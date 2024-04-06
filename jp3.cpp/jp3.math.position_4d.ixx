export module jp3.math:position_4d;

import :mathematical_unit;

namespace jp3::math
{
	export template<mathematical_unit unit_type>
	struct position_4d
	{
		unit_type x;
		unit_type y;
		unit_type z;
		unit_type w;
	};
}