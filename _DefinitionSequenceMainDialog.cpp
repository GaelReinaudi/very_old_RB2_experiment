# include "stdafx.h"
#include "MainDialog.h"
#include "MainFrm.h"

double uneFonc(double x){return x*x;}

unsigned int CMainDialog::DefinitionSequenceMainDialog(Sequence& LaSeq)
{
	int dureeAdvanceImagingShuter = MILLI_SECONDE_INT(m_ParamReal[49]);
	// Loading
	if(m_ParamBool[1])
	{
		if(m_ParamReal[2])
		{
			// on ajoute une Colonne à la fin de la séquence (vide ici)
			LaSeq.InsertNewColonne( AfterLastColonne	,	"Load Zeeman On"
										,	MILLI_SECONDE_INT(m_ParamReal[2]) );
			// on assigne des sorties à la dernière colonne (qui est aussi la premiere puissqu'elle est seule !!!)
			LaSeq.SetCol_32Digitals( LastColonne, 
				/*	_ZEEMAN			,	 0 */	1	,//Sr atomic beam shutter.
				/*	_MOT_AOM		,	 1 */	1	,//Blue MOT AOM (shared by all 6 beams). 
				/*	_MOT_CLIC		,	 2 */	1	,//Blue MOT shutter (shared by all 6 beams) and ZS shutter. Used to be 1. 
				/*	_MAIN_IGBT		,	 3 */	1	,//Not currently used, but 2013-12-2 repurposed for exploratory use as dedicated control of ZS AOM and shutter. Doesn't do anything right now.  
				/*	_REPUMPS		,	 4 */	m_ParamBool[32]	,//Zeeman slower and blue beam AOM and Repumps AOMs. 
				/*	_IMAG_AOM		,	 5 */	1	,//Blue imaging beam dedicated AOM. 2013-12-04 Set to be on by default, to keep imaging AOM warm. 
				/*	_IMAG_CLIC		,	 6 */	0	,//Blue imaging beam dedicated shutter.
				/*	_LATT_SHUT		,	 7 */	DEFAULT_VALUE	,
				/*	_LAT_BACK		,	 8 */	DEFAULT_VALUE	,
				/*	_NUSD_09		,	 9 */	DEFAULT_VALUE	,//Lattice 3Dness, before 2013-12-04, used to turn 3D lattice beam AOM on/off.  (For a while, A-2 was used for that so it could be ramped.) 
				/*	_NUSD_10		,	10 */	0	,//2016-02-01 used to attenuate 689B for Raman pulses, set to 0 here to keep 689B AOM hot (Bart and Chih-Hsi); 2013-08-14 temporarily used to turn on/off 689B during Raman pulses
				/*	_2D_MOT			,	11 */	DEFAULT_VALUE	,//2013-08-14 temporarily used to turn on/off 689D during Raman pulses
				/*	_CAM_TRIGGER	,	12 */	0	,
				/*	_RepumpShutter	,	13 */	0	,//2013-12-2 added. Default of 0 is open. 
				/*	_RedMOTShutter	,	14 */	0	,//2013-12-2 added. Default of 0 is open. (previously, on 2013-08-14 temporarily used to turn on/off 650E during Raman pulses.)
				/*	_IGBT			,	15 */	1	,//2013-12-18 resetup to turn off MOT IGBT after coil current turned off.  Renamed to _IGBT from _TRANS_COOL.  
				/*	_NUSD_16		,	16 */	0	,
				/*	_RBROAD			,	17 */	DEFAULT_VALUE	,//Red MOT AOM and FM broadening (default). 
				/*	_RSINGLE		,	18 */	DEFAULT_VALUE	,//Affects Red MOT AOM and FM broadening... (not normally used!) 
				/*	_NUSD_19		,	19 */	0	,
				/*	_NUSD_20		,	20 */	DEFAULT_VALUE	,
				/*	_NUSD_21		,	21 */	0	,
				/*	_NUSD_22		,	22 */	0	,
				/*	_B_SHUT			,	23 */	1	,//2013-11-21: 689B shutter before fiber. Starts ON (so no advanced opening necessary before PA). 
				/*	_C_SHUT			,	24 */	!m_ParamBool[44]	,//2014-05-05 switched to Bool 44 from bool 30, to allow user to enable/disable shutter. 2013-11-21: 689C shutter before fiber. Starts 0 = OFF (so no leakage light before recovery pulse). 2014-04-30 set to m_ParamBool[30] to allow user to disable this shutter.
				/*	_NUSD_25		,	25 */	0	,
				/*	_NUSD_26		,	26 */	0	,
				/*	_LASER_E_27		,	27 */	1	,//2013-11-21 set to 1 to keep AOM's warm! 
				/*	_LASER_D_28		,	28 */	1	,//2013-11-21 set to 1 to keep AOM's warm! 
				/*	_LASER_C_29		,	29 */	1	,//2013-11-21 set to 1 to keep AOM's warm! 
				/*	_LASER_B_30		,	30 */	1	,//2013-11-21 set to 1 to keep AOM's warm! 
				/*	_PROB_SHUT		,	31 */	0
													);	
			//LaSeq.SetCol_1Digital(-1,   	_LASER_B_30	, DEFAULT_VALUE);//Commented out 2013-11-21. 
			LaSeq.SetCol_1Digital(-1,   	_PROB_SHUT	, 0);
			// on assigne des sorties analogiques à la dernière colonne en donnant "Quelle_Analogique", "Debut" et "Fin"
			// ou bien DEFAULT_VALUE pour prendre les valeurs du port
			// les voie non-spécifiées continue la valeur de la colonne précédente
			// pour la première colonne, elles prennent DEFAULT_VALUE !!!
			// analog outputs are assigned to the last column giving "Which", "Start" 
			// and "End" or DEFAULT_VALUE to take the values ​​of the PORT window
			// unspecified channels continues value from the previous column.
			// For the first column, they take DEFAULT_VALUE.
			LaSeq.SetCol_Analogs_Which_Debut_Fin( LastColonne 	
					//, _PROBE_B_ATTEN,	m_ParamReal[29]	,	m_ParamReal[29]	
				);
		}
	}
// 	// Magnetic trap
// 	if(0)
// 	{
// 		// col 1
// 		double BStartValue = LaSeq.GetColonne( LastColonne)->GetLastPointWf(_PROBE_B_ATTEN).y;
// 		double BToValue = m_ParamReal[5];
// 		if(m_ParamReal[4]) {
// 			LaSeq.InsertNewColonne( AfterLastColonne
// 				,	"Load No Rep"
// 				,	MILLI_SECONDE_INT(m_ParamReal[4]) );
// 			LaSeq.SetCol_Digitals_DefaultPrecedentColonne( LastColonne,   
// 				_REPUMPS		,	0	
// 				);	
// 			LaSeq.SetCol_Analogs_Which_Debut_Fin( LastColonne 
// 			, _PROBE_B_ATTEN,	BStartValue,	BToValue
// 			);
// 		}
// 		LaSeq.SetCol_1Digital(-1,   	_LASER_B_30	, DEFAULT_VALUE);
// 		LaSeq.SetCol_1Digital(-1,   	_LASER_C_29	, DEFAULT_VALUE);
// 		LaSeq.SetCol_1Digital(-1,   	_PROB_SHUT	, 0);
// 
// 		// col 2
// 		BStartValue = m_ParamReal[5];
// 		BToValue = m_ParamReal[6];
// 		if(m_ParamReal[26]) {
// 			LaSeq.InsertNewColonne( AfterLastColonne
// 				,	"Load No Rep"
// 				,	MILLI_SECONDE_INT(m_ParamReal[26]) );
// 			LaSeq.SetCol_Digitals_DefaultPrecedentColonne( LastColonne,   
// 				_REPUMPS		,	0	
// 				);	
// 			LaSeq.SetCol_Analogs_Which_Debut_Fin( LastColonne 
// 				, _PROBE_B_ATTEN,	BStartValue,	BToValue
// 				);
// 		}
// 		// col 3
// 		BStartValue = m_ParamReal[6];
// 		BToValue = m_ParamReal[7];
// 		if(m_ParamReal[27]) {
// 			LaSeq.InsertNewColonne( AfterLastColonne
// 				,	"Load No Rep"
// 				,	MILLI_SECONDE_INT(m_ParamReal[27]) );
// 			LaSeq.SetCol_Digitals_DefaultPrecedentColonne( LastColonne,   
// 				_REPUMPS		,	0	
// 				);	
// 			LaSeq.SetCol_Analogs_Which_Debut_Fin( LastColonne 
// 				, _PROBE_B_ATTEN,	BStartValue,	BToValue
// 				);
// 		}
// 	}
	
	// shut off ZS
	if(m_ParamReal[3])
	{
// 		// On va remonter à la colonne qu'il faut splitter en deux pour ajouter le ClicClac
// 		int dureeAdvance = MILLI_SECONDE_INT(m_ParamReal[3]);
// 		LaSeq.SplitSequence_FromEndOfCol_NbrPeriodeBeforeEndCol_SetDigitals( LastColonne, dureeAdvance, _ZEEMAN, 0);

		LaSeq.InsertNewColonne( AfterLastColonne	,	"Load Zeeman Off"
			,	MILLI_SECONDE_INT(m_ParamReal[3]) );
		// on assigne des sorties à la dernière colonne
		LaSeq.SetCol_Digitals_DefaultPrecedentColonne(	LastColonne
			,   _ZEEMAN		,	0	
			,   _MAIN_IGBT	,	0		//2013-12-2. Turn off ZS AOM and shutter (experimental!). 
			);	
		LaSeq.SetCol_Analogs_Which_Debut_Fin( LastColonne
			);
	}
	// turn Rep back on
	if(0)//m_ParamReal[21])//DISABLED THIS if statement 2014-04-30.  We haven't used it in two years! 
	{
		// On va remonter à la colonne qu'il faut splitter en deux pour ajouter le ClicClac
		int dureeAdvance = MILLI_SECONDE_INT(m_ParamReal[21]);
		LaSeq.SplitSequence_FromEndOfCol_NbrPeriodeBeforeEndCol_SetDigitals( LastColonne, dureeAdvance, _REPUMPS, 1);
	}

	// 2012-08-22
	// On va remonter à la colonne qu'il faut splitter en deux pour ajouter le ClicClac
	//int dureeAdvanceProbeShutter = MILLI_SECONDE_INT(20);//Default 20 ms before 2014-04-30. 
	int dureeAdvanceProbeShutter = MICRO_SECONDE_INT(m_ParamReal[66]);//2014-04-30 set to value in sequencer front panel, so user can adjust (defauly 20000 us). 

	// 8 colonnes
	int LastValue_B = LaSeq.GetColonne( LastColonne)->GetLastPointWf(_PROBE_B_ATTEN).y;
	int LastValue_FM = LaSeq.GetColonne( LastColonne)->GetLastPointWf(_ANA_04).y;
	int LastValue_Det = LaSeq.GetColonne( LastColonne)->GetLastPointWf(_ANA_05).y;
	int LastValue_Amp = LaSeq.GetColonne( LastColonne)->GetLastPointWf(_ANA_06).y;

		//Broad Red MOT FM and Lattice trap. 
		for(int iemeCol = 1; iemeCol <= 8; iemeCol++)
		{
			if(m_ParamReal[40 + iemeCol])//loops over column widths given by param's 41 to 48: iemeCol = 1 to 4 is broad red mot; iemeCol = 7 to 8 is lattice trap; iemeCol = 5 to 6 UNUSED! 
			{
				// pour la 1ere colonne, on prend la valeur de la colonne précédente, sinon
				double B_From	= (iemeCol == 1) ? LastValue_B : m_NiEdit[50 + iemeCol - 1].GetValue();
				double B_To		= m_NiEdit[50 + iemeCol].GetValue();
				double FM_From	= (iemeCol == 1) ? LastValue_FM : m_NiEdit[60 + iemeCol - 1].GetValue();
				double FM_To	= m_NiEdit[60 + iemeCol].GetValue();
				double Det_From = (iemeCol == 1) ? LastValue_Det : m_NiEdit[70 + iemeCol - 1].GetValue();
				double Det_To	= m_NiEdit[70 + iemeCol].GetValue();
				double Amp_From = (iemeCol == 1) ? LastValue_Amp : m_NiEdit[80 + iemeCol - 1].GetValue();
				double Amp_To	= m_NiEdit[80 + iemeCol].GetValue();

				int Duree_Col1_Int = int(MILLI_SECONDE_INT(m_ParamReal[40 + iemeCol]));

				CString strName; 
				strName.Format("Monster%d", iemeCol);
				if(Duree_Col1_Int)
				{
					if(m_ParamBool[5] && iemeCol < 5) {//Broad Red MOT for iemeCol = 1 to 4.
						LaSeq.InsertNewColonne_CopiedFromCol( AfterLastColonne	,	LaSeq.GetColonne(LastColonne)
							,	strName
							,	Duree_Col1_Int );
						LaSeq.SetCol_Digitals_DefaultPrecedentColonne( LastColonne,   
							_REPUMPS			,	0	,
							_RepumpShutter		,	1	, //2013-12-2 added, turns off repumps shutter for the first time. 
							_RedMOTShutter		,	0	, //2013-12-2 added, red MOT shutter still open. 
							_MOT_AOM			,	iemeCol !=1	,
							_MOT_CLIC			,	0	,
							_MAIN_IGBT			,	0	,	//2013-12-2. Turn off ZS AOM and shutter (experimental!). 
							_RBROAD				,	1	,
							_RSINGLE			,	0
							);	
						/////
						//2013-08-14 Bart/Geoff commented out the following two lines, 
						//whose purpose seems to have been to just help debug the Red MOT FM 
						//steps by coloring some extra DigOut channels (24-27) in the graph of the sequence. 
						//LaSeq.SetCol_1Digital(-1,   23 + iemeCol - 1, false);
						//LaSeq.SetCol_1Digital(-1,   23 + iemeCol, true);
						/////
						LaSeq.SetCol_Analogs_Which_Debut_Fin( LastColonne, 
							_ANA_04,	FM_From,	FM_To ,
							_ANA_05,	Det_From,	Det_To ,
							_ANA_06,	Amp_From,	Amp_To 
							);
					}
					if(m_ParamBool[3] && iemeCol >= 7) {//Lattice trap for iemeCol = 7 to 8. Note that iemeCol = 8 was created to allow a slow linear ramp on of 3D lattice.  
						LaSeq.InsertNewColonne_CopiedFromCol( AfterLastColonne	,	LaSeq.GetColonne(LastColonne)
							,	strName
							,	Duree_Col1_Int );
						LaSeq.SetCol_Digitals_DefaultPrecedentColonne( LastColonne,   
							_REPUMPS			,	0	,
// 							_MOT_AOM			,	0	,
							_MOT_CLIC			,	0	,
							_MAIN_IGBT			,	0	,	//2013-12-2. Keep ZS AOM and shutter off (experimental!). 
							_RBROAD				,	0	,
							_RSINGLE			,	0	,
							_LATT_SHUT			,	0	,
							_RepumpShutter		,	1	, //2013-12-2 added, turns off repumps shutter, should be off earlier. 
							_RedMOTShutter		,	1	, //2013-12-2 added, turns off red MOT shutter. This shouldbe early enough that it's closed before PA, given a 20 ms lattice trap time. 
							_LAT_BACK			,	iemeCol == 8 
							);	
//						LaSeq.SetCol_1Digital(-1,   23 + iemeCol - 3, false);
//						LaSeq.SetCol_1Digital(-1,   23 + iemeCol - 2, true);
						LaSeq.SetCol_Analogs_Which_Debut_Fin( LastColonne, 
							_I_MOT,	m_ParamBool[2] ? DEFAULT_VALUE : 0,	m_ParamBool[2] ? DEFAULT_VALUE : 0, //Turns off MOT coil current if button selected, by setting analog output voltage to zero.  
							_MOT_INT, m_pMainFrame->m_PortDialog.m_Ana_1_Out[0] + m_ParamReal[4], m_pMainFrame->m_PortDialog.m_Ana_1_Out[0] + m_ParamReal[4],//This applies V-Bz (pulse value adds to DC value).
							_PROBE_B_ATTEN, m_ParamBool[33] ? m_pMainFrame->m_PortDialog.m_Ana_2_Out[4] : DEFAULT_VALUE, m_ParamBool[33] ? m_pMainFrame->m_PortDialog.m_Ana_2_Out[4] : DEFAULT_VALUE,//Added 2013-12-05 to allow V-Bx pulsing (pulse value does not add to DC value). 
							_REP_INT, m_ParamBool[33] ? m_pMainFrame->m_PortDialog.m_Ana_2_Out[6] : DEFAULT_VALUE, m_ParamBool[33] ? m_pMainFrame->m_PortDialog.m_Ana_2_Out[6] : DEFAULT_VALUE//Added 2013-12-05 to allow V-By pulsing (pulse value does not add to DC value). 
							//_REP_INT, DEFAULT_VALUE, iemeCol == 8 ? m_ParamReal[30] : DEFAULT_VALUE//2013-12-05 Disabled slow 2D lattice ramp-on ability. To reimplement, uncomment this line.  
							);
						//NOTE:  _Rep_INT is analog output channel 2. 
					}
				}
			}
		}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////// Probe panel (PA, Raman, Recovery pulses) ////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//
		//
		if(m_ParamBool[4])//Probe panel's on/off button.  
		{	//=====================================
			//			"aom B [us]"
			//	First PA pulse with 689B laser. 
			//	ASSUMES that the probe shutter is open (lattice trap stage lasts >= 20ms). 
			//=====================================
			if(1)//m_ParamReal[85])
			{	//Start to open probe shutter in advance, and force all probe lasers OFF! (This is the first time this shutter opens.) 
				LaSeq.SplitSequence_FromEndOfCol_NbrPeriodeBeforeEndCol_SetDigitals( LastColonne, dureeAdvanceProbeShutter, _PROB_SHUT, 1);
				LaSeq.SplitSequence_FromEndOfCol_NbrPeriodeBeforeEndCol_SetDigitals( LastColonne, dureeAdvanceProbeShutter, _B_SHUT, 1);	//Added 2013-11-21 to set to 1.
				LaSeq.SplitSequence_FromEndOfCol_NbrPeriodeBeforeEndCol_SetDigitals( LastColonne, dureeAdvanceProbeShutter, _C_SHUT, !m_ParamBool[44]);	//Added 2013-11-21 to set to 0. 2014-04-30 set to m_ParamBool[30] so used can disable this shutter. 2014-05-05 switched to NOT Bool 44 from 30. 
				LaSeq.SplitSequence_FromEndOfCol_NbrPeriodeBeforeEndCol_SetDigitals( LastColonne, dureeAdvanceProbeShutter, _LASER_C_29, 0);
				LaSeq.SplitSequence_FromEndOfCol_NbrPeriodeBeforeEndCol_SetDigitals( LastColonne, dureeAdvanceProbeShutter, _LASER_B_30, 0);
				LaSeq.SplitSequence_FromEndOfCol_NbrPeriodeBeforeEndCol_SetDigitals( LastColonne, dureeAdvanceProbeShutter, _LASER_D_28, 0);
				LaSeq.SplitSequence_FromEndOfCol_NbrPeriodeBeforeEndCol_SetDigitals( LastColonne, dureeAdvanceProbeShutter, _LASER_E_27, 0);
		
				int DureProbe = int(MICRO_SECONDE_INT(m_ParamReal[85]));
				LaSeq.InsertNewColonne( AfterLastColonne	,	"probe", 	DureProbe );
				LaSeq.SetCol_Digitals_DefaultPrecedentColonne( LastColonne,   
					_LASER_B_30			,	1	,
					_MAIN_IGBT			,	0	,//Not actually IGBT!  Experimental... 
					_IGBT				,	m_ParamBool[2] ? 1 : 0	,//2013-12-18 Turn off IGBT if MOT coil current was set to zero during lattice trap.  
					_RBROAD				,	0	,
					_PROB_SHUT			,	1	,
					_B_SHUT				,	!m_ParamBool[43]		//2014-05-05 changed this from m_ParamBool[40] to !m_ParamBool[43], so can separate Raman pulse and shutter controls.  Added 2013-11-21. This turns the shutter OFF early (at start of PA pulse), so it actually closes during Wipe1!  (Shutter starts to block light after 1.5 ms, totally blocked 2 ms +.) Note: useBDuringRaman1 = m_ParamBool[40].
					);	
				LaSeq.SetCol_Analogs_Which_Debut_Fin( LastColonne
					);

			} 
			
			//########################
			//	Probe Shutter planning (recurs periodically below) 
			//########################
			//Decide whether to turn off the probe shutter before Raman1: (added 2013-03-14)  
			bool ProbeShutterStillOn = true;
			//-- Assemble durations of next steps that don't use the probe lasers: 
			int DureDelay = int(MICRO_SECONDE_INT(m_ParamReal[65]));
			int DureWipeWait = int(MICRO_SECONDE_INT(m_ParamReal[88]));
			int DureWipe = int(MICRO_SECONDE_INT(m_ParamReal[86]));
			//-- See if the total time is long enough to justify turning off the probe shutter: 
			if(DureDelay+DureWipeWait+DureWipe>dureeAdvanceProbeShutter)
			{	ProbeShutterStillOn = false;//Turn it off!
			}
			else
			{	ProbeShutterStillOn = true;//Leave it on! 
			}
			//To avoid probe AOM's cooling, plan to turn probe AOM's back on if the shutter is closed.
			//However, for simplicity, will only do this after 20 ms (dureeAdvanceProbeShutter) 
			//has elapsed within "Wait [us]" following "Wipe1". 
			//Note that "Wait [us]" has to be longer than 40 us for this to occur in practice. 
			
			//=====================================
			//			"Delay [us]"
			//	Optional delay after PA but before Wipe1. 
			//=====================================
			//
			if(1)//m_ParamReal[65])//Delay [us]
			{	LaSeq.InsertNewColonne( AfterLastColonne	,	"B delay", 	int(MICRO_SECONDE_INT(m_ParamReal[65])) );
				LaSeq.SetCol_Digitals_DefaultPrecedentColonne( LastColonne,   
					_LASER_B_30			,	0	,
					_LASER_C_29			,	0	,
					_LASER_D_28			,	0	,
					_LASER_E_27			,	0	,
					_PROB_SHUT			,	ProbeShutterStillOn	,
					_B_SHUT				,	!m_ParamBool[43]	//2014-05-05 changed to Bool 43 from 40. Added 2013-11-21; only turns off this shutter if useBDuringRaman1=m_ParamBool[40]=0. 
					);	
				LaSeq.SetCol_Analogs_Which_Debut_Fin( LastColonne
					);
			}
			
			//=====================================
			//			"Wipe1" and "Wipe [us]"
			//	First wipe pulse with blue laser; also sets time for Wipe2 below. 
			//	2013-02-18: Attenuates 689B laser in advance before Raman pulses. 
			//	2013-03-14: Revised. Conditions to turn off imaging shutter changed.  
			//=====================================
			//

			//Get user value 0--2000 [mV] for attenuation in Port panel: 
			//int mVatten = m_pMainFrame->m_PortDialog.m_Ana_2_Out[4];
			////Clamp in acceptable range. 
			//if(mVatten<0)
			//{	mVatten = 0;
			//}
			//if(mVatten>2000)
			//{	mVatten = 2000;
			//}

			if(!m_ParamBool[18]) 
			{	DureWipe = 0;//If Wipe1 button is off, use zero duration. 
			}
 			if(1)//m_ParamBool[18])//Wipe1 button
			{	if(DureWipe)//Wipe [us]
				{	// On va remonter à la colonne qu'il faut splitter en deux pour ajouter le ClicClac
					LaSeq.SplitSequence_FromEndOfCol_NbrPeriodeBeforeEndCol_SetDigitals( LastColonne, dureeAdvanceImagingShuter, _IMAG_CLIC, 1);
					LaSeq.SplitSequence_FromEndOfCol_NbrPeriodeBeforeEndCol_SetDigitals( LastColonne, dureeAdvanceImagingShuter, _IMAG_AOM, 0);//2013-11-22 force AOM off before opening shutter! 
					LaSeq.SetCol_Analogs_Which_Debut_Fin( LastColonne
						);
				}
				if(1)//m_ParamReal[86])//Wipe [us]
				{	LaSeq.InsertNewColonne( AfterLastColonne	,	"wipe", 	DureWipe );
					//Attenuate the 689B laser before any Raman pulses:  
					if((m_ParamReal[26]||m_ParamReal[6])&&!m_ParamBool[33])//2013-12-05 added m_ParamBool[33], to allow user to engage/disengage mVatten (vs V-Bx, V-By control).  
					{	//Get user value 0--2000 [mV] for attenuation in Port panel: 
						int mVatten = m_pMainFrame->m_PortDialog.m_Ana_2_Out[4];
						//Clamp in acceptable range. 
						if(mVatten<0)
						{	mVatten = 0;
						}
						if(mVatten>2000)
						{	mVatten = 2000;
						}
						//Attenuate 689B laser before any Raman pulse: 
						LaSeq.SetCol_Analogs_Which_Debut_Fin( LastColonne 
							, _PROBE_B_ATTEN,	mVatten,	mVatten
							);
					}
					else
					{	//Don't attenuate 689B laser: 
						LaSeq.SetCol_Analogs_Which_Debut_Fin( LastColonne
							);
					}



					//Wipe1
					LaSeq.SetCol_Digitals_DefaultPrecedentColonne( LastColonne,   
						_LASER_B_30			,	0	,
						_LASER_C_29			,	0	,
						_LASER_D_28			,	0	,
						_LASER_E_27			,	0	,
						_PROB_SHUT			,	ProbeShutterStillOn	, 
						_NUSD_09			,	0	,//2013-12-04 set to 0, this is Lattice 3Dness.  No need to force to 1 here! 
						_IMAG_AOM			,	1	,
						_IMAG_CLIC			,	0	,//2013-11-22 start to turn off imaging shutter early, so closes faster! 
						_MAIN_IGBT			,	0	,
						_RBROAD				,	0	,
						_NUSD_10			,	m_ParamBool[49]	//2016-02-01 added this to start attenuation of 689B for remainder of probe pulses (Bart and Chih-Hsi).
						);	
					
					
				}
				if(DureWipeWait)//Wait [us]	 
				{	LaSeq.InsertNewColonne( AfterLastColonne	,	"wipe wait", 	DureWipeWait );
					LaSeq.SetCol_Digitals_DefaultPrecedentColonne( LastColonne,   
						_IMAG_AOM			,	0	,
						_IMAG_CLIC			,	0	,
						_LASER_B_30			,	0	,
						_LASER_C_29			,	0	,
						_LASER_D_28			,	0	,
						_LASER_E_27			,	0	,
						_PROB_SHUT			,	ProbeShutterStillOn	
						);	
					LaSeq.SetCol_Analogs_Which_Debut_Fin( LastColonne
						);

					if(!ProbeShutterStillOn)
					{	// Reopen the 689B/C/D AOM's after the probe shutter has closed, if DureWipeWait is longer than 20ms: 
						// Note that this will only occur if this column is longer than 40 ms, because of how Raman1 reopens the shutter. 
						LaSeq.SplitSequence_FromEndOfCol_NbrPeriodeBeforeEndCol_SetDigitals( LastColonne, DureWipeWait - dureeAdvanceProbeShutter, _LASER_B_30, 1);
						LaSeq.SplitSequence_FromEndOfCol_NbrPeriodeBeforeEndCol_SetDigitals( LastColonne, DureWipeWait - dureeAdvanceProbeShutter, _LASER_C_29, 1);
						LaSeq.SplitSequence_FromEndOfCol_NbrPeriodeBeforeEndCol_SetDigitals( LastColonne, DureWipeWait - dureeAdvanceProbeShutter, _LASER_D_28, 1);
						LaSeq.SplitSequence_FromEndOfCol_NbrPeriodeBeforeEndCol_SetDigitals( LastColonne, DureWipeWait - dureeAdvanceProbeShutter, _LASER_E_27, 1);
					}
				}
			}
			
			//=====================================
			//			"Raman1 [us]"
			//	First Raman pulse with 689D and 689B lasers.
			//	2012-02-18: Created. 
			//  2013-03-14: Revised. Added option to turn off 689B laser in Raman1 and Raman2. 
			//=====================================
			// 
			//###########################################################
			//OPTION: Include the 689B laser in Raman1 and Raman2 pulses? 
			bool useBDuringRaman1 = m_ParamBool[40];
			bool useCDuringRaman1 = m_ParamBool[30];
			bool useDDuringRaman1 = m_ParamBool[41];
			bool useEDuringRaman1 = m_ParamBool[42];
			
			bool useBDuringRaman2 = m_ParamBool[26];//Added 2015-06-24 Bart
			bool useCDuringRaman2 = m_ParamBool[28];
			bool useDDuringRaman2 = m_ParamBool[14];
			bool useEDuringRaman2 = m_ParamBool[17];
			
			//
			int DureRaman1 = int(MICRO_SECONDE_INT(m_ParamReal[26]));
			if(DureRaman1)//Raman1 [us]
			{	if(!ProbeShutterStillOn) 
				{	//Open probe shutter in advance: 
					LaSeq.SplitSequence_FromEndOfCol_NbrPeriodeBeforeEndCol_SetDigitals( LastColonne, dureeAdvanceProbeShutter, _PROB_SHUT, 1);
					ProbeShutterStillOn = true;//The probe shutter is open now! 
					//Warning: Expect probe laser AOM's to be on if Probe Shutter's been closed for a while.
					//In this case, to prevent an accidental probe pulse while probe shutter opens, turn off probe AOM's in advance: 
					LaSeq.SplitSequence_FromEndOfCol_NbrPeriodeBeforeEndCol_SetDigitals( LastColonne, dureeAdvanceProbeShutter, _LASER_B_30, 0);
					LaSeq.SplitSequence_FromEndOfCol_NbrPeriodeBeforeEndCol_SetDigitals( LastColonne, dureeAdvanceProbeShutter, _LASER_C_29, 0);
					LaSeq.SplitSequence_FromEndOfCol_NbrPeriodeBeforeEndCol_SetDigitals( LastColonne, dureeAdvanceProbeShutter, _LASER_D_28, 0);
					LaSeq.SplitSequence_FromEndOfCol_NbrPeriodeBeforeEndCol_SetDigitals( LastColonne, dureeAdvanceProbeShutter, _LASER_E_27, 0);
				}
				
				LaSeq.InsertNewColonne( AfterLastColonne	,	"Raman1", 	DureRaman1 );
				LaSeq.SetCol_Digitals_DefaultPrecedentColonne( LastColonne,   
					_LASER_B_30			,	useBDuringRaman1	,//2015-03-24 normal.  2014-07-29 manually forced 689B only in Raman1. 2014-08-04 manually forced 689E only in Raman1. (changed to 0)
					_LASER_C_29			,	useCDuringRaman1	, //2015-03-24 normal.  2014-07-29 manually forced 689B only in Raman1. 
					_LASER_D_28			,	useDDuringRaman1	,//2015-03-24 normal.  2014-07-29 manually forced 689B only in Raman1. 
					_LASER_E_27			,	useEDuringRaman1 ,//2015-03-24 normal.  2014-07-29 manually forced 689B only in Raman1. 2014-08-04 manually forced 689E only in Raman1. (changed to 1)
					_PROB_SHUT			,	1	,
					_IMAG_AOM			,	0	,
					_IMAG_CLIC			,	0	,
					_MAIN_IGBT			,	0	,//?
					_RBROAD				,	0	//?
					);	
				LaSeq.SetCol_Analogs_Which_Debut_Fin( LastColonne
					);
			}

			//=====================================
			//			"Wait1 [us] / Wipe3 [us]"
			//	Wait after first Raman pulse with 689D and 689B lasers.
			//	2012-02-18: Created. 
			//  2013-03-14: Revised. 
			//  2015-03-24: Revised to include option for substituting a WIPE pulse for the WAIT pulse with the same duration.
			//=====================================
			// 
			int WaitAfterRaman1 = int(MICRO_SECONDE_INT(m_ParamReal[27]));


			if(WaitAfterRaman1)//Wait[us]
			{	
				 //2015-03-24: Created
				
				if(m_ParamBool[8]) //This stuff happened before the original WIPE pulse, so I copy-pasted...
				{	LaSeq.SplitSequence_FromEndOfCol_NbrPeriodeBeforeEndCol_SetDigitals( LastColonne, dureeAdvanceImagingShuter, _IMAG_CLIC, 1);
					//LaSeq.SplitSequence_FromEndOfCol_NbrPeriodeBeforeEndCol_SetDigitals( LastColonne, dureeAdvanceImagingShuter, _IMAG_AOM, 0);//2013-11-22 force AOM off before opening shutter! 
					LaSeq.SetCol_Analogs_Which_Debut_Fin( LastColonne
				);
				}
				//*/
				
				
				LaSeq.InsertNewColonne( AfterLastColonne	,	"Wait after Raman1", 	WaitAfterRaman1 );
				//#########################################
				//Decide what to do with the probe shutter:
				if(!ProbeShutterStillOn)
				{	//Probe shutter is closed, so do nothing.
				}
				else
				{	//Probe shutter is open.  Decide whether to close: 
					if(WaitAfterRaman1 > dureeAdvanceProbeShutter)
					{	ProbeShutterStillOn = false;//close shutter
						//Code to open AOM's is below, if there's enough time. 
					}
				}
				//2015-03-24: Created for WIPE instead of WAIT1
				
				if(m_ParamBool[8])
				{
					LaSeq.SetCol_Digitals_DefaultPrecedentColonne( LastColonne,   
						_LASER_B_30			,	0	,
						_LASER_C_29			,	0	,
						_LASER_D_28			,	0	,
						_LASER_E_27			,	0	,
						_PROB_SHUT			,	ProbeShutterStillOn	, 
						_IMAG_AOM			,	1	,
						_IMAG_CLIC			,	1	//2015-03-24 Leave shutter open so as not to interfere with absorption imaging! 
						);	
					//2015-03-24: Testing whether this will keep the WAIT1=WIPE3 button on
				}
				else
				{
				//*/

				LaSeq.SetCol_Digitals_DefaultPrecedentColonne( LastColonne,   
					_LASER_C_29			,	0	, 
					_LASER_B_30			,	0	,
					_LASER_D_28			,	0	,
					_LASER_E_27			,	0	,
					_PROB_SHUT			,	ProbeShutterStillOn	,
					_IMAG_AOM			,	0	,
					_IMAG_CLIC			,	0	
					);	

				} //2015-03-24: Created for WIPE instead of WAIT1

				LaSeq.SetCol_Analogs_Which_Debut_Fin( LastColonne
					);

				//Schedule AOM's to open if there's enough time (if already open, has no effect): 
				if(!ProbeShutterStillOn)
				{	LaSeq.SplitSequence_FromEndOfCol_NbrPeriodeBeforeEndCol_SetDigitals( LastColonne, WaitAfterRaman1 - dureeAdvanceProbeShutter, _LASER_B_30, 1);
					LaSeq.SplitSequence_FromEndOfCol_NbrPeriodeBeforeEndCol_SetDigitals( LastColonne, WaitAfterRaman1 - dureeAdvanceProbeShutter, _LASER_C_29, 1);
					LaSeq.SplitSequence_FromEndOfCol_NbrPeriodeBeforeEndCol_SetDigitals( LastColonne, WaitAfterRaman1 - dureeAdvanceProbeShutter, _LASER_D_28, 1);
					LaSeq.SplitSequence_FromEndOfCol_NbrPeriodeBeforeEndCol_SetDigitals( LastColonne, WaitAfterRaman1 - dureeAdvanceProbeShutter, _LASER_E_27, 1);
				}
			}

			//=====================================
			//			"Raman2 [us]"
			//	First Raman pulse with 689D and 689B lasers.
			//	2012-02-18: Created. 
			//  2013-03-14: Revised. Added option to turn off 689B laser in Raman1 and Raman2. 
			//=====================================
			// 
			int DureRaman2 = int(MICRO_SECONDE_INT(m_ParamReal[6]));
			if(DureRaman2)//Raman2 [us]
			{	if(!ProbeShutterStillOn) 
				{	//Open probe shutter in advance: 
					LaSeq.SplitSequence_FromEndOfCol_NbrPeriodeBeforeEndCol_SetDigitals( LastColonne, dureeAdvanceProbeShutter, _PROB_SHUT, 1);
					ProbeShutterStillOn = true;//The probe shutter is open now! 
					//Warning: Expect probe laser AOM's to be on if Probe Shutter's been closed for a while.
					//In this case, to prevent an accidental probe pulse while probe shutter opens, turn off probe AOM's in advance: 
					LaSeq.SplitSequence_FromEndOfCol_NbrPeriodeBeforeEndCol_SetDigitals( LastColonne, dureeAdvanceProbeShutter, _LASER_B_30, 0);
					LaSeq.SplitSequence_FromEndOfCol_NbrPeriodeBeforeEndCol_SetDigitals( LastColonne, dureeAdvanceProbeShutter, _LASER_C_29, 0);
					LaSeq.SplitSequence_FromEndOfCol_NbrPeriodeBeforeEndCol_SetDigitals( LastColonne, dureeAdvanceProbeShutter, _LASER_D_28, 0);
					LaSeq.SplitSequence_FromEndOfCol_NbrPeriodeBeforeEndCol_SetDigitals( LastColonne, dureeAdvanceProbeShutter, _LASER_E_27, 0);
				}

				LaSeq.InsertNewColonne( AfterLastColonne	,	"Raman2", 	DureRaman2 );
				LaSeq.SetCol_Digitals_DefaultPrecedentColonne( LastColonne,   
					_LASER_B_30			,	useBDuringRaman2	,
					_LASER_C_29			,	useCDuringRaman2	,
					_LASER_D_28			,	useDDuringRaman2	,
					_LASER_E_27			,	useEDuringRaman2 ,
					_PROB_SHUT			,	1	,
					_MAIN_IGBT			,	0	,//?
					_RBROAD				,	0	,//?
					_IMAG_AOM			,	0	,
					_IMAG_CLIC			,	0	
					);	
				LaSeq.SetCol_Analogs_Which_Debut_Fin( LastColonne
					);
			}

			//=====================================
			//			"Wait2 [us]"
			//	Wait after second Raman pulse with 689D and 689B lasers.
			//	2012-02-18: Created. 
			//  2013-03-14: Revised. 
			//=====================================
			// 
			int WaitAfterRaman2 = int(MICRO_SECONDE_INT(m_ParamReal[7]));
			if(WaitAfterRaman2)//Wait2 [us]
			{	//#########################################
				//Decide what to do with the probe shutter:
				if(!ProbeShutterStillOn)
				{	//Probe shutter is closed, so do nothing.
				}
				else
				{	//Probe shutter is open.  Decide whether to close: 
					if(WaitAfterRaman2 > dureeAdvanceProbeShutter)
					{	ProbeShutterStillOn = false;//close shutter
					//Code to open AOM's is below, if there's enough time. 
					}
				}

				LaSeq.InsertNewColonne( AfterLastColonne	,	"Wait after Raman2", 	WaitAfterRaman2 );
				LaSeq.SetCol_Digitals_DefaultPrecedentColonne( LastColonne,   
					_LASER_B_30			,	0	,
					_LASER_C_29			,	0	,
					_LASER_D_28			,	0	,
					_LASER_E_27			,	0	,
					_PROB_SHUT			,	ProbeShutterStillOn	,
					_IMAG_AOM			,	0	, 
					_IMAG_CLIC			,	0	
					);	
				LaSeq.SetCol_Analogs_Which_Debut_Fin( LastColonne
					//, _PROBE_B_ATTEN,	DEFAULT_VALUE,	DEFAULT_VALUE//Forcibly unattenuate 689B laser. 
					);

				//Schedule AOM's to open if there's enough time (if already open, has no effect): 
				if(!ProbeShutterStillOn)
				{	LaSeq.SplitSequence_FromEndOfCol_NbrPeriodeBeforeEndCol_SetDigitals( LastColonne, WaitAfterRaman2 - dureeAdvanceProbeShutter, _LASER_B_30, 1);
					LaSeq.SplitSequence_FromEndOfCol_NbrPeriodeBeforeEndCol_SetDigitals( LastColonne, WaitAfterRaman2 - dureeAdvanceProbeShutter, _LASER_C_29, 1);
					LaSeq.SplitSequence_FromEndOfCol_NbrPeriodeBeforeEndCol_SetDigitals( LastColonne, WaitAfterRaman2 - dureeAdvanceProbeShutter, _LASER_D_28, 1);
					LaSeq.SplitSequence_FromEndOfCol_NbrPeriodeBeforeEndCol_SetDigitals( LastColonne, WaitAfterRaman2 - dureeAdvanceProbeShutter, _LASER_E_27, 1);
				}
			}

			//=====================================
			//			"aom C [us]"
			//	First recovery pulse with 689C laser. 
			//	If used, forcibly unattenuates 689B laser in advance of "more aom B". 
			//  2013-03-14: Revised. 
			//=====================================
			// 
			int DureMoreProbe = int(MICRO_SECONDE_INT(m_ParamReal[87]));
			if(1)//2016-02-01 made always on, previously: DureMoreProbe)//aom C [us]
			{	if(!ProbeShutterStillOn) 
				{	//Open probe and 689C shutter in advance: 
					LaSeq.SplitSequence_FromEndOfCol_NbrPeriodeBeforeEndCol_SetDigitals( LastColonne, dureeAdvanceProbeShutter, _PROB_SHUT, 1);
					LaSeq.SplitSequence_FromEndOfCol_NbrPeriodeBeforeEndCol_SetDigitals( LastColonne, dureeAdvanceProbeShutter, _B_SHUT, 0);//Added 2013-11-21.
					LaSeq.SplitSequence_FromEndOfCol_NbrPeriodeBeforeEndCol_SetDigitals( LastColonne, dureeAdvanceProbeShutter, _C_SHUT, 1);//Added 2013-11-21.
					ProbeShutterStillOn = true;//The probe shutter is open now! 
					//Warning: Expect probe laser AOM's to be on if Probe Shutter's been closed for a while.
					//In this case, to prevent an accidental probe pulse while probe shutter opens, turn off probe AOM's in advance: 
					LaSeq.SplitSequence_FromEndOfCol_NbrPeriodeBeforeEndCol_SetDigitals( LastColonne, dureeAdvanceProbeShutter, _LASER_B_30, 0);
					LaSeq.SplitSequence_FromEndOfCol_NbrPeriodeBeforeEndCol_SetDigitals( LastColonne, dureeAdvanceProbeShutter, _LASER_C_29, 0);
					LaSeq.SplitSequence_FromEndOfCol_NbrPeriodeBeforeEndCol_SetDigitals( LastColonne, dureeAdvanceProbeShutter, _LASER_D_28, 0);
					LaSeq.SplitSequence_FromEndOfCol_NbrPeriodeBeforeEndCol_SetDigitals( LastColonne, dureeAdvanceProbeShutter, _LASER_E_27, 0);
				}
				if(ProbeShutterStillOn)//Added 2013-11-21. 
				{	//If the probe shutter's still open, then the 689C shutter needs to be opened by itself in advance:  
					//int dureeAdvanceCShutter = MILLI_SECONDE_INT(6.2);	//Originally added 2013-11-21. How many ms in advance to open! (fine tuned 2013-11-21.) 
					int dureeAdvanceCShutter = MICRO_SECONDE_INT(m_ParamReal[5]);	//2014-04-30 now let user fine-tune this timing (default 6200 us). 
					LaSeq.SplitSequence_FromEndOfCol_NbrPeriodeBeforeEndCol_SetDigitals( LastColonne, dureeAdvanceCShutter, _PROB_SHUT, 1);
					LaSeq.SplitSequence_FromEndOfCol_NbrPeriodeBeforeEndCol_SetDigitals( LastColonne, dureeAdvanceCShutter, _C_SHUT, 1);
					//Don't touch 689B shutter (it's controlled elsewhere). 
					//Make sure 689C AOM is off.  Leave all others untouched (otherwise can erase other pulses). 
					//LaSeq.SplitSequence_FromEndOfCol_NbrPeriodeBeforeEndCol_SetDigitals( LastColonne, dureeAdvanceCShutter, _LASER_C_29, 0);//2014-04-30 Don't force AOMC off because now want C pulses earlier. 
				}
				
				LaSeq.InsertNewColonne( AfterLastColonne	,	"moreProbe", 	DureMoreProbe );
				LaSeq.SetCol_Digitals_DefaultPrecedentColonne( LastColonne, 
					_LASER_B_30			,	0	,
					_LASER_C_29			,	1	,//2013-11-21 changed to 1 from m_ParamBool[18].
					_LASER_D_28			,	0	,
					_LASER_E_27			,	m_ParamBool[48]	,//2015-06-26 Bart
					_PROB_SHUT			,	1	,
					_C_SHUT				,	1	,//Added 2013-11-21. 
					_MAIN_IGBT			,	0	,
					_RBROAD				,	0	,
					_IMAG_AOM			,	0	,
					_IMAG_CLIC			,	0	, 
					_NUSD_10			,	0	//2016-02-01 forcibly unattenuate 689B here (Bart and Chih-Hsi). 
					);	
				
				if(!m_ParamBool[33])//2013-12-05 added:  Only forcibly unattenuate mVatten from pulse value when check box enables mVatten! 
				{	
					LaSeq.SetCol_Analogs_Which_Debut_Fin( LastColonne, 
						_PROBE_B_ATTEN,	DEFAULT_VALUE,	DEFAULT_VALUE//Forcibly unattenuate 689B laser. 
						);
				}
				else
				{	//Do nothing. 
					LaSeq.SetCol_Analogs_Which_Debut_Fin( LastColonne
					);
				}
				


			}

			//=====================================
			//			"Wait [us]"
			//	Wait following "aom C", before "more aom B".  
			//	Forcibly unattenuates 689B laser in advance of "more aom B". 
			//  2013-03-14: Revised. 
			//=====================================
			// 
			int DureWipeWaitBC = int(MICRO_SECONDE_INT(m_ParamReal[55]));
			//NOTE: Always do this, so can forcibly unattenuate 689B laser before "more aom B [us]". 
			if(1)//m_ParamReal[55])//Wait[us]
			{	//#########################################
				//Decide what to do with the probe shutter:
				if(!ProbeShutterStillOn)
				{	//Probe shutter is closed, so do nothing.
				}
				else
				{	//Probe shutter is open.  Decide whether to close: 
					if(DureWipeWaitBC > dureeAdvanceProbeShutter)
					{	ProbeShutterStillOn = false;//close shutter
					//Code to open AOM's is below, if there's enough time. 
					}
				}

				LaSeq.InsertNewColonne( AfterLastColonne	,	"wipe wait", 	DureWipeWaitBC );
				LaSeq.SetCol_Digitals_DefaultPrecedentColonne( LastColonne,   
					_LASER_C_29			,	0	,
					_LASER_B_30			,	0	,
					_LASER_D_28			,	0	,
					_LASER_E_27			,	0	,
					_PROB_SHUT			,	ProbeShutterStillOn
					);	
				LaSeq.SetCol_Analogs_Which_Debut_Fin( LastColonne
					, _PROBE_B_ATTEN,	DEFAULT_VALUE,	DEFAULT_VALUE//Forcibly unattenuate 689B laser, or undo V-Bx pulse. 
					, _REP_INT,			DEFAULT_VALUE,	DEFAULT_VALUE//Added 2013-12-05 to undo V-By pulse. 
					);

				//Schedule AOM's to open if there's enough time (if already open, has no effect): 
				if(!ProbeShutterStillOn)
				{	LaSeq.SplitSequence_FromEndOfCol_NbrPeriodeBeforeEndCol_SetDigitals( LastColonne, DureWipeWaitBC - dureeAdvanceProbeShutter, _LASER_B_30, 1);
					LaSeq.SplitSequence_FromEndOfCol_NbrPeriodeBeforeEndCol_SetDigitals( LastColonne, DureWipeWaitBC - dureeAdvanceProbeShutter, _LASER_C_29, 1);
					LaSeq.SplitSequence_FromEndOfCol_NbrPeriodeBeforeEndCol_SetDigitals( LastColonne, DureWipeWaitBC - dureeAdvanceProbeShutter, _LASER_D_28, 1);
					LaSeq.SplitSequence_FromEndOfCol_NbrPeriodeBeforeEndCol_SetDigitals( LastColonne, DureWipeWaitBC - dureeAdvanceProbeShutter, _LASER_E_27, 1);
				}
			}
			
			//=====================================
			//			"more aom B [us]"
			//	2nd round of PA with 689B laser. 
			// (Beware that 689B might be attenuated for Raman1 and Raman2, and needs to be unattenuated before here.) 
			//  2013-03-14: Revised. 
			//=====================================
			// 
			int DureMoreProbeB = int(MICRO_SECONDE_INT(m_ParamReal[46]));
			if(DureMoreProbeB)//m_ParamReal[46])//more aom B [us]
			{	if(!ProbeShutterStillOn) 
				{	//Open probe shutter in advance: 
					LaSeq.SplitSequence_FromEndOfCol_NbrPeriodeBeforeEndCol_SetDigitals( LastColonne, dureeAdvanceProbeShutter, _PROB_SHUT, 1);
					ProbeShutterStillOn = true;//The probe shutter is open now! 
					//Warning: Expect probe laser AOM's to be on if Probe Shutter's been closed for a while.
					//In this case, to prevent an accidental probe pulse while probe shutter opens, turn off probe AOM's in advance: 
					LaSeq.SplitSequence_FromEndOfCol_NbrPeriodeBeforeEndCol_SetDigitals( LastColonne, dureeAdvanceProbeShutter, _LASER_B_30, 0);
					LaSeq.SplitSequence_FromEndOfCol_NbrPeriodeBeforeEndCol_SetDigitals( LastColonne, dureeAdvanceProbeShutter, _LASER_C_29, 0);
					LaSeq.SplitSequence_FromEndOfCol_NbrPeriodeBeforeEndCol_SetDigitals( LastColonne, dureeAdvanceProbeShutter, _LASER_D_28, 0);
					LaSeq.SplitSequence_FromEndOfCol_NbrPeriodeBeforeEndCol_SetDigitals( LastColonne, dureeAdvanceProbeShutter, _LASER_E_27, 0);
				}

				LaSeq.InsertNewColonne( AfterLastColonne	,	"PA2", 	DureMoreProbeB );
				LaSeq.SetCol_Digitals_DefaultPrecedentColonne( LastColonne,   
					_LASER_B_30			,	1	,
					_LASER_C_29			,	0	,
					_LASER_D_28			,	0	, 
					_LASER_E_27			,	0	,
					_PROB_SHUT			,	1	, 
					_MAIN_IGBT			,	0	,
					_RBROAD				,	0	
					);	
				LaSeq.SetCol_Analogs_Which_Debut_Fin( LastColonne
					);
			}
			
			//=====================================
			//			"Wipe2 [us]"
			//	Wipe with blue laser following "more aom B [us]".
			//  2013-03-14: Revised. 
			//=====================================
			// 
			int DureWipeWait2 = int(MICRO_SECONDE_INT(m_ParamReal[75]));
			if(m_ParamBool[15])//Wipe2 button
			{
				if(1)//DureWipe)//Wipe [us] from Wipe1 above. 
				{
					// On va remonter à la colonne qu'il faut splitter en deux pour ajouter le ClicClac
					LaSeq.SplitSequence_FromEndOfCol_NbrPeriodeBeforeEndCol_SetDigitals( LastColonne, dureeAdvanceImagingShuter, _IMAG_CLIC, 1);
					LaSeq.SetCol_Analogs_Which_Debut_Fin( LastColonne
						);
				}
				if(1)
				{
					LaSeq.InsertNewColonne( AfterLastColonne	,	"wipe", 	DureWipe );
					LaSeq.SetCol_Digitals_DefaultPrecedentColonne( LastColonne,   
						_LASER_B_30			,	0	,
						_LASER_C_29			,	0	,
						_LASER_D_28			,	0	,
						_LASER_E_27			,	0	,
						_PROB_SHUT			,	ProbeShutterStillOn	, 
						_NUSD_09			,	0	,//2013-12-04 set to 0, this is Lattice 3Dness.  No need to force to 1 here! 
						_IMAG_AOM			,	1	,
						_IMAG_CLIC			,	1	,
						_MAIN_IGBT			,	0	,
						_RBROAD				,	0	
						);	
					LaSeq.SetCol_Analogs_Which_Debut_Fin( LastColonne
						);
				}
			}

			//=====================================
			//			"Wait [us]"
			//	Wait after "Wipe2" and before "more aom C". 
			//  2013-03-14: Revised. 
			//=====================================
			// 
			if(m_ParamReal[75])//Wait [us]
			{
				LaSeq.InsertNewColonne( AfterLastColonne	,	"wipe wait", 	DureWipeWait2 );
				LaSeq.SetCol_Digitals_DefaultPrecedentColonne( LastColonne,   
					_LASER_B_30			,	0	,
					_LASER_C_29			,	0	,
					_LASER_D_28			,	0	,
					_LASER_E_27			,	0	,
					_PROB_SHUT			,	ProbeShutterStillOn	, 
					_IMAG_AOM			,	0	,
					_IMAG_CLIC			,	0
					);	
				LaSeq.SetCol_Analogs_Which_Debut_Fin( LastColonne
					);

				//Could reopen probe AOM's here if delay is long enough.  
			}

			//=====================================
			//			"more aom C [us]"
			//	Second recovery pulse with 689C laser. 
			//  2013-03-14: Revised. 
			//=====================================
			//
			int DureMoreProbeC = int(MICRO_SECONDE_INT(m_ParamReal[56]));
 			if(DureMoreProbeC)//more aom C [us]
			{	if(!ProbeShutterStillOn) 
				{	//Open probe shutter in advance: 
					LaSeq.SplitSequence_FromEndOfCol_NbrPeriodeBeforeEndCol_SetDigitals( LastColonne, dureeAdvanceProbeShutter, _PROB_SHUT, 1);
					ProbeShutterStillOn = true;//The probe shutter is open now! 
					//Warning: Expect probe laser AOM's to be on if Probe Shutter's been closed for a while.
					//In this case, to prevent an accidental probe pulse while probe shutter opens, turn off probe AOM's in advance: 
					LaSeq.SplitSequence_FromEndOfCol_NbrPeriodeBeforeEndCol_SetDigitals( LastColonne, dureeAdvanceProbeShutter, _LASER_B_30, 0);
					LaSeq.SplitSequence_FromEndOfCol_NbrPeriodeBeforeEndCol_SetDigitals( LastColonne, dureeAdvanceProbeShutter, _LASER_C_29, 0);
					LaSeq.SplitSequence_FromEndOfCol_NbrPeriodeBeforeEndCol_SetDigitals( LastColonne, dureeAdvanceProbeShutter, _LASER_D_28, 0);
					LaSeq.SplitSequence_FromEndOfCol_NbrPeriodeBeforeEndCol_SetDigitals( LastColonne, dureeAdvanceProbeShutter, _LASER_E_27, 0);
				}
				
				LaSeq.InsertNewColonne( AfterLastColonne	,	"evenMoreProbe", 	DureMoreProbeC );
				LaSeq.SetCol_Digitals_DefaultPrecedentColonne( LastColonne, 
					_LASER_B_30			,	0	,
					_LASER_C_29			,	m_ParamBool[15]	,
					_LASER_D_28			,	0	,
					_LASER_E_27			,	0	,
					_PROB_SHUT			,	1	, 
					_MAIN_IGBT			,	0	,
					_RBROAD				,	0	,
					_IMAG_AOM			,	0	,
					_IMAG_CLIC			,	0	
					);	
				LaSeq.SetCol_Analogs_Which_Debut_Fin( LastColonne
					);
			}

		}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////// More Trap panel  ////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		if(m_ParamBool[11])
		{
			if(1)//2013-12-04 used to be m_ParamReal[45])
			{
				int DureTrap = int(MILLI_SECONDE_INT(m_ParamReal[45]));
				LaSeq.InsertNewColonne( AfterLastColonne	,	"trap2", 	DureTrap );
				LaSeq.SetCol_Digitals_DefaultPrecedentColonne( LastColonne,   
					_LASER_C_29			,	0	,
					_LASER_B_30			,	0	,
					_LASER_D_28			,	0	,//Added 2013-03-14.
					_LASER_E_27			,	0	,
					_PROB_SHUT			,	0	,
					_B_SHUT				,	1	,//Added 2013-11-21 to set to 1.
					_C_SHUT				,	!m_ParamBool[44]	,//Added 2013-11-21 to set to 0. 2014-04-30 changed to m_ParamBool[30] so user can disable this shutter. 2014-05-05 switched to NOT Bool 44 from 30. 
					_MAIN_IGBT			,	0	,
					_RBROAD				,	0	,
					_IMAG_AOM			,	0	,
					_IMAG_CLIC			,	0	
					);	
				LaSeq.SetCol_Analogs_Which_Debut_Fin( LastColonne, 
					_MOT_INT, m_pMainFrame->m_PortDialog.m_Ana_1_Out[0], m_pMainFrame->m_PortDialog.m_Ana_1_Out[0]
				);
			}
		}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////// Time of Flight panel  ///////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	if(m_ParamBool[6])
	{
//		if(m_ParamReal[20])
		{
			int DureeTOF = MICRO_SECONDE_INT(m_ParamReal[20]);
			int DureeStern = MILLI_SECONDE_INT(2);
			// si Stern et Gerlach
			if(m_ParamBool[10] && DureeTOF > DureeStern)
				DureeTOF -= DureeStern;

			LaSeq.InsertNewColonne( AfterLastColonne	,	"TOF_µs"
										,	DureeTOF );
			LaSeq.SetCol_Digitals_DefaultPrecedentColonne( LastColonne, 	  
// 				_MOT_AOM		,	0	,
				_MOT_CLIC		,	0	,
				_RBROAD			,	0	,
				_RSINGLE		,	0	,
				_MAIN_IGBT		,	0	,
				_LATT_SHUT		,	1		
				);	
			LaSeq.SetCol_1Digital(-1,   	_LASER_C_29	, 0);
			LaSeq.SetCol_1Digital(-1,   	_LASER_B_30	, 0);
			LaSeq.SetCol_1Digital(-1,   	_LASER_D_28	, 0);//Added 2013-03-14.
			LaSeq.SetCol_1Digital(-1,   	_LASER_E_27	, 0);//Added 2013-09-14.
			LaSeq.SetCol_1Digital(-1,   	_PROB_SHUT	, 0);
			LaSeq.SetCol_1Digital(-1,   	_B_SHUT		, 1);//Added 2013-11-21 to set to 1. 
			LaSeq.SetCol_1Digital(-1,   	_C_SHUT		, m_ParamBool[44]);//Added 2013-11-21 to set to 0. 2014-04-30 set to m_ParamBool[30] so user can disable this shutter. 2014-05-05 switched to Bool 44 from 30. 
			LaSeq.SetCol_1Digital(-1,   	_IMAG_AOM	, 0);

			LaSeq.SetCol_Analogs_Which_Debut_Fin( LastColonne,
				_GUIDE_CURRENT,	m_ParamReal[64],	m_ParamReal[64], 
				_MOT_INT, m_pMainFrame->m_PortDialog.m_Ana_1_Out[0], m_pMainFrame->m_PortDialog.m_Ana_1_Out[0]
				, _REP_INT 			,	DEFAULT_VALUE	,	DEFAULT_VALUE	
				);
			// si Stern et Gerlach
			if(m_ParamBool[10] && DureeTOF > DureeStern)
			{
				// on recopie la colonne pour y changer Stern off
				LaSeq.InsertNewColonne_CopiedFromCol( AfterLastColonne	,	LaSeq.GetColonne(LastColonne)
											,	"SternOff"
											,	DureeStern	);
				LaSeq.SetCol_1Digital(-1, _RepumpShutter, 0);
			}
			for(int iemeCol = 1; iemeCol <= 8; iemeCol++)
				LaSeq.SetCol_1Digital(-1,   23 + iemeCol, false);
		}
	}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////// Absorption Imaging (NEW, Bart 2014-04-30)  //////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//Notes to programmer (YOU, sitting in the chair reading this!): 
	//-- MOT COILS: This section resets the MOT coils back to default if they were pulsed off earlier in the sequence. 
	//-- Precision timing: The trigger advance determines when the camera actually takes a picture, so is the parameter to tweak (in sequence) to get images taken right after recovery.  
	//-- More timing: The camera itself has its own exposure parameter to set in LabExe (usually 450 us), and there's a shutter advance to tweak (in sequence). 
	//-- Wiping collision:  If there's not enough elapsed time between earlier uses of the imaging beam (e.g., wipe1) and this step, there could be some unexpected behavior! 
	//-- Probe lasers: For safety, the probe AOMs are left off until 2nd image (since the probe shutter is probably still closing during the first image).  They're turned on to thermally re-equilibrate. 
	//-- Lot's of things are turned on right after the 3rd image, so if timing's really bad, this could lead to light leaks in the 3rd image.  Hasn't been observed yet, but be warned. 
	//-- 3rd image: Care's needed to not overexpose this image in the following step, since the camera records longer than our pulse.  So, in dead time, don't turn on imaging AOM until camera shutter's actually closed! (This problem would go away if you closed the imaging shutter first, but that risks making the 3rd image not a true background.) 
	//
	if(m_ParamBool[7]) {
		//-- Setup important parameters:  
		int ImagingAOMpulse = MICRO_SECONDE_INT(m_ParamReal[23]);//Length of imaging AOM pulse (usually 50 us). 
		int dureeAdvanceTrig = MICRO_SECONDE_INT(m_ParamReal[59]);//Signal to trigger camera is sent earlier than AOM signal, by this amount (usually 360 us).
		//int dureeAdvanceImagingShuter = MILLI_SECONDE_INT(m_ParamReal[49]);//Already defined at top of document, because it's needed for Wipe (usually 2 ms).  
		int waitBetweenImages = MILLI_SECONDE_INT(m_ParamReal[24]);//Time to wait between imaging pulses (usually 50 ms).

		//-- Take three images in sequence: 
		//	-- [IMAGE #1]: Atoms and imaging light present; lattice set by previous panel (e.g., time of flight). 
		if(1) {
			if(dureeAdvanceImagingShuter) {	//Open camera shutter in advance before the 1st image:
				//Test if probe panel's on or off, and either wiping is on, to see what to do with Imaging AOM.  
				if(m_ParamBool[4]&&(m_ParamBool[18]||m_ParamBool[15])){//Probe panel's on and at least one wiping step is engaged.
					LaSeq.SplitSequence_FromEndOfCol_NbrPeriodeBeforeEndCol_SetDigitals( LastColonne, dureeAdvanceImagingShuter, 
						_IMAG_CLIC		,	1	);//Open shutter.  This used to be m_ParamBool[31] in Gael's code, which corresponded to an unused button?
					//WARNING: Don't force _IMAG_AOM to 0 here to make sure light's off before open shutter.  This can wreak havoc with earlier wipe1 and wipe2 steps.  Just have to assume earlier steps were programmed correctly, and left blue imaging light off. 			
				}else{
					LaSeq.SplitSequence_FromEndOfCol_NbrPeriodeBeforeEndCol_SetDigitals( LastColonne, dureeAdvanceImagingShuter, 
						_IMAG_CLIC		,	1	, //Open shutter.  This used to be m_ParamBool[31] in Gael's code, which corresponded to an unused button?
						_IMAG_AOM		,	0	);//Force imaging AOM off because Probe panel's off or user's not wiping, so can image Red MOT and lattice loading! 
				}
					
			}
			if(dureeAdvanceTrig) { //Trigger camera in advance to take the 1st image:
				LaSeq.SplitSequence_FromEndOfCol_NbrPeriodeBeforeEndCol_SetDigitals(LastColonne, dureeAdvanceTrig, 
					_CAM_TRIGGER	,	1	);//Trigger camera.
			}
			LaSeq.InsertNewColonne( AfterLastColonne	,	"AbsImage1"	,	ImagingAOMpulse );//Create this column.
			LaSeq.SetCol_Digitals_DefaultPrecedentColonne( LastColonne, 	  //Populate digital controls.
				_MOT_CLIC		,	0	,//This BlueMOT/ZS shared AOM should already be off from earlier, but force off here as reminder to the programmer (YOU, BUDDY!).
				_IMAG_AOM		,	1	,//Imaging pulse on during first image. 
				_IMAG_CLIC		,	1	,//Camera shutter should already be open from earlier. 
				_CAM_TRIGGER	,	1	,//Camera trigger should already be on from earlier. 
				_B_SHUT			,	1	,//Added 2014-05-05 to set to 1, to force 689B shutter open if closed.
				_C_SHUT			,	!m_ParamBool[44] //2014-05-05. Puts 689C shutter in desired starting position for next sequence run. 
				);	
			LaSeq.SetCol_Analogs_Which_Debut_Fin( LastColonne //Populate analog controls.
					);
		}
		//	-- Wait between 1st and 2nd images (turn off lattice; resetup MOT coils if pulsed off):
		if(1) {
			LaSeq.InsertNewColonne_CopiedFromCol( AfterLastColonne	,	LaSeq.GetColonne( LastColonne),	"AbsImageWait1to2",	waitBetweenImages);//Create this column.
			LaSeq.SetCol_Digitals_DefaultPrecedentColonne( LastColonne, 	  //Populate digital controls.
				_IMAG_AOM		,	0	,//Stop imaging pulse! 
				_IMAG_CLIC		,	1	,//Camera shutter stays open and ready for 2nd image.
				_CAM_TRIGGER	,	0	,//Stop camera trigger! 
				_LATT_SHUT		,	1	,//Close the lattice shutter right after 1st image. 
				_IGBT			,	1	//Force MOT-coil IGBT on (does nothing unless MOT coils were pulsed off earlier). 
				);	
			LaSeq.SetCol_Analogs_Which_Debut_Fin( LastColonne, //Populate analog controls.
					_I_MOT,	DEFAULT_VALUE,	DEFAULT_VALUE	//Force MOT-coil current to default (does nothing unless MOT coils pulsed off earlier). 
					);
		}
		//	-- [IMAGE #2]: Imaging light but no atoms present; lattice blocked by shutter. (Atoms assumed "wiped" by 1st image and blocking of shutter!)
		if(1) {
			//Don't need to open camera shutter in advance, because it's still open. 
			if(dureeAdvanceTrig) { //Trigger camera in advance to take the 2nd image:
				LaSeq.SplitSequence_FromEndOfCol_NbrPeriodeBeforeEndCol_SetDigitals(LastColonne, dureeAdvanceTrig, 
					_CAM_TRIGGER	,	1	);//Trigger camera.
			}
			LaSeq.InsertNewColonne( AfterLastColonne	,	"AbsImage2"	,	ImagingAOMpulse );//Create this column.
			LaSeq.SetCol_Digitals_DefaultPrecedentColonne( LastColonne, 	  //Populate digital controls.
				_MOT_CLIC		,	0	,//This BlueMOT/ZS shared AOM should already be off from earlier, but force off here as reminder to the programmer (YOU, BUDDY!).
				_IMAG_AOM		,	1	,//Imaging pulse on during second image. 
				_IMAG_CLIC		,	1	,//Camera shutter should already be open from earlier. 
				_CAM_TRIGGER	,	1	,//Camera trigger should already be on from earlier. 
				_PROB_SHUT		,	0	,//Probe shutter should be mechanically closed by now (reminder to programmer). 
				_LASER_B_30		,	1	,//Turn AOM back on to warm up. 
				_LASER_C_29		,	1	,//Turn AOM back on to warm up. 
				_LASER_D_28		,	1	,//Turn AOM back on to warm up. 
				_LASER_E_27		,	1	//Turn AOM back on to warm up. 
				);
			LaSeq.SetCol_Analogs_Which_Debut_Fin( LastColonne //Populate analog controls.
				);
		}
		//	-- Wait between 2nd and 3rd images:
		if(1) {
			LaSeq.InsertNewColonne_CopiedFromCol( AfterLastColonne	,	LaSeq.GetColonne( LastColonne),	"AbsImageWait2to3",	waitBetweenImages);//Create this column.
			LaSeq.SetCol_Digitals_DefaultPrecedentColonne( LastColonne, 	  //Populate digital controls.
				_IMAG_AOM		,	0	,//Stop imaging pulse! 
				_IMAG_CLIC		,	1	,//Camera shutter stays open and ready for 3rd image. (This allows true background measurement, but need to be careful to not turn AOM back on and bleach 3rd image before this shutter is closed!) 
				_CAM_TRIGGER	,	0	//Stop camera trigger! 
				);	
			LaSeq.SetCol_Analogs_Which_Debut_Fin( LastColonne //Populate analog controls.
				);
		}
		//	-- [IMAGE #3]: No atoms or imaging light present; lattice blocked by shutter. 
		if(1) {
			//Don't need to open camera shutter in advance, because it's still open. 
			if(dureeAdvanceTrig) { //Trigger camera in advance to take the 3rd image:
				LaSeq.SplitSequence_FromEndOfCol_NbrPeriodeBeforeEndCol_SetDigitals(LastColonne, dureeAdvanceTrig, 
					_CAM_TRIGGER	,	1	);//Trigger camera.
			}
			LaSeq.InsertNewColonne( AfterLastColonne	,	"AbsImage3"	,	ImagingAOMpulse );//Create this column.
			LaSeq.SetCol_Digitals_DefaultPrecedentColonne( LastColonne, 	  //Populate digital controls.
				_MOT_CLIC		,	0	,//This BlueMOT/ZS shared AOM should already be off from earlier, but force off here as reminder to the programmer (YOU, BUDDY!).
				_IMAG_AOM		,	0	,//Imaging pulse stays OFF during third image. 
				_IMAG_CLIC		,	1	,//Camera shutter should already be open from earlier. 
				_CAM_TRIGGER	,	1	//Camera trigger should already be on from earlier. 
				);	
			LaSeq.SetCol_Analogs_Which_Debut_Fin( LastColonne //Populate analog controls.
				);
		}
	}


//
///////////////////////////////////////////// GAEL's OLD Imaging ("fluo") panel (Don't use after 2014-04-30) //////////////////////
//	if(m_ParamBool[7] && !m_ParamBool[8] && LaSeq.GetColonne( LastColonne)) {
//		// On va remonter à la colonne qu'il faut splitter en deux pour ajouter le ClicClac
//		LaSeq.SplitSequence_FromEndOfCol_NbrPeriodeBeforeEndCol_SetDigitals( LastColonne, dureeAdvanceImagingShuter, _IMAG_CLIC, m_ParamBool[31], _IMAG_AOM, 0);//Added _IMAG_AOM line on 2013-12-05 to make sure no light leak during 1st image! 
//
//		if(m_ParamReal[59]) {//First imaging pulse camera advanced trigger.
//			int dureeAdvanceTrig = MICRO_SECONDE_INT(m_ParamReal[59]);
//			LaSeq.SplitSequence_FromEndOfCol_NbrPeriodeBeforeEndCol_SetDigitals(LastColonne, dureeAdvanceTrig, _CAM_TRIGGER, 1);
//		}
//		if(m_ParamReal[23]) {
//			LaSeq.InsertNewColonne( AfterLastColonne	,	"Fluo1"	,	MICRO_SECONDE_INT(m_ParamReal[23]) );//First imaging pulse, with atoms and light present.  
//			LaSeq.SetCol_Digitals_DefaultPrecedentColonne( LastColonne, 	  
//				_MOT_CLIC		,	0	,
//				_ZEEMAN			,	0	,
//				_IMAG_AOM		,	1	,
//				_IMAG_CLIC		,	1	,
//				_CAM_TRIGGER	,	1	,
//				_LAT_BACK		,	0	);	
//			LaSeq.SetCol_1Digital(-1,   	_LASER_C_29	, 0);
//			LaSeq.SetCol_1Digital(-1,   	_LASER_B_30	, 0);
//			LaSeq.SetCol_1Digital(-1,   	_LASER_D_28	, 0);//Added 2013-03-14.
//			LaSeq.SetCol_1Digital(-1,   	_LASER_E_27	, 0);//Added 2013-08-14.
//			LaSeq.SetCol_1Digital(-1,   	_PROB_SHUT	, 0);
//			LaSeq.SetCol_Analogs_Which_Debut_Fin( LastColonne, 
//				_MOT_INT			,	DEFAULT_VALUE	,	DEFAULT_VALUE	,
//				_ZSL_DET 			,	DEFAULT_VALUE	,	DEFAULT_VALUE	);
//		}
//	}	
//
///////////////////////////////////////////// GAEL's OLD Absorption panel (Don't use after 2014-04-30) ////////////////////////////
//	if(m_ParamBool[7] && m_ParamBool[8] && LaSeq.GetColonne( LastColonne)) {
//		// On va remonter à la colonne qu'il faut splitter en deux pour ajouter le ClicClac
//		int dureeAdvance = MILLI_SECONDE_INT(m_ParamReal[49]);
//		LaSeq.SplitSequence_FromEndOfCol_NbrPeriodeBeforeEndCol_SetDigitals( LastColonne, dureeAdvance, _IMAG_CLIC, m_ParamBool[31], _IMAG_AOM, 0);//2013-12-05 added _IMAG_AOM to zero to prevent light leak. 
//
//		if(m_ParamReal[59]) {//Second imaging pulse camera advanced trigger.
//			int dureeAdvanceTrig = MICRO_SECONDE_INT(m_ParamReal[59]);
//			LaSeq.SplitSequence_FromEndOfCol_NbrPeriodeBeforeEndCol_SetDigitals( LastColonne, dureeAdvanceTrig, _CAM_TRIGGER, 1);
//		}
//		int testNperiodes = MICRO_SECONDE_INT(m_ParamReal[23]);
//		LaSeq.InsertNewColonne( AfterLastColonne, "Abs1" , testNperiodes);//Second imaging pulse, with light but no atoms.  
//		LaSeq.SetCol_Digitals_DefaultPrecedentColonne( LastColonne, 	  
//				_MOT_CLIC		,	0	,
//				_ZEEMAN			,	0	,
//				_IMAG_AOM		,	1	,
//				_IMAG_CLIC		,	1	,
//				_CAM_TRIGGER	,	1	);	
//		LaSeq.SetCol_1Digital(-1,   	_LASER_C_29	, 0);
//		LaSeq.SetCol_1Digital(-1,   	_LASER_B_30	, 0);
//		LaSeq.SetCol_1Digital(-1,   	_LASER_D_28	, 0);//Added 2013-03-14.
//		LaSeq.SetCol_1Digital(-1,   	_LASER_E_27	, 0);//Added 2013-08-14.
//		LaSeq.SetCol_1Digital(-1,   	_PROB_SHUT	, 0);
//
//		LaSeq.SetCol_Analogs_Which_Debut_Fin( LastColonne, 
//				_MOT_INT			,	DEFAULT_VALUE	,	DEFAULT_VALUE	,
//				_ZSL_DET 			,	DEFAULT_VALUE	,	DEFAULT_VALUE	);
//	
//		// on recopie la colonne pour y changer 2 sorties analogiques
//		LaSeq.InsertNewColonne_CopiedFromCol( AfterLastColonne	,	LaSeq.GetColonne( LastColonne),	"Abs2",	MILLI_SECONDE_INT(m_ParamReal[24]));//50 ms delay between 1st and 2nd images.  
//		LaSeq.SetCol_1Digital(-1,   	_IMAG_AOM	, 0);
//		LaSeq.SetCol_1Digital(-1,   	_CAM_TRIGGER, 0);
//		LaSeq.SetCol_1Digital(-1,   	_LATT_SHUT	, 1);//Close the lattice shutter right after the 1st image.  
//		LaSeq.SetCol_1Digital(-1,   	_IGBT	, 1);//2013-12-18 Force MOT-coil IGBT on for the first time (does nothing unless you pulsed the MOT coils off earlier...). 
//		LaSeq.SetCol_1Digital(-2,		 _LASER_B_30, 0);
//		LaSeq.SetCol_1Digital(-2,		 _LASER_C_29, 0);
//		LaSeq.SetCol_1Digital(-2,		 _LASER_D_28, 0);//Added 2013-03-14.
//		LaSeq.SetCol_1Digital(-2,		 _LASER_E_27, 0);//Added 2013-08-14.
//		LaSeq.SetCol_1Digital(-1,   	_PROB_SHUT	, 0);
//		LaSeq.SetCol_1Digital(-1,   	_LAT_BACK	, 0);
//		LaSeq.SetCol_Analogs_Which_Debut_Fin( LastColonne, 
//			_I_MOT,	DEFAULT_VALUE,	DEFAULT_VALUE
//		);
//
//
//		LaSeq.InsertNewColonne_CopiedFromCol( AfterLastColonne	,	LaSeq.GetColonne(-2), "Abs3"	);
//		LaSeq.SetCol_1Digital(-1,   	_LATT_SHUT	, 1);
//		LaSeq.SetCol_1Digital(-1,   	_IGBT	, 1);//2013-12-18 Force MOT-coil IGBT on (does nothing unless you pulsed the MOT coils off earlier...). 
//		LaSeq.SetCol_1Digital(-1,   	_LASER_C_29	, 1); 
//		LaSeq.SetCol_1Digital(-1,   	_LASER_B_30	, DEFAULT_VALUE);
//		LaSeq.SetCol_1Digital(-1,   	_LASER_D_28	, DEFAULT_VALUE);//Added 2013-03-14.
//		LaSeq.SetCol_1Digital(-1,   	_LASER_E_27	, DEFAULT_VALUE);//Added 2013-08-14.
//		LaSeq.SetCol_1Digital(-1,   	_PROB_SHUT	, 0);
//		LaSeq.SetCol_1Digital(-1,   	_LAT_BACK	, 0);
//		LaSeq.SetCol_Analogs_Which_Debut_Fin( LastColonne, 
//			_I_MOT,	DEFAULT_VALUE,	DEFAULT_VALUE
//			);
//	
//		LaSeq.InsertNewColonne_CopiedFromCol( AfterLastColonne	,	LaSeq.GetColonne(-2), "Abs4"	);
//		LaSeq.SetCol_1Digital(-1,   	_LASER_C_29	, DEFAULT_VALUE);
//		LaSeq.SetCol_1Digital(-1,   	_LASER_B_30	, DEFAULT_VALUE);
//		LaSeq.SetCol_1Digital(-1,   	_LASER_D_28	, DEFAULT_VALUE);//Added 2013-03-14.
//		LaSeq.SetCol_1Digital(-1,   	_LASER_E_27	, DEFAULT_VALUE);//Added 2013-08-14.
//		LaSeq.SetCol_1Digital(-1,   	_PROB_SHUT	, 0);
//	
//		LaSeq.InsertNewColonne_CopiedFromCol( AfterLastColonne	,	LaSeq.GetColonne(-2), "Abs5"	);
//		LaSeq.SetCol_1Digital(-1,   	_IMAG_AOM	, 0);
//		LaSeq.SetCol_1Digital(-1,   	_LASER_C_29	, DEFAULT_VALUE);
//		LaSeq.SetCol_1Digital(-1,   	_LASER_B_30	, DEFAULT_VALUE);
//		LaSeq.SetCol_1Digital(-1,   	_LASER_D_28	, DEFAULT_VALUE);//Added 2013-03-14.
//		LaSeq.SetCol_1Digital(-1,   	_LASER_E_27	, DEFAULT_VALUE);//Added 2013-08-14.
//		LaSeq.SetCol_1Digital(-1,   	_PROB_SHUT	, 0);
//		
//		//This inserts the advanced triggering of the CCD camera ahead of the AOM imaging pulse for the 2nd and 3rd absorption images, but not the 1st. 
//		if(m_ParamReal[59]) {
//			int dureeAdvanceTrig = MICRO_SECONDE_INT(m_ParamReal[59]);
//			LaSeq.SplitSequence_FromEndOfCol_NbrPeriodeBeforeEndCol_SetDigitals( LastColonne - 3, dureeAdvanceTrig, _CAM_TRIGGER, 1);
//			LaSeq.SplitSequence_FromEndOfCol_NbrPeriodeBeforeEndCol_SetDigitals( LastColonne - 1, dureeAdvanceTrig, _CAM_TRIGGER, 1);
//		}
//		
//		//The following three lines (added 2013-12-04) tell the imaging shutter to close right after the 2nd imaging pulse (mechanical delay means it'll take a few ms to close). 
//		//This way, there's no blue imaging leakage light on the CCD during the 3rd "dark" absorption image, which could be a problem because the imaging AOM's orders overlap a bit on the imaging fiber's face. 
//		LaSeq.SetCol_1Digital(-1,   	_IMAG_CLIC	, 0);//2013-12-03. 
//		LaSeq.SetCol_1Digital(-2,   	_IMAG_CLIC	, 0);//2013-12-03.
//		LaSeq.SetCol_1Digital(-3,   	_IMAG_CLIC	, 0);//2013-12-03.
//
//		//This sets the probe laser AOM's to their default states, and seems to override much of the above code, right? 
//		//This happens roughly 20 ms after the 1st absorption image, which should be enough time for the probe shutter to close. 
//		LaSeq.SplitSequence_FromEndOfCol_NbrPeriodeBeforeEndCol_SetDigitals( -4, MILLI_SECONDE_INT(m_ParamReal[24]) - dureeAdvanceProbeShutter, _LASER_B_30, DEFAULT_VALUE);
//		LaSeq.SplitSequence_FromEndOfCol_NbrPeriodeBeforeEndCol_SetDigitals( -4, MILLI_SECONDE_INT(m_ParamReal[24]) - dureeAdvanceProbeShutter, _LASER_C_29, DEFAULT_VALUE);
//		LaSeq.SplitSequence_FromEndOfCol_NbrPeriodeBeforeEndCol_SetDigitals( -4, MILLI_SECONDE_INT(m_ParamReal[24]) - dureeAdvanceProbeShutter, _LASER_D_28, DEFAULT_VALUE);//Added 2013-03-14.
//		LaSeq.SplitSequence_FromEndOfCol_NbrPeriodeBeforeEndCol_SetDigitals( -4, MILLI_SECONDE_INT(m_ParamReal[24]) - dureeAdvanceProbeShutter, _LASER_E_27, DEFAULT_VALUE);//Added 2013-08-14.
//	}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////// Dead Time panel  ////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// This step has two purposes: (1) it adds "dead time" after the sequence, and (2) it adds "dead MOT time" (no atoms) before the next sequence. 
// Purpose (2) shows up as the first column (usually 50 ms) of the sequence! 
	if(m_ParamBool[9])
	{
		if(m_ParamReal[25])//Dead time at end of sequence. 
		{
			LaSeq.InsertNewColonne( AfterLastColonne	,	"Dead1"
										,	MILLI_SECONDE_INT(m_ParamReal[25]) );
			LaSeq.SetCol_Digitals_DefaultPrecedentColonne( LastColonne, 	  
				_ZEEMAN			,	0	,
// 				_MOT_AOM		,	0	,
				_MOT_CLIC		,	1	,	//2013-11-25 Changed from 0 to 1 to eliminate Zeeman shutter catching blue light when ZS AOM turns on (dead time should be set to 9ms). (Shouldn't affect 3rd image because of mechanical delay?) 
				_MAIN_IGBT		,	1	,	//2013-12-2. Turn back ON ZS AOM and shutter (experimental!). 
				_REPUMPS		,	0	,	//2014-05-01 keep OFF, so (Zeeman slower and blue beam AOM and Repumps AOM) turning back on don't affect imaging beam intensity (shared AOM's ...). 
				_IMAG_AOM		,	0	,	//2013-05-01 keep imaging AOM off, because now left imaging shutter open during 3rd image.  Turning AOM on here overesposes 3rd absorption image. 
				_IMAG_CLIC		,	0	,
				_LATT_SHUT		,	1	,	//2013-12-04 Added this (nothing previously): keeps lattice off during dead time, so nothing possibly trapped before MOT parts.  
				_LAT_BACK		,	0	,
				_NUSD_10		,	0	,	//2016-02-01 forced off to prevent 689B AOM cooling, used to be: DEFAULT_VALUE	,//2014-02-05 used to be 0.
				_2D_MOT			,	0	,
				_CAM_TRIGGER	,	0	,
				_RepumpShutter	,	0	,//2013-12-2, turns on repumps shutter. 
				_RedMOTShutter	,	0	,//2013-12-2, turns on red MOT shutter. 
				_NUSD_16		,	0	,
				_RBROAD			,	0	,
				_RSINGLE		,	0	,
				_NUSD_19		,	0	,
				_NUSD_20		,	0	,
				_NUSD_21		,	0	,
				_NUSD_22		,	0	,
				_IGBT			,	1		//2014-04-30 added. Forces IGBT back on (does nothing unless MOT coils were pulsed off earlier).  The Absorption Imaging section should've already taken care of this.
														);	
			LaSeq.SetCol_1Digital(-1,   	_LASER_C_29	, DEFAULT_VALUE);
			LaSeq.SetCol_1Digital(-1,   	_LASER_B_30	, DEFAULT_VALUE);
			LaSeq.SetCol_1Digital(-1,   	_LASER_D_28	, DEFAULT_VALUE);//Added 2013-03-14. 
			LaSeq.SetCol_1Digital(-1,   	_LASER_E_27	, DEFAULT_VALUE);//Added 2013-08-14. 
			LaSeq.SetCol_1Digital(-1,   	_PROB_SHUT	, 0);
			LaSeq.SetCol_Analogs_Which_Debut_Fin( LastColonne
				, _REP_INT 			,	DEFAULT_VALUE	,	DEFAULT_VALUE	
				, _I_MOT			,	DEFAULT_VALUE	,	DEFAULT_VALUE	//2014-04-30 added. Forces MOT coils current to default (does nothing unless MOT coils were pulsed off earlier).  The Absorption Imaging section should've already taken care of this.
				);
		}




		if(m_ParamReal[22])//Dead MOT time at beginning of next sequence. 
		{
			LaSeq.InsertNewColonne_CopiedFromCol(0	,	LaSeq.GetColonne(1)
										,	"Load00"
										,	MILLI_SECONDE_INT(m_ParamReal[22]));
			LaSeq.SetCol_Digitals_DefaultPrecedentColonne( 1 , 	  
				_ZEEMAN			,	0	,
				_MOT_AOM		,	1	,
				_MOT_CLIC		,	1	,
				_MAIN_IGBT		,	1	,//2013-12-2. Turn on ZS AOM and shutter (experimental!). 
				_REPUMPS		,	1	,//Set to 1 on 2013-11-22 so that Zeeman beam and repumps are ON before Sr beam starts! 
				_IMAG_AOM		,	1	,//2013-12-04 Set to be on by default, to keep imaging AOM warm.
				_IMAG_CLIC		,	0	,
				_LAT_BACK		,	0	,
				_NUSD_10		,	0	,//2016-02-01 forced on to prevent 689B AOM cooling, used to be: DEFAULT_VALUE	,//2014-02-05 used to be 0.
				_2D_MOT			,	0	,
				_CAM_TRIGGER	,	0	,
				_RepumpShutter	,	0	,//2013-12-2, turns on repumps shutter. 
				_RedMOTShutter	,	0	,//2013-12-2, turns on red MOT shutter. 
				_NUSD_16		,	0	,
				_RBROAD			,	1	,//Set to 1 on 2013-11-22 so that Red MOT and its FM broadening is ready before Sr beam starts! 
				_RSINGLE		,	0	,
				_NUSD_19		,	0	,
				_NUSD_20		,	0	,
				_NUSD_21		,	0	,
				_NUSD_22		,	0
														);	
			LaSeq.SetCol_1Digital(1,   	_LASER_B_30	, 1);// 2013-11-21 set to 1, to keep AOM warm.
			LaSeq.SetCol_1Digital(1,   	_LASER_C_29	, 1);// 2013-11-21 set to 1, to keep AOM warm.
			LaSeq.SetCol_1Digital(1,   	_LASER_D_28	, 1);//Added 2013-03-14. 2013-11-21 set to 1, to keep AOM warm.
			LaSeq.SetCol_1Digital(1,   	_LASER_E_27	, 1);//Added 2013-08-14. 2013-11-21 set to 1, to keep AOM warm.
			LaSeq.SetCol_1Digital(1,   	_PROB_SHUT	, 0);
			LaSeq.SetCol_1Digital(1,   	_B_SHUT	, 1);// Added 2013-11-21, set to starting value 1. 
			LaSeq.SetCol_1Digital(1,   	_C_SHUT	, !m_ParamBool[44]);// Added 2013-11-21, set to starting value 0. 2014-04-30 set to m_ParamBool[30] so user can disable this shutter.  2014-05-05 switched to NOT Bool 44 from 30. 
			LaSeq.SetCol_Analogs_Continue( LastColonne);
	}
	}
return LaSeq.CalculateNbrPeriodes();
}	