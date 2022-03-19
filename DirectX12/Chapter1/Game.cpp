#include <windows.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <iostream>
using namespace std;
using namespace DirectX;
using namespace DirectX::PackedVector;

ostream& XM_CALLCONV operator << (ostream& os, FXMVECTOR v)
{
	XMFLOAT3 dest;
	XMStoreFloat3(&dest, v);

	os << "(" << dest.x << ", " << dest.y << ", " << dest.z << ")";
	return os;
}

int main() {
	cout.setf(ios_base::boolalpha);

	//����Ƿ�֧��SSE2ָ�,(Pentium4,AMD K8��������汾������)
	if (!XMVerifyCPUSupport()) {
		cout << "��֧��DirectX" << endl;
		return 0;
	}

	XMVECTOR p = XMVectorZero();
	XMVECTOR q = XMVectorSplatOne();
	XMVECTOR u = XMVectorSet(1.0f,2.0f,3.0f,0.0f);
	XMVECTOR v = XMVectorReplicate(-2.0f);
	XMVECTOR m = XMVectorSplatZ(u);

	cout << "p = " << p << endl;
	cout << "q = " << q << endl;
	cout << "u = " << u << endl;
	cout << "v = " << v << endl;
	cout << "m = " << m << endl;

	XMVECTOR p1 = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
	XMVECTOR p2 = XMVectorSet(1.0f, 2.0f, 3.0f, 0.0f);
	XMVECTOR p3 = XMVectorSet(-2.0f, 1.0f, -3.0f, 0.0f);
	XMVECTOR p4 = XMVectorSet(0.707f, 0.707f, 0.0f, 0.0f);

	//�����ӷ�
	XMVECTOR r1 = p1 + p2;

	//��������
	XMVECTOR r2 = p1 - p2;

	//�����˷�
	XMVECTOR r3 = 10.0f * p1;

	//||p1||
	XMVECTOR r4 = XMVector3Length(p1);

	//p1��һ��
	XMVECTOR r5 = XMVector3Normalize(p1);

	//���
	XMVECTOR r6 = XMVector3Dot(p2,p3);//����Ľ����ֵ,���Ի��������ÿ��λ��

	//���
	XMVECTOR r7 = XMVector3Cross(p1, p2);

	//��ͶӰ
	XMVECTOR projW;//projW��p4��p1�ϵ�ͶӰ
	XMVECTOR perpW;//perpW��p4�� p1���������� �ķ���
	//����perpW��Ȼ��ֱ��projW
	XMVector3ComponentsFromNormal(&projW, &perpW, p4, p1);

	//projW+perpW == p4?
	bool equal = XMVector3Equal(projW + perpW, p4) != 0;

	//projW��perp֮��ļн�Ӧ����90��
	XMVECTOR angleVec = XMVector3AngleBetweenVectors(projW, perpW);
	float angleRadians = XMVectorGetX(angleVec);	//����ֵ
	float angleDegrees = XMConvertToDegrees(angleRadians);	//�Ƕ�ֵ

	cout << "p1 = " << p1 << endl;
	cout << "p2 = " << p2 << endl;
	cout << "p3 = " << p3 << endl;
	cout << "p4 = " << p4 << endl;
	cout << "r1 = p1 + p2 " << r1 << endl;
	cout << "r2 = p1 - p2 " << r2 << endl;
	cout << "r3 = 10.0 * p1 " << r3 << endl;
	cout << "r4 = ||p1|| " << r4 << endl;
	cout << "r5 = p1 / ||p1|| " << r5 << endl;
	cout << "r6 = p2 �� p3 " << r6 << endl;
	cout << "r7 = p1 x p2 " << r7 << endl;
	cout << "proW = " << projW << endl;
	cout << "�Ƕ� = " << angleDegrees << endl;


	return 0;
}