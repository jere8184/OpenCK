
#include <Culture.hpp>

namespace openck::simulator
{
	template<>
	Base<GraphicalCulture>::FieldSetters Base<GraphicalCulture>::field_setters =
	{
		{"skin_tone", [](GraphicalCulture* graphical_culture, const Node& node){return node.get_value(graphical_culture->skin_tone);}},
		{"christian_portraits", [](GraphicalCulture* graphical_culture, const Node& node){return node.get_value(graphical_culture->christian_portraits);}}
	};
}