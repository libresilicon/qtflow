#include "techdesignrule.h"

TechDesignRule::TechDesignRule() :
	m_minWidth(0)
{

}

void TechDesignRule::setName(QString s)
{
	m_name = s;
}

void TechDesignRule::setMinimumWidth(int i)
{
	m_minWidth = i;
}

void TechDesignRule::setWidthMessage(QString m)
{
	m_widthMessage = m;
}

void TechDesignRule::setSpacing(QString name, int d, QString message, bool touching_ok)
{
	TechDesignRuleSpacing spacing;
	spacing.setTouchingOK(touching_ok);
	m_spacingList[name]=spacing;
}
