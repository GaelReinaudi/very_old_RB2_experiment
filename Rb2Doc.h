// Rb2Doc.h : interface de la classe CRb2Doc
//


#pragma once

class CRb2Doc : public CDocument
{
protected: // création à partir de la sérialisation uniquement
	CRb2Doc();
	DECLARE_DYNCREATE(CRb2Doc)

// Attributs
public:

// Opérations
public:

// Substitutions
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// Implémentation
public:
	virtual ~CRb2Doc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Fonctions générées de la table des messages
protected:
	DECLARE_MESSAGE_MAP()
};


