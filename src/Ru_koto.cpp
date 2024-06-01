#include "Ru_koto.h"


void RuKoto::Config::SetUniversalContent(UniversalContent* m_class){
	m_UniversalContent = m_class;
}

RuKoto::UniversalContent* RuKoto::UniversalContent::get_m_UniversalContent()
{
	return m_UniversalContent;
}
