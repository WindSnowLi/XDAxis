#include "QExpandOpenGLWidget.h"

#include <ft2build.h>
#include FT_FREETYPE_H

unsigned int QExpandOpenGLWidget::TextureFromFile(const char* path, const std::string& directory, QExpandOpenGLWidget* parent) {
	std::string filename = std::string(path);
	filename = directory + '/' + filename;

	unsigned int textureID;
	parent->glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format{};
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		parent->glBindTexture(GL_TEXTURE_2D, textureID);
		parent->glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		parent->glGenerateMipmap(GL_TEXTURE_2D);

		parent->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		parent->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		parent->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		parent->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}

void QExpandOpenGLWidget::resizeGL(int w, int h) {
	glViewport(0, 0, w, h);
	projection = glm::perspective(glm::radians(m_fov), (float)this->frameGeometry().width() / this->frameGeometry().height(), 0.1f, 100.0f);
}

void QExpandOpenGLWidget::keyPressEvent(QKeyEvent* event) {
	float cameraSpeed = 0.2f;
	if (event->key() == Qt::Key_Up) {
		cameraDist -= cameraSpeed;
		cameraDist <= 0.2;
		cameraDist = 0.2;
	}
	else if (event->key() == Qt::Key_Down) {
		cameraDist += cameraSpeed;
	}
	float camX = sin(m_yaw) * cameraDist;
	float camZ = cos(m_yaw) * cameraDist;
	float camY = sin(m_pitch) * cameraDist;

	view = glm::lookAt(glm::vec3(camX, camY, camZ), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
}

void QExpandOpenGLWidget::wheelEvent(QWheelEvent* event) {
	QPoint numDegrees = event->angleDelta();
	m_fov -= numDegrees.y() / 100;                // 进行放大
	qDebug() << m_fov;
	if (m_fov <= 1.0f)
		m_fov = 1.0f;
	if (m_fov >= 45.0f)
		m_fov = 45.0f;
	projection = glm::perspective(glm::radians(m_fov), (float)this->frameGeometry().width() / this->frameGeometry().height(), 0.1f, 100.0f);
}

void QExpandOpenGLWidget::mouseMoveEvent(QMouseEvent* event) {
	if (m_leftMouseButton && !m_firstClick)
	{
		float xoffset = event->x() - m_point.x();
		float yoffset = event->y() - m_point.y(); // 注意这里是相反的，因为y坐标是从底部往顶部依次增大的

		float xSensitivity = 0.03f, ySensitivity = 0.03f;
		xoffset *= xSensitivity;
		yoffset *= ySensitivity;
		m_yaw += xoffset;
		m_pitch += yoffset;
		if (m_pitch > 360.0) {
			m_pitch -= 360.f;
		}
		if (m_yaw > 360.0) {
			m_yaw -= 360.f;
		}

		float camX = sin(m_yaw) * cameraDist;
		float camZ = cos(m_yaw) * cameraDist;
		float camY = sin(m_pitch) * cameraDist;
		qDebug() << m_yaw << "\t" << m_pitch;
		qDebug() << camX << "\t" << camY << "\t" << camZ << "\n";

		view = glm::lookAt(glm::vec3(camX, camY, camZ), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));

		//view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
	}
	m_point = event->pos();
	event->ignore();
}

void QExpandOpenGLWidget::mouseReleaseEvent(QMouseEvent* event) {
	if (event->button() == Qt::LeftButton) {
		m_leftMouseButton = false;
	}
	event->ignore();
}

void QExpandOpenGLWidget::mousePressEvent(QMouseEvent* event) {
	if (event->button() == Qt::LeftButton)
	{
		m_point = event->pos();
		m_leftMouseButton = true;
		if (m_firstClick) {
			m_firstClick = false;
		}
	}
}
unsigned int QExpandOpenGLWidget::loadTexture(char const* path)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format{};
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}

// 加载立方体纹理
unsigned int QExpandOpenGLWidget::loadCubemap(std::vector<std::string> faces) {
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrComponents;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrComponents, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
}

void QExpandOpenGLWidget::Shader::readShaderFile(const char* vertexPath, const char* fragmentPath) {

	QFile file(vertexPath);
	QString vertexStr, fragmentStr;
	if (file.open(QIODevice::ReadOnly))
	{
		QByteArray array = file.readAll();//读取文本中全部文件
		vertexStr = QString(array);
	}
	file.close();
	file.setFileName(fragmentPath);
	if (file.open(QIODevice::ReadOnly))
	{
		QByteArray array = file.readAll();//读取文本中全部文件
		fragmentStr = QString(array);
	}
	file.close();
	readShaderStr(vertexStr.toStdString().c_str(), fragmentStr.toStdString().c_str());
}

