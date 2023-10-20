# Weekly#0 (10.15-10.22)

### Daily#1(10.16)

先嫩了个markdown编辑器

https://blog.csdn.net/qq_44627608/article/details/121734682

##### 软光栅（Software Renderer）

安装线性代数库eigen

https://blog.csdn.net/weixin_43940314/article/details/115456199

配置OpenGL环境：

https://blog.csdn.net/weixin_41962350/article/details/109345558

OpenGL基本操作：

https://zhuanlan.zhihu.com/p/103925920

配置FreeImage环境：

http://www.51hei.com/bbs/dpj-33593-1.html

##### 遇到的好几个坑：

解决引用FreeImage函数的报错（error LNK2019: 无法解析的外部符号）：

https://blog.csdn.net/bashana102/article/details/54956719

解决FreeImage保存的图片全黑：

禁用深度测试和模板测试。

![image-20231016153922585](C:\Users\Deli\AppData\Roaming\Typora\typora-user-images\image-20231016153922585.png)

解决蓝色图片经过Freeimage保存到本地后变为红色：

![image-20231016154126469](C:\Users\Deli\AppData\Roaming\Typora\typora-user-images\image-20231016154126469.png)

后来猥老师说要用ppm，遂删掉了OpenGL库，不得不说确实方便多了。

![image-20231016184949791](C:\Users\Deli\AppData\Roaming\Typora\typora-user-images\image-20231016184949791.png)

![image-20231016184933294](C:\Users\Deli\AppData\Roaming\Typora\typora-user-images\image-20231016184933294.png)



### Daily#3(10.18)

上午：上大β课

下午：上大β课

晚上：先去双创帮忙面试，然后修bug↓

​	修复：图片太大导致三角形显示不正确的bug（原因是判断两个int异号时溢出了）

​	修复：图片长宽不一致时显示不正确的bug（原因是遍历长宽时脑袋蒙了）

​	试着做了SSAA的第一步放大和第二步旋转，旋转效果并不理想，中间有许多空洞。原因大概是算三角函数的浮点，以及图像内旋转后的锯齿没有完全贴合。

放大前：

![Screenshot 2023-10-18 225926](E:\Projects\Unique\Weekly\0\Screenshot 2023-10-18 225926.png)

放大后：

![Screenshot 2023-10-18 225932](E:\Projects\Unique\Weekly\0\Screenshot 2023-10-18 225932.png)

放大并且旋转30°：![Screenshot 2023-10-18 225903](E:\Projects\Unique\Weekly\0\Screenshot 2023-10-18 225903.png)



### Daily#4(10.19)

上午：补觉

下午：补作业，计基实验、大雾、马++、离散

晚上：实现了三角形顶点有z轴深度后的坐标转换（世界→相机→透视→屏幕）然后渲染。



### Daily#5(10.20)

上午：补觉+体育课，已累瘫

下午：（c++课上摸鱼→）根据正方体的旋转后算出了顶点坐标，6个面当作12个三角形进行渲染。晚上实现了渲染的深度测试，修复了旋转后坐标不正确的bug。

