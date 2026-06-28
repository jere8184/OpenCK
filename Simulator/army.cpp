
#include "army.hpp"

namespace openck::simulator
{
	UnitType light_infantry {"light_infantry"};
	UnitType heavy_infantry {"heavy_infantry"};
	UnitType pikemen {"pikemen"};
	UnitType light_cavalry {"light_cavalry"};
	UnitType knights {"knights"};
	UnitType archers {"archers"};
	UnitType special_troops {"special_troops"};

	std::vector<UnitType> unit_vector {light_infantry,
											heavy_infantry,
											pikemen,
											light_cavalry,
											knights,
											archers,
											special_troops};

	UnitType::UnitType(std::string name) : Base(name)
	{}

	void UnitType::init_static_objects()
	{
		map =
		{
			{"light_infantry", UnitType("light_infantry")},
			{"heavy_infantry", UnitType("heavy_infantry")},
			{"pikemen", UnitType("pikemen")},
			{"light_cavalry", UnitType("light_cavalry")},
			{"knights", UnitType("knights")},
			{"archers", UnitType("archers")},
			{"special_troops", UnitType("special_troops")}
		};
	}
}