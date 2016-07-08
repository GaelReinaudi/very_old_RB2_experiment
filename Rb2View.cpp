// Rb2View.cpp : implémentation de la classe CRb2View
//

#include "stdafx.h"
#include "Rb2Manip.h"

#include "Rb2Doc.h"

#include "MainFrm.h"
#include ".\rb2view.h"

	CPen lPenRouge(PS_SOLID, 1, RGB(255, 0, 0));
	CPen lPenMauve(PS_SOLID, 2, RGB(255, 100, 255));
	CPen lPenJaune(PS_SOLID, 1, RGB(255, 255, 0));
	CPen lPenVert(PS_SOLID, 2, RGB(0, 255, 0));

#define PeriodesParPixel 20 // pour l'instant, on fixe le nombre de periodes représentées par une ligne verticale de pixels

/*
Renvoye la valeur du nindex ième bit de la représentation binaire de ndec en nombre 32 bits
nindex commence à 0 pour le premier bit
*/
bool IntToBool(unsigned long ndec, unsigned char nindex)
{
	unsigned long compare = 1;
	for(int i = 0; i <= nindex - 1; i++)
	{
		compare *=2;
	}
	bool bit = (ndec & compare) != 0;
return bit;
}

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CRb2View

IMPLEMENT_DYNCREATE(CRb2View, CScrollView)

BEGIN_MESSAGE_MAP(CRb2View, CScrollView)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_MOUSEWHEEL()
	ON_WM_SIZE()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

// construction ou destruction de CRb2View

CRb2View::CRb2View()
{
	m_LastPointMoved = CPoint(0,0);
	m_pSeqCourrante = NULL;
/////////////////////////// CF : Doc&Astuces  : "CView Access From Anywhere"
	CRb2ManipApp* pApp = (CRb2ManipApp*)AfxGetApp();
	pApp->StoreMyView(this);
/////////////////////////// End CF : Doc&Astuces  : "CView Access From Anywhere"
}

CRb2View::~CRb2View()
{
	if(m_pMainFrame->m_MainDialog.pLaSeqView)
	{
		delete m_pMainFrame->m_MainDialog.pLaSeqView;
		m_pMainFrame->m_MainDialog.pLaSeqView = NULL;
	}
}

BOOL CRb2View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO : Changez ici la classe ou les styles Window en modifiant
	//  CREATESTRUCT cs

	return CScrollView::PreCreateWindow(cs);
}
void CRb2View::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();
//ResizeParentToFit(false);
//CRect r;
//GetClientRect(r);
//SetScaleToFitSize(CSize(r.right, r.bottom));
//return;
	CSize sizeTotal;
	// TODO: calculate the total size of this view
	sizeTotal.cx = sizeTotal.cy = 0;
	SetScrollSizes(MM_TEXT, sizeTotal);
}
void CRb2View::OnSize(UINT nType, int cx, int cy)
{
	CScrollView::OnSize(nType, cx, cy);// calls CScrollView, which Invalidates to get scroll bars (yuck)
	//CView::OnSize(nType, cx, cy);
	if(this == &m_View && m_pMainFrame->m_Affichage < CAM)
	{
//		// y a t il les scrollbars ?
//		BOOL horizBar, vertBar;
//		m_View.CheckScrollBars(horizBar, vertBar);
		CRect rectView;
		m_View.GetWindowRect(rectView);
		m_pMainFrame->m_splitterPosition = rectView.Height();//cy;
	}
}
//void CRb2View::OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/)
//{
//	// TODO : ajoutez ici votre code spécialisé et/ou l'appel de la classe de base
//}
void CRb2View::OnDraw(CDC* pDC)
{
	CRect LeRectToutDessine(0,0,0,0);
	if(m_pMainFrame->m_Affichage < CAM)
	{
		m_pSeqCourrante = &(m_pMainFrame->m_MainDialog.LaSeqMainDialog);
		if(m_pMainFrame->m_MainDialog.m_bUseSeqView == true)
		{
				m_pSeqCourrante = m_pMainFrame->m_MainDialog.pLaSeqView;
		}
		// si on dessine la view1 , on dessine la view2 aussi, hein?
		if(this == &m_View)
			m_View2.Invalidate();
		if(this == &m_View)
			LeRectToutDessine = m_pSeqCourrante->DessineSeqView1(*pDC);
		if(this == &m_View2)
			LeRectToutDessine = m_pSeqCourrante->DessineSeqView2(*pDC);
	}
// 	else if(m_pMainFrame->m_Affichage >= CAM && m_pMainFrame->m_TraitImDialog.m_bWasPaintedOnce)
// 	{
// 		m_pMainFrame->m_TraitImDialog.ActualiseAffichage4Image();
// 	}
	// on actualise les ScrollBar de CScrollView
	SetScrollSizes(MM_TEXT, CSize(LeRectToutDessine.BottomRight()));
//	ShowScrollBar(SB_HORZ, FALSE);
//	ShowScrollBar(SB_VERT, TRUE);
}


