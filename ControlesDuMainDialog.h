///////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////// Gestion de tous les controls de la MaindDialog ///////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
enum{	m_i_bActiveLoad					= 1	,
		m_i_bActiveCompress				,
		m_i_bActiveMelasse				,
		m_i_bActivePolar				,
		m_i_bActivePreguide				,
		m_i_bActiveTof					,
		m_i_bActiveFluo					,
		m_i_bActiveAbsorp				,
		m_i_bActiveDeadTime				,
		m_i_bTrigLoad					,
		m_i_bTrigCompress				,
		m_i_bTrigMelasse				,
		m_i_bTrigPolar					,
		m_i_bTrigPreguide				,
		m_i_bTrigTof					,
		m_i_bTrigFluo					,
		m_i_bTrigAbsorp					,
		m_i_bTrigDeadTime				,

		m_i_bMelasseHSpeed				,
		m_i_bMelasseVSpeed				,
		m_i_bPolarMagneticField			,
		m_i_bPolarPumpLight				,
		m_i_bPolarRepumpLight			,
		m_i_bPolarDepumpLight			,
		m_i_bPolarDepumpRepump			,
		m_i_bPreguideBias				,
		m_i_bFluoRepump					,
		m_i_bFluoCycling				,
		m_i_bAbsorpRepump				,
		m_i_bAbsorpCycling				,

		m_i_LoadMotDet					,
		m_i_LoadZeemanTimeOn			,
		m_i_LoadZeemanTimeOff			,
		m_i_CompressDuration			,
		m_i_CompressDetuning			,
		m_i_CompressMotIntensity		,
		m_i_CompressRepIntensity		,
		m_i_CompressGradient			,
		m_i_Compress3dGradient			,
		m_i_CompressRampDuration		,
		m_i_CompressIntDuration			,
		m_i_MelassePlateauDuration		,
		m_i_MelasseDebutDetuning		,
		m_i_MelasseDebutMotIntensity	,
		m_i_MelasseDebutRepIntensity	,
		m_i_MelasseSlopeDuration		,
		m_i_MelasseFinDetuning			,
		m_i_MelasseFinMotIntensity		,
		m_i_MelasseFinRepIntensity		,
		m_i_PolarPumpDuration			,
		m_i_PolarDepumpDuration			,
		m_i_PreguideDuration			,
		m_i_PreguideGradient			,
		m_i_TofDuration					,
		m_i_FluoDuration				,
		m_i_AbsorpDuration				,
		m_i_AbsorpIntensity				,
		m_i_DeadTimeDuration			
};

