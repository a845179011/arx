#include "stdafx.h"
#include <dbmleader.h>
#include "CDwgDatabaseUtil.h"
#include "Commands.h"
#include <list>

//需要定义宏_USE_MATH_DEFINES
#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif 
#include <math.h>


void Commands::Init()
{
	acedRegCmds->addCommand(_T("TG"), _T("CreateLine"), _T("CreateLine"), ACRX_CMD_MODAL, CreateLine);
	acedRegCmds->addCommand(_T("TG"), _T("ReadModel"), _T("ReadModel"), ACRX_CMD_MODAL, readModel);
	acedRegCmds->addCommand(_T("TG"), _T("CreateRadial"), _T("CreateRadial"), ACRX_CMD_MODAL, createRadialDemension);
	acedRegCmds->addCommand(_T("TG"), _T("CreateDiametric"), _T("CreateDiametric"), ACRX_CMD_MODAL, createDiametricDimension);
	acedRegCmds->addCommand(_T("TG"), _T("CreateAngular"), _T("CreateAngular"), ACRX_CMD_MODAL, createAngularDimension);
	acedRegCmds->addCommand(_T("TG"), _T("CreateLeader"), _T("CreateLeader"), ACRX_CMD_MODAL, createLeader);
	acedRegCmds->addCommand(_T("TG"), _T("ReadLeader"), _T("ReadLeader"), ACRX_CMD_MODAL, readLeader);
	acedRegCmds->addCommand(_T("TG"), _T("ArcTest"), _T("ArcTest"), ACRX_CMD_MODAL, arcTest);
	acedRegCmds->addCommand(_T("TG"), _T("ReadArc"), _T("ReadArc"), ACRX_CMD_MODAL, readArc);
	acedRegCmds->addCommand(_T("TG"), _T("ReadMText"), _T("ReadMText"), ACRX_CMD_MODAL, readMText);
}

void Commands::Unload()
{
	acedRegCmds->removeGroup(_T("TG"));
}

void Commands::CreateLine()
{
	AcGePoint3d pStart(0, 0, 0);
	AcGePoint3d pEnd(100, 100, 0);

	AcDbLine * pLine = new AcDbLine(pStart, pEnd);
	AcDbBlockTable *pTable;
	acdbHostApplicationServices()->workingDatabase()->getBlockTable(pTable, AcDb::kForRead);

	AcDbBlockTableRecord* pRecord;
	pTable->getAt(ACDB_MODEL_SPACE, pRecord, AcDb::kForWrite);
	pRecord->appendAcDbEntity(pLine);

	pTable->close();
	pRecord->close();
	pLine->close();
}

void Commands::readModel()
{
	AcDbBlockTable *pBlkTbl;
	acdbHostApplicationServices()->workingDatabase()->getBlockTable(pBlkTbl, AcDb::kForRead);
	//获得模型空间的块表记录
	AcDbBlockTableRecord *pBlkTblRcd;
	pBlkTbl->getAt(ACDB_MODEL_SPACE, pBlkTblRcd, AcDb::kForRead);
	pBlkTbl->close();

	//创建块表记录遍历器
	AcDbBlockTableRecordIterator *pItr; //块表记录遍历器
	pBlkTblRcd->newIterator(pItr);
	pBlkTblRcd->close();
	AcDbEntity *pEnt = NULL; //遍历的临时实体指针
	for (pItr->start(); !pItr->done(); pItr->step()) {
		//利用遍历器获得每一个实体
		Acad::ErrorStatus es = pItr->getEntity(pEnt, AcDb::kForWrite);
		CString str;
		str.Format(_T("实体类型：%s"), pEnt->isA()->name());
		if(Acad::eOk!=es)
			continue;
		pEnt->close();

		if (pEnt->isKindOf(AcDbPolyline::desc()))//云线不能进去？但是pEnt->isA()->name()是AcDbPolyline
		{
			AcDbPolyline* pC = (AcDbPolyline*)pEnt;
			unsigned int c = pC->numVerts();
			double bulge = 0;
			pC->getBulgeAt(0, bulge);
		}
		if (pEnt->isKindOf(AcDbLine::desc()))
		{
			AcDbLine* pL = (AcDbLine*)pEnt;
			AcGeVector3d vec = pL->endPoint() - pL->startPoint();
			AcGePoint3d center = pL->startPoint() + vec / 2;
		}
	}
	delete pItr;
}

void Commands::createRadialDemension()
{
	AcDbCircle* pcir = new AcDbCircle(AcGePoint3d(100, 100, 0), AcGeVector3d::kZAxis, 100);
	CDwgDatabaseUtil::PostToModelSpace(pcir);
	AcDbRadialDimension* pDim = new AcDbRadialDimension(AcGePoint3d(100, 100, 0),
		AcGePoint3d(200, 100, 0), 50);
	CDwgDatabaseUtil::PostToModelSpace(pDim);
	AcDbRadialDimension* pDim2 = new AcDbRadialDimension(AcGePoint3d(100, 100, 0),
		AcGePoint3d(200, 100, 0), -20);
	CDwgDatabaseUtil::PostToModelSpace(pDim2);
}

