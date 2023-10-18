//
//
//
//
//#include <iostream>
//#include <Eigen/Dense>
//#include <windows.h>  // for MS Windows
//#include <GL/glut.h>  // GLUT, include glu.h and gl.h
//#include<FreeImage/FreeImage.h>
//#pragma comment(lib,"FreeImage.lib")
//using namespace std;
////using Eigen::MatrixXd;
//
//GLint x = 0;  // ��ʼ���ص�X����
//GLint y = 0;  // ��ʼ���ص�Y����
//GLsizei width = 100;  // ͼ����
//GLsizei height = 100; // ͼ��߶�
//int ssaa_times = 2; // SSAA����
////GLubyte pixels_plus[3000000]; // RGBA�������ݣ�ÿ������4���ֽ�
//GLubyte* pixels = new GLubyte[width * height * 4]; // RGBA�������ݣ�ÿ������4���ֽ�
//GLubyte* pixels_plus = new GLubyte[width * height * 4 * ssaa_times * ssaa_times];
//void MySavePSD()
//{
//    
//    glReadPixels(x, y, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
//
//    for (int i = 0; i < width * height; i++) {
//        int temp = pixels[i * 4 + 0];
//        pixels[i * 4 + 0] = pixels[i * 4 + 2]; // ��ɫͨ�� -> ��ɫͨ��
//        pixels[i * 4 + 1] = pixels[i * 4 + 1]; // ��ɫͨ�����ֲ���
//        pixels[i * 4 + 2] = temp;              // ��ɫͨ�� -> ��ɫͨ��
//        pixels[i * 4 + 3] = pixels[i * 4 + 3]; // ͸����ͨ�����ֲ���
//    }
//
//    //FIBITMAP* image = FreeImage_ConvertFromRawBits(pixels_plus, width, height, width * 4, 32, FI_RGBA_RED_MASK, FI_RGBA_GREEN_MASK, FI_RGBA_BLUE_MASK, false);
//    FIBITMAP* image = FreeImage_ConvertFromRawBits(pixels, width, height, width * 4, 32, 0x00FF0000, 0XFF000000, 0X0000FF00, false);
//    // ����ͼ��ΪPSD�ļ�
//    const char* outputFilename = "outputTEST_1.psd";  // ���ͼ���ļ�·��
//    FreeImage_Save(FIF_PSD, image, outputFilename);
//    // �ͷ�FIBITMAP��������������ڴ�
//    FreeImage_Unload(image);
//}
//void MySavePlusPSD()
//{
//    glReadPixels(x, y, width*ssaa_times, height * ssaa_times, GL_RGBA, GL_UNSIGNED_BYTE, pixels_plus);
//    for (int i = 0; i < width * height * ssaa_times * ssaa_times; i++) {
//        int temp = pixels_plus[i * 4 + 0];
//        pixels_plus[i * 4 + 0] = pixels_plus[i * 4 + 2]; // ��ɫͨ�� -> ��ɫͨ��
//        pixels_plus[i * 4 + 1] = pixels_plus[i * 4 + 1]; // ��ɫͨ�����ֲ���
//        pixels_plus[i * 4 + 2] = temp;              // ��ɫͨ�� -> ��ɫͨ��
//        pixels_plus[i * 4 + 3] = pixels_plus[i * 4 + 3]; // ͸����ͨ�����ֲ���
//    }
//    FIBITMAP* image = FreeImage_ConvertFromRawBits(pixels_plus, width * ssaa_times, height * ssaa_times, width * ssaa_times * 4, 32, 0x00FF0000, 0XFF000000, 0X0000FF00, false);
//    const char* outputFilename = "outputTEST_1_plus.psd";  // ���ͼ���ļ�·��
//    FreeImage_Save(FIF_PSD, image, outputFilename);
//    // �ͷ�FIBITMAP��������������ڴ�
//    FreeImage_Unload(image);
//}
//void MySetPixel(int index_pixel,int index_pixel_x,int index_pixel_y,int newLine,int newColumn)
//{
//    int index_pixel_plus = index_pixel_y *ssaa_times*4 + width*ssaa_times*ssaa_times*4* index_pixel_x + newLine* width * ssaa_times*4 + newColumn *4;
//    for (int i = 0; i < 4; i++)
//    {
//        pixels_plus[index_pixel_plus] = pixels[index_pixel];
//        index_pixel_plus++;
//        index_pixel++;
//    }
//}
//void SSAA()
//{
//    
//    int index_in_line = 0;
//    int index_in_column = 0;
//    for (int i = 0; i < width * height * 4; i+= 4, index_in_column++)
//    {
//        if (index_in_column == width)
//        {
//            index_in_column = 0;
//            index_in_line++;
//        }
//        for (int new_line = 0; new_line < ssaa_times; new_line++)
//        {
//            for (int new_column = 0; new_column < ssaa_times; new_column++)
//            {
//                MySetPixel(i,index_in_line,index_in_column,new_line,new_column);
//			}
//        }
//	}
//}
//void display() {
//    glDisable(GL_DEPTH_TEST);
//    glDisable(GL_STENCIL_TEST);
//    glClear(GL_COLOR_BUFFER_BIT);   // Clear the color buffer with current clearing color
//
//    glBegin(GL_TRIANGLES);          // Each set of 3 vertices form a triangle
//    glColor3f(0.0f, 0.0f, 1.0f); // Blue
//    glVertex2f(-0.5f, 0.5f);
//    glVertex2f(0.2f, 0.5f);
//    glVertex2f(0, -1);
//
//
//    //glColor3f(1.0f, 0.0f, 0.0f); // Red
//    //glVertex2f(0.3f, -0.4f);
//    //glColor3f(0.0f, 1.0f, 0.0f); // Green
//    //glVertex2f(0.9f, -0.4f);
//    //glColor3f(0.0f, 0.0f, 1.0f); // Blue
//    //glVertex2f(0.6f, -0.9f);
//    glEnd();
//    glFlush();
//}
//void MyIntreactiveKeys(unsigned char key, int x, int y)
//{
//    if (key == 'q')
//    {
//		exit(0);
//	}
//    else if (key == 's')
//    {
//		MySavePSD();
//        SSAA();
//        MySavePlusPSD();
//	}
//}
//
////int main(int argc, char** argv)
////{
////    glutCreateWindow("OpenGL Setup Test");
////    glutReshapeWindow(width, height);
////    glutPositionWindow(500, 500);
////    glutDisplayFunc(display);
////    //glReadPixels(x, y, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels_plus); 
////    //glReadPixels(x, y, width, height, GL_RGB, GL_UNSIGNED_BYTE, pixels_plus);
////    /*for (int i = 0; i < width * height * 4; i += 4)
////    {
////		pixels_plus[i] = 255;
////		pixels_plus[i + 1] = 0;
////		pixels_plus[i + 2] = 0;
////		pixels_plus[i + 3] = 255;
////	}*/
////    glutKeyboardFunc(MyIntreactiveKeys);
////    glutMainLoop();
////    return 0;
////}