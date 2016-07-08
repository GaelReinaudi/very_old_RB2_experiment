// DialogWaveForm.cpp : fichier d'implémentation
//

#include "stdafx.h"
#include "Rb2Manip.h"
#include ".\dialogwaveform.h"


// Boîte de dialogue CDialogWaveForm

IMPLEMENT_DYNAMIC(CDialogWaveForm, CDialog)
CDialogWaveForm::CDialogWaveForm(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogWaveForm::IDD, pParent)
{
}

CDialogWaveForm::CDialogWaveForm(CWaveForme* pTheWaveForm, CWnd* pParent /*=NULL*/)
	: CDialog(CDialogWaveForm::IDD, pParent)
{
	m_pWf = pTheWaveForm;
	m_IndexPointMouseDown = NO_POINT;
	m_IndexLastPointMouseDown = NO_POINT;
}

CDialogWaveForm::~CDialogWaveForm()
{
}

BEGIN_EVENTSINK_MAP(CDialogWaveForm, CDialog)
	ON_EVENT(CDialogWaveForm, IDC_WF_GRAPH, 9, PlotAreaMouseMoveWfGraph, VTS_PI2 VTS_PI2 VTS_PVARIANT VTS_PVARIANT)
	ON_EVENT(CDialogWaveForm, IDC_WF_GRAPH, 2, CursorMouseMoveWfGraph, VTS_PI2 VTS_PI2 VTS_PVARIANT VTS_PVARIANT VTS_PI2 VTS_PI4)
	ON_EVENT(CDialogWaveForm, IDC_WF_GRAPH, 6, PlotMouseMoveWfGraph, VTS_PI2 VTS_PI2 VTS_PVARIANT VTS_PVARIANT VTS_PI2 VTS_PI4)
	ON_EVENT(CDialogWaveForm, IDC_WF_GRAPH, 8, PlotAreaMouseDownWfGraph, VTS_PI2 VTS_PI2 VTS_PVARIANT VTS_PVARIANT)
	ON_EVENT(CDialogWaveForm, IDC_WF_GRAPH, 4, CursorMouseDownWfGraph, VTS_PI2 VTS_PI2 VTS_PVARIANT VTS_PVARIANT VTS_PI2 VTS_PI4)
	ON_EVENT(CDialogWaveForm, IDC_WF_GRAPH, 5, PlotMouseDownWfGraph, VTS_PI2 VTS_PI2 VTS_PVARIANT VTS_PVARIANT VTS_PI2 VTS_PI4)
	ON_EVENT(CDialogWaveForm, IDC_WF_GRAPH, 7, PlotMouseUpWfGraph, VTS_PI2 VTS_PI2 VTS_PVARIANT VTS_PVARIANT VTS_PI2 VTS_PI4)
	ON_EVENT(CDialogWaveForm, IDC_WF_EDIT_X, 1, ValueChangedWfEditX, VTS_PVARIANT VTS_PVARIANT VTS_BOOL)
	ON_EVENT(CDialogWaveForm, IDC_WF_EDIT_Y, 1, ValueChangedWfEditY, VTS_PVARIANT VTS_PVARIANT VTS_BOOL)
END_EVENTSINK_MAP()

void CDialogWaveForm::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_WF_GRAPH, m_Graph);
	DDX_Control(pDX, IDC_CWNUMEDIT1, m_EditNbrPoints);
	DDX_Control(pDX, IDC_WF_EDIT_X, m_EditX);
	DDX_Control(pDX, IDC_WF_EDIT_Y, m_EditY);
}


BEGIN_MESSAGE_MAP(CDialogWaveForm, CDialog)
END_MESSAGE_MAP()


// Gestionnaires de messages CDialogWaveForm

