#include "XDAxis.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

XDAxis::XDAxis(QWidget* parent)
{
}

void XDAxis::initializeGL()
{
	this->initializeOpenGLFunctions();
	this->resize(960, 540);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	ss = std::make_shared<Shader>(this);
	ss->readShaderFile(":/XDAxis/Resource/arrow.vs", ":/XDAxis/Resource/arrow.fs");
	textShader = std::make_shared<Shader>(this);
	textShader->readShaderFile(":/XDAxis/Resource/text.vs", ":/XDAxis/Resource/text.fs");
	loadText();


	glGenVertexArrays(1, &lineVAO);
	glGenBuffers(1, &lineVBO);
	glBindVertexArray(lineVAO);

	glBindBuffer(GL_ARRAY_BUFFER, lineVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// ¼ÓÔØ¼ýÍ·
	glGenVertexArrays(1, &arrowVAO);
	glGenBuffers(1, &arrowVBO);
	glGenBuffers(1, &arrowEBO);
	glBindVertexArray(arrowVAO);

	glBindBuffer(GL_ARRAY_BUFFER, arrowVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(arrow), arrow, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, arrowEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexArrow), indexArrow, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

}

void XDAxis::resizeGL(int w, int h)
{
	QExpandOpenGLWidget::resizeGL(w, h);
}

void XDAxis::paintGL()
{
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	build3Axis();
	this->update();
}

void XDAxis::mousePressEvent(QMouseEvent* event)
{
	QExpandOpenGLWidget::mousePressEvent(event);
}

void XDAxis::mouseReleaseEvent(QMouseEvent* event)
{
	QExpandOpenGLWidget::mouseReleaseEvent(event);
}

void XDAxis::mouseMoveEvent(QMouseEvent* event)
{
	QExpandOpenGLWidget::mouseMoveEvent(event);
}

void XDAxis::wheelEvent(QWheelEvent* event)
{
	QExpandOpenGLWidget::wheelEvent(event);
}

void XDAxis::keyPressEvent(QKeyEvent* event)
{
	QExpandOpenGLWidget::keyPressEvent(event);
}

void XDAxis::build3Axis()
{
	ss->use();
	static float trasnCount = 0;
	trasnCount += 0.05f;
	view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
	ss->setMat4("view", view);
	ss->setMat4("projection", projection);
	glm::mat4 model = glm::mat4(1.0f);
	glLineWidth(4.0f);
	glPointSize(80);

	ss->setMat4("model", model = glm::mat4(1.0f));
	ss->setVec3("ourColor", glm::vec3(255, 0, 0));
	glBegin(GL_LINES);
	glVertex3f(vertices[0], vertices[1], vertices[2]);
	glVertex3f(vertices[3], vertices[4], vertices[5]);
	glVertex3f(vertices[0], vertices[1], vertices[2]);
	glVertex3f(-vertices[3], -vertices[4], -vertices[5]);
	glEnd();
	model = glm::translate(model, glm::vec3(vertices[3], vertices[4], vertices[5]));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
	ss->setMat4("model", model);
	glBindVertexArray(arrowVAO);
	glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, 0);

	textShader->use();
	textShader->setVec3("textColor", glm::vec3(255, 0, 0));
	renderText("+X");

	ss->use();
	ss->setMat4("model", model = glm::mat4(1.0f));
	ss->setVec3("ourColor", glm::vec3(0, 255, 0));
	glBegin(GL_LINES);
	glVertex3f(vertices[0], vertices[1], vertices[2]);
	glVertex3f(vertices[6], vertices[7], vertices[8]);
	glVertex3f(vertices[0], vertices[1], vertices[2]);
	glVertex3f(-vertices[6], -vertices[7], -vertices[8]);
	glEnd();
	model = glm::translate(model, glm::vec3(vertices[6], vertices[7], vertices[8]));
	model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
	ss->setMat4("model", model);
	glBindVertexArray(arrowVAO);
	glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, 0);


	ss->use();
	ss->setVec3("ourColor", glm::vec3(0, 0, 255));
	ss->setMat4("model", model = glm::mat4(1.0f));
	glBegin(GL_LINES);
	glVertex3f(vertices[0], vertices[1], vertices[2]);
	glVertex3f(vertices[9], vertices[10], vertices[11]);
	glVertex3f(vertices[0], vertices[1], vertices[2]);
	glVertex3f(-vertices[9], -vertices[10], -vertices[11]);
	glEnd();
	model = glm::translate(model, glm::vec3(vertices[9], vertices[10], vertices[11]));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
	ss->setMat4("model", model);
	glBindVertexArray(arrowVAO);
	glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, 0);

	glFlush();
}