void Commands::createDiametricDimension()
{
	AcDbCircle* pcir = new AcDbCircle(AcGePoint3d(100, 100, 0), AcGeVector3d::kZAxis, 100);
	CDwgDatabaseUtil::PostToModelSpace(pcir);
	AcDbDiametricDimension* pDim = new AcDbDiametricDimension(AcGePoint3d(200, 100, 0),
		AcGePoint3d(0, 100, 0), -20);
	CDwgDatabaseUtil::PostToModelSpace(pDim);
}

void Commands::createAngularDimension()
{
	//AcGePoint3d origin(0.0, 0.0, 0.0);
	//AcGePoint3d p1(100.0, 0.0, 0.0);
	//AcGePoint3d p2(100.0, 100.0, 0.0);
	//AcGePoint3d arcPt(-100.0, -1.0, 0.0);
	//AcDb2LineAngularDimension* pDim = new AcDb2LineAngularDimension(origin, p1, origin, p2, arcPt);
	//CDwgDatabaseUtil::PostToModelSpace(pDim);

	//（2）
	AcGePoint3d p1(-100.0, -100.0, 0.0);
	AcGePoint3d p2(100.0, 100.0, 0.0);
	AcGePoint3d p3(100.0, -100.0, 0.0);
	AcGePoint3d p4(-100.0, 100.0, 0.0);
	AcGePoint3d p(200.0, 0.0, 0.0);
	//AcDb2LineAngularDimension* pDim = new AcDb2LineAngularDimension(p1, p2, p3, p4, p);
	//CDwgDatabaseUtil::PostToModelSpace(pDim);

	//(2.2)
	AcDb2LineAngularDimension* pDim = new AcDb2LineAngularDimension(p2, p1, p3, p4, p);
	CDwgDatabaseUtil::PostToModelSpace(pDim);
}

void Commands::createLeader()
{
	//(1)引线
	AcDbLeader* pLeader = new AcDbLeader();
	pLeader->appendVertex(AcGePoint3d(0.0, 0.0, 0.0));
	pLeader->appendVertex(AcGePoint3d(100.0, 0.0, 0.0));
	pLeader->appendVertex(AcGePoint3d(200.0,100.0, 0.0));
	//pLeader->setDimensionStyle()
	CDwgDatabaseUtil::PostToModelSpace(pLeader);
	return;

	//(2)多重引线
	//AcDbMLeader *pEnt = new AcDbMLeader();
	//AcGePoint3d ptStart(0, 0, 0);
	//AcGePoint3d ptEnd1(-30, -10, 0);
	//AcGeVector3d vec(0, -1, 0);
	//int i;
	//pEnt->addLeader(i);
	//pEnt->addLeaderLine(ptStart, i);
	//pEnt->addLastVertex(i, ptEnd1);
	//pEnt->setDoglegLength(10);
	//pEnt->setDoglegDirection(i, vec);
	//CDwgDatabaseUtil::PostToModelSpace(pEnt);

	//(2.1)
	AcApDocument *pActiveDoc = acDocManager->mdiActiveDocument();
	AcDbDatabase *pDB = pActiveDoc->database();

	Acad::ErrorStatus es;
	AcDbBlockTable *pBT = NULL;
	es = pDB->getBlockTable(pBT, AcDb::OpenMode::kForRead);
	AcDbBlockTableRecord *pBTR = NULL;
	es = acdbOpenObject(pBTR, pDB->currentSpaceId(), AcDb::OpenMode::kForRead);

	AcDbObjectId arrowBlockId = AcDbObjectId::kNull;
	if (pBT->getAt(_T("Arrow"), arrowBlockId) != Acad::eOk)
	{
		pBT->close();
		return;
	}

	AcDbObjectId contentBlockId = AcDbObjectId::kNull;
	if (pBT->getAt(_T("Content"), contentBlockId) != Acad::eOk)
	{
		pBT->close();
		return;
	}
	AcGePoint3d startPt(0, 0, 0);
	AcGePoint3d endPt(-30, 10, 0);
	AcDbMLeader *pMLeader = new AcDbMLeader();
	pMLeader->setDatabaseDefaults();
	int leaderIndex = 0;
	pMLeader->addLeader(leaderIndex);
	int leaderLineIndex = 0;
	pMLeader->addLeaderLine(leaderIndex, leaderLineIndex);
	pMLeader->addFirstVertex(leaderLineIndex, startPt);
	pMLeader->addLastVertex(leaderLineIndex, endPt);
	AcGeVector3d vec(0, -1, 0);
	pMLeader->setDoglegLength(10);
	pMLeader->setDoglegDirection(leaderIndex, vec);		//不设置狗腿方向，文字永远在引线右侧
	pMLeader->setContentType(AcDbMLeaderStyle::kMTextContent);
	//pMLeader->setBlockConnectionType(AcDbMLeaderStyle::kConnectBase);
	pMLeader->setArrowSymbolId(arrowBlockId);
	
	//pMLeader->setBlockContentId(contentBlockId);
	AcDbMText *pMText = new AcDbMText();
	pMText->setDatabaseDefaults();
	pMText->setContents(ACRX_T("Autodesk"));
	//pMText->setLocation(endPt + vec * 10);		//没有效果
	pMLeader->setMText(pMText);
	pMLeader->setTextLocation(endPt + vec * 20);			//很关键
	//pMLeader->setTextAlignmentType(AcDbMLeaderStyle::kCenterAlignment);
	//CDwgDatabaseUtil::PostToModelSpace(pMLeader);
	
	pBTR->upgradeOpen();
	pBTR->appendAcDbEntity(pMLeader);
	pMText->close();
	pMLeader->close();
	pBTR->close();
	pBT->close();
}

