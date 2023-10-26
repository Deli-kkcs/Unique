#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#include <corecrt_math.h>
#include <time.h>
char file_pos[100] = "E:\\Projects\\Unique\\Renderer\\Project1\\TEST_1.ppm";
char filePlus_pos[100] = "E:\\Projects\\Unique\\Renderer\\Project1\\TEST_1_Plus.ppm";
char file90_pos[100] = "E:\\Projects\\Unique\\Renderer\\Project1\\TEST_1_90.ppm";
char fileSSAA_pos[100] = "E:\\Projects\\Unique\\Renderer\\Project1\\TEST_1_SSAA.ppm";
int width;
int height;
float viewDepth; //观察者在相机后方viewDepth个单位
int ssaa_times; // SSAA倍数
int index_face = 0;
int count_grid = 10;
int red[12] =   { 0xff,0xff,0xbc,0x5b,0x05,0x3d,0x3d,0x3d,0x00,0x00,0xcd,0xff };
int green[12] = { 0x00,0x6b,0x05,0x05,0x06,0x7c,0xb2,0xf4,0xe0,0xff,0xff,0x9a };
int blue[12] =  { 0x00,0x6b,0x05,0xff,0xff,0xfa,0xfa,0xfa,0x78,0x03,0x00,0x00 };

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

class Vector3
{
public:
	float x, y, z;
	Vector3() {};
	Vector3(float x, float y, float z)
		: x(x), y(y), z(z) {};
};
void RotateVector3(Vector3 vertex_center,Vector3* vec, float rx, float ry, float rz)
{
	float radian_x = rx * 3.1415926f / 180.0f;
	float radian_y = ry * 3.1415926f / 180.0f;
	float radian_z = rz * 3.1415926f / 180.0f;
	float x = vec->x;
	float y = vec->y;
	float z = vec->z;
	vec->x = vertex_center.x+x * cos(radian_y) * cos(radian_z) + y * (sin(radian_x) * sin(radian_y) * cos(radian_z) - cos(radian_x) * sin(radian_z)) + z * (cos(radian_x) * sin(radian_y) * cos(radian_z) + sin(radian_x) * sin(radian_z));
	vec->y = vertex_center.y+x * cos(radian_y) * sin(radian_z) + y * (sin(radian_x) * sin(radian_y) * sin(radian_z) + cos(radian_x) * cos(radian_z)) + z * (cos(radian_x) * sin(radian_y) * sin(radian_z) - sin(radian_x) * cos(radian_z));
	vec->z = vertex_center.z-x * sin(radian_y) + y * sin(radian_x) * cos(radian_y) + z * cos(radian_x) * cos(radian_y);
}

