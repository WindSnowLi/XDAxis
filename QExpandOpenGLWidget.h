#pragma once

#include <QOpenGLWidget>
#include <QOpenGLExtraFunctions>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/ext.hpp>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <QDebug>
#include <QMouseEvent>
#include <QPoint>
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#define STB_IMAGE_STATIC
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <QFile>

#define MAX_BONE_INFLUENCE 4

class QExpandOpenGLWidget : public QOpenGLWidget, protected QOpenGLExtraFunctions
{
public:
	class Shader;
	struct Vertex;
	struct Texture;
	class Mesh;
	class Model;
	struct Character;
protected:
	// 相机位置
	glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 2.0f);
	// 相机距离
	float cameraDist = 2.0f;
	// 相机上向量
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	// 投影矩阵
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)this->frameGeometry().width() / this->frameGeometry().height(), 0.1f, 100.0f);
	// 观察矩阵
	glm::mat4 view = glm::lookAt(cameraPos, glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));;
	// 上一次鼠标点击的位置
	QPoint m_point;
	// 是否是鼠标第一次点击
	bool m_firstClick = true;
	// 鼠标左键是否按下
	bool m_leftMouseButton = false;
	// 偏移角度
	float m_yt = 0.0f, m_xt = -0.0f, m_fov = 45.0f;
	// 渲染字体
	void renderText(std::string text, glm::mat4 projection, glm::mat4 view, glm::mat4 model);
	GLuint textVAO{}, textVBO{};
	std::map<GLchar, Character> Characters;
	std::shared_ptr<Shader> textShader = nullptr;

	void loadText();
public:
	QExpandOpenGLWidget() {}

	// 鼠标按下事件
	void mousePressEvent(QMouseEvent* event);

	// 鼠标松开事件
	void mouseReleaseEvent(QMouseEvent* event);

	void mouseMoveEvent(QMouseEvent* event);

	// 滑轮事件
	void wheelEvent(QWheelEvent* event);

	//键盘按下事件
	void keyPressEvent(QKeyEvent* event);

	void resizeGL(int w, int h);

	static unsigned int TextureFromFile(const char* path, const std::string& directory, QExpandOpenGLWidget* m_parent);

	unsigned int loadTexture(const char* path);

	// 加载立方体纹理
	unsigned int loadCubemap(std::vector<std::string> faces);
};

class QExpandOpenGLWidget::Shader {
public:
	unsigned int shaderProgram{};
	QExpandOpenGLWidget* m_parent = nullptr;
	Shader(QExpandOpenGLWidget* parent) : m_parent(parent) {}

	// 读取shader配置文件
	void readShaderFile(const char* vertexPath, const char* fragmentPath);

	// 读取shader配置字符串
	void readShaderStr(const char* vertexStr, const char* fragmentStr);

	// utility function for checking shader compilation/linking errors.
	// ------------------------------------------------------------------------
	void checkCompileErrors(GLuint shader, std::string type);

	// activate the shader
	// ------------------------------------------------------------------------
	void use();

	// utility uniform functions
	// ------------------------------------------------------------------------
	void setBool(const std::string& name, bool value);

	// ------------------------------------------------------------------------
	void setInt(const std::string& name, int value);

	// ------------------------------------------------------------------------
	void setFloat(const std::string& name, float value);

	// ------------------------------------------------------------------------
	void setVec2(const std::string& name, const glm::vec2& value);

	void setVec2(const std::string& name, float x, float y);

	// ------------------------------------------------------------------------
	void setVec3(const std::string& name, const glm::vec3& value);

	void setVec3(const std::string& name, float x, float y, float z);

	// ------------------------------------------------------------------------
	void setVec4(const std::string& name, const glm::vec4& value);

	void setVec4(const std::string& name, float x, float y, float z, float w);

	// ------------------------------------------------------------------------
	void setMat2(const std::string& name, const glm::mat2& mat);

	// ------------------------------------------------------------------------
	void setMat3(const std::string& name, const glm::mat3& mat);

	// ------------------------------------------------------------------------
	void setMat4(const std::string& name, const glm::mat4& mat);
};

struct QExpandOpenGLWidget::Vertex {
	// position
	glm::vec3 Position;
	// normal
	glm::vec3 Normal;
	// texCoords
	glm::vec2 TexCoords;
	// tangent
	glm::vec3 Tangent;
	// bitangent
	glm::vec3 Bitangent;
	//bone indexes which will influence this vertex
	int m_BoneIDs[MAX_BONE_INFLUENCE];
	//weights from each bone
	float m_Weights[MAX_BONE_INFLUENCE];
};

struct QExpandOpenGLWidget::Texture {
	unsigned int id;
	std::string type;
	std::string path;
};

class QExpandOpenGLWidget::Mesh {
public:
	// mesh Data
	std::vector<Vertex>       vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture>      textures;
	unsigned int VAO;
	QExpandOpenGLWidget* m_parent;
	// constructor
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures, QExpandOpenGLWidget* parent);

	// render the mesh
	void Draw(Shader& shader);

private:
	// render data
	unsigned int VBO, EBO;

	// initializes all the buffer objects/arrays
	void setupMesh();

};

class QExpandOpenGLWidget::Model
{
public:
	// model data
	std::vector<Texture> textures_loaded;	// stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
	std::vector<Mesh>    meshes;
	std::string directory;
	QExpandOpenGLWidget* m_parent;
	bool gammaCorrection;

	// constructor, expects a filepath to a 3D model.
	Model(std::string const& path, QExpandOpenGLWidget* parent, bool gamma = false) : gammaCorrection(gamma)
	{
		this->m_parent = parent;
		loadModel(path);
	}

	// draws the model, and thus all its meshes
	void Draw(Shader& shader);

private:
	// loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
	void loadModel(std::string const& path);

	// processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
	void processNode(aiNode* node, const aiScene* scene);

	Mesh processMesh(aiMesh* mesh, const aiScene* scene);

	// checks all material textures of a given type and loads the textures if they're not loaded yet.
	// the required info is returned as a Texture struct.
	std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
};

struct QExpandOpenGLWidget::Character {
	GLuint TextureID;   // ID handle of the glyph texture
	glm::ivec2 Size;    // Size of glyph
	glm::ivec2 Bearing;  // Offset from baseline to left/top of glyph
	GLuint Advance;    // Horizontal offset to advance to next glyph
};
