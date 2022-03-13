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
	// ��갴���¼�
	void mousePressEvent(QMouseEvent* event);

	// ����ɿ��¼�
	void mouseReleaseEvent(QMouseEvent* event);

	void mouseMoveEvent(QMouseEvent* event);

	// �����¼�
	void wheelEvent(QWheelEvent* event);

	//���̰����¼�
	void keyPressEvent(QKeyEvent* event);
private:
	std::shared_ptr<Shader> ss = nullptr;
	GLuint lineVBO, lineVAO, arrowVAO, arrowVBO, arrowEBO;
	void build3Axis();
	// �����᷽��
	const float vertices[12]{
	0.0f, 0.0f, 0.0f, // 0.0  
	 0.5f, 0.0f, 0.0f, // +X 
	 0.0f,  0.5f, 0.0f,  // +Y   
	 0.0f,  0.0f, 0.5f  // +Z   
	};

	// ��ͷ��������
	const float arrow[15]{
		0, 0.5, 0, // top
		-0.25, -0.5, 0.25, // left before
		0.25, -0.5, 0.25, // right before
		-0.25, -0.5, -0.25, //left after
		0.25, -0.5, -0.25//right after
	};

	// ��ͷ������������
	const GLuint indexArrow[18]{
		0, 1, 2, // ����ǰ
		0, 1, 3, // ����
		0, 2, 4, // ����
		0, 3, 4, // ����
		3, 1, 4, // ����
		1, 2, 4  // ����
	};
};
