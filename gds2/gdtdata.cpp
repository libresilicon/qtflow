#include "gdsfile.h"
#include "gdtdata.h"

GDTData::GDTData(QString fileName)
{
	//qDebug() << __FUNCTION__ << " Loading file: " << fileName;
	m_fileName = fileName;
	m_file = new GDSFile(fileName.toStdString().c_str(), READ);

	m_file->rdstrm();  // header
	//goThroughFile();
	buildDataStructure();
}

GDSCell* GDTData::getCell(QString name)
{
	foreach(GDSCell *cell, m_cells) {
		if(cell->getName()==name) return cell;
	}
	return NULL;
}

QVector<GDSCell*> GDTData::getCells()
{
	return m_cells;
}

bool GDTData::containsCell(QString name)
{
	foreach(GDSCell *cell, m_cells) {
		if(cell->getName()==name) return true;
	}
	return false;
}

void GDTData::buildDataStructure()
{
	GDSCell* m_recentCell = NULL;
	GDSBoundary* m_recentBoundary = NULL;
	int layer = 0;
	int rectyp = 0;
	int xcoord = 0;
	int ycoord = 0;

	while (! m_file->eof())
	{
		m_file->rdstrm();
		rectyp = m_file->rectyp();
		if (rectyp == BGNSTR) {
			//qDebug() << "BGNSTR:";
		} else if (rectyp == STRNAME) {
			QString cellname(m_file->record());
			m_recentCell = new GDSCell(cellname);
			m_cells.append(m_recentCell);
			//qDebug() << "STRNAME: " << cellname;
		} else if (rectyp == LAYER) {
			layer = m_file->getI16();
			//qDebug() << "\t Layer : " << layer;
		} else if (rectyp == BOX) {
			//qDebug() << "Box:";
		} else if (rectyp == PATH) {
			//qDebug() << "Path:";
		} else if (rectyp == DATATYPE) {
			//qDebug() << "\t Data type:";
		} else if (rectyp == TEXT) {
			//qDebug() << "\t Text";
		} else if (rectyp == TEXTTYPE) {
			//qDebug() << "\t TEXTTYPE";
			//qDebug() << m_file->getI16();
		} else if (rectyp == STRING) {
			//qDebug() << QString(m_file->record());
		} else if (rectyp == ENDEL) {
			if(m_recentCell && m_recentBoundary) m_recentCell->addBoundary(m_recentBoundary);
			m_recentBoundary = NULL;
			//qDebug() << "\t End entry";
		} else if (rectyp == BOUNDARY) {
			if(m_recentCell) {
				//qDebug() << "Boundary:";
				m_recentBoundary = new GDSBoundary(layer);
			}
		} else if (rectyp == ENDSTR) {
			//qDebug() << "End Str";
		} else if (rectyp == BOXTYPE) {
			//qDebug() << "Box type:";
		} else if (rectyp == XY) {
			for(int i=0; i< m_file->length(); i+=8) {
				xcoord = m_file->getI32(i);
				ycoord = m_file->getI32(i+4);
				//qDebug() << "\t\t x : " << xcoord << " y : " << ycoord;
				if(m_recentBoundary) m_recentBoundary->addPoint(xcoord,ycoord);
			}
		} else if (rectyp == SREF) {
			//qDebug() << "SREF";
		} else if (rectyp == SNAME) {
			//qDebug() << "SNAME: " << QString(m_file->record());
		} else if (rectyp == STRANS) {
			//qDebug() << "STRANS: " << m_file->getI16();
		} else {
			//qDebug() << "entry type: " << rectyp;
		}
	}
}

