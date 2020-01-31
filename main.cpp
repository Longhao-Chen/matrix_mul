#include <stdio.h>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "theread.h"
#include "matrix.h"

int mul(mat a,mat b,mat out)
{
	if(a.c!=b.r){
		std::cout << "ERROR::a.c!=b.r" << std::endl;
		exit(-1);
	}
	//创建绘制窗体
	int argc;
	char** argv;
	glutInit(&argc,argv);
	glutCreateWindow("GPGPU");
	glewInit();
	
	//片段着色器
	GLuint f,pr;
	f=glCreateShader(GL_FRAGMENT_SHADER);
	char *ff=readTheFile("opengl.frag");
	glShaderSource(f,1,&ff,NULL);
	glCompileShader(f);
	GLint successf,successv;
	glGetShaderiv(f, GL_COMPILE_STATUS, &successf);
	if(!(successf)){
		char info[512];
		printf("error\n");
		glGetProgramInfoLog(f, 512, NULL, info);
		std::cout << info <<std::endl;
		exit(-1);
	}
	pr=glCreateProgram();
	glAttachShader(pr,f);
	glLinkProgram(pr);
	glUseProgram(pr);

	//创建FBO并绑定
	GLuint fb;
	glGenFramebuffersEXT(1,&fb);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT,fb);
 
	//创建纹理对象并绑定
	GLuint tex,tex1,tex2;
	glGenTextures(1,&tex);
	glGenTextures(1,&tex1);
	glGenTextures(1,&tex2);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_RECTANGLE_ARB,tex);

	//设置纹理參数
	glTexParameteri(GL_TEXTURE_RECTANGLE_ARB,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_RECTANGLE_ARB,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_RECTANGLE_ARB,GL_TEXTURE_WRAP_S,GL_CLAMP);
	glTexParameteri(GL_TEXTURE_RECTANGLE_ARB,GL_TEXTURE_WRAP_T,GL_CLAMP);
 
 
	//定义纹理数据单元类型
	glTexImage2D(GL_TEXTURE_RECTANGLE_ARB,0,GL_R32F,a.c,a.r,0,GL_RED,GL_FLOAT,0);
 
	//将传输数据到纹理缓存
	glTexSubImage2D(GL_TEXTURE_RECTANGLE_ARB,0,0,0,a.c,a.r,GL_RED,GL_FLOAT,a.data);

	//正交投影
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0,out.c,0.0,out.r);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	//视口与纹理图等尺寸
	glViewport(0.0,0.0,out.c,out.r);

	//将FBO设置为绘制缓存
	glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT);
	//正面，填充
	glPolygonMode(GL_FRONT,GL_FILL);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_RECTANGLE_ARB,tex1);
	//设置纹理參数
	glTexParameteri(GL_TEXTURE_RECTANGLE_ARB,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_RECTANGLE_ARB,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_RECTANGLE_ARB,GL_TEXTURE_WRAP_S,GL_CLAMP);
	glTexParameteri(GL_TEXTURE_RECTANGLE_ARB,GL_TEXTURE_WRAP_T,GL_CLAMP);
	//定义纹理数据单元类型
	glTexImage2D(GL_TEXTURE_RECTANGLE_ARB,0,GL_R32F,b.c,b.r,0,GL_RED,GL_FLOAT,0);
	glTexSubImage2D(GL_TEXTURE_RECTANGLE_ARB,0,0,0,b.c,b.r,GL_RED,GL_FLOAT,b.data);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_RECTANGLE_ARB,tex2);
	//设置纹理參数
	glTexParameteri(GL_TEXTURE_RECTANGLE_ARB,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_RECTANGLE_ARB,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_RECTANGLE_ARB,GL_TEXTURE_WRAP_S,GL_CLAMP);
	glTexParameteri(GL_TEXTURE_RECTANGLE_ARB,GL_TEXTURE_WRAP_T,GL_CLAMP);
	//定义纹理数据单元类型
	glTexImage2D(GL_TEXTURE_RECTANGLE_ARB,0,GL_R32F,out.c,out.r,0,GL_RED,GL_FLOAT,0);
	 //将纹理关联到FBO
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT,GL_COLOR_ATTACHMENT0_EXT,GL_TEXTURE_RECTANGLE_ARB,tex2,0);

	glUniform1i(glGetUniformLocation(pr, "ourTexture0"), 0);
	glUniform1i(glGetUniformLocation(pr, "ourTexture1"), 1);
	glUniform1i(glGetUniformLocation(pr, "ourTexture2"), 2);
	glUniform1i(glGetUniformLocation(pr, "size"),a.c);
	//绘制矩阵
	glBegin(GL_QUADS);
	glTexCoord2f(0.0,0.0);
	glVertex2f(0.0,0.0);
	glTexCoord2f(out.c,0.0);
	glVertex2f(out.c,0.0);
	glTexCoord2f(out.c,out.r);
	glVertex2f(out.c,out.r);
	glTexCoord2f(0.0,out.r);
	glVertex2f(0.0,out.r);
	glEnd();

	//读取数据
	glReadBuffer(GL_COLOR_ATTACHMENT0_EXT);
	glReadPixels(0,0,out.c,out.r,GL_RED,GL_FLOAT,out.data);
 
	//清除资源
	glDeleteFramebuffersEXT(1,&fb);
	glDeleteTextures(1,&tex);
	glDeleteTextures(1,&tex1);
	glDeleteTextures(1,&tex2);
	return 0;
}


int main(){
	mat a,b,c;
	a.c=2;
	a.r=3;
	b.c=2;
	b.r=2;
	c.c=2;
	c.r=3;
	a.data=new float[a.r*a.c];
	b.data=new float[b.r*b.c];
	c.data=new float[c.r*c.c];
	a.data[0]=1;
	a.data[1]=2;
	a.data[2]=3;
	a.data[3]=4;
	a.data[4]=5;
	a.data[5]=6;
	b.data[0]=10;
	b.data[1]=20;
	b.data[2]=30;
	b.data[3]=40;
	mul(a,b,c);
	for (int i=0;i<c.c*c.r;++i){
		std::cout << c.data[i] << "\t";
		if((i+1)%c.c==0)
			std::cout << std::endl;
	}
}