BOOL CDialogWaveForm::OnInitDialog()
{
	CDialog::OnInitDialog();
	// pour pouvoir capturer les events de la souris
	m_Graph.SetTrackMode(CNiGraph::TrackAllEvents);
	ActualiseGraph();
	// rescale sur les points
	m_Graph.GetAxes().Item("XAxis").SetMinMax(m_Wf.GetPoint(0).x, m_Wf.GetPoint(-1).x);
	m_Graph.GetAxes().Item("YAxis-1").AutoScaleNow();
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION : les pages de propriétés OCX devraient retourner FALSE
}
void CDialogWaveForm::ActualiseGraph()
{
	int nPointWf = m_Wf.GetnPoints();
	m_EditNbrPoints.SetValue(nPointWf);
	m_Graph.ClearData();
	for(int i = 0; i < nPointWf; i++)
	{
		CPointDbl& iemePoint = m_Wf.GetPoint(i);
		m_Graph.ChartXY(iemePoint.x, iemePoint.y);
	}
	if(m_IndexPointMouseDown != NO_POINT)
	{
		m_EditX.SetValue(m_Wf.GetPoint(m_IndexPointMouseDown).x);
		m_EditY.SetValue(m_Wf.GetPoint(m_IndexPointMouseDown).y);
	}
	else if(m_IndexLastPointMouseDown != NO_POINT)
	{
		m_EditX.SetValue(m_Wf.GetPoint(m_IndexLastPointMouseDown).x);
		m_EditY.SetValue(m_Wf.GetPoint(m_IndexLastPointMouseDown).y);
	}
}

void CDialogWaveForm::PlotAreaMouseMoveWfGraph(short* Button, short* Shift, VARIANT* XPos, VARIANT* YPos)
{
	m_Graph.GetCursors().Item(1).SetColor(CNiColor(0,0,255));
	m_Graph.GetCursors().Item(1).SetPosition(XPos->dblVal, YPos->dblVal);
	// si on bouge un point a la limite du graph, on agrandie la région vu
	if(*Button == CNiGraph::LeftButton && m_IndexPointMouseDown != NO_POINT)
	{
		double posX = m_Graph.GetCursors().Item(1).GetXPosition();
		double posY = m_Graph.GetCursors().Item(1).GetYPosition();
		double maxX = m_Graph.GetAxes().Item("XAxis").GetMaximum();
		double maxY = m_Graph.GetAxes().Item("YAxis-1").GetMaximum();
		double minX = m_Graph.GetAxes().Item("XAxis").GetMinimum();
		double minY = m_Graph.GetAxes().Item("YAxis-1").GetMinimum();
		if(posX >= (maxX - minX) * 0.99 + minX)
			m_Graph.GetAxes().Item("XAxis").SetMaximum((maxX - minX) * 1.01 + minX);
		if(posY >= (maxY - minY) * 0.99 + minY)
			m_Graph.GetAxes().Item("YAxis-1").SetMaximum((maxY - minY) * 1.01 + minY);
		if(posX <= maxX - (maxX - minX) * 0.99)
			m_Graph.GetAxes().Item("XAxis").SetMinimum(maxX - (maxX - minX) * 1.01);
		if(posY <= maxY - (maxY - minY) * 0.99)
			m_Graph.GetAxes().Item("YAxis-1").SetMinimum(maxY - (maxY - minY) * 1.01);
	}
}

void CDialogWaveForm::CursorMouseMoveWfGraph(short* Button, short* Shift, VARIANT* XPos, VARIANT* YPos, short* CursorIndex, long* CursorPart)
{
	HCURSOR lhCursor = AfxGetApp()->LoadStandardCursor(0);	
	SetCursor(lhCursor);
	m_Graph.GetCursors().Item(1).SetColor(CNiColor(0,0,255));
	m_Graph.GetCursors().Item(1).SetPosition(XPos->dblVal, YPos->dblVal);
	if(*Button == CNiGraph::LeftButton && m_IndexPointMouseDown != NO_POINT)
	{
		// on ne bouge le point qe si on s'ai éloigner une fois au moins de 5 pixel de la ou on a cliqué
		CPoint PointclicNow;
		GetCursorPos(&PointclicNow);
		if(abs((PointclicNow - m_WherePointMouseDown).cx) > 5
			|| abs((PointclicNow - m_WherePointMouseDown).cy) > 5)
			m_EloignementOK = true;
		if(m_EloignementOK)
		{
			m_Wf.MovePoint(m_IndexPointMouseDown, XPos->dblVal, YPos->dblVal);
			m_Wf.SortListePoints(&m_IndexPointMouseDown);
			m_IndexLastPointMouseDown = m_IndexPointMouseDown;
		}
			ActualiseGraph();
	}
}

void CDialogWaveForm::PlotMouseMoveWfGraph(short* Button, short* Shift, VARIANT* xData, VARIANT* YData, short* PlotIndex, long* PointIndex)
{
	HCURSOR lhCursor = AfxGetApp()->LoadStandardCursor(0);	
	SetCursor(lhCursor);
	m_Graph.GetCursors().Item(1).SetColor(CNiColor(255,255,0));
}

