#include "CTriangle.h"

OpenUtility::CTriangle::SShaders *OpenUtility::CTriangle::DefaultShader=NULL;
unsigned int OpenUtility::CTriangle::DefShaderInUse=0;

static const char QuadVertex[]="\
attribute vec4 vPos;\n\
attribute vec3 vNorm;\n\
attribute vec4 vColor;\n\
\n\
uniform mat4 u_MVPmatrix;\n\
uniform mat4 u_Nmatrix;\n\
\n\
varying vec4 v_Color;\n\
varying vec3 v_Normal;\n\
\n\
void main()\n\
{\n\
	v_Color = vColor;\n\
	v_Normal = mat3(u_Nmatrix) * vNorm;\n\
	gl_Position = u_MVPmatrix * vPos;\n\
}\n\
";

static const char QuadFragment[]="\
precision mediump float;\n\
\n\
uniform float u_Alpha;\n\
\n\
varying vec4 v_Color;\n\
varying vec3 v_Normal;\n\
\n\
void main()\n\
{\n\
	gl_FragColor = vec4(v_Color.rgb,v_Color.a*u_Alpha);\n\
}\n\
";

//#############################################################################################

OpenUtility::CTriangle::CTriangle(double x1,double y1,double x2,double y2, double x3, double y3,const OpenUtility::RGBAd &color,bool useDefaultShader) :
	IsDefaultShaderInUse(false),
	Alpha(1.0)
{
	SVertex VertexArray[3];

	UseDefaultShader(useDefaultShader);
	Update(VertexArray,x1,y1,x2,y2,x3,y3,color);
	glGenBuffers(1,&VBObuffer);
	glBindBuffer(GL_ARRAY_BUFFER,VBObuffer);
	glBufferData(GL_ARRAY_BUFFER,sizeof(VertexArray),VertexArray,GL_DYNAMIC_DRAW);
}

OpenUtility::CTriangle::~CTriangle()
{
	UseDefaultShader(false);
	glDeleteBuffers(1,&VBObuffer);
}

void OpenUtility::CTriangle::UseDefaultShader(bool useDefShader)
{
	if (useDefShader)
	{
		if (!DefaultShader)
		{
			DefaultShader=new SShaders;
			if (!DefaultShader->ShaderVertex.LoadStream(QuadVertex))
				std::cout << "-----------------------------------\nCTaskBar: Erreur vertex shader :\n" << DefaultShader->ShaderVertex.GetLog() << std::endl << "--------------------------" << std::endl;
			if (!DefaultShader->ShaderFragment.LoadStream(QuadFragment))
				std::cout << "-----------------------------------\nCTaskBar: Erreur fragment shader :\n" << DefaultShader->ShaderFragment.GetLog() << std::endl << "--------------------------" << std::endl;
			if (!DefaultShader->RenderingShader.LinkProgram())
				std::cout << "-----------------------------------\nCTaskBar: Erreur shader program :\n" << DefaultShader->RenderingShader.GetLog() << std::endl << "--------------------------" << std::endl;
		}
		if (!IsDefaultShaderInUse) DefShaderInUse++;
	}
	else
	{
		if (IsDefaultShaderInUse)
		{
			DefShaderInUse--;
			if (DefShaderInUse==0)
			{
				delete DefaultShader;
				DefaultShader=NULL;
			}
		}
	}
	IsDefaultShaderInUse=useDefShader;
}

void OpenUtility::CTriangle::UpdateCoord(double x1,double y1,double x2,double y2, double x3, double y3,double r,double g,double b,double a)
{
	OpenUtility::RGBAd color(r,g,b,a);
	UpdateCoord(x1,y1,x2,y2,x3,y3,color);
}

void OpenUtility::CTriangle::UpdateCoord(double x1,double y1,double x2,double y2, double x3, double y3,const OpenUtility::RGBAd &color)
{
	SVertex VertexArray[4];

	Update(VertexArray,x1,y1,x2,y2,x3,y3,color);
	glBindBuffer(GL_ARRAY_BUFFER,VBObuffer);
	glBufferSubData(GL_ARRAY_BUFFER,0,sizeof(VertexArray),VertexArray);
}

void OpenUtility::CTriangle::Update(SVertex VertexArray[3],double x1,double y1,double x2,double y2, double x3, double y3,const OpenUtility::RGBAd &color)
{
	SetVertex(VertexArray[0],x1,y1,color.r,color.g,color.b,color.a);
	SetVertex(VertexArray[1],x2,y2,color.r,color.g,color.b,color.a);
	SetVertex(VertexArray[2],x3,y3,color.r,color.g,color.b,color.a);
}

void OpenUtility::CTriangle::SetVertex(SVertex &vertex,double posX,double posY,double r,double g,double b,double a)
{
	vertex.position[0]=posX;
	vertex.position[1]=posY;
	vertex.position[2]=0;
	vertex.normal[0]=0;
	vertex.normal[1]=0;
	vertex.normal[2]=1.0;
	vertex.color[0]=r;
	vertex.color[1]=g;
	vertex.color[2]=b;
	vertex.color[3]=a;
}

void OpenUtility::CTriangle::AttachAttribToData(GLuint vPos,GLuint vNorm,GLuint vColor)
{
	glBindBuffer(GL_ARRAY_BUFFER,VBObuffer);
	glVertexAttribPointer(vPos,3,GL_FLOAT,GL_FALSE,sizeof(SVertex),(void*)offsetof(SVertex,position));
	glVertexAttribPointer(vNorm,3,GL_FLOAT,GL_FALSE,sizeof(SVertex),(void*)offsetof(SVertex,normal));
	glVertexAttribPointer(vColor,4,GL_FLOAT,GL_FALSE,sizeof(SVertex),(void*)offsetof(SVertex,color));
	glEnableVertexAttribArray(vPos);
	glEnableVertexAttribArray(vNorm);
	glEnableVertexAttribArray(vColor);
}

void OpenUtility::CTriangle::Draw()
{
	if (IsDefaultShaderInUse && DefaultShader)
	{
		DefaultShader->RenderingShader.UseProgram();
		glUniform1f(DefaultShader->RenderingShader["u_Alpha"],Alpha);
		glUniformMatrix4fv(DefaultShader->RenderingShader["u_MVPmatrix"],1,GL_FALSE,DefMVPmatrix.GetMatrix());
		glUniformMatrix4fv(DefaultShader->RenderingShader["u_Nmatrix"],1,GL_FALSE,DefNmatrix.GetMatrix());
		AttachAttribToData(DefaultShader->RenderingShader["vPos"],DefaultShader->RenderingShader["vNorm"],DefaultShader->RenderingShader["vColor"]);
	}
	glBindBuffer(GL_ARRAY_BUFFER,VBObuffer);
	glDrawArrays(GL_TRIANGLE_STRIP,0,3);
	glBindBuffer(GL_ARRAY_BUFFER,0);
}
