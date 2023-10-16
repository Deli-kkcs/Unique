#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
char file_pos[50] = "E:\\Projects\\Unique\\Renderer\\Project1\\TEST_1.ppm";
class Triangle
{
public:
	float x1, y1;
	float x2, y2;
	float x3, y3;
	Triangle(float x1, float y1, float x2, float y2, float x3, float y3)
		: x1(x1), y1(y1), x2(x2), y2(y2), x3(x3), y3(y3) {}
};

class _ppm
{
private:
	int pX;								//类型
	int sizeX, sizeY;					//大小
	unsigned char maxColor;				//颜色范围
	unsigned char* image;				//保存像素

public:
	~_ppm()
	{
		if (image != NULL)
			free(image);
		image = NULL;
	}
	void write_image(char* ch);
	void creat_triangle(int width,int height,Triangle tri);
};
//struct Vector2
//{
//
//};
int CrossMultiply(int x1, int y1, int x2, int y2)
{
	return x1 * y2 - x2 * y1;
}
bool IsInTriangle(int width,int height, int x, int y, Triangle tri)
{
	float A_X = tri.x1 * width;
	float A_Y = tri.y1 * height;
	float B_X = tri.x2 * width;
	float B_Y = tri.y2 * height;
	float C_X = tri.x3 * width;
	float C_Y = tri.y3 * height;
	int t1 = CrossMultiply(x - A_X, y - A_Y, B_X - A_X, B_Y - A_Y);		// AX x AB
	int t2 = CrossMultiply(C_X - A_X, C_Y - A_Y, B_X - A_X, B_Y - A_Y);	// AC x AB
	if(t1 * t2 < 0)
		return false;
	int t3 = CrossMultiply(x - B_X, y - B_Y, C_X - B_X, C_Y - B_Y);		// BX x BC
	int t4 = CrossMultiply(A_X - B_X, A_Y - B_Y, C_X - B_X, C_Y - B_Y);	// BA x BC
	if(t3 * t4 < 0)
		return false;
	int t5 = CrossMultiply(x - C_X, y - C_Y, A_X - C_X, A_Y - C_Y);		// CX x CA
	int t6 = CrossMultiply(B_X - C_X, B_Y - C_Y, A_X - C_X, A_Y - C_Y);	// CB x CA
	if(t5 * t6 < 0)
		return false;
	return true;
}

void _ppm::write_image(char* ch)
{//图片输出路径
	FILE* fp = fopen(ch, "wb");
	fprintf(fp, "P%d\n%d\n%d\n%d\n", pX, sizeX, sizeY, maxColor);
	if (pX == 5)
	{
		fwrite(image, sizeof(unsigned char), sizeX * sizeY, fp);
	}
	else if (pX == 6)
	{
		fwrite(image, sizeof(unsigned char), sizeX * sizeY * 3, fp);
	}
	fclose(fp);
	fp = NULL;
}

void _ppm::creat_triangle(int width, int height, Triangle tri)
{
	//pX = 5;	//黑白图片
	pX = 6;		//彩色图片
	sizeX = width;//长度
	sizeY = height;//宽度
	maxColor = 255;//色彩范围
	if(pX == 6)
		image = (unsigned char*)calloc(sizeX * sizeY*3, sizeof(unsigned char));
	if (pX == 5)
		image = (unsigned char*)calloc(sizeX * sizeY, sizeof(unsigned char));
	for (int i = 0; i < sizeX; i++)
	{
		for (int j = 0; j < sizeY; j++)
		{
			if (IsInTriangle(this->sizeX,this->sizeY,i,j,tri))
				*(image + (i * sizeY + j) * 3 + 0) = 255;
			else
				*(image + (i * sizeY + j) * 3 + 0) = 0;
			*(image+(i * sizeY + j) * 3 + 1) = 0;
			*(image+(i * sizeY + j) * 3 + 2) = 0;
		}
	}
}

int main()
{
	_ppm img;
	Triangle tri(.3f, .2f, .2f, 0.6f, .6f, .8f);
	img.creat_triangle(400,400, tri);
	img.write_image(file_pos);

	return 0;
}