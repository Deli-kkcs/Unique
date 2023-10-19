#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <corecrt_math.h>
char file_pos[100] = "E:\\Projects\\Unique\\Renderer\\Project1\\TEST_1.ppm";
char filePlus_pos[100] = "E:\\Projects\\Unique\\Renderer\\Project1\\TEST_1_Plus.ppm";
char file90_pos[100] = "E:\\Projects\\Unique\\Renderer\\Project1\\TEST_1_90.ppm";
int width = 100;
int height = 100;
int ssaa_times = 2; // SSAA倍数

class Triangle
{
public:
	float x1, y1,z1;
	float x2, y2,z2;
	float x3, y3,z3;
	Triangle() {};
	Triangle(float x1, float y1, float x2, float y2, float x3, float y3)
		: x1(x1), y1(y1), x2(x2), y2(y2), x3(x3), y3(y3) {};
	Triangle(float x1, float y1,float z1, float x2, float y2,float z2, float x3, float y3,float z3)
		: x1(x1), y1(y1),z1(z1), x2(x2), y2(y2),z2(z2), x3(x3), y3(y3),z3(z3) {};
};
//[-1,1]的倍数转换成绝对像素坐标
void RelativeToAbsolute(Triangle* tri)
{
	tri->x1 = tri->x1*height;
	tri->y1 = tri->y1*width;
	tri->x2 = tri->x2*height;
	tri->y2 = tri->y2*width;
	tri->x3 = tri->x3*height;
	tri->y3 = tri->y3*width;
}
//将三角形的相机坐标转换为透视坐标,观察者在相机后方viewDepth个单位,x,y结果在[-1,1]范围外不显示在相机中
void ImageToPerspective(Triangle* tri)
{
	float viewDepth = 1.f;
	tri->x1 /= (tri->z1 / viewDepth + 1);
	tri->y1 /= (tri->z1 / viewDepth + 1);
	tri->z1 /= (tri->z1 / viewDepth + 1);
	tri->x2 /= (tri->z2 / viewDepth + 1);
	tri->y2 /= (tri->z2 / viewDepth + 1);
	tri->z2 /= (tri->z2 / viewDepth + 1);
	tri->x3 /= (tri->z3 / viewDepth + 1);
	tri->y3 /= (tri->z3 / viewDepth + 1);
	tri->z3 /= (tri->z3 / viewDepth + 1);
}
//将三角形的透视坐标转换为屏幕坐标
void PerspectiveToScreen(Triangle* tri)
{
	tri->x1 = (tri->x1 + 1) / 2 * height;
	tri->y1 = (tri->y1 + 1) / 2 * width;
	tri->x2 = (tri->x2 + 1) / 2 * height;
	tri->y2 = (tri->y2 + 1) / 2 * width;
	tri->x3 = (tri->x3 + 1) / 2 * height;
	tri->y3 = (tri->y3 + 1) / 2 * width;
}
class Cube
{
public:
	float x, y, z;		//中心点
	float length;		//边长
	float rx, ry, rz;	//旋转角度
	Triangle face[12];	//六个面的12个三角形

	Cube(float x, float y, float z, float length, float rx, float ry, float rz)
		:x(x), y(y), z(z), length(length), rx(rx), ry(ry), rz(rz)
	{	

	};
};
//TODO 将cude的六个面分别用三角形表示


class _ppm
{
private:
	int pX;								//类型
	int sizeX, sizeY;					//大小
	unsigned char maxColor;				//颜色范围
	unsigned char* image;				//保存像素
	unsigned char* pixels_plus;
	unsigned char* pixels_rotated;
public:
	~_ppm()
	{
		if (image != NULL)
			free(image);
		image = NULL;
	}
	void write_image(char* ch);
	void WriteImagePlus();
	void WriteImageRotated();
	void creat_triangle(int width,int height,Triangle tri);
	void SSAA();
	void Rotate(float degree);
	void MySetPixel(int index_pixel, int index_pixel_x, int index_pixel_y, int newLine, int newColumn);
};

bool CheckDiffirentSign(float x, float y)
{
	if (x >= 0 && y <= 0)
		return true;
	if (x <= 0 && y >= 0)
		return true;
	return false;
}
float CrossMultiply(float x1, float y1, float x2, float y2)
{
	return x1 * y2 - x2 * y1;
}
bool IsInTriangle(int height,int width, int x, int y, Triangle tri)
{
	float A_X = tri.x1;
	float A_Y = tri.y1;
	float B_X = tri.x2;
	float B_Y = tri.y2;
	float C_X = tri.x3;
	float C_Y = tri.y3;
	float t1 = CrossMultiply(x - A_X, y - A_Y, B_X - A_X, B_Y - A_Y);		// AX x AB
	float t2 = CrossMultiply(C_X - A_X, C_Y - A_Y, B_X - A_X, B_Y - A_Y);	// AC x AB
	if(CheckDiffirentSign(t1,t2))
		return false;
	float t3 = CrossMultiply(x - B_X, y - B_Y, C_X - B_X, C_Y - B_Y);		// BX x BC
	float t4 = CrossMultiply(A_X - B_X, A_Y - B_Y, C_X - B_X, C_Y - B_Y);	// BA x BC
	if (CheckDiffirentSign(t3, t4))
		return false;
	float t5 = CrossMultiply(x - C_X, y - C_Y, A_X - C_X, A_Y - C_Y);		// CX x CA
	float t6 = CrossMultiply(B_X - C_X, B_Y - C_Y, A_X - C_X, A_Y - C_Y);	// CB x CA
	if (CheckDiffirentSign(t5, t6))
		return false;
	return true;
}

