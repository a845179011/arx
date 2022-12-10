#include "stdafx.h"
#include <list>
#include "CDwgDatabaseUtil.h"

AcDbObjectId CDwgDatabaseUtil::PostToModelSpace(AcDbEntity* pEnt)
{
	assert(pEnt);

	//��õ�ǰͼ�����ݿ�Ŀ��
	AcDbBlockTable* pBlkTbl = NULL;
	acdbHostApplicationServices()->workingDatabase()->getBlockTable(pBlkTbl, AcDb::kForRead);

	//���ģ�Ϳռ��Ӧ�Ŀ���¼
	AcDbBlockTableRecord* pBlkTblRcd = NULL;
	pBlkTbl->getAt(ACDB_MODEL_SPACE, pBlkTblRcd, AcDb::kForWrite);
	pBlkTbl->close();

	//��ʵ����ӵ�ģ�Ϳռ�Ŀ���¼
	AcDbObjectId entId;
	Acad::ErrorStatus es = pBlkTblRcd->appendAcDbEntity(entId, pEnt);
	if (es != Acad::eOk)
	{
		pBlkTblRcd->close();
		delete pEnt;
		pEnt = NULL;
		return AcDbObjectId::kNull;
	}

	//�ر�ģ�Ϳռ����¼��ʵ��
	pBlkTblRcd->close();
	pEnt->close();
	return entId;
}

bool CDwgDatabaseUtil::GetSelObjects(const TCHAR * prompt, std::list<AcDbObjectId>& selIds)
{
	ads_name sset;
	//ads_point pt;
	//if (acedEntSel(TEXT("\n ѡ��ʵ�壺"), sset, pt) != RTNORM)	//ѡ�񵥸�ʵ��
	if (acedSSGet(NULL, NULL, NULL, NULL, sset) != RTNORM)
	{
		return false;
	}
	//ȡ��ѡ�񼯳���
	int length = 0;
	acedSSLength(sset, &length);

	//����ѡ��
	Acad::ErrorStatus es;
	AcDbEntity* pEnt = NULL;
	for (int i = 0; i < length; i++)
	{
		ads_name ename;
		acedSSName(sset, i, ename);
		AcDbObjectId curEntId;
		es = acdbGetObjectId(curEntId, ename);
		if (es != Acad::eOk)
		{
			continue;
		}
		//��ʵ��
		es = acdbOpenObject(pEnt, curEntId, AcDb::kForRead);
		if (es != Acad::eOk)
			continue;
		selIds.push_back(pEnt->objectId());
		pEnt->close();//�ر�ʵ��
	}
	acedSSFree(sset);//�ͷ�ѡ��
	return (selIds.size() > 0);
}