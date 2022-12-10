#ifndef CDWGDATABASEUTIL_H 
#define CDWGDATABASEUTIL_H
#include <list>

class CDwgDatabaseUtil
{
public:
	static AcDbObjectId PostToModelSpace(AcDbEntity* pEnt);
	static bool GetSelObjects(const TCHAR * prompt, std::list<AcDbObjectId>& selIds);
};

#endif // CDWGDATABASEUTIL_H 