void Commands::readLeader()
{
	std::list<AcDbObjectId> selectedIds;
	if (CDwgDatabaseUtil::GetSelObjects(TEXT(""), selectedIds))
	{
		AcDbEntity* pEnt = NULL;
		Acad::ErrorStatus es = acdbOpenObject(pEnt, selectedIds.front(), AcDb::kForRead);
		if (es == Acad::eOk)
		{
			if (pEnt->isA() ==AcDbMLeader::desc())
			{
				AcDbMLeader* pLeader = static_cast<AcDbMLeader*>(pEnt);
				int numLeaders = pLeader->numLeaders();
				int numLeaderLines = pLeader->numLeaderLines();
				int vertexNum;
				pLeader->numVertices(0, vertexNum);
				std::list<AcGePoint3d> pts;
				for (int i = 0; i != vertexNum; i++)
				{
					AcGePoint3d pt;
					pLeader->getVertex(0, i, pt);	//与用代码创建的多重引线不一定一致，cad会修改
					pts.push_back(pt);
				}
				double legLen;
				pLeader->doglegLength(0, legLen);
				AcGeVector3d vec;
				pLeader->getDoglegDirection(0, vec);
				AcGePoint3d textPos;
				pLeader->getTextLocation(textPos);
				AcGePoint3d textLoc = pLeader->mtext()->location();
				auto textAlignmentType = pLeader->textAlignmentType();	//
				auto textAttachmentDirection = pLeader->textAttachmentDirection();
				auto  textAttachmentType = pLeader->textAttachmentType();
			}
			else if (pEnt->isA() == AcDbLine::desc())
			{
				AcDbLine* pl = static_cast<AcDbLine*>(pEnt);
				AcCmColor color = pl->color();
				int r = color.red();
				int g = color.green();
				int b = color.blue();
				int colorIdx = pl->colorIndex();
				ACHAR* layer = pl->layer();
				AcDbObjectId layerId = pl->layerId();
				//AcDbEntity* pLayer = nullptr;
				//Acad::ErrorStatus es = acdbOpenObject(pLayer, layerId);
				//if (es == Acad::eOk)		//打开失败
				//{
				//	CString name = pLayer->isA()->name();
				//	pLayer->close();
				//	return;
				//}

				AcDbLayerTable *pLayerTable;
				acdbHostApplicationServices()->workingDatabase()->getSymbolTable(pLayerTable, AcDb::kForWrite);
				AcDbLayerTableRecord* pLTR = nullptr;
				Acad::ErrorStatus es = pLayerTable->getAt(layer, pLTR);
				if (es == Acad::eOk)
				{
					int layerColorIdx = pLTR->color().colorIndex();	//bylayer->7
					int layerRed = pLTR->color().red();		//bylayer->0
					int layerGreen = pLTR->color().green();//bylayer->0
					int layerBlue = pLTR->color().blue();//bylayer->0
					bool isByLayer = pLTR->color().isByLayer();
					bool isByACI = pLTR->color().isByACI();	//true，AutoCAD颜色索引
					bool isByBlock = pLTR->color().isByBlock();
					bool isByColor = pLTR->color().isByColor();
					bool isByPen = pLTR->color().isByPen();
					bool isForeground = pLTR->color().isForeground();
				}
				pLayerTable->close();
				pLTR->close();
			}
			else if (pEnt->isA() == AcDbText::desc())
			{
				AcDbText* text = static_cast<AcDbText*>(pEnt);
				const ACHAR* textStr = text->textStringConst();
				//text->setTextString();
				double widthFactor = text->widthFactor();
				double height = text->height();
				double rotate = text->rotation();
				double oblique = text->oblique();
				bool isMirrorX = text->isMirroredInX();
				bool isMirrorY = text->isMirroredInY();
			}
			else if (pEnt->isA() == AcDbMText::desc())
			{
				AcDbMText* mtext = static_cast<AcDbMText*>(pEnt);
				const ACHAR* textStr = mtext->text();		//L"你好abc\r\n文字123"
				const ACHAR* contents = mtext->contents();	//L"{\\fSimSun|b0|i0|c134|p2;你好}abc\\P{\\fSimSun|b0|i0|c134|p2;文字}123"
				const ACHAR* const oChange = mtext->obliqueChange();	// L"\\Q"
				//text没有set函数
				//mtext->setContents()
			}
		}

	}
}

