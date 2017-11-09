#include "techdesignrule.h"

TechDesignRule::TechDesignRule() :
	m_minWidth(0)
{

}

void TechDesignRule::setName(QString s)
{
	m_name = s;
}

QString TechDesignRule::getName()
{
	return m_name;
}

void TechDesignRule::setMinimumWidth(double i)
{
	m_minWidth = i;
}

qreal TechDesignRule::getMinimumWidth()
{
	return m_minWidth;
}

void TechDesignRule::setWidthMessage(QString m)
{
	m_widthMessage = m;
}

QString TechDesignRule::getWidthMessage()
{
	return m_widthMessage;
}

void TechDesignRule::setSpacing(QString name, int d, QString message, bool touching_ok)
{
	TechDesignRuleSpacing spacing;
	QString material;
	QStringList nl;

	nl = name.split(',');

	foreach(material, nl) {
		if(m_spacingList.contains(material))
			spacing = m_spacingList[material];

		spacing.setMaterial(material);
		spacing.setTouchingOK(touching_ok);
		spacing.setSpacing(d);

		m_spacingList[material]=spacing;
	}
}

QStringList TechDesignRule::getSpacingRules()
{
	return m_spacingList.keys();
}

TechDesignRuleSpacing TechDesignRule::getSpacingRule(QString k)
{
	if(m_spacingList.contains(k))
		return m_spacingList[k];
	return TechDesignRuleSpacing();
}