BEGIN_EVENTSINK_MAP(CMainDialog, CProDialog)

	ON_EVENT(CMainDialog, IDC_MAIN_BUTTON_START_SEQUENCE, 1, ValueChangedButtonStartSequence, VTS_BOOL)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_MODE_REPET, DISPID_CLICK, ClickEditModeRepet, VTS_NONE)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_MODE_REPET, 1, ValueChangedMainEditModeRepet, VTS_PVARIANT VTS_PVARIANT VTS_BOOL)
	ON_EVENT(CMainDialog, IDC_MAIN_CHECK_WAIT_TRIGGER, 1, ValueChangedMainCheckWaitTrigger, VTS_BOOL)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_WAIT_BEFORE_LOAD, 1, ValueChangedMainEditWaitBeforeLoad, VTS_PVARIANT VTS_PVARIANT VTS_BOOL)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_WAIT_BEFORE_LOAD, DISPID_MOUSEUP, MouseUpMainEditWaitBeforeLoad, VTS_I2 VTS_I2 VTS_I4 VTS_I4)

	ON_EVENT(CMainDialog, IDC_MAIN_CHECK_01	, 1, ValueChangedMainCheck01	, VTS_BOOL)
	ON_EVENT(CMainDialog, IDC_MAIN_CHECK_02	, 1, ValueChangedMainCheck02	, VTS_BOOL)
	ON_EVENT(CMainDialog, IDC_MAIN_CHECK_03	, 1, ValueChangedMainCheck03	, VTS_BOOL)
	ON_EVENT(CMainDialog, IDC_MAIN_CHECK_04	, 1, ValueChangedMainCheck04	, VTS_BOOL)
	ON_EVENT(CMainDialog, IDC_MAIN_CHECK_05	, 1, ValueChangedMainCheck05	, VTS_BOOL)
	ON_EVENT(CMainDialog, IDC_MAIN_CHECK_06	, 1, ValueChangedMainCheck06	, VTS_BOOL)
	ON_EVENT(CMainDialog, IDC_MAIN_CHECK_07	, 1, ValueChangedMainCheck07	, VTS_BOOL)
	ON_EVENT(CMainDialog, IDC_MAIN_CHECK_08	, 1, ValueChangedMainCheck08	, VTS_BOOL)
	ON_EVENT(CMainDialog, IDC_MAIN_CHECK_09	, 1, ValueChangedMainCheck09	, VTS_BOOL)
	ON_EVENT(CMainDialog, IDC_MAIN_CHECK_10	, 1, ValueChangedMainCheck10	, VTS_BOOL)
	ON_EVENT(CMainDialog, IDC_MAIN_CHECK_11	, 1, ValueChangedMainCheck11	, VTS_BOOL)
	ON_EVENT(CMainDialog, IDC_MAIN_CHECK_12	, 1, ValueChangedMainCheck12	, VTS_BOOL)
	ON_EVENT(CMainDialog, IDC_MAIN_CHECK_13	, 1, ValueChangedMainCheck13	, VTS_BOOL)
	ON_EVENT(CMainDialog, IDC_MAIN_CHECK_14	, 1, ValueChangedMainCheck14	, VTS_BOOL)
	ON_EVENT(CMainDialog, IDC_MAIN_CHECK_15	, 1, ValueChangedMainCheck15	, VTS_BOOL)
	ON_EVENT(CMainDialog, IDC_MAIN_CHECK_16	, 1, ValueChangedMainCheck16	, VTS_BOOL)
	ON_EVENT(CMainDialog, IDC_MAIN_CHECK_17	, 1, ValueChangedMainCheck17	, VTS_BOOL)
	ON_EVENT(CMainDialog, IDC_MAIN_CHECK_18	, 1, ValueChangedMainCheck18	, VTS_BOOL)
	ON_EVENT(CMainDialog, IDC_MAIN_CHECK_19	, 1, ValueChangedMainCheck19	, VTS_BOOL)
	ON_EVENT(CMainDialog, IDC_MAIN_CHECK_20	, 1, ValueChangedMainCheck20	, VTS_BOOL)
	ON_EVENT(CMainDialog, IDC_MAIN_CHECK_21	, 1, ValueChangedMainCheck21	, VTS_BOOL)
	ON_EVENT(CMainDialog, IDC_MAIN_CHECK_22	, 1, ValueChangedMainCheck22	, VTS_BOOL)
	ON_EVENT(CMainDialog, IDC_MAIN_CHECK_23	, 1, ValueChangedMainCheck23	, VTS_BOOL)
	ON_EVENT(CMainDialog, IDC_MAIN_CHECK_24	, 1, ValueChangedMainCheck24	, VTS_BOOL)
	ON_EVENT(CMainDialog, IDC_MAIN_CHECK_25	, 1, ValueChangedMainCheck25	, VTS_BOOL)
	ON_EVENT(CMainDialog, IDC_MAIN_CHECK_26	, 1, ValueChangedMainCheck26	, VTS_BOOL)
	ON_EVENT(CMainDialog, IDC_MAIN_CHECK_27	, 1, ValueChangedMainCheck27	, VTS_BOOL)
	ON_EVENT(CMainDialog, IDC_MAIN_CHECK_28	, 1, ValueChangedMainCheck28	, VTS_BOOL)
	ON_EVENT(CMainDialog, IDC_MAIN_CHECK_28	, 1, ValueChangedMainCheck29	, VTS_BOOL)
	ON_EVENT(CMainDialog, IDC_MAIN_CHECK_30	, 1, ValueChangedMainCheck30	, VTS_BOOL)
	ON_EVENT(CMainDialog, IDC_MAIN_CHECK_31	, 1, ValueChangedMainCheck31	, VTS_BOOL)
	ON_EVENT(CMainDialog, IDC_MAIN_CHECK_32	, 1, ValueChangedMainCheck32	, VTS_BOOL)
	ON_EVENT(CMainDialog, IDC_MAIN_CHECK_33	, 1, ValueChangedMainCheck33	, VTS_BOOL)
	ON_EVENT(CMainDialog, IDC_MAIN_CHECK_34	, 1, ValueChangedMainCheck34	, VTS_BOOL)
	ON_EVENT(CMainDialog, IDC_MAIN_CHECK_35	, 1, ValueChangedMainCheck35	, VTS_BOOL)
	ON_EVENT(CMainDialog, IDC_MAIN_CHECK_36	, 1, ValueChangedMainCheck36	, VTS_BOOL)
	ON_EVENT(CMainDialog, IDC_MAIN_CHECK_37	, 1, ValueChangedMainCheck37	, VTS_BOOL)
	ON_EVENT(CMainDialog, IDC_MAIN_CHECK_38	, 1, ValueChangedMainCheck38	, VTS_BOOL)
	ON_EVENT(CMainDialog, IDC_MAIN_CHECK_38	, 1, ValueChangedMainCheck39	, VTS_BOOL)
	ON_EVENT(CMainDialog, IDC_MAIN_CHECK_40	, 1, ValueChangedMainCheck40	, VTS_BOOL)
	ON_EVENT(CMainDialog, IDC_MAIN_CHECK_41	, 1, ValueChangedMainCheck41	, VTS_BOOL)
	ON_EVENT(CMainDialog, IDC_MAIN_CHECK_42	, 1, ValueChangedMainCheck42	, VTS_BOOL)
	ON_EVENT(CMainDialog, IDC_MAIN_CHECK_43	, 1, ValueChangedMainCheck43	, VTS_BOOL)
	ON_EVENT(CMainDialog, IDC_MAIN_CHECK_44	, 1, ValueChangedMainCheck44	, VTS_BOOL)
	ON_EVENT(CMainDialog, IDC_MAIN_CHECK_45	, 1, ValueChangedMainCheck45	, VTS_BOOL)
	ON_EVENT(CMainDialog, IDC_MAIN_CHECK_46	, 1, ValueChangedMainCheck46	, VTS_BOOL)
	ON_EVENT(CMainDialog, IDC_MAIN_CHECK_47	, 1, ValueChangedMainCheck47	, VTS_BOOL)
	ON_EVENT(CMainDialog, IDC_MAIN_CHECK_48	, 1, ValueChangedMainCheck48	, VTS_BOOL)
	ON_EVENT(CMainDialog, IDC_MAIN_CHECK_49	, 1, ValueChangedMainCheck49	, VTS_BOOL)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_01	, 1, ValueChangedMainEdit01		, VTS_PVARIANT VTS_PVARIANT VTS_BOOL)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_02	, 1, ValueChangedMainEdit02		, VTS_PVARIANT VTS_PVARIANT VTS_BOOL)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_03	, 1, ValueChangedMainEdit03		, VTS_PVARIANT VTS_PVARIANT VTS_BOOL)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_04	, 1, ValueChangedMainEdit04		, VTS_PVARIANT VTS_PVARIANT VTS_BOOL)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_05	, 1, ValueChangedMainEdit05		, VTS_PVARIANT VTS_PVARIANT VTS_BOOL)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_06	, 1, ValueChangedMainEdit06		, VTS_PVARIANT VTS_PVARIANT VTS_BOOL)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_07	, 1, ValueChangedMainEdit07		, VTS_PVARIANT VTS_PVARIANT VTS_BOOL)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_08	, 1, ValueChangedMainEdit08		, VTS_PVARIANT VTS_PVARIANT VTS_BOOL)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_09	, 1, ValueChangedMainEdit09		, VTS_PVARIANT VTS_PVARIANT VTS_BOOL)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_10	, 1, ValueChangedMainEdit10		, VTS_PVARIANT VTS_PVARIANT VTS_BOOL)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_11	, 1, ValueChangedMainEdit11		, VTS_PVARIANT VTS_PVARIANT VTS_BOOL)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_12	, 1, ValueChangedMainEdit12		, VTS_PVARIANT VTS_PVARIANT VTS_BOOL)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_13	, 1, ValueChangedMainEdit13		, VTS_PVARIANT VTS_PVARIANT VTS_BOOL)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_14	, 1, ValueChangedMainEdit14		, VTS_PVARIANT VTS_PVARIANT VTS_BOOL)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_15	, 1, ValueChangedMainEdit15		, VTS_PVARIANT VTS_PVARIANT VTS_BOOL)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_16	, 1, ValueChangedMainEdit16		, VTS_PVARIANT VTS_PVARIANT VTS_BOOL)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_17	, 1, ValueChangedMainEdit17		, VTS_PVARIANT VTS_PVARIANT VTS_BOOL)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_18	, 1, ValueChangedMainEdit18		, VTS_PVARIANT VTS_PVARIANT VTS_BOOL)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_19	, 1, ValueChangedMainEdit19		, VTS_PVARIANT VTS_PVARIANT VTS_BOOL)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_20	, 1, ValueChangedMainEdit20		, VTS_PVARIANT VTS_PVARIANT VTS_BOOL)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_21	, 1, ValueChangedMainEdit21		, VTS_PVARIANT VTS_PVARIANT VTS_BOOL)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_22	, 1, ValueChangedMainEdit22		, VTS_PVARIANT VTS_PVARIANT VTS_BOOL)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_23	, 1, ValueChangedMainEdit23		, VTS_PVARIANT VTS_PVARIANT VTS_BOOL)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_24	, 1, ValueChangedMainEdit24		, VTS_PVARIANT VTS_PVARIANT VTS_BOOL)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_25	, 1, ValueChangedMainEdit25		, VTS_PVARIANT VTS_PVARIANT VTS_BOOL)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_26	, 1, ValueChangedMainEdit26		, VTS_PVARIANT VTS_PVARIANT VTS_BOOL)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_27	, 1, ValueChangedMainEdit27		, VTS_PVARIANT VTS_PVARIANT VTS_BOOL)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_28	, 1, ValueChangedMainEdit28		, VTS_PVARIANT VTS_PVARIANT VTS_BOOL)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_29	, 1, ValueChangedMainEdit29		, VTS_PVARIANT VTS_PVARIANT VTS_BOOL)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_30	, 1, ValueChangedMainEdit30		, VTS_PVARIANT VTS_PVARIANT VTS_BOOL)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_31	, 1, ValueChangedMainEdit31		, VTS_PVARIANT VTS_PVARIANT VTS_BOOL)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_32	, 1, ValueChangedMainEdit32		, VTS_PVARIANT VTS_PVARIANT VTS_BOOL)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_33	, 1, ValueChangedMainEdit33		, VTS_PVARIANT VTS_PVARIANT VTS_BOOL)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_34	, 1, ValueChangedMainEdit34		, VTS_PVARIANT VTS_PVARIANT VTS_BOOL)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_35	, 1, ValueChangedMainEdit35		, VTS_PVARIANT VTS_PVARIANT VTS_BOOL)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_36	, 1, ValueChangedMainEdit36		, VTS_PVARIANT VTS_PVARIANT VTS_BOOL)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_37	, 1, ValueChangedMainEdit37		, VTS_PVARIANT VTS_PVARIANT VTS_BOOL)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_38	, 1, ValueChangedMainEdit38		, VTS_PVARIANT VTS_PVARIANT VTS_BOOL)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_39	, 1, ValueChangedMainEdit39		, VTS_PVARIANT VTS_PVARIANT VTS_BOOL)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_40	, 1, ValueChangedMainEdit40		, VTS_PVARIANT VTS_PVARIANT VTS_BOOL)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_41	, 1, ValueChangedMainEdit41		, VTS_PVARIANT VTS_PVARIANT VTS_BOOL)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_42	, 1, ValueChangedMainEdit42		, VTS_PVARIANT VTS_PVARIANT VTS_BOOL)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_43	, 1, ValueChangedMainEdit43		, VTS_PVARIANT VTS_PVARIANT VTS_BOOL)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_44	, 1, ValueChangedMainEdit44		, VTS_PVARIANT VTS_PVARIANT VTS_BOOL)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_45	, 1, ValueChangedMainEdit45		, VTS_PVARIANT VTS_PVARIANT VTS_BOOL)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_46	, 1, ValueChangedMainEdit46		, VTS_PVARIANT VTS_PVARIANT VTS_BOOL)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_47	, 1, ValueChangedMainEdit47		, VTS_PVARIANT VTS_PVARIANT VTS_BOOL)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_48	, 1, ValueChangedMainEdit48		, VTS_PVARIANT VTS_PVARIANT VTS_BOOL)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_49	, 1, ValueChangedMainEdit49		, VTS_PVARIANT VTS_PVARIANT VTS_BOOL)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_50	, 1, ValueChangedMainEdit50		, VTS_PVARIANT VTS_PVARIANT VTS_BOOL)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_51	, 1, ValueChangedMainEdit51		, VTS_PVARIANT VTS_PVARIANT VTS_BOOL)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_52	, 1, ValueChangedMainEdit52		, VTS_PVARIANT VTS_PVARIANT VTS_BOOL)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_53	, 1, ValueChangedMainEdit53		, VTS_PVARIANT VTS_PVARIANT VTS_BOOL)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_54	, 1, ValueChangedMainEdit54		, VTS_PVARIANT VTS_PVARIANT VTS_BOOL)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_55	, 1, ValueChangedMainEdit55		, VTS_PVARIANT VTS_PVARIANT VTS_BOOL)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_56	, 1, ValueChangedMainEdit56		, VTS_PVARIANT VTS_PVARIANT VTS_BOOL)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_57	, 1, ValueChangedMainEdit57		, VTS_PVARIANT VTS_PVARIANT VTS_BOOL)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_58	, 1, ValueChangedMainEdit58		, VTS_PVARIANT VTS_PVARIANT VTS_BOOL)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_59	, 1, ValueChangedMainEdit59		, VTS_PVARIANT VTS_PVARIANT VTS_BOOL)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_60	, 1, ValueChangedMainEdit60		, VTS_PVARIANT VTS_PVARIANT VTS_BOOL)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_61	, 1, ValueChangedMainEdit61		, VTS_PVARIANT VTS_PVARIANT VTS_BOOL)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_62	, 1, ValueChangedMainEdit62		, VTS_PVARIANT VTS_PVARIANT VTS_BOOL)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_63	, 1, ValueChangedMainEdit63		, VTS_PVARIANT VTS_PVARIANT VTS_BOOL)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_64	, 1, ValueChangedMainEdit64		, VTS_PVARIANT VTS_PVARIANT VTS_BOOL)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_65	, 1, ValueChangedMainEdit65		, VTS_PVARIANT VTS_PVARIANT VTS_BOOL)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_66	, 1, ValueChangedMainEdit66		, VTS_PVARIANT VTS_PVARIANT VTS_BOOL)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_67	, 1, ValueChangedMainEdit67		, VTS_PVARIANT VTS_PVARIANT VTS_BOOL)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_68	, 1, ValueChangedMainEdit68		, VTS_PVARIANT VTS_PVARIANT VTS_BOOL)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_69	, 1, ValueChangedMainEdit69		, VTS_PVARIANT VTS_PVARIANT VTS_BOOL)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_70	, 1, ValueChangedMainEdit70		, VTS_PVARIANT VTS_PVARIANT VTS_BOOL)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_71	, 1, ValueChangedMainEdit71		, VTS_PVARIANT VTS_PVARIANT VTS_BOOL)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_72	, 1, ValueChangedMainEdit72		, VTS_PVARIANT VTS_PVARIANT VTS_BOOL)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_73	, 1, ValueChangedMainEdit73		, VTS_PVARIANT VTS_PVARIANT VTS_BOOL)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_74	, 1, ValueChangedMainEdit74		, VTS_PVARIANT VTS_PVARIANT VTS_BOOL)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_75	, 1, ValueChangedMainEdit75		, VTS_PVARIANT VTS_PVARIANT VTS_BOOL)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_76	, 1, ValueChangedMainEdit76		, VTS_PVARIANT VTS_PVARIANT VTS_BOOL)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_77	, 1, ValueChangedMainEdit77		, VTS_PVARIANT VTS_PVARIANT VTS_BOOL)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_78	, 1, ValueChangedMainEdit78		, VTS_PVARIANT VTS_PVARIANT VTS_BOOL)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_79	, 1, ValueChangedMainEdit79		, VTS_PVARIANT VTS_PVARIANT VTS_BOOL)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_80	, 1, ValueChangedMainEdit80		, VTS_PVARIANT VTS_PVARIANT VTS_BOOL)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_81	, 1, ValueChangedMainEdit81		, VTS_PVARIANT VTS_PVARIANT VTS_BOOL)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_82	, 1, ValueChangedMainEdit82		, VTS_PVARIANT VTS_PVARIANT VTS_BOOL)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_83	, 1, ValueChangedMainEdit83		, VTS_PVARIANT VTS_PVARIANT VTS_BOOL)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_84	, 1, ValueChangedMainEdit84		, VTS_PVARIANT VTS_PVARIANT VTS_BOOL)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_85	, 1, ValueChangedMainEdit85		, VTS_PVARIANT VTS_PVARIANT VTS_BOOL)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_86	, 1, ValueChangedMainEdit86		, VTS_PVARIANT VTS_PVARIANT VTS_BOOL)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_87	, 1, ValueChangedMainEdit87		, VTS_PVARIANT VTS_PVARIANT VTS_BOOL)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_88	, 1, ValueChangedMainEdit88		, VTS_PVARIANT VTS_PVARIANT VTS_BOOL)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_89	, 1, ValueChangedMainEdit89		, VTS_PVARIANT VTS_PVARIANT VTS_BOOL)
	
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_01	, DISPID_MOUSEUP, MouseUpMainEdit01		, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_02	, DISPID_MOUSEUP, MouseUpMainEdit02		, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_03	, DISPID_MOUSEUP, MouseUpMainEdit03		, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_04	, DISPID_MOUSEUP, MouseUpMainEdit04		, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_05	, DISPID_MOUSEUP, MouseUpMainEdit05		, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_06	, DISPID_MOUSEUP, MouseUpMainEdit06		, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_07	, DISPID_MOUSEUP, MouseUpMainEdit07		, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_08	, DISPID_MOUSEUP, MouseUpMainEdit08		, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_09	, DISPID_MOUSEUP, MouseUpMainEdit09		, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_10	, DISPID_MOUSEUP, MouseUpMainEdit10		, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_11	, DISPID_MOUSEUP, MouseUpMainEdit11		, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_12	, DISPID_MOUSEUP, MouseUpMainEdit12		, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_13	, DISPID_MOUSEUP, MouseUpMainEdit13		, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_14	, DISPID_MOUSEUP, MouseUpMainEdit14		, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_15	, DISPID_MOUSEUP, MouseUpMainEdit15		, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_16	, DISPID_MOUSEUP, MouseUpMainEdit16		, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_17	, DISPID_MOUSEUP, MouseUpMainEdit17		, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_18	, DISPID_MOUSEUP, MouseUpMainEdit18		, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_19	, DISPID_MOUSEUP, MouseUpMainEdit19		, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_20	, DISPID_MOUSEUP, MouseUpMainEdit20		, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_21	, DISPID_MOUSEUP, MouseUpMainEdit21		, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_22	, DISPID_MOUSEUP, MouseUpMainEdit22		, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_23	, DISPID_MOUSEUP, MouseUpMainEdit23		, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_24	, DISPID_MOUSEUP, MouseUpMainEdit24		, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_25	, DISPID_MOUSEUP, MouseUpMainEdit25		, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_26	, DISPID_MOUSEUP, MouseUpMainEdit26		, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_27	, DISPID_MOUSEUP, MouseUpMainEdit27		, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_28	, DISPID_MOUSEUP, MouseUpMainEdit28		, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_29	, DISPID_MOUSEUP, MouseUpMainEdit29		, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_30	, DISPID_MOUSEUP, MouseUpMainEdit30		, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_31	, DISPID_MOUSEUP, MouseUpMainEdit31		, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_32	, DISPID_MOUSEUP, MouseUpMainEdit32		, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_33	, DISPID_MOUSEUP, MouseUpMainEdit33		, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_34	, DISPID_MOUSEUP, MouseUpMainEdit34		, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_35	, DISPID_MOUSEUP, MouseUpMainEdit35		, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_36	, DISPID_MOUSEUP, MouseUpMainEdit36		, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_37	, DISPID_MOUSEUP, MouseUpMainEdit37		, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_38	, DISPID_MOUSEUP, MouseUpMainEdit38		, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_39	, DISPID_MOUSEUP, MouseUpMainEdit39		, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_40	, DISPID_MOUSEUP, MouseUpMainEdit40		, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_41	, DISPID_MOUSEUP, MouseUpMainEdit41		, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_42	, DISPID_MOUSEUP, MouseUpMainEdit42		, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_43	, DISPID_MOUSEUP, MouseUpMainEdit43		, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_44	, DISPID_MOUSEUP, MouseUpMainEdit44		, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_45	, DISPID_MOUSEUP, MouseUpMainEdit45		, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_46	, DISPID_MOUSEUP, MouseUpMainEdit46		, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_47	, DISPID_MOUSEUP, MouseUpMainEdit47		, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_48	, DISPID_MOUSEUP, MouseUpMainEdit48		, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_49	, DISPID_MOUSEUP, MouseUpMainEdit49		, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_50	, DISPID_MOUSEUP, MouseUpMainEdit50		, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_51	, DISPID_MOUSEUP, MouseUpMainEdit51		, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_52	, DISPID_MOUSEUP, MouseUpMainEdit52		, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_53	, DISPID_MOUSEUP, MouseUpMainEdit53		, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_54	, DISPID_MOUSEUP, MouseUpMainEdit54		, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_55	, DISPID_MOUSEUP, MouseUpMainEdit55		, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_56	, DISPID_MOUSEUP, MouseUpMainEdit56		, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_57	, DISPID_MOUSEUP, MouseUpMainEdit57		, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_58	, DISPID_MOUSEUP, MouseUpMainEdit58		, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_59	, DISPID_MOUSEUP, MouseUpMainEdit59		, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_60	, DISPID_MOUSEUP, MouseUpMainEdit60		, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_61	, DISPID_MOUSEUP, MouseUpMainEdit61		, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_62	, DISPID_MOUSEUP, MouseUpMainEdit62		, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_63	, DISPID_MOUSEUP, MouseUpMainEdit63		, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_64	, DISPID_MOUSEUP, MouseUpMainEdit64		, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_65	, DISPID_MOUSEUP, MouseUpMainEdit65		, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_66	, DISPID_MOUSEUP, MouseUpMainEdit66		, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_67	, DISPID_MOUSEUP, MouseUpMainEdit67		, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_68	, DISPID_MOUSEUP, MouseUpMainEdit68		, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_69	, DISPID_MOUSEUP, MouseUpMainEdit69		, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_70	, DISPID_MOUSEUP, MouseUpMainEdit70		, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_71	, DISPID_MOUSEUP, MouseUpMainEdit71		, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_72	, DISPID_MOUSEUP, MouseUpMainEdit72		, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_73	, DISPID_MOUSEUP, MouseUpMainEdit73		, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_74	, DISPID_MOUSEUP, MouseUpMainEdit74		, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_75	, DISPID_MOUSEUP, MouseUpMainEdit75		, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_76	, DISPID_MOUSEUP, MouseUpMainEdit76		, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_77	, DISPID_MOUSEUP, MouseUpMainEdit77		, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_78	, DISPID_MOUSEUP, MouseUpMainEdit78		, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_79	, DISPID_MOUSEUP, MouseUpMainEdit79		, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_80	, DISPID_MOUSEUP, MouseUpMainEdit80		, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_81	, DISPID_MOUSEUP, MouseUpMainEdit81		, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_82	, DISPID_MOUSEUP, MouseUpMainEdit82		, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_83	, DISPID_MOUSEUP, MouseUpMainEdit83		, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_84	, DISPID_MOUSEUP, MouseUpMainEdit84		, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_85	, DISPID_MOUSEUP, MouseUpMainEdit85		, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_86	, DISPID_MOUSEUP, MouseUpMainEdit86		, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_87	, DISPID_MOUSEUP, MouseUpMainEdit87		, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_88	, DISPID_MOUSEUP, MouseUpMainEdit88		, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CMainDialog, IDC_MAIN_EDIT_89	, DISPID_MOUSEUP, MouseUpMainEdit89		, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
/**/
	ON_EVENT(CMainDialog, IDC_MAIN_CHECK_VAR_00, 1, ValueChangedMainCheckVar00, VTS_BOOL)
	ON_EVENT(CMainDialog, IDC_MAIN_CHECK_VAR_01, 1, ValueChangedMainCheckVar01, VTS_BOOL)
	ON_EVENT(CMainDialog, IDC_MAIN_CHECK_VAR_02, 1, ValueChangedMainCheckVar02, VTS_BOOL)
	ON_EVENT(CMainDialog, IDC_MAIN_CHECK_VAR_03, 1, ValueChangedMainCheckVar03, VTS_BOOL)
	ON_EVENT(CMainDialog, IDC_MAIN_CHECK_VAR_04, 1, ValueChangedMainCheckVar04, VTS_BOOL)
	ON_EVENT(CMainDialog, IDC_MAIN_CHECK_VAR_05, 1, ValueChangedMainCheckVar05, VTS_BOOL)
