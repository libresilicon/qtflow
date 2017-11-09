#ifndef CONTACTPLACEMENT_H
#define CONTACTPLACEMENT_H

#include "ui_contactplacement.h"

#include "project.h"

class ContactPlacement
{
public:
	ContactPlacement();
	void setProject(Project *p);

private:
	void refreshTables();

	Project *project;
};

#endif // CONTACTPLACEMENT_H
