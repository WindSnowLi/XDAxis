#pragma once

#include "QExpandOpenGLWidget.h"

class XDAxis : public QExpandOpenGLWidget
{
	Q_OBJECT

public:
	XDAxis(QWidget* parent = nullptr);
	virtual void initializeGL();
	virtual void resizeGL(int w, int h);
	virtual void paintGL();
	// 鼠标按下事件
	void mousePressEvent(QMouseEvent* event);

	// 鼠标松开事件
	void mouseReleaseEvent(QMouseEvent* event);

	void mouseMoveEvent(QMouseEvent* event);

	// 滑轮事件
	void wheelEvent(QWheelEvent* event);

	//键盘按下事件
	void keyPressEvent(QKeyEvent* event);
private:
	std::shared_ptr<Shader> ss = nullptr;
	GLuint lineVBO, lineVAO, arrowVAO, arrowVBO, arrowEBO;
	void build3Axis();
	// 坐标轴方向
	const float vertices[12]{
	0.0f, 0.0f, 0.0f, // 0.0  
	 0.5f, 0.0f, 0.0f, // +X 
	 0.0f,  0.5f, 0.0f,  // +Y   
	 0.0f,  0.0f, 0.5f  // +Z   
	};

	// 箭头顶点坐标
	const float arrow[15]{
		0, 0.5, 0, // top
		-0.25, -0.5, 0.25, // left before
		0.25, -0.5, 0.25, // right before
		-0.25, -0.5, -0.25, //left after
		0.25, -0.5, -0.25//right after
	};

	// 箭头顶点坐标索引
	const GLuint indexArrow[18]{
		0, 1, 2, // 正面前
		0, 1, 3, // 左面
		0, 2, 4, // 右面
		0, 3, 4, // 后面
		3, 1, 4, // 下左
		1, 2, 4  // 下右
	};
};