/**/
ON_EVENT(CMainDialog, IDC_MAIN_CHECK_16, DISPID_DBLCLICK, DblClickMainCheck16, VTS_NONE)
END_EVENTSINK_MAP()

/////////////////////////////////////////////////////////////////////////////////////////////////
//						Bouttons Check de la séquence
/////////////////////////////////////////////////////////////////////////////////////////////////

void CMainDialog::ValueChangedMainCheck01(BOOL Value){	m_ParamBool[ 1]	=	Value ? true : false;	if(m_bUpdateAuto) DrawSeq();	}
void CMainDialog::ValueChangedMainCheck02(BOOL Value){	m_ParamBool[ 2]	=	Value ? true : false;	if(m_bUpdateAuto) DrawSeq();	}
void CMainDialog::ValueChangedMainCheck03(BOOL Value){	m_ParamBool[ 3]	=	Value ? true : false;	if(m_bUpdateAuto) DrawSeq();	}
void CMainDialog::ValueChangedMainCheck04(BOOL Value){	m_ParamBool[ 4]	=	Value ? true : false;	if(m_bUpdateAuto) DrawSeq();	}
void CMainDialog::ValueChangedMainCheck05(BOOL Value){	m_ParamBool[ 5]	=	Value ? true : false;	if(m_bUpdateAuto) DrawSeq();	}
void CMainDialog::ValueChangedMainCheck06(BOOL Value){	m_ParamBool[ 6]	=	Value ? true : false;	if(m_bUpdateAuto) DrawSeq();	}
void CMainDialog::ValueChangedMainCheck07(BOOL Value){	m_ParamBool[ 7]	=	Value ? true : false;	if(m_bUpdateAuto) DrawSeq();	}
void CMainDialog::ValueChangedMainCheck08(BOOL Value){	m_ParamBool[ 8]	=	Value ? true : false;	if(m_bUpdateAuto) DrawSeq();	}
void CMainDialog::ValueChangedMainCheck09(BOOL Value){	m_ParamBool[ 9]	=	Value ? true : false;	if(m_bUpdateAuto) DrawSeq();	}
void CMainDialog::ValueChangedMainCheck10(BOOL Value){	m_ParamBool[10]	= 	Value ? true : false;	if(m_bUpdateAuto) DrawSeq();	}
void CMainDialog::ValueChangedMainCheck11(BOOL Value){	m_ParamBool[11]	= 	Value ? true : false;	if(m_bUpdateAuto) DrawSeq();	}
void CMainDialog::ValueChangedMainCheck12(BOOL Value){	m_ParamBool[12]	= 	Value ? true : false;	if(m_bUpdateAuto) DrawSeq();	}
void CMainDialog::ValueChangedMainCheck13(BOOL Value){	m_ParamBool[13]	= 	Value ? true : false;	if(m_bUpdateAuto) DrawSeq();	}
void CMainDialog::ValueChangedMainCheck14(BOOL Value){	m_ParamBool[14]	=	Value ? true : false;	if(m_bUpdateAuto) DrawSeq();	}
void CMainDialog::ValueChangedMainCheck15(BOOL Value){	m_ParamBool[15]	= 	Value ? true : false;	if(m_bUpdateAuto) DrawSeq();	}
void CMainDialog::ValueChangedMainCheck16(BOOL Value){	m_ParamBool[16]	=	Value ? true : false;	if(m_bUpdateAuto) DrawSeq();	CheckDlgButton(	IDC_MAIN_CHECK_VAR_00,m_ParamBool[16]);}
void CMainDialog::ValueChangedMainCheck17(BOOL Value){	m_ParamBool[17]	=	Value ? true : false;	if(m_bUpdateAuto) DrawSeq();	}
void CMainDialog::ValueChangedMainCheck18(BOOL Value){	m_ParamBool[18]	=	Value ? true : false;	if(m_bUpdateAuto) DrawSeq();	}
void CMainDialog::ValueChangedMainCheck19(BOOL Value){	m_ParamBool[19]	=	Value ? true : false;	if(m_bUpdateAuto) DrawSeq();	}
void CMainDialog::ValueChangedMainCheck20(BOOL Value){	m_ParamBool[20]	=	Value ? true : false;	if(m_bUpdateAuto) DrawSeq();	}
void CMainDialog::ValueChangedMainCheck21(BOOL Value){	m_ParamBool[21]	= 	Value ? true : false;	if(m_bUpdateAuto) DrawSeq();	}
void CMainDialog::ValueChangedMainCheck22(BOOL Value){	m_ParamBool[22]	= 	Value ? true : false;	if(m_bUpdateAuto) DrawSeq();	}
void CMainDialog::ValueChangedMainCheck23(BOOL Value){	m_ParamBool[23]	= 	Value ? true : false;	if(m_bUpdateAuto) DrawSeq();	}
void CMainDialog::ValueChangedMainCheck24(BOOL Value){	m_ParamBool[24]	=	Value ? true : false;	if(m_bUpdateAuto) DrawSeq();	}
void CMainDialog::ValueChangedMainCheck25(BOOL Value){	m_ParamBool[25]	= 	Value ? true : false;	if(m_bUpdateAuto) DrawSeq();	}
void CMainDialog::ValueChangedMainCheck26(BOOL Value){	m_ParamBool[26]	=	Value ? true : false;	if(m_bUpdateAuto) DrawSeq();	}
void CMainDialog::ValueChangedMainCheck27(BOOL Value){	m_ParamBool[27]	=	Value ? true : false;	if(m_bUpdateAuto) DrawSeq();	}
void CMainDialog::ValueChangedMainCheck28(BOOL Value){	m_ParamBool[28]	=	Value ? true : false;	if(m_bUpdateAuto) DrawSeq();	}
void CMainDialog::ValueChangedMainCheck29(BOOL Value){	m_ParamBool[29]	=	Value ? true : false;	if(m_bUpdateAuto) DrawSeq();	}
void CMainDialog::ValueChangedMainCheck30(BOOL Value){	m_ParamBool[30]	=	Value ? true : false;	if(m_bUpdateAuto) DrawSeq();	}
void CMainDialog::ValueChangedMainCheck31(BOOL Value){	m_ParamBool[31]	= 	Value ? true : false;	if(m_bUpdateAuto) DrawSeq();	}
void CMainDialog::ValueChangedMainCheck32(BOOL Value){	m_ParamBool[32]	= 	Value ? true : false;	if(m_bUpdateAuto) DrawSeq();	}
void CMainDialog::ValueChangedMainCheck33(BOOL Value){	m_ParamBool[33]	= 	Value ? true : false;	if(m_bUpdateAuto) DrawSeq();	}
void CMainDialog::ValueChangedMainCheck34(BOOL Value){	m_ParamBool[34]	=	Value ? true : false;	if(m_bUpdateAuto) DrawSeq();	}
void CMainDialog::ValueChangedMainCheck35(BOOL Value){	m_ParamBool[35]	= 	Value ? true : false;	if(m_bUpdateAuto) DrawSeq();	}
void CMainDialog::ValueChangedMainCheck36(BOOL Value){	m_ParamBool[36]	=	Value ? true : false;	if(m_bUpdateAuto) DrawSeq();	}
void CMainDialog::ValueChangedMainCheck37(BOOL Value){	m_ParamBool[37]	=	Value ? true : false;	if(m_bUpdateAuto) DrawSeq();	}
void CMainDialog::ValueChangedMainCheck38(BOOL Value){	m_ParamBool[38]	=	Value ? true : false;	if(m_bUpdateAuto) DrawSeq();	}
void CMainDialog::ValueChangedMainCheck39(BOOL Value){	m_ParamBool[39]	=	Value ? true : false;	if(m_bUpdateAuto) DrawSeq();	}
void CMainDialog::ValueChangedMainCheck40(BOOL Value){	m_ParamBool[40]	=	Value ? true : false;	if(m_bUpdateAuto) DrawSeq();	}
void CMainDialog::ValueChangedMainCheck41(BOOL Value){	m_ParamBool[41]	= 	Value ? true : false;	if(m_bUpdateAuto) DrawSeq();	}
void CMainDialog::ValueChangedMainCheck42(BOOL Value){	m_ParamBool[42]	= 	Value ? true : false;	if(m_bUpdateAuto) DrawSeq();	}
void CMainDialog::ValueChangedMainCheck43(BOOL Value){	m_ParamBool[43]	= 	Value ? true : false;	if(m_bUpdateAuto) DrawSeq();	}
void CMainDialog::ValueChangedMainCheck44(BOOL Value){	m_ParamBool[44]	=	Value ? true : false;	if(m_bUpdateAuto) DrawSeq();	}
void CMainDialog::ValueChangedMainCheck45(BOOL Value){	m_ParamBool[45]	= 	Value ? true : false;	if(m_bUpdateAuto) DrawSeq();	}
void CMainDialog::ValueChangedMainCheck46(BOOL Value){	m_ParamBool[46]	=	Value ? true : false;	if(m_bUpdateAuto) DrawSeq();	}
void CMainDialog::ValueChangedMainCheck47(BOOL Value){	m_ParamBool[47]	=	Value ? true : false;	if(m_bUpdateAuto) DrawSeq();	}
void CMainDialog::ValueChangedMainCheck48(BOOL Value){	m_ParamBool[48]	=	Value ? true : false;	if(m_bUpdateAuto) DrawSeq();	}
void CMainDialog::ValueChangedMainCheck49(BOOL Value){	m_ParamBool[49]	=	Value ? true : false;	if(m_bUpdateAuto) DrawSeq();	}

