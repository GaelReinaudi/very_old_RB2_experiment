// Rb2View.h : interface de la classe CRb2View
//

#pragma once
#include "Sequence.h"


class CRb2View : public CScrollView
{
protected: // création à partir de la sérialisation uniquement
public:// rendu public par gael pour pemrettre de declarer m_TheView dans CMainFrame
	CRb2View();
	DECLARE_DYNCREATE(CRb2View)

// Attributs
public:
	CRb2Doc* GetDocument() const;
protected:
	Sequence* m_pSeqCourrante;

// Opérations
public:

// Substitutions
public:
	virtual void OnInitialUpdate();
	virtual void OnDraw(CDC* pDC);  // substitué pour dessiner cette vue
virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Implémentation
public:
	virtual ~CRb2View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif


// Fonctions générées de la table des messages
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	CPoint m_LastPointMoved;
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnSize(UINT nType, int cx, int cy);
protected:
//	virtual void OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/);
public:
};

#ifndef _DEBUG  // version de débogage dans Rb2View.cpp
inline CRb2Doc* CRb2View::GetDocument() const
   { return reinterpret_cast<CRb2Doc*>(m_pDocument); }
#endif