void GDTData::goThroughFile()
{
	int     i,
			rectyp,
			length,
			debug=0,
			extn,
			width,
			precision = 0,
			haveInputName=0,
			haveOutputName=0,
			inInFileName=0,
			inOutFileName=0,
			inPrecision=0,
			inProperty=0,
			inBoundary=0;
	short   Ref, columns, rows, layer, dataType, textType, propAttr,
			year, month, day, hour, min, sec,
			version_num;
	double  userUnits=0.001,           //1e-3 is common default
			dataBaseUnits=0.000000001, //1e-9 is common default
			Ang=0.0,
			epsilon=(userUnits/2000.0), //use to "fix" floating point error
			Mag=0.0;
	char    oneChar,
			precisionString[128] = "0.001",
			bigString[204800] = "",
			string512[513] = ""; // 512 + room for '\0'
	stringL inputFile, outputFile, tmpFile, strname, tmpString1, tmpString2;
	stringS xstring, ystring, extnstring, wstring;
	FILE    *fpOut;
	long    xcoord=0,
			ycoord=0;

	while (! m_file->eof())
	{
		m_file->rdstrm();
		rectyp = m_file->rectyp();
		if (debug) cerr << "DEBUG line:" << __LINE__ << "rectyp=" << rectyp << endl;
		if ((rectyp < 0) || (rectyp > 59))
		{
			fprintf(stderr, "ERROR: invalid record type:%d found in gds2 file. Note: May have just overflowed due to a super long record\n", rectyp);
			exit(1);
		}
		//if (rectyp == LIBNAME)
		//{
			//m_file->libName((char*) m_file->record());
			//printf("lib '%s'",m_file->record());
			//strcpy(strname, m_file->record());
		//}
		//else
		if (rectyp == BGNSTR)
		{
			year  = m_file->getI16(0);
				if (year < 999) year += 1900;
				month = m_file->getI16(2);
				day   = m_file->getI16(4);
				hour  = m_file->getI16(6);
				min   = m_file->getI16(8);
				sec   = m_file->getI16(10);
			printf("cell{c=%d-%02d-%02d %02d:%02d:%02d",year,month,day,hour,min,sec);
				year  = m_file->getI16(12);
				if (year < 999) year += 1900;
				month = m_file->getI16(14);
				day   = m_file->getI16(16);
				hour  = m_file->getI16(18);
				min   = m_file->getI16(20);
				sec   = m_file->getI16(22);
			printf(" m=%d-%02d-%02d %02d:%02d:%02d",year,month,day,hour,min,sec);
		}
		else if (rectyp == UNITS)
		{
			userUnits     = m_file->getDbl();   // Calma default is 1.0e-3
			epsilon = (userUnits/2000.0);  //reset
			dataBaseUnits = m_file->getDbl(8);  // Calma default is 1.0e-9
			/*if (precision <= 0) // then not set to positive integer on command line
			{
				sprintf(tmpString1,"%0.8f",userUnits);
				//sRemoveTrailingZeros(tmpString1,precisionString);
				precision = strlen(precisionString) - 2; // "0.001" -> 3, "0.05" -> 2
				if (debug) cerr << "DEBUG line:" << __LINE__ << " precisionString=" << precisionString << " precision=" << precision << endl;
			}*/
			if (debug) cerr << "DEBUG line:" << __LINE__ << " userUnits=" << userUnits << " dataBaseUnits=" << dataBaseUnits << endl;
			printf(" %g %g\n",userUnits,dataBaseUnits);
			printf("# lines above need to stay as is (may be read by other tools)\n");
			//printf("# File created by gds2gdt  http://sourceforge.net/projects/gds2/  version:%s \n", sourceForgeVersion);
			printf("# Key: <required> [optional]\n");
			printf("# File format:\n");
			printf("# gds2{<ver>\n");
			printf("# m=<modificationTimeStamp> a=<accessTimeStamp>\n");
			printf("# lib '<libName>' <userUnits> <dataUnits>\n");
			printf("# <cellDefine>\n");
			printf("# }\n");
			printf("# - - - - -\n");
			printf("# cellDefine is one of more of:\n");
			printf("# cell {c=<creationTimeStamp> m=<modificationTimeStamp> '<cellName>'\n");
			printf("# <cellStuff>*\n");
			printf("# }\n");
			printf("# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -\n");
			printf("## <cellStuff>\n");
			printf("# cellStuff is one or more of:\n");
			printf("# boundary:\n");
			printf("# b{<layer> [dt<dataType>] xy(<xyList>) [property]*}\n");
			printf("#\n");
			printf("# path:\n");
			printf("# p{<layer> [dt<dataType>] [pt<pathType>] [w<real>] [bx<real>] [ex<real>] xy(<xyList>) [property]*}\n");
			printf("#\n");
			printf("# text:\n");
			printf("# t{<layer> [tt<textType>] [f<fontType>] [<textJust>] [pt<pathType>] [fx] [w<real>] [m<magification>] [a<angle>] xy(<xyList>) <'text'> [property]*}\n");
			printf("#\n");
			printf("# sref:\n");
			printf("# s{<'cellName'> [fx] [a<angle>] xy(<xyList>) [property]*}\n");
			printf("#\n");
			printf("# aref:\n");
			printf("# a{<'cellName'> [fx] [a<angle>] cr(<columns> <rows>) xy(<xyList>) [property]*}\n");
			printf("#   aref xyList: 1st coord: origin, 2nd coord: X of col * xSpacing + origin, 3rd coord: Y of row * ySpacing + origin\n");
			printf("#\n");
			printf("# box:\n");
			printf("# x{<layer> [xt<boxType>] xy(<xyList>) [property]*}\n");
			printf("#\n");
			printf("# node:\n");
			printf("# n{<layer> [nt<boxType>] xy(<xyList>) [property]*}\n");
			printf("#\n");
			printf("# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -\n");
			printf("# property : pr{<propAttr> <'propValue'>}\n");
			printf("# <text> : ASCII String\n");
			printf("# <textJust> : two letter combination of bmt (bottom,middle,top) and rcl (right,center,left) e.g. bl (default is tl)\n");
			printf("# <propAttr> : a 2 byte (small) integer\n");
			printf("# <propValue> : ASCII String\n");
			printf("#\n");
			printf("# x==origin\n");
			printf("#     _____\n");
			printf("#    |    a0      x_________        a180    | x             a270\n");
			printf("#    |--                |   |               |        |\n");
			printf("#    |                      |             --|        |___|______\n");
			printf("#  x |                  a90            _____|                   x\n");
			printf("#\n");
			printf("#                   fx a90             _____          __________x\n");
			printf("#  x |    fx                |               |        |   |\n");
			printf("#    |            ______|___|             --|        |\n");
			printf("#    |--          x              fx a180    |            fx a270\n");
			printf("#    |_____                                 | x\n");
			printf("#\n");
			printf("# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -\n");
			printf("# # as first character on a line is a comment\n");
		}
		else if (rectyp == STRNAME)
		{
			strcpy(strname, m_file->record());
			printf(" '%s'\n",strname);
			//cell{c=1998-08-17 14:31:10 m=1998-08-17 14:33:47 'CC1804_R1'  //...}
		}
		else if (rectyp == BOUNDARY)
		{
			inBoundary = 1;
			printf("b{");
		}
		else if (rectyp == PATH)
		{
			printf("p{");
		}
		else if ((rectyp == ENDEL) || (rectyp == ENDSTR) || (rectyp == ENDLIB))
		{
			inBoundary = 0;
			printf("}\n");
		}
		else if (rectyp == COLROW)
		{
			columns = m_file->getI16();
			rows = m_file->getI16(2);
			printf(" cr(%d %d)",columns,rows);
		}
		else if (rectyp == PATHTYPE)
		{
			Ref = m_file->getI16();
			if (Ref != 0)
			{
				printf(" pt%d",Ref);
			}
		}
		else if (rectyp == STRANS)
		{
			Ref = m_file->getI16();
			if (Ref & 0x8000)
			{
				printf(" fx");
			}
		}
		else if (rectyp == PRESENTATION)
		{
			Ref = m_file->getI16();
			//font number
			if ((Ref & 0x30) == 0x30)
			{
				printf(" f3");
			}
			else if ((Ref & 0x20) == 0x20)
			{
				printf(" f2");
			}
			else if ((Ref & 0x10) == 0x10)
			{
				printf(" f1");
			}
			//else sprintf(tmpString1," f0");

			tmpString1[0] = '\0';
			// bottom middle top
			if ((Ref & 0x8) == 0x8)
			{
				sprintf(tmpString1,"b");
			}
			else if ((Ref & 0x4) == 0x4)
			{
				sprintf(tmpString1,"m");
			}
			else
			{
				sprintf(tmpString1,"t");
			}
			// right center left
			if ((Ref & 0x2) == 0x2)
			{
				strcat(tmpString1,"r");
			}
			else if ((Ref & 0x1) == 0x1)
			{
				strcat(tmpString1,"c");
			}
			else
			{
				strcat(tmpString1,"l");
			}
			if (strncmp("tl",tmpString1,2)) printf(" %s",tmpString1); //save space - default
		}
		else if (rectyp == TEXT)
		{
			printf("t{");
		}
		else if (rectyp == SREF)
		{
			printf("s{");
		}
		else if (rectyp == AREF)
		{
			printf("a{");
		}
		else if (rectyp == SNAME)
		{
			printf("'%s'", m_file->record());
		}
		else if ((rectyp == STRING) || (rectyp == PROPVALUE))
		{
			printf(" '");
			strcpy(bigString,m_file->record());
			strncpy(string512,bigString,512);
			for (i=0; i<strlen(string512); i++)
			{
				oneChar = string512[i];
				if (i==512)
				{
					string512[512] = '\0';
					//cerr << "WARNING **** STRING may be longer than 512 characters - truncated to" << string512 << argv[i] << endl;
					break;
				}
				if (oneChar == '\r')
				{
					//printf("%c", '\¤'); //do nothing
				}
				else if (oneChar == '\n')
				{
					printf("%c", '\r'); // will print out as ^M char
				}
				else if (oneChar == '\'')
				{
					printf("%c%c", '\\','\''); // escape
				}
				else if (oneChar == '\\')
				{
					printf("%c%c", '\\','\\'); // escape
				}
				else
				{
					printf("%c", oneChar);
				}
			}
			printf("'");
			if (inProperty) printf("}");
			inProperty=0;
		}
		else if (rectyp == XY)
		{
			length = m_file->length();
			if (debug) cerr << "DEBUG line:" << __LINE__ << " XY length=" << length << endl;
			if ((length > 65524) || (length < 0)) // 0xffff - 4 for header / 8 = 8191 full 8 byte pairs -- 0=>overflow
			{
				fprintf(stderr, "ERROR: unsupport XY length (%d - more than 8191 points) found in gds2 file.\n",length);
				exit(1);
			}
			if (inBoundary) length -= 8; //remove closure -- last 2 points
			printf(" xy(");
			int skip=0;
			for(i=0; i<length; i+=8) {
				if (i == MAXPAIRSXY) {
					cerr << "ERROR: element has > " << MAXPAIRSXY << " coordinates. GDT output lines xy() list is truncated." << endl;
					skip=1;
				}

				xcoord = m_file->getI32(i);
				ycoord = m_file->getI32(i+4);

				//if (xcoord < 0.0)
				//{
				sprintf(xstring,"%0.*f", precision, (xcoord * userUnits) - epsilon);
				//}
				//else
				//{
				//	sprintf(xstring,"%0.*f",precision,(xcoord * userUnits) + epsilon);
				//}
				//if (ycoord < 0.0)
				//{
				sprintf(ystring,"%0.*f", precision, (ycoord * userUnits) - epsilon);
				//}
				//else
				//{
				//	sprintf(ystring,"%0.*f",precision,(ycoord * userUnits) + epsilon);
				//}
				//if (! skip) printf("%s%s %s",(i?" ":""),sRemoveTrailingZeros(xstring,tmpString1),sRemoveTrailingZeros(ystring,tmpString2));
			}
			//qDebug() << "(" << xstring << "," << ystring << ")";

			//if (debug) cerr << "DEBUG line:" << __LINE__ << " LAST XY i=" << i << " xstring=" << xstring << " ystring=" << ystring << endl;
			inBoundary = 0; //done
			printf(")");
		}
		else if (rectyp == LAYER)
		{
			layer = m_file->getI16();
			printf("%d",layer);
		}
		else if (rectyp == WIDTH)
		{
			width = m_file->getI32();
			if (width != 0)
			{
				sprintf(wstring,"%0.5f",((double)width * userUnits) + epsilon);
				//printf(" w%s",sRemoveTrailingZeros(wstring,tmpString1));
			}
		}
		else if (rectyp == DATATYPE)
		{
			dataType = m_file->getI16();
			if (dataType != 0) printf(" dt%d",dataType);
		}
		else if (rectyp == TEXTTYPE)
		{
			textType = m_file->getI16();
			if (textType != 0) printf(" tt%d",textType);
		}
		else if (rectyp == ANGLE)
		{
			Ang = m_file->getDbl();
			if (Ang != 0.0)
			{
				sprintf(xstring,"%0.5f",Ang);
				//printf(" a%s",sRemoveTrailingZeros(xstring,tmpString1));
			}
		} // end ANGLE
		else if (rectyp == MAG)
		{
			Mag = m_file->getDbl();
			if (Mag != 1.0)
			{
				sprintf(xstring,"%0.5f",Mag);
				//printf(" m%s",sRemoveTrailingZeros(xstring,tmpString1));
			}
		} // end MAG
		else if (rectyp == BGNEXTN)
		{
			extn = m_file->getI32();
			sprintf(extnstring,"%0.5f",((double)extn * userUnits) + epsilon);
			//printf(" bx%s",sRemoveTrailingZeros(extnstring,tmpString1));
		}
		else if (rectyp == ENDEXTN)
		{
			extn = m_file->getI32();
			sprintf(extnstring,"%0.5f",((double)extn * userUnits) + epsilon);
			//printf(" ex%s",sRemoveTrailingZeros(extnstring,tmpString1));
		}
		else if (rectyp == PROPATTR)
		{
			propAttr = m_file->getI16();
			printf(" pr{%d",propAttr);
			inProperty=1;
		}
		else if (rectyp == NODE)
		{
			printf("n{");
		}
		else if (rectyp == NODETYPE)
		{
			Ref = m_file->getI16();
			if (Ref != 0)
			{
				printf(" nt%d",Ref);
			}
		}
		else if (rectyp == BOX)
		{
			printf("n{");
		}
		else if (rectyp == BOXTYPE)
		{
			Ref = m_file->getI16();
			if (Ref != 0)
			{
				printf(" xt%d",Ref);
			}
		}
	}
}