/////////////////////////////////////////////////////////////////////////////////////////////////
//						Les EditBox ActiveX de National Instruments 
/////////////////////////////////////////////////////////////////////////////////////////////////
void CMainDialog::ValueChangedMainEdit01(BLABLA00){ m_ParamReal[ 1]	=	Value->dblVal;	if(m_bUpdateAuto) DrawSeq();	}
void CMainDialog::ValueChangedMainEdit02(BLABLA00){ m_ParamReal[ 2]	=	Value->dblVal;	if(m_bUpdateAuto) DrawSeq();	}
void CMainDialog::ValueChangedMainEdit03(BLABLA00){ m_ParamReal[ 3]	=	Value->dblVal;	if(m_bUpdateAuto) DrawSeq();	}
void CMainDialog::ValueChangedMainEdit04(BLABLA00){ m_ParamReal[ 4]	=	Value->dblVal;	if(m_bUpdateAuto) DrawSeq();	}
void CMainDialog::ValueChangedMainEdit05(BLABLA00){ m_ParamReal[ 5]	=	Value->dblVal;	if(m_bUpdateAuto) DrawSeq();	}
void CMainDialog::ValueChangedMainEdit06(BLABLA00){ m_ParamReal[ 6]	=	Value->dblVal;	if(m_bUpdateAuto) DrawSeq();	}
void CMainDialog::ValueChangedMainEdit07(BLABLA00){ m_ParamReal[ 7]	=	Value->dblVal;	if(m_bUpdateAuto) DrawSeq();	}
void CMainDialog::ValueChangedMainEdit08(BLABLA00){ m_ParamReal[ 8]	=	Value->dblVal;	if(m_bUpdateAuto) DrawSeq();	}
void CMainDialog::ValueChangedMainEdit09(BLABLA00){ m_ParamReal[ 9]	=	Value->dblVal;	if(m_bUpdateAuto) DrawSeq();	}
void CMainDialog::ValueChangedMainEdit10(BLABLA00){ m_ParamReal[10]	=	Value->dblVal;	if(m_bUpdateAuto) DrawSeq();	}
void CMainDialog::ValueChangedMainEdit11(BLABLA00){ m_ParamReal[11]	=	Value->dblVal;	if(m_bUpdateAuto) DrawSeq();	}
void CMainDialog::ValueChangedMainEdit12(BLABLA00){ m_ParamReal[12]	=	Value->dblVal;	if(m_bUpdateAuto) DrawSeq();	}
void CMainDialog::ValueChangedMainEdit13(BLABLA00){ m_ParamReal[13]	=	Value->dblVal;	if(m_bUpdateAuto) DrawSeq();	}
void CMainDialog::ValueChangedMainEdit14(BLABLA00){ m_ParamReal[14]	=	Value->dblVal;	if(m_bUpdateAuto) DrawSeq();	}
void CMainDialog::ValueChangedMainEdit15(BLABLA00){ m_ParamReal[15]	=	Value->dblVal;	if(m_bUpdateAuto) DrawSeq();	}
void CMainDialog::ValueChangedMainEdit16(BLABLA00){ m_ParamReal[16]	=	Value->dblVal;	if(m_bUpdateAuto) DrawSeq();	}
void CMainDialog::ValueChangedMainEdit17(BLABLA00){ m_ParamReal[17]	=	Value->dblVal;	if(m_bUpdateAuto) DrawSeq();	}
void CMainDialog::ValueChangedMainEdit18(BLABLA00){ m_ParamReal[18]	=	Value->dblVal;	if(m_bUpdateAuto) DrawSeq();	}
void CMainDialog::ValueChangedMainEdit19(BLABLA00){ m_ParamReal[19]	=	Value->dblVal;	if(m_bUpdateAuto) DrawSeq();	}
void CMainDialog::ValueChangedMainEdit20(BLABLA00){ m_ParamReal[20]	=	Value->dblVal;	if(m_bUpdateAuto) DrawSeq();	}
void CMainDialog::ValueChangedMainEdit21(BLABLA00){ m_ParamReal[21]	=	Value->dblVal;	if(m_bUpdateAuto) DrawSeq();	}
void CMainDialog::ValueChangedMainEdit22(BLABLA00){ m_ParamReal[22]	=	Value->dblVal;	if(m_bUpdateAuto) DrawSeq();	}
void CMainDialog::ValueChangedMainEdit23(BLABLA00){ m_ParamReal[23]	=	Value->dblVal;	if(m_bUpdateAuto) DrawSeq();	}
void CMainDialog::ValueChangedMainEdit24(BLABLA00){ m_ParamReal[24]	=	Value->dblVal;	if(m_bUpdateAuto) DrawSeq();	}
void CMainDialog::ValueChangedMainEdit25(BLABLA00){ m_ParamReal[25]	=	Value->dblVal;	if(m_bUpdateAuto) DrawSeq();	}
void CMainDialog::ValueChangedMainEdit26(BLABLA00){ m_ParamReal[26]	=	Value->dblVal;	if(m_bUpdateAuto) DrawSeq();	}
void CMainDialog::ValueChangedMainEdit27(BLABLA00){ m_ParamReal[27]	=	Value->dblVal;	if(m_bUpdateAuto) DrawSeq();	}
void CMainDialog::ValueChangedMainEdit28(BLABLA00){ m_ParamReal[28]	=	Value->dblVal;	if(m_bUpdateAuto) DrawSeq();	}
void CMainDialog::ValueChangedMainEdit29(BLABLA00){ m_ParamReal[29]	=	Value->dblVal;	if(m_bUpdateAuto) DrawSeq();	}
void CMainDialog::ValueChangedMainEdit30(BLABLA00){ m_ParamReal[30]	=	Value->dblVal;	if(m_bUpdateAuto) DrawSeq();	}
void CMainDialog::ValueChangedMainEdit31(BLABLA00){ m_ParamReal[31]	=	Value->dblVal;	if(m_bUpdateAuto) DrawSeq();	}
void CMainDialog::ValueChangedMainEdit32(BLABLA00){ m_ParamReal[32]	=	Value->dblVal;	if(m_bUpdateAuto) DrawSeq();	}
void CMainDialog::ValueChangedMainEdit33(BLABLA00){ m_ParamReal[33]	=	Value->dblVal;	if(m_bUpdateAuto) DrawSeq();	}
void CMainDialog::ValueChangedMainEdit34(BLABLA00){ m_ParamReal[34]	=	Value->dblVal;	if(m_bUpdateAuto) DrawSeq();	}
void CMainDialog::ValueChangedMainEdit35(BLABLA00){ m_ParamReal[35]	=	Value->dblVal;	if(m_bUpdateAuto) DrawSeq();	}
void CMainDialog::ValueChangedMainEdit36(BLABLA00){ m_ParamReal[36]	=	Value->dblVal;	if(m_bUpdateAuto) DrawSeq();	}
void CMainDialog::ValueChangedMainEdit37(BLABLA00){ m_ParamReal[37]	=	Value->dblVal;	if(m_bUpdateAuto) DrawSeq();	}
void CMainDialog::ValueChangedMainEdit38(BLABLA00){ m_ParamReal[38]	=	Value->dblVal;	if(m_bUpdateAuto) DrawSeq();	}
void CMainDialog::ValueChangedMainEdit39(BLABLA00){ m_ParamReal[39]	=	Value->dblVal;	if(m_bUpdateAuto) DrawSeq();	}
void CMainDialog::ValueChangedMainEdit40(BLABLA00){ m_ParamReal[40]	=	Value->dblVal;	if(m_bUpdateAuto) DrawSeq();	}
void CMainDialog::ValueChangedMainEdit41(BLABLA00){ m_ParamReal[41]	=	Value->dblVal;	if(m_bUpdateAuto) DrawSeq();	}
void CMainDialog::ValueChangedMainEdit42(BLABLA00){ m_ParamReal[42]	=	Value->dblVal;	if(m_bUpdateAuto) DrawSeq();	}
void CMainDialog::ValueChangedMainEdit43(BLABLA00){ m_ParamReal[43]	=	Value->dblVal;	if(m_bUpdateAuto) DrawSeq();	}
void CMainDialog::ValueChangedMainEdit44(BLABLA00){ m_ParamReal[44]	=	Value->dblVal;	if(m_bUpdateAuto) DrawSeq();	}
void CMainDialog::ValueChangedMainEdit45(BLABLA00){ m_ParamReal[45]	=	Value->dblVal;	if(m_bUpdateAuto) DrawSeq();	}
void CMainDialog::ValueChangedMainEdit46(BLABLA00){ m_ParamReal[46]	=	Value->dblVal;	if(m_bUpdateAuto) DrawSeq();	}
void CMainDialog::ValueChangedMainEdit47(BLABLA00){ m_ParamReal[47]	=	Value->dblVal;	if(m_bUpdateAuto) DrawSeq();	}
void CMainDialog::ValueChangedMainEdit48(BLABLA00){ m_ParamReal[48]	=	Value->dblVal;	if(m_bUpdateAuto) DrawSeq();	}
void CMainDialog::ValueChangedMainEdit49(BLABLA00){ m_ParamReal[49]	=	Value->dblVal;	if(m_bUpdateAuto) DrawSeq();	}
void CMainDialog::ValueChangedMainEdit50(BLABLA00){ m_ParamReal[50]	=	Value->dblVal;	if(m_bUpdateAuto) DrawSeq();	}
void CMainDialog::ValueChangedMainEdit51(BLABLA00){ m_ParamReal[51]	=	Value->dblVal;	if(m_bUpdateAuto) DrawSeq();	}
void CMainDialog::ValueChangedMainEdit52(BLABLA00){ m_ParamReal[52]	=	Value->dblVal;	if(m_bUpdateAuto) DrawSeq();	}
void CMainDialog::ValueChangedMainEdit53(BLABLA00){ m_ParamReal[53]	=	Value->dblVal;	if(m_bUpdateAuto) DrawSeq();	}
void CMainDialog::ValueChangedMainEdit54(BLABLA00){ m_ParamReal[54]	=	Value->dblVal;	if(m_bUpdateAuto) DrawSeq();	}
void CMainDialog::ValueChangedMainEdit55(BLABLA00){ m_ParamReal[55]	=	Value->dblVal;	if(m_bUpdateAuto) DrawSeq();	}
void CMainDialog::ValueChangedMainEdit56(BLABLA00){ m_ParamReal[56]	=	Value->dblVal;	if(m_bUpdateAuto) DrawSeq();	}
void CMainDialog::ValueChangedMainEdit57(BLABLA00){ m_ParamReal[57]	=	Value->dblVal;	if(m_bUpdateAuto) DrawSeq();	}
void CMainDialog::ValueChangedMainEdit58(BLABLA00){ m_ParamReal[58]	=	Value->dblVal;	if(m_bUpdateAuto) DrawSeq();	}
void CMainDialog::ValueChangedMainEdit59(BLABLA00){ m_ParamReal[59]	=	Value->dblVal;	if(m_bUpdateAuto) DrawSeq();	}
void CMainDialog::ValueChangedMainEdit60(BLABLA00){ m_ParamReal[60]	=	Value->dblVal;	if(m_bUpdateAuto) DrawSeq();	}
void CMainDialog::ValueChangedMainEdit61(BLABLA00){ m_ParamReal[61]	=	Value->dblVal;	if(m_bUpdateAuto) DrawSeq();	}
void CMainDialog::ValueChangedMainEdit62(BLABLA00){ m_ParamReal[62]	=	Value->dblVal;	if(m_bUpdateAuto) DrawSeq();	}
void CMainDialog::ValueChangedMainEdit63(BLABLA00){ m_ParamReal[63]	=	Value->dblVal;	if(m_bUpdateAuto) DrawSeq();	}
void CMainDialog::ValueChangedMainEdit64(BLABLA00){ m_ParamReal[64]	=	Value->dblVal;	if(m_bUpdateAuto) DrawSeq();	}
void CMainDialog::ValueChangedMainEdit65(BLABLA00){ m_ParamReal[65]	=	Value->dblVal;	if(m_bUpdateAuto) DrawSeq();	}
void CMainDialog::ValueChangedMainEdit66(BLABLA00){ m_ParamReal[66]	=	Value->dblVal;	if(m_bUpdateAuto) DrawSeq();	}
void CMainDialog::ValueChangedMainEdit67(BLABLA00){ m_ParamReal[67]	=	Value->dblVal;	if(m_bUpdateAuto) DrawSeq();	}
void CMainDialog::ValueChangedMainEdit68(BLABLA00){ m_ParamReal[68]	=	Value->dblVal;	if(m_bUpdateAuto) DrawSeq();	}
void CMainDialog::ValueChangedMainEdit69(BLABLA00){ m_ParamReal[69]	=	Value->dblVal;	if(m_bUpdateAuto) DrawSeq();	}
void CMainDialog::ValueChangedMainEdit70(BLABLA00){ m_ParamReal[70]	=	Value->dblVal;	if(m_bUpdateAuto) DrawSeq();	}
void CMainDialog::ValueChangedMainEdit71(BLABLA00){ m_ParamReal[71]	=	Value->dblVal;	if(m_bUpdateAuto) DrawSeq();	}
void CMainDialog::ValueChangedMainEdit72(BLABLA00){ m_ParamReal[72]	=	Value->dblVal;	if(m_bUpdateAuto) DrawSeq();	}
void CMainDialog::ValueChangedMainEdit73(BLABLA00){ m_ParamReal[73]	=	Value->dblVal;	if(m_bUpdateAuto) DrawSeq();	}
void CMainDialog::ValueChangedMainEdit74(BLABLA00){ m_ParamReal[74]	=	Value->dblVal;	if(m_bUpdateAuto) DrawSeq();	}
void CMainDialog::ValueChangedMainEdit75(BLABLA00){ m_ParamReal[75]	=	Value->dblVal;	if(m_bUpdateAuto) DrawSeq();	}
void CMainDialog::ValueChangedMainEdit76(BLABLA00){ m_ParamReal[76]	=	Value->dblVal;	if(m_bUpdateAuto) DrawSeq();	}
void CMainDialog::ValueChangedMainEdit77(BLABLA00){ m_ParamReal[77]	=	Value->dblVal;	if(m_bUpdateAuto) DrawSeq();	}
void CMainDialog::ValueChangedMainEdit78(BLABLA00){ m_ParamReal[78]	=	Value->dblVal;	if(m_bUpdateAuto) DrawSeq();	}
void CMainDialog::ValueChangedMainEdit79(BLABLA00){ m_ParamReal[79]	=	Value->dblVal;	if(m_bUpdateAuto) DrawSeq();	}
void CMainDialog::ValueChangedMainEdit80(BLABLA00){ m_ParamReal[80]	=	Value->dblVal;	if(m_bUpdateAuto) DrawSeq();	}
void CMainDialog::ValueChangedMainEdit81(BLABLA00){ m_ParamReal[81]	=	Value->dblVal;	if(m_bUpdateAuto) DrawSeq();	}
void CMainDialog::ValueChangedMainEdit82(BLABLA00){ m_ParamReal[82]	=	Value->dblVal;	if(m_bUpdateAuto) DrawSeq();	}
void CMainDialog::ValueChangedMainEdit83(BLABLA00){ m_ParamReal[83]	=	Value->dblVal;	if(m_bUpdateAuto) DrawSeq();	}
void CMainDialog::ValueChangedMainEdit84(BLABLA00){ m_ParamReal[84]	=	Value->dblVal;	if(m_bUpdateAuto) DrawSeq();	}
void CMainDialog::ValueChangedMainEdit85(BLABLA00){ m_ParamReal[85]	=	Value->dblVal;	if(m_bUpdateAuto) DrawSeq();	}
void CMainDialog::ValueChangedMainEdit86(BLABLA00){ m_ParamReal[86]	=	Value->dblVal;	if(m_bUpdateAuto) DrawSeq();	}
void CMainDialog::ValueChangedMainEdit87(BLABLA00){ m_ParamReal[87]	=	Value->dblVal;	if(m_bUpdateAuto) DrawSeq();	}
void CMainDialog::ValueChangedMainEdit88(BLABLA00){ m_ParamReal[88]	=	Value->dblVal;	if(m_bUpdateAuto) DrawSeq();	}
void CMainDialog::ValueChangedMainEdit89(BLABLA00){ m_ParamReal[89]	=	Value->dblVal;	if(m_bUpdateAuto) DrawSeq();	}


