#pragma once
#include "resource.h"

// Boîte de dialogue CDialogWaveForm
#define m_Wf (*m_pWf)
#define NO_POINT -3145256

class CDialogWaveForm : public CDialog
{
	DECLARE_DYNAMIC(CDialogWaveForm)

private:
	CWaveForme* m_pWf;
	// index du point que l'on tient avec la souris
	int m_IndexPointMouseDown;
	// index du point que l'on a tenu avec la souris dernierement
	int m_IndexLastPointMouseDown;
	// les coords du point ou on a cliqué
	CPoint m_WherePointMouseDown;
	// si on s'en est éloigné depuis qu'on a cliqué dessus
	bool m_EloignementOK;

public:
	CDialogWaveForm(CWnd* pParent = NULL);   // constructeur standard
	CDialogWaveForm(CWaveForme* pTheWaveForm, CWnd* pParent = NULL);   // constructeur standard
	virtual ~CDialogWaveForm();

// Données de boîte de dialogue
	enum { IDD = IDD_DIALOG_WAVE_FORM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Prise en charge DDX/DDV

	DECLARE_MESSAGE_MAP()
public:
	CNiGraph m_Graph;
	virtual BOOL OnInitDialog();
	void ActualiseGraph();
	CNiNumEdit m_EditNbrPoints, m_EditX, m_EditY;
	DECLARE_EVENTSINK_MAP()
	void PlotAreaMouseMoveWfGraph(short* Button, short* Shift, VARIANT* XPos, VARIANT* YPos);
	void CursorMouseMoveWfGraph(short* Button, short* Shift, VARIANT* XPos, VARIANT* YPos, short* CursorIndex, long* CursorPart);
	void PlotMouseMoveWfGraph(short* Button, short* Shift, VARIANT* xData, VARIANT* YData, short* PlotIndex, long* PointIndex);
	void PlotAreaMouseDownWfGraph(short* Button, short* Shift, VARIANT* XPos, VARIANT* YPos);
	void CursorMouseDownWfGraph(short* Button, short* Shift, VARIANT* XPos, VARIANT* YPos, short* CursorIndex, long* CursorPart);
	void PlotMouseDownWfGraph(short* Button, short* Shift, VARIANT* xData, VARIANT* YData, short* PlotIndex, long* PointIndex);
	void PlotMouseUpWfGraph(short* Button, short* Shift, VARIANT* xData, VARIANT* YData, short* PlotIndex, long* PointIndex);
	void ValueChangedWfEditX(VARIANT* Value, VARIANT* PreviousValue, BOOL OutOfRange);
	void ValueChangedWfEditY(VARIANT* Value, VARIANT* PreviousValue, BOOL OutOfRange);
protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
};