void QExpandOpenGLWidget::Shader::readShaderStr(const char* vertexStr, const char* fragmentStr) {
	GLuint vertexShader = m_parent->glCreateShader(GL_VERTEX_SHADER);
	m_parent->glShaderSource(vertexShader, 1, &vertexStr, NULL);
	m_parent->glCompileShader(vertexShader);

	int success;
	char infoLog[512];
	m_parent->glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		m_parent->glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		qDebug() << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog;
	}
	GLuint fragmentShader = m_parent->glCreateShader(GL_FRAGMENT_SHADER);

	m_parent->glShaderSource(fragmentShader, 1, &fragmentStr, NULL);
	m_parent->glCompileShader(fragmentShader);

	m_parent->glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		m_parent->glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		qDebug() << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog;
	}
	shaderProgram = m_parent->glCreateProgram();
	m_parent->glAttachShader(shaderProgram, vertexShader);
	m_parent->glAttachShader(shaderProgram, fragmentShader);
	m_parent->glLinkProgram(shaderProgram);

	m_parent->glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		m_parent->glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		qDebug() << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog;
	}
	m_parent->glDeleteShader(vertexShader);
	m_parent->glDeleteShader(fragmentShader);

}

void QExpandOpenGLWidget::Shader::checkCompileErrors(GLuint shader, std::string type) {
	GLint success;
	GLchar infoLog[1024];
	if (type != "PROGRAM")
	{
		m_parent->glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)

		{
			m_parent->glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}
	else
	{
		m_parent->glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success)
		{
			m_parent->glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}
}

void QExpandOpenGLWidget::Shader::use()
{
	m_parent->glUseProgram(shaderProgram);
}

void QExpandOpenGLWidget::Shader::setBool(const std::string& name, bool value)
{
	m_parent->glUniform1i(m_parent->glGetUniformLocation(shaderProgram, name.c_str()), (int)value);
}

void QExpandOpenGLWidget::Shader::setInt(const std::string& name, int value)
{
	m_parent->glUniform1i(m_parent->glGetUniformLocation(shaderProgram, name.c_str()), value);
}

// ------------------------------------------------------------------------
void QExpandOpenGLWidget::Shader::setFloat(const std::string& name, float value)
{
	m_parent->glUniform1f(m_parent->glGetUniformLocation(shaderProgram, name.c_str()), value);
}

// ------------------------------------------------------------------------
void QExpandOpenGLWidget::Shader::setVec2(const std::string& name, const glm::vec2& value)
{
	m_parent->glUniform2fv(m_parent->glGetUniformLocation(shaderProgram, name.c_str()), 1, &value[0]);
}

void QExpandOpenGLWidget::Shader::setVec2(const std::string& name, float x, float y)
{
	m_parent->glUniform2f(m_parent->glGetUniformLocation(shaderProgram, name.c_str()), x, y);
}

// ------------------------------------------------------------------------
void QExpandOpenGLWidget::Shader::setVec3(const std::string& name, const glm::vec3& value)
{
	m_parent->glUniform3fv(m_parent->glGetUniformLocation(shaderProgram, name.c_str()), 1, &value[0]);
}

void QExpandOpenGLWidget::Shader::setVec3(const std::string& name, float x, float y, float z)
{
	m_parent->glUniform3f(m_parent->glGetUniformLocation(shaderProgram, name.c_str()), x, y, z);
}

// ------------------------------------------------------------------------
void QExpandOpenGLWidget::Shader::setVec4(const std::string& name, const glm::vec4& value)
{
	m_parent->glUniform4fv(m_parent->glGetUniformLocation(shaderProgram, name.c_str()), 1, &value[0]);
}

void QExpandOpenGLWidget::Shader::setVec4(const std::string& name, float x, float y, float z, float w)
{
	m_parent->glUniform4f(m_parent->glGetUniformLocation(shaderProgram, name.c_str()), x, y, z, w);
}

