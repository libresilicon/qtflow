#!/usr/bin/env python
# -*- coding: utf-8 -*-

def write_def_header(fdef):
	fdef.write("VERSION 5.6 ;")
	fdef.write("\n")
	fdef.write("NAMESCASESENSITIVE ON ;")
	fdef.write("\n")
	fdef.write("DIVIDERCHAR \"/\" ;")
	fdef.write("\n")
	fdef.write("BUSBITCHARS \"<>\" ;")
	fdef.write("\n")
	fdef.write("DESIGN "+project_settings.getTopLevel()+" ;")
	fdef.write("\n")
	fdef.write("UNITS DISTANCE MICRONS "+str(project_settings.getDistanceUnits())+" ;")
	fdef.write("\n")

def write_die_area(fdef):
	cellxbot = project_settings.getDistanceUnits()
	cellybot = project_settings.getDistanceUnits()

	fdef.write("DIEAREA ( -500 500 ) ( -500 500 ) ;")
	fdef.write("\n")

def place2def():
	pl1name = path.join(project_settings.getLayoutDir(), project_settings.getTopLevel()+".pl1")
	pl2name = path.join(project_settings.getLayoutDir(), project_settings.getTopLevel()+".pl2")
	pinname = path.join(project_settings.getLayoutDir(), project_settings.getTopLevel()+".pin")
	defname = path.join(project_settings.getLayoutDir(), project_settings.getTopLevel()+".def")
	obsname = path.join(project_settings.getLayoutDir(), project_settings.getTopLevel()+".obs")
	infoname = path.join(project_settings.getLayoutDir(), project_settings.getTopLevel()+".info")

	fdef = open(defname,"w")

	write_def_header(fdef)
	write_die_area(fdef)

	fdef.write("END DESIGN\n")

	fdef.close()

	print project_settings.getTopLevel()
