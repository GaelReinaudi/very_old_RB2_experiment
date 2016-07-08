#include "StdAfx.h"
#include "RelationAnalog.h"

CRelationAnalog::CRelationAnalog(Colonne* pOwningColonne)
{
	m_pCol = pOwningColonne;
//	for(int indRel = 0; indRel < NUMBER_MAX_RELATION_PER_ANALOG; indRel++)
//	{
		FromOtherAnalog = -1;
		Relation = "";
		AfterCol = 0;
//	}
}
//int CRelationAnalog::NbrOfSuccessiveRelations()
//{
//	int nbrRel = 0;
//	for(int indRel = 0; indRel < NUMBER_MAX_RELATION_PER_ANALOG; indRel++)
//		if(FromOtherAnalog[indRel] != -1 && Relation[indRel] != "")
//			nbrRel++;
//	return nbrRel;
//}