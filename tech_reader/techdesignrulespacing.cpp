#include "techdesignrulespacing.h"

TechDesignRuleSpacing::TechDesignRuleSpacing() :
	b_touchingOK(false)
{

}

void TechDesignRuleSpacing::setTouchingOK(bool t)
{
	 b_touchingOK = t;
}

QString TechDesignRuleSpacing::getMaterial()
{
	return m_material;
}

void TechDesignRuleSpacing::setMaterial(QString m)
{
	m_material = m;
}

void TechDesignRuleSpacing::setSpacing(int i)
{
	m_spacing = i;
}

int TechDesignRuleSpacing::getSpacing()
{
	return m_spacing;
}

bool TechDesignRuleSpacing::getTouchingOK()
{
	return b_touchingOK;
}