// ------------------------------------------------------------------------
void QExpandOpenGLWidget::Shader::setMat2(const std::string& name, const glm::mat2& mat)
{
	m_parent->glUniformMatrix2fv(m_parent->glGetUniformLocation(shaderProgram, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

// ------------------------------------------------------------------------
void QExpandOpenGLWidget::Shader::setMat3(const std::string& name, const glm::mat3& mat)
{
	m_parent->glUniformMatrix3fv(m_parent->glGetUniformLocation(shaderProgram, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

// ------------------------------------------------------------------------
void QExpandOpenGLWidget::Shader::setMat4(const std::string& name, const glm::mat4& mat)
{
	m_parent->glUniformMatrix4fv(m_parent->glGetUniformLocation(shaderProgram, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

// constructor
QExpandOpenGLWidget::Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures, QExpandOpenGLWidget* parent)
{
	this->vertices = vertices;
	this->indices = indices;
	this->textures = textures;
	this->m_parent = parent;
	// now that we have all the required data, set the vertex buffers and its attribute pointers.
	setupMesh();
}

// render the mesh
void QExpandOpenGLWidget::Mesh::Draw(Shader& shader)
{
	// bind appropriate textures
	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;
	unsigned int normalNr = 1;
	unsigned int heightNr = 1;
	for (unsigned int i = 0; i < textures.size(); i++)
	{
		m_parent->glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding
		// retrieve texture number (the N in diffuse_textureN)
		std::string number;
		std::string name = textures[i].type;
		if (name == "texture_diffuse")
			number = std::to_string(diffuseNr++);
		else if (name == "texture_specular")
			number = std::to_string(specularNr++); // transfer unsigned int to string
		else if (name == "texture_normal")
			number = std::to_string(normalNr++); // transfer unsigned int to string
		else if (name == "texture_height")
			number = std::to_string(heightNr++); // transfer unsigned int to string

		// now set the sampler to the correct texture unit
		m_parent->glUniform1i(m_parent->glGetUniformLocation(shader.shaderProgram, (name + number).c_str()), i);
		// and finally bind the texture
		m_parent->glBindTexture(GL_TEXTURE_2D, textures[i].id);
	}

	// draw mesh
	m_parent->glBindVertexArray(VAO);
	m_parent->glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, 0);
	m_parent->glBindVertexArray(0);

	// always good practice to set everything back to defaults once configured.
	m_parent->glActiveTexture(GL_TEXTURE0);
}

// initializes all the buffer objects/arrays
void QExpandOpenGLWidget::Mesh::setupMesh()
{
	// create buffers/arrays
	m_parent->glGenVertexArrays(1, &VAO);
	m_parent->glGenBuffers(1, &VBO);
	m_parent->glGenBuffers(1, &EBO);

	m_parent->glBindVertexArray(VAO);
	// load data into vertex buffers
	m_parent->glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// A great thing about structs is that their memory layout is sequential for all its items.
	// The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
	// again translates to 3/2 floats which translates to a byte array.
	m_parent->glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	m_parent->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	m_parent->glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	// set the vertex attribute pointers
	// vertex Positions
	m_parent->glEnableVertexAttribArray(0);
	m_parent->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	// vertex normals
	m_parent->glEnableVertexAttribArray(1);
	m_parent->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
	// vertex texture coords
	m_parent->glEnableVertexAttribArray(2);
	m_parent->glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
	// vertex tangent
	m_parent->glEnableVertexAttribArray(3);
	m_parent->glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
	// vertex bitangent
	m_parent->glEnableVertexAttribArray(4);
	m_parent->glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));
	// ids
	m_parent->glEnableVertexAttribArray(5);
	m_parent->glVertexAttribIPointer(5, 4, GL_INT, sizeof(Vertex), (void*)offsetof(Vertex, m_BoneIDs));

	// weights
	m_parent->glEnableVertexAttribArray(6);
	m_parent->glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_Weights));
	m_parent->glBindVertexArray(0);
}

void QExpandOpenGLWidget::Model::Draw(Shader& shader)
{
	for (unsigned int i = 0; i < meshes.size(); i++)
		meshes[i].Draw(shader);
}

// loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
void QExpandOpenGLWidget::Model::loadModel(std::string const& path)
{
	// read file via ASSIMP
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
	// check for errors
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
	{
		std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
		return;
	}
	// retrieve the directory path of the filepath
	directory = path.substr(0, path.find_last_of('/'));

	// process ASSIMP's root node recursively
	processNode(scene->mRootNode, scene);
}

// processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
void QExpandOpenGLWidget::Model::processNode(aiNode* node, const aiScene* scene)
{
	// process each mesh located at the current node
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		// the node object only contains indices to index the actual objects in the scene.
		// the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(processMesh(mesh, scene));
	}
	// after we've processed all of the meshes (if any) we then recursively process each of the children nodes
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene);
	}

}

