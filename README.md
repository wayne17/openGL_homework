# openGL_homework

西工大莫蓉老师所授的openGL课程大作业的代码（cpp），献丑分享出来大家共同学习！

## 题目
![image](https://user-images.githubusercontent.com/22266512/159003475-7b3a3622-0619-4fee-b742-21f73a087208.png)

![image](https://user-images.githubusercontent.com/22266512/159003557-886a66e0-d039-4040-99e3-db5f861a44d4.png)


## 思路

### 作业一
因为此题目涉及到平移、旋转、缩放等操作，所以拟设计以下的参数方程来确定等边三角形的三个点坐标（ver1.0版本）

![image](https://user-images.githubusercontent.com/22266512/159008723-5d6e2c8e-0e8e-4017-9056-4fcf336204ce.png)

![image](https://user-images.githubusercontent.com/22266512/159008883-410aba2a-0d41-486a-82af-0f559e36ccb2.png)

为了实现动画过程，采用了定时器机制，通过调整定时器执行间隔时间来生成变换动画，并通过该参数来实现变换速度的调节。根据不同的变换操作，在定时器执行间隔内对相应的变量进行增减,同时线型（线条、填充）的控制即可通过不同的菜单回调函数实现。菜单的话，在动画渲染前进行菜单注册，并对不同的操作绑定不同的变换函数。代码见task2ver1.0.cpp

后来做第二题时发现一种更简单的实现方法，即通过对坐标系进行平移、旋转、缩放变换来实现相应的变换效果，而且OPENGL对这三种操作都提供了现成的函数。那样的话就不需要再时刻精确地计算三点坐标，大大降低代码量。然后定时器的话也可以完全用for循环来替代，以死循环来实现动画绘制生成，用变量变化的步长来控制变换速度的快慢。相应的代码见task2ver2.0.cpp。

### 作业二
由要求中的零件示意图可以看出该零件有好几个面，而且包括一些很难常规方法很难实现的凹面，比如四个红色通孔以及中间的蓝色凹面。最初的想法是opengl里有没有一些平面（以及曲面）截取的函数，这样在画出一个长方体及通孔的前提下通过一些平面及曲面的截取来实现零件效果，但经过代码实践后发现opengl只有平面截取的函数，没有曲面截取的函数，而且该实验还要求要实现控制旋转功能，但平面截取的函数貌似是基于世界坐标系而非当前回吐坐标系，所以在控制零件旋转时，设置的截取平面并不会跟着旋转，所以也达不到要求的效果，所以放弃此法。
后来将零件分为以下六大块分别绘制：

![image](https://user-images.githubusercontent.com/22266512/159009569-0919f4de-f35d-4ff7-a43c-2ce2c14c04f4.png)

对于图中(a)，(c)，(d)，(f)中通孔的处理，这里采用先画内部圆柱面并取消消隐选项，来使得圆柱面内侧显示出渲染效果，然后对于通孔的前后两侧采用以下方法来绘制面： 

![image](https://user-images.githubusercontent.com/22266512/159009659-6390f904-177b-4c0e-9353-8e540059ad61.png)

对于前侧面图，先分成1，2，3，4四个矩形，然后还有a，b，c，d四个弧边三角形，矩形就使用GL_QUADS方法绘制，弧三角形则先对弧边进行微分采样（获取坐标），然后使用GL_TRIANGLE_FAN方法进行绘制，后侧面的绘制过程与此同理，上下左右四个面则都是规则的矩形面，使用GL_QUADS方法绘制即可。需要注意的是六大块图中(a)，(c)中通孔并不是规则的圆柱面，因此需要使用空间中三维圆的参数方程来采样圆的坐标点进行绘制。想要实现背景图不随绘制的零件图的旋转而旋转只需要将背景图的绘制放在glPushMatrix()和glPopMatrix()方法的前面，而将旋转变换代码放到glPushMatrix()和glPopMatrix()之间，这样即可实现所需效果。

鼠标拖拽视角实现：鼠标左键按下拖拽视角实现主要依托于glutMouseFunc(mouseCB);，glutMotionFunc(mouseMotionCB)两个函数，其中glutMouseFunc()用来检测鼠标按键事件，但是要想鼠标拖动控制旋转，还需要对鼠标动作进行检测，这时用glutMotionFunc()来检测并计算各方向偏移量，然后传入glRotatef()函数来控制坐标（视野）旋转。


渐变色背景的需求在三维绘图场景下确实不太容易实现，因为绘制场景为三维场景，而且有三维旋转的需要，对于单一背景色可以简单地通过主函数里初始化场景进行设置，但这种方法无法设置渐变背景色。渐变背景色一般都会依托于渐变色多边形作为背景图来实现，但是要在三维场景中绘制不随旋转操作而变化位置的背景多边形就需要依托于glPushMatrix()和glPopMatrix()方法。

## 成品效果

![动画](https://user-images.githubusercontent.com/22266512/159007380-0288e25f-676e-466d-9126-6b9b5662ce55.gif)

![动画](https://user-images.githubusercontent.com/22266512/159007762-da99e448-6c7c-4590-9048-1a493e18c146.gif)

