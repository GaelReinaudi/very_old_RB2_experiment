/*------------------------------------------------------------------------------*
 * File Name:				 													*
 * Creation: 																	*
 * Purpose: OriginC Source C file												*
 * Copyright (c) ABCD Corp.	2003, 2004, 2005, 2006, 2007, 2008, 2009, 2010		*
 * All Rights Reserved															*
 * 																				*
 * Modification Log:															*
 *------------------------------------------------------------------------------*/
 
////////////////////////////////////////////////////////////////////////////////////
// Including the system header file Origin.h should be sufficient for most Origin
// applications and is recommended. Origin.h includes many of the most common system
// header files and is automatically pre-compiled when Origin runs the first time.
// Programs including Origin.h subsequently compile much more quickly as long as
// the size and number of other included header files is minimized. All NAG header
// files are now included in Origin.h and no longer need be separately included.
//
// Right-click on the line below and select 'Open "Origin.h"' to open the Origin.h
// system header file.
#include <Origin.h>
////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////
// Include your own header files here.


////////////////////////////////////////////////////////////////////////////////////
// Start your functions here.
ImportFilePlot(string filename, int wichToPlot1 = -1, int wichToPlot2 = -1, int wichToPlot3 = -1, int wichToPlot4 = -1, int wichToPlot5 = -1, int wichToPlot6 = -1, int wichToPlot7 = -1, int wichToPlot8 = -1, int wichToPlot9 = -1, int wichToPlot10 = -1)
{
	StringArray sa;
	int iCount = 1;
	if(filename == "")
		iCount = GetMultiOpenBox(sa, FDLOG_ASCII, NULL, NULL, NULL, false);
	else
	{
		sa.SetSize(1);
		sa[0] = filename;
	}

	if( !iCount ) return;

//	GraphPage gpAll;
//	gpAll.Create("Origin.otp");

	for(int i=0; i < iCount; i++)
	{
		Worksheet wks;
		// create wks from template
		wks.Create("Origin.otw");

		ASCIMP ascimp; // ASCII options (see OC_types.h)
		wks.GetASCIMP(ascimp);
//		ascimp.iHeaderLines = 3;
//		ascimp.iSubHeaderLines = 0;
//		ascimp.iAutoSubHeaderLines = 0;
//		ascimp.iAutoColTypes = 0;
//		ascimp.iRenameWks = 0;
//		ascimp.iRenameCols = 0;
//		ascimp.iMode = 0;
//		wks.SetASCIMP(ascimp);

		wks.ImportASCII(sa[i], ascimp);
		wks.GetPage().Label = sa[i];
		wks.LT_execute("page.title=3");

//		Worksheet wks_interessant;
//		wks_interessant.CreateCopy(wks, CREATE_VISIBLE);
//		wks_interessant.GetPage().Label = sa[i];
//		wks_interessant.LT_execute("page.title=3");
//		string newname = "_" + wks.GetPage().GetName();
//		wks_interessant.GetPage().Rename(wks.GetPage().GetName());
//		wks.GetPage().Rename(newname);

		// pour chaque parametre numérique, on check qu'il existe bien (!= -1)
		// puis on plot la colonne correspondante
		for(int j = 10; j >= 1; j--)
		{
			int wichToPlot = -1;
			switch(j)
			{
				case 1 : wichToPlot = wichToPlot1 ; break;
				case 2 : wichToPlot = wichToPlot2 ; break;
				case 3 : wichToPlot = wichToPlot3 ; break;
				case 4 : wichToPlot = wichToPlot4 ; break;
				case 5 : wichToPlot = wichToPlot5 ; break;
				case 6 : wichToPlot = wichToPlot6 ; break;
				case 7 : wichToPlot = wichToPlot7 ; break;
				case 8 : wichToPlot = wichToPlot8 ; break;
				case 9 : wichToPlot = wichToPlot9 ; break;
				case 10: wichToPlot = wichToPlot10; break;
			}
			if(wichToPlot == -1)
				continue;
			
			Curve crv(wks, 0, wichToPlot);
			//		gpAll.Layers(0).AddPlot(crv, IDM_PLOT_LINE);
			GraphPage gp;
			gp.Create("Origin.otw");
			gp.Label = sa[i];
			gp.LT_execute("page.title=3");
			gp.Layers(0).AddPlot(crv, IDM_PLOT_LINE); // see PlotIDs.h for all PlotIDs
			gp.Layers(0).LT_execute("legend");
			gp.Layers(0).Rescale();
			
			// nom des axes
			LT_set_str("%P", "index");
			LT_execute("lab -s -xb %P;");
			LT_set_str("%P", wks.Columns(wichToPlot).GetLabel());
			LT_execute("lab -s -yl %P");
		}
	}

//	gpAll.Layers(0).LT_execute("legend");
//	gpAll.Layers(0).Rescale();
	
//	MessageBox( GetWindow(), filename);
}

SaveProject(string Path = "")
{
	string strPath = Path;
	if(strPath == "")
		strPath = GetSaveAsBox();
	if( strPath.IsEmpty() )
		return;
	// Create a Project object
	Project		proj;
	proj.Save(strPath);

}