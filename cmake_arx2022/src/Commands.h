#pragma once
class Commands
{
public: 
	static void Init();
	static void Unload();

	//������
	static void CreateLine();
	//����ģ��
	static void readModel();
	//�����뾶��ע
	static void createRadialDemension();
	//����ֱ����ע
	static void createDiametricDimension();
	//�����Ƕȱ�ע
	static void createAngularDimension();
	//��������
	static void createLeader();
	//��ȡ����
	static void readLeader();

	static void arcTest();
	static void readArc();
};