QExpandOpenGLWidget::Mesh QExpandOpenGLWidget::Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
	// data to fill
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	// walk through each of the mesh's vertices
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		glm::vec3 vector; // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
		// positions
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.Position = vector;
		// normals
		if (mesh->HasNormals())
		{
			vector.x = mesh->mNormals[i].x;
			vector.y = mesh->mNormals[i].y;
			vector.z = mesh->mNormals[i].z;
			vertex.Normal = vector;
		}
		// texture coordinates
		if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
		{
			glm::vec2 vec;
			// a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't
			// use models where a vertex can have multiple texture coordinates so we always take the first set (0).
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.TexCoords = vec;
			// tangent
			vector.x = mesh->mTangents[i].x;
			vector.y = mesh->mTangents[i].y;
			vector.z = mesh->mTangents[i].z;
			vertex.Tangent = vector;
			// bitangent
			vector.x = mesh->mBitangents[i].x;
			vector.y = mesh->mBitangents[i].y;
			vector.z = mesh->mBitangents[i].z;
			vertex.Bitangent = vector;
		}
		else
			vertex.TexCoords = glm::vec2(0.0f, 0.0f);

		vertices.push_back(vertex);
	}
	// now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		// retrieve all indices of the face and store them in the indices vector
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}
	// process materials
	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
	// we assume a convention for sampler names in the shaders. Each diffuse texture should be named
	// as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER.
	// Same applies to other texture as the following list summarizes:
	// diffuse: texture_diffuseN
	// specular: texture_specularN
	// normal: texture_normalN

	// 1. diffuse maps
	std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
	textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
	// 2. specular maps
	std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
	textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	// 3. normal maps
	std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
	textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
	// 4. height maps
	std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
	textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

	// return a mesh object created from the extracted mesh data
	return QExpandOpenGLWidget::Mesh(vertices, indices, textures, m_parent);
}

// checks all material textures of a given type and loads the textures if they're not loaded yet.
// the required info is returned as a Texture struct.
std::vector<QExpandOpenGLWidget::Texture> QExpandOpenGLWidget::Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
{
	std::vector<QExpandOpenGLWidget::Texture> textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		// check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
		bool skip = false;
		for (unsigned int j = 0; j < textures_loaded.size(); j++)
		{
			if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
			{
				textures.push_back(textures_loaded[j]);
				skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
				break;
			}
		}
		if (!skip)
		{   // if texture hasn't been loaded already, load it
			QExpandOpenGLWidget::Texture texture;
			texture.id = QExpandOpenGLWidget::TextureFromFile(str.C_Str(), this->directory, m_parent);
			texture.type = typeName;
			texture.path = str.C_Str();
			textures.push_back(texture);
			textures_loaded.push_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
		}
	}
	return textures;
}

void QExpandOpenGLWidget::loadText()
{
	textShader->use();
	// FreeType
	FT_Library ft;
	// All functions return a value different than 0 whenever an error occurred
	if (FT_Init_FreeType(&ft))
		std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;

	// Load font as face
	FT_Face face;
	if (FT_New_Face(ft, "D:/Tool/FiraCode_3.1/ttf/FiraCode-Bold.ttf", 0, &face))
		std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;

	// Set size to load glyphs as
	FT_Set_Pixel_Sizes(face, 0, 48);

	// Disable byte-alignment restriction
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	// Load first 128 characters of ASCII set
	for (GLubyte c = 0; c < 128; c++)
	{
		// Load character glyph 
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
			continue;
		}
		// Generate texture
		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			face->glyph->bitmap.width,
			face->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			face->glyph->bitmap.buffer
		);
		// Set texture options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// Now store character for later use
		Character character = {
			texture,
			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			face->glyph->advance.x
		};
		Characters.insert(std::pair<GLchar, Character>(c, character));
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	// Destroy FreeType once we're finished
	FT_Done_Face(face);
	FT_Done_FreeType(ft);


	// Configure VAO/VBO for texture quads
	glGenVertexArrays(1, &textVAO);
	glGenBuffers(1, &textVBO);
	glBindVertexArray(textVAO);
	glBindBuffer(GL_ARRAY_BUFFER, textVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 5 * 6, NULL, GL_STREAM_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

}


void QExpandOpenGLWidget::renderText(std::string text, glm::mat4 projection, glm::mat4 view, glm::mat4 model)
{
	GLfloat x = 0.0f, y = 0.0f;
	textShader->use();
	textShader->setMat4("view", view);
	textShader->setMat4("projection", projection);
	textShader->setMat4("model", model);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(textVAO);
	// 遍历文本中所有的字符
	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); c++)
	{
		Character ch = QExpandOpenGLWidget::Characters[*c];

		GLfloat vertices[] = {
			-0.5,0.5,0, 0.0f, 1.0f,
			-0.5,-0.5,0,0.0f, 0.0f,

			0.5,0.5,0,1.0f, 1.0f,

			0.5,0.5,0,1.0f, 1.0f,

			-0.5,-0.5,0,0.0f, 0.0f,
			0.5,-0.5,0,1.0f, 0.0f

		};

		// 在四边形上绘制字形纹理
		glBindTexture(GL_TEXTURE_2D, ch.TextureID);
		// 更新VBO内存的内容
		glBindBuffer(GL_ARRAY_BUFFER, textVBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glActiveTexture(GL_TEXTURE0);
		// 绘制四边形
		glDrawArrays(GL_TRIANGLES, 0, 6);
		// 更新位置到下一个字形的原点，注意单位是1/64像素
		x += (ch.Advance >> 6); // 位偏移6个单位来获取单位为像素的值 (2^6 = 64)
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}
