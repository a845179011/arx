#pragma once
class Commands
{
public: 
	static void Init();
	static void Unload();

	//创建线
	static void CreateLine();
	//遍历模型
	static void readModel();
	//创建半径标注
	static void createRadialDemension();
	//创建直径标注
	static void createDiametricDimension();
	//创建角度标注
	static void createAngularDimension();
	//创建引线
	static void createLeader();
	//读取引线
	static void readLeader();

	static void arcTest();
	static void readArc();
};