// diagnostics pour CRb2View

#ifdef _DEBUG
void CRb2View::AssertValid() const
{
	CScrollView::AssertValid();
}

void CRb2View::Dump(CDumpContext& undc) const
{
	CScrollView::Dump(undc);
}

CRb2Doc* CRb2View::GetDocument() const // la version non déboguée est en ligne
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CRb2Doc)));
	return (CRb2Doc*)m_pDocument;
}
#endif //_DEBUG




void CRb2View::OnMouseMove(UINT nFlags, CPoint point)
{
	CPoint Point_plus_Scroll = point + GetScrollPosition();
	if((m_pMainFrame->m_Affichage & CAM) || (m_pMainFrame->m_Affichage & TRAITIMAGE) || (m_pMainFrame->m_Affichage & FIT))
	{
		m_LastPointMoved = Point_plus_Scroll;
		// Load the cross cursor
		HCURSOR lhCursor = AfxGetApp()->LoadStandardCursor(IDC_CROSS);	SetCursor(lhCursor);
// 		m_pMainFrame->m_TraitImDialog.MousseMovedOnView(nFlags, m_LastPointMoved);
// 		m_pMainFrame->m_FitDialog.MousseMovedOnView(nFlags, m_LastPointMoved);
	}

	if(m_pMainFrame->m_Affichage & SEQVIEW)
	{
		if(this == &m_View)
			m_pMainFrame->m_SeqViewDialog.MousseMoved(1, nFlags, Point_plus_Scroll);
		if(this == &m_View2)
			m_pMainFrame->m_SeqViewDialog.MousseMoved(2, nFlags, Point_plus_Scroll);
	}
	CScrollView::OnMouseMove(nFlags, point);
}
void CRb2View::OnLButtonDown(UINT nFlags, CPoint point)
{
	CPoint Point_ScrollPosition = GetScrollPosition();
	CPoint Point_plus_Scroll = point + Point_ScrollPosition;
// 	if((m_pMainFrame->m_Affichage & CAM) || (m_pMainFrame->m_Affichage & TRAITIMAGE) || (m_pMainFrame->m_Affichage & FIT))
// 	{
// 		m_pMainFrame->m_FitDialog.MousseLeftDown(nFlags, Point_plus_Scroll);
// 	}
	if(m_pMainFrame->m_Affichage < SEQVIEW)
	{
		if(!m_pMainFrame->m_MainDialog.m_bUseSeqView)
		{
			if(m_pMainFrame->m_MainDialog.pLaSeqView)
			{
				delete m_pMainFrame->m_MainDialog.pLaSeqView;
				m_pMainFrame->m_MainDialog.pLaSeqView = NULL;
			}
			m_pMainFrame->m_MainDialog.pLaSeqView = new Sequence(m_pMainFrame->m_MainDialog.LaSeqMainDialog);
		}
		m_pMainFrame->m_MainDialog.m_bUseSeqView = true;
		m_pMainFrame->m_MainDialog.m_bVarieActive = false;
		CRect rectChanged(0,0,0,0);
		if(this == &m_View)
			rectChanged = m_pMainFrame->m_MainDialog.pLaSeqView->ClickedDigitalToSwitch(Point_plus_Scroll);
		if(this == &m_View2)
			rectChanged = m_pMainFrame->m_MainDialog.pLaSeqView->ClickedAnalogToSet(Point_plus_Scroll);
		if(rectChanged == CRect(0,0,0,0))
			return;
		rectChanged.OffsetRect(-Point_ScrollPosition);
		InvalidateRect(rectChanged);
	}

	if(m_pMainFrame->m_Affichage & SEQVIEW)
	{
		if(this == &m_View)
			m_pMainFrame->m_SeqViewDialog.MousseLeftDown(1, nFlags, Point_plus_Scroll);
		if(this == &m_View2)
			m_pMainFrame->m_SeqViewDialog.MousseLeftDown(2, nFlags, Point_plus_Scroll);
	}

	CScrollView::OnLButtonDown(nFlags, point);
}
void CRb2View::OnRButtonDown(UINT nFlags, CPoint point)
{
	CPoint Point_plus_Scroll = point + GetScrollPosition();

	SetCapture();
// 	if((m_pMainFrame->m_Affichage & CAM) || (m_pMainFrame->m_Affichage & TRAITIMAGE) || (m_pMainFrame->m_Affichage & FIT))
// 	{
// 		m_pMainFrame->m_FitDialog.MousseRightDown(nFlags, Point_plus_Scroll);
// 	}
	if(m_pMainFrame->m_Affichage & SEQVIEW)
	{
		if(this == &m_View)
			m_pMainFrame->m_SeqViewDialog.MousseRightDown(1, nFlags, Point_plus_Scroll);
		if(this == &m_View2)
			m_pMainFrame->m_SeqViewDialog.MousseRightDown(2, nFlags, Point_plus_Scroll);
	}
	CScrollView::OnRButtonDown(nFlags, point);
}

