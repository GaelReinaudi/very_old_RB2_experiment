class Colonne;
// cette classe stocke tr�s peut d'information
// elle stocke les donn�es utiles pour �tablir une relation donnant la WaveForm d'une Analog en fonction
// d'une formule, ou bien d'une autre analog dans une colonne

class CRelationAnalog
{
public:
	CRelationAnalog(Colonne* pOwningColonne);
private:
	// la sequence � laquelle on appartient
	Colonne* m_pCol;
	// l'analogique master
	int FromOtherAnalog;
	// la relation
	CString Relation;
	// Le numero de la colonne � partir de laquelle cette relation prend effet
	int AfterCol;
public:
	// renvoye le nombre de relation sucs�ssive qui sont d�finies tout au long de la s�quence
	int NbrOfSuccessiveRelations();
	// �limine les relations a partir de la AfterCol
	void RemoveMaster(int AfterCol = 0);
};