void CDialogWaveForm::PlotAreaMouseDownWfGraph(short* Button, short* Shift, VARIANT* XPos, VARIANT* YPos)
{
	Beep(5000,100);// TODO : ajoutez ici le code de votre gestionnaire de messages
}

void CDialogWaveForm::CursorMouseDownWfGraph(short* Button, short* Shift, VARIANT* XPos, VARIANT* YPos, short* CursorIndex, long* CursorPart)
{
	// ajout d'un point a l'endroit du cliquage
	if(*Shift == CNiGraph::CtrlKey)
	{
		m_Wf.AddPoint(XPos->dblVal, YPos->dblVal);
		m_Wf.SortListePoints();
	}
	ActualiseGraph();
}

void CDialogWaveForm::PlotMouseDownWfGraph(short* Button, short* Shift, VARIANT* xData, VARIANT* YData, short* PlotIndex, long* PointIndex)
{
	// remove d'un point a l'endroit du cliquage
	if(*Shift == CNiGraph::CtrlKey + CNiGraph::ShiftKey)
	{
		m_Wf.RemovePoint(*PointIndex);
	}
	// Prépare à bouger un point
	if(*Shift == 0)
	{
		GetCursorPos(&m_WherePointMouseDown);
		m_EloignementOK = false;
		m_IndexPointMouseDown = *PointIndex;
		m_IndexLastPointMouseDown = m_IndexPointMouseDown;
	}
	ActualiseGraph();
}

void CDialogWaveForm::PlotMouseUpWfGraph(short* Button, short* Shift, VARIANT* xData, VARIANT* YData, short* PlotIndex, long* PointIndex)
{
	// Fini de bouger un point
	if(*Shift == 0)
	{
		m_IndexLastPointMouseDown = m_IndexPointMouseDown;
		m_IndexPointMouseDown = NO_POINT;
	}
	// rescale sur les points
	m_Graph.GetAxes().Item("XAxis").SetMinMax(m_Wf.GetPoint(0).x, m_Wf.GetPoint(-1).x);
	m_Graph.GetAxes().Item("YAxis-1").AutoScaleNow();

}

void CDialogWaveForm::ValueChangedWfEditX(VARIANT* Value, VARIANT* PreviousValue, BOOL OutOfRange)
{
	// si on a cliqué sur un point récemment, c'est ce point qui va etre affecté par la valeur
	if(m_IndexLastPointMouseDown != NO_POINT && m_IndexPointMouseDown == NO_POINT)
	{
		m_Wf.MovePoint(m_IndexLastPointMouseDown, Value->dblVal, m_Wf.GetPoint(m_IndexLastPointMouseDown).y);
		m_Wf.SortListePoints(&m_IndexLastPointMouseDown);
		ActualiseGraph();
		// rescale sur les points
		m_Graph.GetAxes().Item("XAxis").SetMinMax(m_Wf.GetPoint(0).x, m_Wf.GetPoint(-1).x);
		m_Graph.GetAxes().Item("YAxis-1").AutoScaleNow();
	}
}

void CDialogWaveForm::ValueChangedWfEditY(VARIANT* Value, VARIANT* PreviousValue, BOOL OutOfRange)
{
	// si on a cliqué sur un point récemment, c'est ce point qui va etre affecté par la valeur
	if(m_IndexLastPointMouseDown != NO_POINT && m_IndexPointMouseDown == NO_POINT)
	{
		m_Wf.MovePoint(m_IndexLastPointMouseDown, m_Wf.GetPoint(m_IndexLastPointMouseDown).x, Value->dblVal);
		m_Wf.SortListePoints(&m_IndexLastPointMouseDown);
		ActualiseGraph();
		// rescale sur les points
		m_Graph.GetAxes().Item("XAxis").SetMinMax(m_Wf.GetPoint(0).x, m_Wf.GetPoint(-1).x);
		m_Graph.GetAxes().Item("YAxis-1").AutoScaleNow();
	}
}

BOOL CDialogWaveForm::OnCommand(WPARAM wParam, LPARAM lParam)
{
	CWnd *pWnd = GetFocus();
	switch(wParam)
	{
	case IDOK: 
		if(pWnd!=GetDlgItem(IDOK))
		{
			reinterpret_cast<CDialog *>(this)->NextDlgCtrl();
			reinterpret_cast<CDialog *>(this)->PrevDlgCtrl();
			return FALSE;
		}
	}
	return CDialog::OnCommand(wParam, lParam);
}