void Commands::arcTest()
{
	//AcDbArc* arc1 = new AcDbArc(AcGePoint3d(0.0, 0.0, 0.0), AcGeVector3d(0.0, 0.0, 1.0),
	//	100, M_PI / 4.0,  M_PI);
	//CDwgDatabaseUtil::PostToModelSpace(arc1);
	//AcDbArc* arc2 = new AcDbArc(AcGePoint3d(200.0, 0.0, 0.0), AcGeVector3d(0.0, 0.0, -1.0),
	//	100, M_PI / 4.0, M_PI);
	//CDwgDatabaseUtil::PostToModelSpace(arc2);

	AcDbEllipse* ellipse1 = new AcDbEllipse(AcGePoint3d(0.0, 0.0, 0.0), AcGeVector3d(0.0, 0.0, 1.0),
		AcGeVector3d(100.0, 0.0, 0.0), 0.5, M_PI / 4.0, M_PI);	//radiusRatio不能大于1，否则有问题
	ellipse1->setStartParam(M_PI / 4.0);
	ellipse1->setEndParam(M_PI);
	CDwgDatabaseUtil::PostToModelSpace(ellipse1);
	AcDbEllipse* ellipse2 = new AcDbEllipse(AcGePoint3d(200.0, 0.0, 0.0), AcGeVector3d(0.0, 0.0, -1.0),
		AcGeVector3d(100.0, 0.0, 0.0), 0.5, M_PI / 4.0, M_PI);
	ellipse2->setStartParam(M_PI / 4.0);
	ellipse2->setEndParam(M_PI);
	CDwgDatabaseUtil::PostToModelSpace(ellipse2);
	AcDbEllipse* ellipse3 = new AcDbEllipse(AcGePoint3d(400.0, 0.0, 0.0), AcGeVector3d(0.0, 0.0, 1.0),
		AcGeVector3d(-100.0, 0.0, 0.0), 0.5, M_PI / 4.0, M_PI);
	ellipse3->setStartParam(M_PI / 4.0);
	ellipse3->setEndParam(M_PI);
	CDwgDatabaseUtil::PostToModelSpace(ellipse3);
	AcDbEllipse* ellipse4 = new AcDbEllipse(AcGePoint3d(600.0, 0.0, 0.0), AcGeVector3d(0.0, 0.0, -1.0),
		AcGeVector3d(-100.0, 0.0, 0.0), 0.5, M_PI / 4.0, M_PI);
	ellipse4->setStartParam(M_PI / 4.0);
	ellipse4->setEndParam(M_PI);
	CDwgDatabaseUtil::PostToModelSpace(ellipse4);

}

void Commands::readArc()
{
	std::list<AcDbObjectId> selectedIds;
	if (CDwgDatabaseUtil::GetSelObjects(TEXT(""), selectedIds))
	{
		AcDbEntity* pEnt = NULL;
		Acad::ErrorStatus es = acdbOpenObject(pEnt, selectedIds.front(), AcDb::kForRead);
		if (es == Acad::eOk)
		{
			if (pEnt->isA() == AcDbArc::desc())
			{
				AcDbArc* pArc = static_cast<AcDbArc*>(pEnt);
				AcGeVector3d normal = pArc->normal();
			}
		}
	}
}

void Commands::readMText()
{
	std::list<AcDbObjectId> selectedIds;
	if (CDwgDatabaseUtil::GetSelObjects(TEXT(""), selectedIds))
	{
		AcDbEntity* pEnt = NULL;
		Acad::ErrorStatus es = acdbOpenObject(pEnt, selectedIds.front(), AcDb::kForRead);
		if (es == Acad::eOk)
		{
			if (pEnt->isA() == AcDbMText::desc())
			{
				AcDbMText* pMText = static_cast<AcDbMText*>(pEnt);
				const ACHAR* contents = pMText->contents();
				CString cont(contents);
			}
		}
	}
}
