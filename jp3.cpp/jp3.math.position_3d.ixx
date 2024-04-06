export module jp3.math:position_3d;

import :mathematical_unit;

namespace jp3::math
{
	export template<mathematical_unit unit_type>
	struct position_3d
	{
		unit_type x;
		unit_type y;
		unit_type z;
	};
}