class Triangle
{
public:
	float x1, y1,z1;
	float x2, y2,z2;
	float x3, y3,z3;
	Triangle() {};
	/*Triangle(float x1, float y1, float x2, float y2, float x3, float y3)
		: x1(x1), y1(y1), x2(x2), y2(y2), x3(x3), y3(y3) {};*/
	Triangle(Vector3 v1,Vector3 v2,Vector3 v3)
		:x1(v1.x),y1(v1.y),z1(v1.z),x2(v2.x),y2(v2.y),z2(v2.z),x3(v3.x),y3(v3.y),z3(v3.z){};
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
//将三角形的相机坐标转换为透视坐标,x,y结果在[-1,1]范围外不显示在相机中
void ImageToPerspective(Triangle* tri)
{
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
bool IsInTriangle(int x, int y, Triangle tri)
{
	float A_X = tri.x1;
	float A_Y = tri.y1;
	float B_X = tri.x2;
	float B_Y = tri.y2;
	float C_X = tri.x3;
	float C_Y = tri.y3;
	float t1 = CrossMultiply(x - A_X, y - A_Y, B_X - A_X, B_Y - A_Y);		// AX x AB
	float t2 = CrossMultiply(C_X - A_X, C_Y - A_Y, B_X - A_X, B_Y - A_Y);	// AC x AB
	if (CheckDiffirentSign(t1, t2))
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
bool IsInGrid(int x, int y, Triangle tri)
{
	return true;
}
class Cube
{
public:
	Vector3 center;		//中心点
	float length;		//边长
	float rx, ry, rz;	//旋转角度
	Vector3 vertex[8];	//八个顶点
	Triangle face[12];	//六个面的12个三角形

	Cube(Vector3 v, float length, float rx, float ry, float rz)
		:center(v), length(length), rx(rx), ry(ry), rz(rz)
	{	
		vertex[0] = Vector3(- length / 2, - length / 2, - length / 2);
		vertex[1] = Vector3(- length / 2, - length / 2, + length / 2);
		vertex[2] = Vector3(- length / 2, + length / 2, - length / 2);
		vertex[3] = Vector3(- length / 2, + length / 2, + length / 2);
		vertex[4] = Vector3(+ length / 2, - length / 2, - length / 2);
		vertex[5] = Vector3(+ length / 2, - length / 2, + length / 2);
		vertex[6] = Vector3(+ length / 2, + length / 2, - length / 2);
		vertex[7] = Vector3(+ length / 2, + length / 2, + length / 2);
		//根据rx ry rz旋转顶点位置
		for (int i = 0; i < 8; i++)
			RotateVector3(center,&vertex[i], rx, ry, rz);
		face[0] = Triangle(vertex[0],vertex[1],vertex[2]);
		face[1] = Triangle(vertex[3],vertex[1],vertex[2]);

		face[2] = Triangle(vertex[4],vertex[5],vertex[6]);
		face[3] = Triangle(vertex[7],vertex[5],vertex[6]);

		face[4] = Triangle(vertex[1],vertex[3],vertex[5]);
		face[5] = Triangle(vertex[7],vertex[3],vertex[5]);

		face[6] = Triangle(vertex[0],vertex[2],vertex[4]);
		face[7] = Triangle(vertex[6],vertex[2],vertex[4]);

		face[8] = Triangle(vertex[2],vertex[3],vertex[6]);
		face[9] = Triangle(vertex[7],vertex[3],vertex[6]);

		face[10] = Triangle(vertex[0],vertex[1],vertex[4]);
		face[11] = Triangle(vertex[5],vertex[1],vertex[4]);

	};
};

class _ppm
{
private:
	int pX;								//类型
	int sizeX, sizeY;					//大小
	unsigned char maxColor;				//颜色范围
	unsigned char* pixels;				//保存像素
	unsigned char* pixels_plus;
	unsigned char* pixels_rotated;
	unsigned char* pixels_SSAA;
	float* depth;
public:
	_ppm() { Initialize(); };
	void Initialize();
	void WriteImage(char* ch);
	void WriteImagePlus();
	void WriteImageRotated();
	void WriteImageSSAA();
	void DrawTriangle(Triangle tri);
	void SSAA();
	void Rotate(float degree);
	void MySetPixel_plus(int index_pixel, int index_pixel_x, int index_pixel_y, int newLine, int newColumn);
	void MySetPixel_SSAA(int index_pixel, int value);
	int MyGetPixel(int index_pixel_x, int index_pixel_y, int newLine, int newColumn);
	bool IsCovered(int x, int y, Triangle tri);
};
bool _ppm::IsCovered(int x, int y, Triangle tri)
{
	
	//求三角形平面方程Ax + By + Cz + D = 0
	float z = -1.f;
	float A = (tri.y2 - tri.y1) * (tri.z3 - tri.z1) - (tri.z2 - tri.z1) * (tri.y3 - tri.y1);
	float B = (tri.z2 - tri.z1) * (tri.x3 - tri.x1) - (tri.x2 - tri.x1) * (tri.z3 - tri.z1);
	float C = (tri.x2 - tri.x1) * (tri.y3 - tri.y1) - (tri.y2 - tri.y1) * (tri.x3 - tri.x1);
	float D = 0 - (A * tri.x1 + B * tri.y1 + C * tri.z1);
	//根据x y确定三角形平面上的z
	if (C != 0)
	{
		z = 0 - (A * x + B * y + D) / C;
	}
	else
		return false;
	if (depth[x * sizeY + y] == 0 || depth[x * sizeY + y] >= z)
	//if (depth[x * sizeY + y] <= z)
	{
		depth[x * sizeY + y] = z;
		return false;
	}
	return true;
}
void _ppm::Initialize()
{	
	//pX = 5;	//黑白图片
	pX = 6;		//彩色图片
	sizeX = height;//x，高度
	sizeY = width;//y，宽度
	maxColor = 255;//色彩范围
	if (pX == 6)
		pixels = (unsigned char*)calloc(sizeX * sizeY * 3, sizeof(unsigned char));
	if (pX == 5)
		pixels = (unsigned char*)calloc(sizeX * sizeY, sizeof(unsigned char));
	depth = (float*)calloc(sizeX * sizeY, sizeof(float));
}
void _ppm::WriteImage(char* ch)
{
	FILE* fp = fopen(ch, "wb");
	fprintf(fp, "P%d\n%d\n%d\n%d\n", pX, sizeY, sizeX, maxColor);
	if (pX == 5)
	{
		fwrite(pixels, sizeof(unsigned char), sizeX * sizeY, fp);
	}
	else if (pX == 6)
	{
		fwrite(pixels, sizeof(unsigned char), sizeX * sizeY * 3, fp);
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
void _ppm::WriteImageSSAA()
{
	FILE* fp = fopen(fileSSAA_pos, "wb");
	fprintf(fp, "P%d\n%d\n%d\n%d\n", 6, width, height, 255);
	fwrite(pixels_SSAA, sizeof(unsigned char), width * height * 3, fp);
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
void _ppm::DrawTriangle(Triangle tri)
{
	for (int i = 0; i < sizeX; i++)
	{
		for (int j = 0; j < sizeY; j++)
		{
			if (IsInTriangle(i, j, tri))
			{
				if (!IsCovered(i, j, tri))
				{
					if (IsInGrid(i, j, tri))
					{
						*(pixels + (i * sizeY + j) * 3 + 0) = red[index_face];
						*(pixels + (i * sizeY + j) * 3 + 1) = green[index_face];
						*(pixels + (i * sizeY + j) * 3 + 2) = blue[index_face];
					}
					else
					{
						*(pixels + (i * sizeY + j) * 3 + 0) = 0;
						*(pixels + (i * sizeY + j) * 3 + 1) = 0;
						*(pixels + (i * sizeY + j) * 3 + 2) = 0;
					}
					
				}
			}
		}
	}
}
void _ppm::SSAA()
{
	
	pixels_SSAA = (unsigned char *)calloc(width * height * 3, sizeof(unsigned char));

	int index_in_line = 0;
	int index_in_column = 0;
	for (int i = 0; ; i += 3* ssaa_times, index_in_column++)
	{
		if (index_in_column * ssaa_times == width)
		{
			index_in_column = 0;
			index_in_line++;
		}
		if (index_in_line * ssaa_times == height)
			break;
		int total[3] = {0,0,0};

		for (int newLine = 0; newLine < ssaa_times; newLine++)
		{
			for (int newColumn = 0; newColumn < ssaa_times; newColumn++)
			{
				int index_pixel = index_in_column * ssaa_times * 3 + width * ssaa_times * 3 * index_in_line + newLine * width * 3 + newColumn * 3;
				for(int i = 0;i<3;i++)
					total[i] += pixels[index_pixel+i];
			}
		}
		for (int i = 0; i < 3; i++)
		{
			int index_pixel_SSAA = (index_in_line * width / ssaa_times + index_in_column) * 3;
			pixels_SSAA[index_pixel_SSAA+i] = total[i] / (ssaa_times * ssaa_times);
		}
	}
	width /= ssaa_times;
	height /= ssaa_times;
	WriteImageSSAA();
	//WriteImagePlus();
	//Rotate(45.0f);
	//WriteImageRotated();
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
void _ppm::MySetPixel_plus(int index_pixel, int index_pixel_x, int index_pixel_y, int newLine, int newColumn)
{
	int index_pixel_plus = index_pixel_y * ssaa_times * 3 + width * ssaa_times * ssaa_times * 3 * index_pixel_x + newLine * width * ssaa_times * 3 + newColumn * 3;
	for (int i = 0; i < 3; i++)
	{
		pixels_plus[index_pixel_plus] = *(pixels+index_pixel);
		index_pixel_plus++;
		index_pixel++;
	}
}

void DrawCube(Cube cube,_ppm pixels)
{
	for (int i = 0; i < 12; i++)
	{
		index_face = i;
		ImageToPerspective(&cube.face[i]);
		PerspectiveToScreen(&cube.face[i]);
		/*printf("tri2.x1 = %f,tri2.y1 = %f\n", cube.face[i].x1, cube.face[i].y1);
		printf("tri2.x2 = %f,tri2.y2 = %f\n", cube.face[i].x2, cube.face[i].y2);
		printf("tri2.x3 = %f,tri2.y3 = %f\n", cube.face[i].x3, cube.face[i].y3);
		printf("________________________________\n");*/
		printf("face : %d\n", index_face);
		pixels.DrawTriangle(cube.face[i]);
	}
	printf("\n");
}
int main()
{
	bool isSSAA = true;
	ssaa_times = 2;
	width = 800;
	height = 800;
	viewDepth = 1.f;
	_ppm img;
	/*
	Triangle tri2(Vector3(.70f, .10f, 100.f), Vector3(.20f, .60f, 100.f), Vector3(.80f, .60f, 100.f));
	
	
	RelativeToAbsolute(&tri2);
	ImageToPerspective(&tri2);
	PerspectiveToScreen(&tri2);
	img.DrawTriangle(tri2);
	
	img.WriteImage(file_pos);
	printf("tri2.x1 = %f,tri2.y1 = %f\n", tri2.x1, tri2.y1);
	printf("tri2.x2 = %f,tri2.y2 = %f\n", tri2.x2, tri2.y2);
	printf("tri2.x3 = %f,tri2.y3 = %f\n", tri2.x3, tri2.y3);*/
	
	Cube cube(Vector3(0, 0, 60), 20, 45, 45, 0);
	Cube cube2(Vector3(10, 10, 60), 20, 10, 20, 10);
	DrawCube(cube,img);
	DrawCube(cube2, img);
	img.WriteImage(file_pos);


	if (isSSAA)
	{
		width *= ssaa_times;
		height *= ssaa_times;
		img.Initialize();
		DrawCube(cube, img);
		DrawCube(cube2, img);
		img.SSAA();
	}
	
	
	return 0;
}