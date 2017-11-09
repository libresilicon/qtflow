#ifndef TECHDESIGNRULE_H
#define TECHDESIGNRULE_H

#include <QString>
#include <QPair>
#include <QMap>

#include "techdesignrulespacing.h"

class TechDesignRule
{
public:
	TechDesignRule();
	void setName(QString);
	QString getName();
	void setMinimumWidth(double i);
	void setWidthMessage(QString m);
	QString getWidthMessage();
	void setSpacing(QString name, int d, QString message, bool touching_ok);
	QStringList getSpacingRules();
	TechDesignRuleSpacing getSpacingRule(QString k);
	qreal getMinimumWidth();

private:
	qreal m_minWidth;
	QString m_name;
	QString m_widthMessage;
	QMap<QString,TechDesignRuleSpacing> m_spacingList;
};

#endif // TECHDESIGNRULE_H