/*
Update tous les controles de la MainDialog au cas ou on les aurait changés programmaticaly
Renvoye...?
*/
int CMainDialog::UpdateControls()
{
	CheckDlgButton(	IDC_MAIN_EDIT_WAIT_BEFORE_LOAD		,	m_WaitBeforeLoad			);		
	CheckDlgButton(	IDC_MAIN_CHECK_WAIT_TRIGGER			,	m_WaitTrigger				);		
	CheckDlgButton(	IDC_MAIN_CHECK_VAR_00				,	m_bVarieActive				);		

	for(int iemeParamBool = 1; iemeParamBool < NOMBRE_NI_BUTTON; iemeParamBool++)
		m_NiCheck[iemeParamBool].SetValue(bool(m_ParamBool[iemeParamBool]));
	for(int iemeParamInt = 1; iemeParamInt < NOMBRE_NI_EDIT; iemeParamInt++)
		m_NiEdit[iemeParamInt].SetValue(m_ParamReal[iemeParamInt]);

	RedrawWindow(0,0, RDW_UPDATENOW );
return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////////
//						Gestion des Ctrl + click sur les controles des paramètres à varier 
/////////////////////////////////////////////////////////////////////////////////////////////////

/*
ici : on ouvre une CDialogVariation Box.
Si on clique IDOK : On crée un CParametre qu'on ajoute à la "m_LaVariation" de "LaSequence"
*/
void CMainDialog::AddToVariation(void* LePointeur) 
{
	CParametre* pParam = CParametre::Param(LePointeur);
	if(pParam == NULL)
		return;
	CDialogVariation VarDlg(&LaSeqMainDialog.m_LaVariation, pParam);												
	// si on a déjà mis ce parametre dans la variation
	int existeDejaIemeVarie = LaSeqMainDialog.m_LaVariation.ParamAlready(pParam->m_Numero);
	if(existeDejaIemeVarie)										
	{
		LaSeqMainDialog.m_LaVariation.RemoveVarie(existeDejaIemeVarie);
	//	MessageBox("Already In The Variation List"); return;
	}
																								
	if(VarDlg.DoModal() != IDOK) {MessageBeep(1); return;}										

	m_bVarieActive = 1;
	OnBnClickedRadioModeRepet1();																							
	VarDlg.Verifie();																			
	CParametre* pLeParam = VarDlg.ActualiseCParametreValues();												
																								
	LaSeqMainDialog.m_LaVariation.AddParametre(pLeParam);											
																								
	sprintf(m_pMainFrame->m_temp, "%s", LaSeqMainDialog.m_LaVariation.GetTextAllParametre());		
	UpdateControls();	
	m_pMainFrame->m_PortDialog.UpdateControls();																
	UpdateCheckBoxesVarie();																	
	UpdateStaticText();
	UpdateEditColor();	
	m_pMainFrame->m_PortDialog.UpdateEditColor();
}
void CMainDialog::MouseUpMainEditWaitBeforeLoad			(short Button, short Shift, long x, long y)	{	if(Shift != 2)	{return;}	AddToVariation(&m_WaitBeforeLoad);}
void CMainDialog::MouseUpMainEdit01	(short Button, short Shift, long x, long y)	{	if(Shift != 2)	{return;}	AddToVariation(&m_ParamReal[ 1]);}
void CMainDialog::MouseUpMainEdit02	(short Button, short Shift, long x, long y)	{	if(Shift != 2)	{return;}	AddToVariation(&m_ParamReal[ 2]);}
void CMainDialog::MouseUpMainEdit03	(short Button, short Shift, long x, long y)	{	if(Shift != 2)	{return;}	AddToVariation(&m_ParamReal[ 3]);}
void CMainDialog::MouseUpMainEdit04	(short Button, short Shift, long x, long y)	{	if(Shift != 2)	{return;}	AddToVariation(&m_ParamReal[ 4]);}
void CMainDialog::MouseUpMainEdit05	(short Button, short Shift, long x, long y)	{	if(Shift != 2)	{return;}	AddToVariation(&m_ParamReal[ 5]);}
void CMainDialog::MouseUpMainEdit06	(short Button, short Shift, long x, long y)	{	if(Shift != 2)	{return;}	AddToVariation(&m_ParamReal[ 6]);}
void CMainDialog::MouseUpMainEdit07	(short Button, short Shift, long x, long y)	{	if(Shift != 2)	{return;}	AddToVariation(&m_ParamReal[ 7]);}
void CMainDialog::MouseUpMainEdit08	(short Button, short Shift, long x, long y)	{	if(Shift != 2)	{return;}	AddToVariation(&m_ParamReal[ 8]);}
void CMainDialog::MouseUpMainEdit09	(short Button, short Shift, long x, long y)	{	if(Shift != 2)	{return;}	AddToVariation(&m_ParamReal[ 9]);}
void CMainDialog::MouseUpMainEdit10	(short Button, short Shift, long x, long y)	{	if(Shift != 2)	{return;}	AddToVariation(&m_ParamReal[10]);}
void CMainDialog::MouseUpMainEdit11	(short Button, short Shift, long x, long y)	{	if(Shift != 2)	{return;}	AddToVariation(&m_ParamReal[11]);}
void CMainDialog::MouseUpMainEdit12	(short Button, short Shift, long x, long y)	{	if(Shift != 2)	{return;}	AddToVariation(&m_ParamReal[12]);}
void CMainDialog::MouseUpMainEdit13	(short Button, short Shift, long x, long y)	{	if(Shift != 2)	{return;}	AddToVariation(&m_ParamReal[13]);}
void CMainDialog::MouseUpMainEdit14	(short Button, short Shift, long x, long y)	{	if(Shift != 2)	{return;}	AddToVariation(&m_ParamReal[14]);}
void CMainDialog::MouseUpMainEdit15	(short Button, short Shift, long x, long y)	{	if(Shift != 2)	{return;}	AddToVariation(&m_ParamReal[15]);}
void CMainDialog::MouseUpMainEdit16	(short Button, short Shift, long x, long y)	{	if(Shift != 2)	{return;}	AddToVariation(&m_ParamReal[16]);}
void CMainDialog::MouseUpMainEdit17	(short Button, short Shift, long x, long y)	{	if(Shift != 2)	{return;}	AddToVariation(&m_ParamReal[17]);}
void CMainDialog::MouseUpMainEdit18	(short Button, short Shift, long x, long y)	{	if(Shift != 2)	{return;}	AddToVariation(&m_ParamReal[18]);}
void CMainDialog::MouseUpMainEdit19	(short Button, short Shift, long x, long y)	{	if(Shift != 2)	{return;}	AddToVariation(&m_ParamReal[19]);}
void CMainDialog::MouseUpMainEdit20	(short Button, short Shift, long x, long y)	{	if(Shift != 2)	{return;}	AddToVariation(&m_ParamReal[20]);}
void CMainDialog::MouseUpMainEdit21	(short Button, short Shift, long x, long y)	{	if(Shift != 2)	{return;}	AddToVariation(&m_ParamReal[21]);}
void CMainDialog::MouseUpMainEdit22	(short Button, short Shift, long x, long y)	{	if(Shift != 2)	{return;}	AddToVariation(&m_ParamReal[22]);}
void CMainDialog::MouseUpMainEdit23	(short Button, short Shift, long x, long y)	{	if(Shift != 2)	{return;}	AddToVariation(&m_ParamReal[23]);}
void CMainDialog::MouseUpMainEdit24	(short Button, short Shift, long x, long y)	{	if(Shift != 2)	{return;}	AddToVariation(&m_ParamReal[24]);}
void CMainDialog::MouseUpMainEdit25	(short Button, short Shift, long x, long y)	{	if(Shift != 2)	{return;}	AddToVariation(&m_ParamReal[25]);}
void CMainDialog::MouseUpMainEdit26	(short Button, short Shift, long x, long y)	{	if(Shift != 2)	{return;}	AddToVariation(&m_ParamReal[26]);}
void CMainDialog::MouseUpMainEdit27	(short Button, short Shift, long x, long y)	{	if(Shift != 2)	{return;}	AddToVariation(&m_ParamReal[27]);}
void CMainDialog::MouseUpMainEdit28	(short Button, short Shift, long x, long y)	{	if(Shift != 2)	{return;}	AddToVariation(&m_ParamReal[28]);}
void CMainDialog::MouseUpMainEdit29	(short Button, short Shift, long x, long y)	{	if(Shift != 2)	{return;}	AddToVariation(&m_ParamReal[29]);}
void CMainDialog::MouseUpMainEdit30	(short Button, short Shift, long x, long y)	{	if(Shift != 2)	{return;}	AddToVariation(&m_ParamReal[30]);}
void CMainDialog::MouseUpMainEdit31	(short Button, short Shift, long x, long y)	{	if(Shift != 2)	{return;}	AddToVariation(&m_ParamReal[31]);}
void CMainDialog::MouseUpMainEdit32	(short Button, short Shift, long x, long y)	{	if(Shift != 2)	{return;}	AddToVariation(&m_ParamReal[32]);}
void CMainDialog::MouseUpMainEdit33	(short Button, short Shift, long x, long y)	{	if(Shift != 2)	{return;}	AddToVariation(&m_ParamReal[33]);}
void CMainDialog::MouseUpMainEdit34	(short Button, short Shift, long x, long y)	{	if(Shift != 2)	{return;}	AddToVariation(&m_ParamReal[34]);}
void CMainDialog::MouseUpMainEdit35	(short Button, short Shift, long x, long y)	{	if(Shift != 2)	{return;}	AddToVariation(&m_ParamReal[35]);}
void CMainDialog::MouseUpMainEdit36	(short Button, short Shift, long x, long y)	{	if(Shift != 2)	{return;}	AddToVariation(&m_ParamReal[36]);}
void CMainDialog::MouseUpMainEdit37	(short Button, short Shift, long x, long y)	{	if(Shift != 2)	{return;}	AddToVariation(&m_ParamReal[37]);}
void CMainDialog::MouseUpMainEdit38	(short Button, short Shift, long x, long y)	{	if(Shift != 2)	{return;}	AddToVariation(&m_ParamReal[38]);}
void CMainDialog::MouseUpMainEdit39	(short Button, short Shift, long x, long y)	{	if(Shift != 2)	{return;}	AddToVariation(&m_ParamReal[39]);}
void CMainDialog::MouseUpMainEdit40	(short Button, short Shift, long x, long y)	{	if(Shift != 2)	{return;}	AddToVariation(&m_ParamReal[40]);}
void CMainDialog::MouseUpMainEdit41	(short Button, short Shift, long x, long y)	{	if(Shift != 2)	{return;}	AddToVariation(&m_ParamReal[41]);}
void CMainDialog::MouseUpMainEdit42	(short Button, short Shift, long x, long y)	{	if(Shift != 2)	{return;}	AddToVariation(&m_ParamReal[42]);}
void CMainDialog::MouseUpMainEdit43	(short Button, short Shift, long x, long y)	{	if(Shift != 2)	{return;}	AddToVariation(&m_ParamReal[43]);}
void CMainDialog::MouseUpMainEdit44	(short Button, short Shift, long x, long y)	{	if(Shift != 2)	{return;}	AddToVariation(&m_ParamReal[44]);}
void CMainDialog::MouseUpMainEdit45	(short Button, short Shift, long x, long y)	{	if(Shift != 2)	{return;}	AddToVariation(&m_ParamReal[45]);}
void CMainDialog::MouseUpMainEdit46	(short Button, short Shift, long x, long y)	{	if(Shift != 2)	{return;}	AddToVariation(&m_ParamReal[46]);}
void CMainDialog::MouseUpMainEdit47	(short Button, short Shift, long x, long y)	{	if(Shift != 2)	{return;}	AddToVariation(&m_ParamReal[47]);}
void CMainDialog::MouseUpMainEdit48	(short Button, short Shift, long x, long y)	{	if(Shift != 2)	{return;}	AddToVariation(&m_ParamReal[48]);}
void CMainDialog::MouseUpMainEdit49	(short Button, short Shift, long x, long y)	{	if(Shift != 2)	{return;}	AddToVariation(&m_ParamReal[49]);}
void CMainDialog::MouseUpMainEdit50	(short Button, short Shift, long x, long y)	{	if(Shift != 2)	{return;}	AddToVariation(&m_ParamReal[50]);}
void CMainDialog::MouseUpMainEdit51	(short Button, short Shift, long x, long y)	{	if(Shift != 2)	{return;}	AddToVariation(&m_ParamReal[51]);}
void CMainDialog::MouseUpMainEdit52	(short Button, short Shift, long x, long y)	{	if(Shift != 2)	{return;}	AddToVariation(&m_ParamReal[52]);}
void CMainDialog::MouseUpMainEdit53	(short Button, short Shift, long x, long y)	{	if(Shift != 2)	{return;}	AddToVariation(&m_ParamReal[53]);}
void CMainDialog::MouseUpMainEdit54	(short Button, short Shift, long x, long y)	{	if(Shift != 2)	{return;}	AddToVariation(&m_ParamReal[54]);}
void CMainDialog::MouseUpMainEdit55	(short Button, short Shift, long x, long y)	{	if(Shift != 2)	{return;}	AddToVariation(&m_ParamReal[55]);}
void CMainDialog::MouseUpMainEdit56	(short Button, short Shift, long x, long y)	{	if(Shift != 2)	{return;}	AddToVariation(&m_ParamReal[56]);}
void CMainDialog::MouseUpMainEdit57	(short Button, short Shift, long x, long y)	{	if(Shift != 2)	{return;}	AddToVariation(&m_ParamReal[57]);}
void CMainDialog::MouseUpMainEdit58	(short Button, short Shift, long x, long y)	{	if(Shift != 2)	{return;}	AddToVariation(&m_ParamReal[58]);}
void CMainDialog::MouseUpMainEdit59	(short Button, short Shift, long x, long y)	{	if(Shift != 2)	{return;}	AddToVariation(&m_ParamReal[59]);}
void CMainDialog::MouseUpMainEdit60	(short Button, short Shift, long x, long y)	{	if(Shift != 2)	{return;}	AddToVariation(&m_ParamReal[60]);}
void CMainDialog::MouseUpMainEdit61	(short Button, short Shift, long x, long y)	{	if(Shift != 2)	{return;}	AddToVariation(&m_ParamReal[61]);}
void CMainDialog::MouseUpMainEdit62	(short Button, short Shift, long x, long y)	{	if(Shift != 2)	{return;}	AddToVariation(&m_ParamReal[62]);}
void CMainDialog::MouseUpMainEdit63	(short Button, short Shift, long x, long y)	{	if(Shift != 2)	{return;}	AddToVariation(&m_ParamReal[63]);}
void CMainDialog::MouseUpMainEdit64	(short Button, short Shift, long x, long y)	{	if(Shift != 2)	{return;}	AddToVariation(&m_ParamReal[64]);}
void CMainDialog::MouseUpMainEdit65	(short Button, short Shift, long x, long y)	{	if(Shift != 2)	{return;}	AddToVariation(&m_ParamReal[65]);}
void CMainDialog::MouseUpMainEdit66	(short Button, short Shift, long x, long y)	{	if(Shift != 2)	{return;}	AddToVariation(&m_ParamReal[66]);}
void CMainDialog::MouseUpMainEdit67	(short Button, short Shift, long x, long y)	{	if(Shift != 2)	{return;}	AddToVariation(&m_ParamReal[67]);}
void CMainDialog::MouseUpMainEdit68	(short Button, short Shift, long x, long y)	{	if(Shift != 2)	{return;}	AddToVariation(&m_ParamReal[68]);}
void CMainDialog::MouseUpMainEdit69	(short Button, short Shift, long x, long y)	{	if(Shift != 2)	{return;}	AddToVariation(&m_ParamReal[69]);}
void CMainDialog::MouseUpMainEdit70	(short Button, short Shift, long x, long y)	{	if(Shift != 2)	{return;}	AddToVariation(&m_ParamReal[70]);}
void CMainDialog::MouseUpMainEdit71	(short Button, short Shift, long x, long y)	{	if(Shift != 2)	{return;}	AddToVariation(&m_ParamReal[71]);}
void CMainDialog::MouseUpMainEdit72	(short Button, short Shift, long x, long y)	{	if(Shift != 2)	{return;}	AddToVariation(&m_ParamReal[72]);}
void CMainDialog::MouseUpMainEdit73	(short Button, short Shift, long x, long y)	{	if(Shift != 2)	{return;}	AddToVariation(&m_ParamReal[73]);}
void CMainDialog::MouseUpMainEdit74	(short Button, short Shift, long x, long y)	{	if(Shift != 2)	{return;}	AddToVariation(&m_ParamReal[74]);}
void CMainDialog::MouseUpMainEdit75	(short Button, short Shift, long x, long y)	{	if(Shift != 2)	{return;}	AddToVariation(&m_ParamReal[75]);}
void CMainDialog::MouseUpMainEdit76	(short Button, short Shift, long x, long y)	{	if(Shift != 2)	{return;}	AddToVariation(&m_ParamReal[76]);}
void CMainDialog::MouseUpMainEdit77	(short Button, short Shift, long x, long y)	{	if(Shift != 2)	{return;}	AddToVariation(&m_ParamReal[77]);}
void CMainDialog::MouseUpMainEdit78	(short Button, short Shift, long x, long y)	{	if(Shift != 2)	{return;}	AddToVariation(&m_ParamReal[78]);}
void CMainDialog::MouseUpMainEdit79	(short Button, short Shift, long x, long y)	{	if(Shift != 2)	{return;}	AddToVariation(&m_ParamReal[79]);}
void CMainDialog::MouseUpMainEdit80	(short Button, short Shift, long x, long y)	{	if(Shift != 2)	{return;}	AddToVariation(&m_ParamReal[80]);}
void CMainDialog::MouseUpMainEdit81	(short Button, short Shift, long x, long y)	{	if(Shift != 2)	{return;}	AddToVariation(&m_ParamReal[81]);}
void CMainDialog::MouseUpMainEdit82	(short Button, short Shift, long x, long y)	{	if(Shift != 2)	{return;}	AddToVariation(&m_ParamReal[82]);}
void CMainDialog::MouseUpMainEdit83	(short Button, short Shift, long x, long y)	{	if(Shift != 2)	{return;}	AddToVariation(&m_ParamReal[83]);}
void CMainDialog::MouseUpMainEdit84	(short Button, short Shift, long x, long y)	{	if(Shift != 2)	{return;}	AddToVariation(&m_ParamReal[84]);}
void CMainDialog::MouseUpMainEdit85	(short Button, short Shift, long x, long y)	{	if(Shift != 2)	{return;}	AddToVariation(&m_ParamReal[85]);}
void CMainDialog::MouseUpMainEdit86	(short Button, short Shift, long x, long y)	{	if(Shift != 2)	{return;}	AddToVariation(&m_ParamReal[86]);}
void CMainDialog::MouseUpMainEdit87	(short Button, short Shift, long x, long y)	{	if(Shift != 2)	{return;}	AddToVariation(&m_ParamReal[87]);}
void CMainDialog::MouseUpMainEdit88	(short Button, short Shift, long x, long y)	{	if(Shift != 2)	{return;}	AddToVariation(&m_ParamReal[88]);}
void CMainDialog::MouseUpMainEdit89	(short Button, short Shift, long x, long y)	{	if(Shift != 2)	{return;}	AddToVariation(&m_ParamReal[89]);}
							
/////////////////////////////////////////////////////////////////////////////////////////////////
//						Gestion des info sur les paramètres qui varient
/////////////////////////////////////////////////////////////////////////////////////////////////
void CMainDialog::ValueChangedMainCheckVar00(BOOL Value)
{
	m_bVarieActive = Value ? 1 : 0;

	EnableVariationCtrl(m_bVarieActive);
	if(m_bVarieActive)
	{
		OnBnClickedRadioModeRepet1();
		return;
	}
	else
		OnBnClickedRadioModeRepet3();
}
void CMainDialog::ValueChangedMainCheckVar01(BOOL Value)
{
	if(Value == TRUE)
	{
		return;
	}
	UpdateEditColor(true);	m_pMainFrame->m_PortDialog.UpdateEditColor(true);
	LaSeqMainDialog.m_LaVariation.RemoveVarie(1);
	UpdateEditColor();	m_pMainFrame->m_PortDialog.UpdateEditColor();
	UpdateCheckBoxesVarie();																			
	UpdateStaticText();
}
void CMainDialog::ValueChangedMainCheckVar02(BOOL Value)
{
	if(Value == TRUE)
		return;
	UpdateEditColor(true);	m_pMainFrame->m_PortDialog.UpdateEditColor(true);
	LaSeqMainDialog.m_LaVariation.RemoveVarie(2);
	UpdateEditColor();	m_pMainFrame->m_PortDialog.UpdateEditColor();
	UpdateCheckBoxesVarie();																			
	UpdateStaticText();
}
void CMainDialog::ValueChangedMainCheckVar03(BOOL Value)
{
	if(Value == TRUE)
		return;
	UpdateEditColor(true);	m_pMainFrame->m_PortDialog.UpdateEditColor(true);
	LaSeqMainDialog.m_LaVariation.RemoveVarie(3);
	UpdateEditColor();	m_pMainFrame->m_PortDialog.UpdateEditColor();
	UpdateCheckBoxesVarie();																			
	UpdateStaticText();
}
void CMainDialog::ValueChangedMainCheckVar04(BOOL Value)
{
	if(Value == TRUE)
		return;
	UpdateEditColor(true);	m_pMainFrame->m_PortDialog.UpdateEditColor(true);
	LaSeqMainDialog.m_LaVariation.RemoveVarie(4);
	UpdateEditColor();	m_pMainFrame->m_PortDialog.UpdateEditColor();
	UpdateCheckBoxesVarie();																			
	UpdateStaticText();
}
void CMainDialog::ValueChangedMainCheckVar05(BOOL Value)
{
	if(Value == TRUE)
		return;
	UpdateEditColor(true);	m_pMainFrame->m_PortDialog.UpdateEditColor(true);
	LaSeqMainDialog.m_LaVariation.RemoveVarie(5);
	UpdateEditColor();	m_pMainFrame->m_PortDialog.UpdateEditColor();
	UpdateCheckBoxesVarie();																			
	UpdateStaticText();
}

// Update Check boxes
#define VARIEMAX 	(&( m_pMainFrame->m_MainDialog.LaSeqMainDialog.m_LaVariation ))->GetMaxVarie()
void CMainDialog::UpdateCheckBoxesVarie(int OrdreSuivantVariant /* =0 */)
{
	if(VARIEMAX == 0)
	{
		m_bVarieActive = 0;
		UpdateControls();
	}
	CheckDlgButton(IDC_MAIN_CHECK_VAR_01, VARIEMAX >= 1);
	CheckDlgButton(IDC_MAIN_CHECK_VAR_02, VARIEMAX >= 2);
	CheckDlgButton(IDC_MAIN_CHECK_VAR_03, VARIEMAX >= 3);
	CheckDlgButton(IDC_MAIN_CHECK_VAR_04, VARIEMAX >= 4);
	CheckDlgButton(IDC_MAIN_CHECK_VAR_05, VARIEMAX >= 5);
}

// Update Static Text
void CMainDialog::UpdateStaticText()
{
	CVariation& LaVar =  m_pMainFrame->m_MainDialog.LaSeqMainDialog.m_LaVariation;
	unsigned int nVariantsSeqMainDialog = LaVar.nVariants;
	for(unsigned int nStaticText = 1; nStaticText <= 5; nStaticText++)
	{
		// on récupère le texte des parametres qui varient dans en ième position de la CVariation
		CString text("");
		for(unsigned int i = 1; i <= nVariantsSeqMainDialog; i++)
		{
			if(LaVar.GetParametreIemeVariant(i)->Varie == nStaticText)
				text.Append(LaVar.GetTextAllParametre(i));
		}

		// et on l'affiche au bon endroit
		if(nStaticText == 1)	SetDlgItemText(IDC_MAIN_VAR_TEXTE_0INFO_1, text);
		if(nStaticText == 2)	SetDlgItemText(IDC_MAIN_VAR_TEXTE_0INFO_2, text);
		if(nStaticText == 3)	SetDlgItemText(IDC_MAIN_VAR_TEXTE_0INFO_3, text);
		if(nStaticText == 4)	SetDlgItemText(IDC_MAIN_VAR_TEXTE_0INFO_4, text);
		if(nStaticText == 5)	SetDlgItemText(IDC_MAIN_VAR_TEXTE_0INFO_5, text);

		if(nStaticText == 1)	SetDlgItemText(IDC_MAIN_VAR_TEXTE_01, "Varie 1");
		if(nStaticText == 2)	SetDlgItemText(IDC_MAIN_VAR_TEXTE_02, "Varie 2");
		if(nStaticText == 3)	SetDlgItemText(IDC_MAIN_VAR_TEXTE_03, "Varie 3");
		if(nStaticText == 4)	SetDlgItemText(IDC_MAIN_VAR_TEXTE_04, "Varie 4");
		if(nStaticText == 5)	SetDlgItemText(IDC_MAIN_VAR_TEXTE_05, "Varie 5");
	}


}
// Update la Back Color des NumEdit
void CMainDialog::UpdateEditColor(bool TousBlancs /* = false */)
{	
	for(int iemeParamInt = 1; iemeParamInt < NOMBRE_NI_EDIT; iemeParamInt++)
	{	m_NiEdit[iemeParamInt]	.SetBackColor( CNiColor(255,255,255) ); }	

	CVariation& LaVar =  m_pMainFrame->m_MainDialog.LaSeqMainDialog.m_LaVariation;
	unsigned int nVariantsSeqMainDialog = LaVar.nVariants;

	for(unsigned int i = 1; i <= nVariantsSeqMainDialog; i++)
	{
		CParametre* pLePar = LaVar.GetParametreIemeVariant(i);
		if(true)
		{
			if(pLePar->Get_pVal() ==	&m_WaitBeforeLoad			){	m_NiEditWaitBeforeLoad			.SetBackColor( CNiColor(255,255, (pLePar->Varie && !TousBlancs) ? 0 : 255) ); }	

			for(int iemeParamInt = 1; iemeParamInt < NOMBRE_NI_EDIT; iemeParamInt++)
				if(pLePar->Get_pVal() ==	&m_ParamReal[iemeParamInt]	)
				{	m_NiEdit[iemeParamInt]	.SetBackColor( CNiColor(255,255, (pLePar->Varie && !TousBlancs) ? 0 : 255) ); }	
		}																																	   
	}																																		   
}

/////////////////////////////////////////////////////////////////////////////////////////////////
//						Gestion des bouttons "radio" pour la répétition de la séquence
/////////////////////////////////////////////////////////////////////////////////////////////////
#define UPDATE_RADIO 										\
	CheckDlgButton(IDC_MAIN_RADIO_MODE_REPET_1, m_Repete == 1);	\
	CheckDlgButton(IDC_MAIN_RADIO_MODE_REPET_2, m_Repete >= 2);	\
	CheckDlgButton(IDC_MAIN_RADIO_MODE_REPET_3, m_Repete == 0);	\


void CMainDialog::OnBnClickedRadioModeRepet1()
{	m_Repete = 1;	UPDATE_RADIO
	LaSeqMainDialog.m_LaVariation.m_Repete = m_Repete;
	EnableVariationCtrl(m_bVarieActive);
}
void CMainDialog::OnBnClickedRadioModeRepet2()
{	m_Repete = GetDlgItemInt(IDC_MAIN_EDIT_MODE_REPET);	UPDATE_RADIO
	LaSeqMainDialog.m_LaVariation.m_Repete = m_Repete;
	EnableVariationCtrl(m_bVarieActive);
}
void CMainDialog::OnBnClickedRadioModeRepet3()
{	m_Repete = 0;	UPDATE_RADIO
	LaSeqMainDialog.m_LaVariation.m_Repete = m_Repete;
	EnableVariationCtrl(false);
}
void CMainDialog::ClickEditModeRepet()
{	OnBnClickedRadioModeRepet2();
}
void CMainDialog::ValueChangedMainEditModeRepet(BLABLA00)
{	//m_Repete	=	int( Value->dblVal );	UPDATE_RADIO
	OnBnClickedRadioModeRepet2();
}
void CMainDialog::EnableVariationCtrl(bool State)
{
	GetDlgItem( IDC_MAIN_CHECK_VAR_01			)->EnableWindow(State);
	GetDlgItem( IDC_MAIN_VAR_TEXTE_01			)->EnableWindow(State);
	GetDlgItem( IDC_MAIN_VAR_TEXTE_0INFO_1		)->EnableWindow(State);
	GetDlgItem( IDC_MAIN_VAR_PROGRESS_01					)->ShowWindow(State);

	GetDlgItem( IDC_MAIN_CHECK_VAR_02			)->EnableWindow(State);
	GetDlgItem( IDC_MAIN_VAR_TEXTE_02			)->EnableWindow(State);
	GetDlgItem( IDC_MAIN_VAR_TEXTE_0INFO_2		)->EnableWindow(State);
	GetDlgItem( IDC_MAIN_VAR_PROGRESS_02					)->ShowWindow(State);

	GetDlgItem( IDC_MAIN_CHECK_VAR_03			)->EnableWindow(State);
	GetDlgItem( IDC_MAIN_VAR_TEXTE_03			)->EnableWindow(State);
	GetDlgItem( IDC_MAIN_VAR_TEXTE_0INFO_3		)->EnableWindow(State);
	GetDlgItem( IDC_MAIN_VAR_PROGRESS_03					)->ShowWindow(State);

	GetDlgItem( IDC_MAIN_CHECK_VAR_04			)->EnableWindow(State);
	GetDlgItem( IDC_MAIN_VAR_TEXTE_04			)->EnableWindow(State);
	GetDlgItem( IDC_MAIN_VAR_TEXTE_0INFO_4		)->EnableWindow(State);
	GetDlgItem( IDC_MAIN_VAR_PROGRESS_04					)->ShowWindow(State);

	GetDlgItem( IDC_MAIN_CHECK_VAR_05			)->EnableWindow(State);
	GetDlgItem( IDC_MAIN_VAR_TEXTE_05			)->EnableWindow(State);
	GetDlgItem( IDC_MAIN_VAR_TEXTE_0INFO_5		)->EnableWindow(State);
	GetDlgItem( IDC_MAIN_VAR_PROGRESS_05					)->ShowWindow(State);
	UpdateEditColor( !State );
	if(m_pMainFrame->m_PortDialog.m_bWasPaintedOnce)
		m_pMainFrame->m_PortDialog.UpdateEditColor( !State );

}

