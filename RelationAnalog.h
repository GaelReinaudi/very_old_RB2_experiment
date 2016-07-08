class Colonne;
// cette classe stocke très peut d'information
// elle stocke les données utiles pour établir une relation donnant la WaveForm d'une Analog en fonction
// d'une formule, ou bien d'une autre analog dans une colonne

class CRelationAnalog
{
public:
	CRelationAnalog(Colonne* pOwningColonne);
private:
	// la sequence à laquelle on appartient
	Colonne* m_pCol;
	// l'analogique master
	int FromOtherAnalog;
	// la relation
	CString Relation;
	// Le numero de la colonne à partir de laquelle cette relation prend effet
	int AfterCol;
public:
	// renvoye le nombre de relation sucséssive qui sont définies tout au long de la séquence
	int NbrOfSuccessiveRelations();
	// élimine les relations a partir de la AfterCol
	void RemoveMaster(int AfterCol = 0);
};
