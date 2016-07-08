// Rb2Doc.cpp : implémentation de la classe CRb2Doc
//

#include "stdafx.h"
#include "Rb2Manip.h"

#include "Rb2Doc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CRb2Doc

IMPLEMENT_DYNCREATE(CRb2Doc, CDocument)

BEGIN_MESSAGE_MAP(CRb2Doc, CDocument)
END_MESSAGE_MAP()


// construction ou destruction de CRb2Doc

CRb2Doc::CRb2Doc()
{
	// TODO : ajoutez ici le code d'une construction unique

}

CRb2Doc::~CRb2Doc()
{
}

BOOL CRb2Doc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO : ajoutez ici le code de réinitialisation
	// (les documents SDI réutiliseront ce document)

	return TRUE;
}




// sérialisation de CRb2Doc

void CRb2Doc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO : ajoutez ici le code de stockage
	}
	else
	{
		// TODO : ajoutez ici le code de chargement
	}
}


// diagnostics pour CRb2Doc

#ifdef _DEBUG
void CRb2Doc::AssertValid() const
{
	CDocument::AssertValid();
}

void CRb2Doc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// commandes pour CRb2Doc
