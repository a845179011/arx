#include "stdafx.h"
#include <list>
#include "CDwgDatabaseUtil.h"

AcDbObjectId CDwgDatabaseUtil::PostToModelSpace(AcDbEntity* pEnt)
{
	assert(pEnt);

	//获得当前图形数据库的块表
	AcDbBlockTable* pBlkTbl = NULL;
	acdbHostApplicationServices()->workingDatabase()->getBlockTable(pBlkTbl, AcDb::kForRead);

	//获得模型空间对应的块表记录
	AcDbBlockTableRecord* pBlkTblRcd = NULL;
	pBlkTbl->getAt(ACDB_MODEL_SPACE, pBlkTblRcd, AcDb::kForWrite);
	pBlkTbl->close();

	//将实体添加到模型空间的块表记录
	AcDbObjectId entId;
	Acad::ErrorStatus es = pBlkTblRcd->appendAcDbEntity(entId, pEnt);
	if (es != Acad::eOk)
	{
		pBlkTblRcd->close();
		delete pEnt;
		pEnt = NULL;
		return AcDbObjectId::kNull;
	}

	//关闭模型空间块表记录和实体
	pBlkTblRcd->close();
	pEnt->close();
	return entId;
}

bool CDwgDatabaseUtil::GetSelObjects(const TCHAR * prompt, std::list<AcDbObjectId>& selIds)
{
	ads_name sset;
	//ads_point pt;
	//if (acedEntSel(TEXT("\n 选择实体："), sset, pt) != RTNORM)	//选择单个实体
	if (acedSSGet(NULL, NULL, NULL, NULL, sset) != RTNORM)
	{
		return false;
	}
	//取得选择集长度
	int length = 0;
	acedSSLength(sset, &length);

	//遍历选择集
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
		//打开实体
		es = acdbOpenObject(pEnt, curEntId, AcDb::kForRead);
		if (es != Acad::eOk)
			continue;
		selIds.push_back(pEnt->objectId());
		pEnt->close();//关闭实体
	}
	acedSSFree(sset);//释放选择集
	return (selIds.size() > 0);
}