#pragma comment (lib, "ws2_32")

#include <stdio.h>
#include <WinSock2.h>

BOOL test_func()
{
	int value = 0x1234;
	int i = 0, result;
	char* origin_ptr = (char*)&value;
	char* result_ptr = (char*)&result;
	result = htons(value);

	printf("origin : 0x%x result : 0x%x\n", origin_ptr[0], result_ptr[0]);
	return origin_ptr[0] == result_ptr[0];
}

BOOL IsLittleEndian()
{
	return !test_func();
}

BOOL IsBigEndian()
{
	return test_func();
}

int main(int argc, char* argv)
{

	if (IsLittleEndian())
	{
		printf("�ش� �ý����� ��Ʋ ������Դϴ�.\n");
	}
	else if (IsBigEndian())
	{
		printf("�ش� �ý����� �� ������Դϴ�.\n");
	}
}