void CRb2View::OnRButtonUp(UINT nFlags, CPoint point)
{
	CPoint Point_plus_Scroll = point + GetScrollPosition();

	if (GetCapture() == this)
        ReleaseCapture();
// 	if((m_pMainFrame->m_Affichage & CAM) || (m_pMainFrame->m_Affichage & TRAITIMAGE) || (m_pMainFrame->m_Affichage & FIT))
// 	{
// 		m_pMainFrame->m_FitDialog.MousseRightUp(nFlags, Point_plus_Scroll);
// 	}
	CScrollView::OnRButtonUp(nFlags, point);
}



void CRb2View::OnLButtonUp(UINT nFlags, CPoint point)
{
	CPoint Point_plus_Scroll = point + GetScrollPosition();

	if (GetCapture() == this)
        ReleaseCapture();
// 	if((m_pMainFrame->m_Affichage & CAM) || (m_pMainFrame->m_Affichage & TRAITIMAGE) || (m_pMainFrame->m_Affichage & FIT))
// 	{
// 		m_pMainFrame->m_FitDialog.MousseLeftUp(nFlags, Point_plus_Scroll);
// 	}
	CScrollView::OnLButtonUp(nFlags, point);
}

void CRb2View::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if(this == &m_View && m_View2 && this == GetFocus())
	{
		m_View2.OnHScroll(nSBCode, nPos, pScrollBar);
	}
	if(this == &m_View2 && m_View && this == GetFocus())
	{
		m_View.OnHScroll(nSBCode, nPos, pScrollBar);
	}
	CScrollView::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CRb2View::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	CScrollView::OnVScroll(nSBCode, nPos, pScrollBar);
}

BOOL CRb2View::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// si on n'appuie pas sur CTRL ou autre
	if(nFlags == 0)
		return CScrollView::DoMouseWheel(nFlags, zDelta, pt);
	if(nFlags == MK_CONTROL)
	{
		CRect clientrect; CRb2View::GetWindowRect(clientrect);
		CPoint Point_plus_Scroll = pt + GetScrollPosition() - clientrect.TopLeft();
		if(m_pSeqCourrante)
			if(m_pSeqCourrante->MouseWheelOnSeq(nFlags, zDelta, Point_plus_Scroll))
				Invalidate();
	}
	return TRUE;
}
