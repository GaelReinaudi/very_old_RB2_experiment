// Rb2Doc.h : interface de la classe CRb2Doc
//


#pragma once

class CRb2Doc : public CDocument
{
protected: // cr�ation � partir de la s�rialisation uniquement
	CRb2Doc();
	DECLARE_DYNCREATE(CRb2Doc)

// Attributs
public:

// Op�rations
public:

// Substitutions
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// Impl�mentation
public:
	virtual ~CRb2Doc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Fonctions g�n�r�es de la table des messages
protected:
	DECLARE_MESSAGE_MAP()
};