void _ppm::write_image(char* ch)
{//图片输出路径
	FILE* fp = fopen(ch, "wb");
	fprintf(fp, "P%d\n%d\n%d\n%d\n", pX, sizeY, sizeX, maxColor);
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
void _ppm::WriteImagePlus()
{
	FILE* fp = fopen(filePlus_pos, "wb");
	fprintf(fp, "P%d\n%d\n%d\n%d\n", 6, width * ssaa_times, height * ssaa_times, 255);
	fwrite(pixels_plus, sizeof(unsigned char), width * height * 3 * ssaa_times * ssaa_times, fp);
	fclose(fp);
	fp = NULL;
}
void _ppm::WriteImageRotated()
{
	FILE* fp = fopen(file90_pos, "wb");
	fprintf(fp, "P%d\n%d\n%d\n%d\n", 6, width * ssaa_times, height * ssaa_times, 255);
	fwrite(pixels_rotated, sizeof(unsigned char), width * height * 3 * ssaa_times * ssaa_times, fp);
	fclose(fp);
	fp = NULL;
}
void _ppm::creat_triangle(int width, int height, Triangle tri)
{
	//pX = 5;	//黑白图片
	pX = 6;		//彩色图片
	sizeX = height;//x，高度
	sizeY = width;//y，宽度
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
void _ppm::SSAA()
{
	pixels_plus = (unsigned char *)calloc(width * height * 3*ssaa_times*ssaa_times, sizeof(unsigned char));
	int index_in_line = 0;
	int index_in_column = 0;
	for (int i = 0; i < width * height * 3; i += 3, index_in_column++)
	{
		if (index_in_column == width)
		{
			index_in_column = 0;
			index_in_line++;
		}
		for (int new_line = 0; new_line < ssaa_times; new_line++)
		{
			for (int new_column = 0; new_column < ssaa_times; new_column++)
			{
				_ppm::MySetPixel(i, index_in_line, index_in_column, new_line, new_column);
			}
		}
	}
	WriteImagePlus();
	Rotate(45.0f);
	WriteImageRotated();
}
//pixels_plus逆时针旋转degree度到pixels_rotated
void _ppm::Rotate(float degree)
{
	pixels_rotated = (unsigned char*)calloc(sizeX * sizeY * 3 * ssaa_times * ssaa_times, sizeof(unsigned char));
	for (int line = 0; line < sizeX * ssaa_times; line++)
		for (int column = 0; column < sizeY * ssaa_times; column++)
		{
			float radian = degree * 3.1415926f / 180.0f;
			float center_x = sizeX * ssaa_times / 2.0f;
			float center_y = sizeY * ssaa_times / 2.0f;
			int newLine = (line - center_x) * cos(radian) - (column - center_y / 2.0f) * sin(radian) + center_x;
			int newColumn = (line - center_x) * sin(radian) + (column - center_y / 2.0f) * cos(radian) + center_y;
			if (newLine >= 0 && newLine < sizeX * ssaa_times && newColumn >= 0 && newColumn < sizeY * ssaa_times)
			{
				int index_pixel = line * sizeY * ssaa_times * 3 + column * 3;
				int index_pixel_rotated = newLine * sizeY * ssaa_times * 3 + newColumn * 3;
				for (int i = 0; i < 3; i++)
				{
					pixels_rotated[index_pixel_rotated] = pixels_plus[index_pixel];
					index_pixel_rotated++;
					index_pixel++;
				}
			}
		}
}

void _ppm::MySetPixel(int index_pixel, int index_pixel_x, int index_pixel_y, int newLine, int newColumn)
{
	int index_pixel_plus = index_pixel_y * ssaa_times * 3 + width * ssaa_times * ssaa_times * 3 * index_pixel_x + newLine * width * ssaa_times * 3 + newColumn * 3;
	for (int i = 0; i < 3; i++)
	{
		pixels_plus[index_pixel_plus] = *(image+index_pixel);
		index_pixel_plus++;
		index_pixel++;
	}
}

int main()
{
	_ppm img;

	Triangle tri2(-0.7f, 0.5f,100.f, 0.6f, 0.8f, 100.f, -0.2f, -0.4f, 100.f);
	RelativeToAbsolute(&tri2);
	ImageToPerspective(&tri2);
	PerspectiveToScreen(&tri2);

	img.creat_triangle(width, height, tri2);
	img.write_image(file_pos);
	printf("tri2.x1 = %f,tri2.y1 = %f\n", tri2.x1, tri2.y1);
	printf("tri2.x2 = %f,tri2.y2 = %f\n", tri2.x2, tri2.y2);
	printf("tri2.x3 = %f,tri2.y3 = %f\n", tri2.x3, tri2.y3);
	return 0;
}