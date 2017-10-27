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
