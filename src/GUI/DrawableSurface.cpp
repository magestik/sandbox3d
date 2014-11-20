#include "DrawableSurface.h"

#include "MainWindow.h"
#include "MeshListWidget.h"

#include <QDir>
#include <QMessageBox>
#include <QMouseEvent>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <assert.h>

#include <Misc/Timer.h>

struct MeshVertex
{
	vec3 position;
	vec2 uv;
	vec3 normal;
};

/**
 * @brief drawable::drawable
 * @param parent
 */
DrawableSurface::DrawableSurface(QWidget *parent)
: QGLWidget(parent)
, m_camera()
, m_vLastPos(0, 0)
, m_eRenderType(Rendering::FINAL)
, m_bDebugWireframe(false)
{
	setAutoBufferSwap(false);
	setFocusPolicy(Qt::StrongFocus);
	makeCurrent();
}

/**
 * @brief drawable::~drawable
 */
DrawableSurface::~DrawableSurface(void)
{

}

/**
 * @brief DrawableSurface::ResetCamera
 */
void DrawableSurface::ResetCamera(void)
{
	m_camera = Camera();
	update();
}

/**
 * @brief DrawableSurface::AddObject
 * @param name
 */
void DrawableSurface::AddObject(const std::string & name)
{
	Mesh * mesh = g_Meshes[name];

	if (nullptr != mesh)
	{
		m_renderer.onCreate(mesh);
		update();
	}
}

/**
 * @brief DrawableSurface::DebugDiffuse
 */
void DrawableSurface::DebugDiffuse()
{
	m_eRenderType = Rendering::DIFFUSE;
	update();
}

/**
 * @brief DrawableSurface::DebugNormal
 */
void DrawableSurface::DebugNormal()
{
	m_eRenderType = Rendering::NORMAL;
	update();
}

/**
 * @brief DrawableSurface::DebugPosition
 */
void DrawableSurface::DebugPosition()
{
	m_eRenderType = Rendering::POSITION;
	update();
}

/**
 * @brief DrawableSurface::DebugDepth
 */
void DrawableSurface::DebugDepth()
{
	m_eRenderType = Rendering::DEPTH;
	update();
}

/**
 * @brief DrawableSurface::DebugShadows
 */
void DrawableSurface::DebugShadows()
{
	m_eRenderType = Rendering::SHADOWS;
	update();
}

/**
 * @brief DrawableSurface::DebugWireframe
 * @param d
 */
void DrawableSurface::DebugWireframe(bool d)
{
	m_bDebugWireframe = d;
	update();
}

/**
 * @brief DrawableSurface::DebugNone
 */
void DrawableSurface::DebugFinal()
{
	m_eRenderType = Rendering::FINAL;
	update();
}

/**
 * @brief initializeGL
 */
void DrawableSurface::initializeGL(void)
{
	GLint v;
	glGetIntegerv(GL_CONTEXT_FLAGS, &v);
	assert (v & GL_CONTEXT_FLAG_DEBUG_BIT);

	glEnable(GL_CULL_FACE);

	loadShaders();
	loadMeshes();

	m_renderer.onInitializeComplete();
}

/**
 * @brief drawable::resizeGL
 * @param w
 * @param h
 */
void DrawableSurface::resizeGL(int w, int h)
{
	m_renderer.onResize(w, h);
}

/**
 * @brief drawable::paintGL
 */
void DrawableSurface::paintGL(void)
{
	Timer t;

	t.Start();

	const mat4x4 & matView = m_camera.getViewMatrix();
	m_renderer.onUpdate(matView, m_bDebugWireframe, m_eRenderType);

	swapBuffers();

	t.Stop();

	static_cast<MainWindow*>(parent())->SetRenderTime(t.getElapsedTimeInMs());
}

/**
 * @brief drawable::mousePressEvent
 * @param event
 */
void DrawableSurface::mousePressEvent(QMouseEvent * event)
{
	m_vLastPos.x = event->x();
	m_vLastPos.y = event->y();
	update();
}

/**
 * @brief drawable::mouseMoveEvent
 * @param event
 */
void DrawableSurface::mouseMoveEvent(QMouseEvent * event)
{
	ivec2 pos(event->x(), event->y());

	//if (event->button() & Qt::MidButton)
	{
		ivec2 diff = pos - m_vLastPos;

		if (event->modifiers() & Qt::ControlModifier)
		{
			// Translate X/Y
			vec2 translation(diff.x / 100.0f, diff.y / -100.0f);
			m_camera.TranslateXY(translation);
		}
		else if (event->modifiers() & Qt::ShiftModifier)
		{
			// Translate Z
			float translation = diff.y / -100.0f;
			m_camera.TranslateZ(translation);
		}
		else
		{
			vec2 rotation(diff.x / -500.0f, diff.y / -500.0f);
			m_camera.Rotate(rotation);
		}
	}

	m_vLastPos = pos;

	update();
}

/**
 * @brief drawable::mouseReleaseEvent
 * @param event
 */
void DrawableSurface::mouseReleaseEvent(QMouseEvent * event)
{
	// ...
	update();
}

/**
 * @brief wheelEvent
 * @param event
 */
void DrawableSurface::wheelEvent(QWheelEvent * event)
{
	// ...
	update();
}

/**
 * @brief DrawableSurface::loadShaders
 */
void DrawableSurface::loadShaders(void)
{
	QDir dir("data/shaders");
	QStringList list = dir.entryList(QDir::Files);

	bool successful = true;

	for (QString & filename : list)
	{
		QFile f(dir.filePath(filename));
		f.open(QFile::ReadOnly);
		QByteArray source = f.readAll();

		if (filename.endsWith("vs"))
		{
			GPU::Shader<GL_VERTEX_SHADER> * vs = new GPU::Shader<GL_VERTEX_SHADER>();
			successful &= vs->compileFromSource(source.data());
			//assert(successful);
			g_VertexShaders.insert(std::pair<std::string, GPU::Shader<GL_VERTEX_SHADER> *>(filename.toStdString(), vs));
		}
		else if (filename.endsWith("fs"))
		{
			GPU::Shader<GL_FRAGMENT_SHADER> * fs = new GPU::Shader<GL_FRAGMENT_SHADER>();
			successful &= fs->compileFromSource(source.data());
			//assert(successful);
			g_FragmentShaders.insert(std::pair<std::string, GPU::Shader<GL_FRAGMENT_SHADER> *>(filename.toStdString(), fs));
		}

		f.close();
	}

	if (!successful)
	{
		int ret = QMessageBox::warning(this, tr("Sandbox 3D"), tr("All Shaders were not compiled successfully !"));
	}
}

/**
 * @brief DrawableSurface::loadMeshes
 */
void DrawableSurface::loadMeshes(void)
{
	QDir dir("data/meshes");
	QStringList list = dir.entryList(QDir::Files);

	bool successful = true;

	for (QString & filename : list)
	{
		std::string filepath = dir.filePath(filename).toStdString();

		if (filename.endsWith("obj") || filename.endsWith("dae"))
		{
			Assimp::Importer importer;

			const aiScene * scene = importer.ReadFile(filepath.data(), aiProcessPreset_TargetRealtime_MaxQuality);

			if (!scene)
			{
				successful = false;
				continue;
			}

			GPU::Buffer<GL_ARRAY_BUFFER> * vertexBuffer = new GPU::Buffer<GL_ARRAY_BUFFER>();
			GPU::Buffer<GL_ELEMENT_ARRAY_BUFFER> * indexBuffer = new GPU::Buffer<GL_ELEMENT_ARRAY_BUFFER>();

			std::vector<MeshVertex> vertices;
			std::vector<unsigned int> triangles;

			uint NumVertices = 0;
			uint NumIndices = 0;

			// Count the number of vertices and indices
			for (int i = 0 ; i < scene->mNumMeshes ; i++)
			{
				NumVertices += scene->mMeshes[i]->mNumVertices;
				NumIndices  += scene->mMeshes[i]->mNumFaces;
			}

			// Reserve space in the vectors for the vertex attributes and indices
			vertices.reserve(NumVertices);
			triangles.reserve(NumIndices*3);

			int start = 0;
			int base = 0;

			for (int i = 0; i < scene->mNumMeshes; ++i)
			{
				aiMesh * mesh = scene->mMeshes[i];

				// Populate the vertex attribute vectors
				for (int j = 0 ; j < mesh->mNumVertices ; ++j)
				{
					MeshVertex vertex;

					vertex.position.x = mesh->mVertices[j].x;
					vertex.position.y = mesh->mVertices[j].y;
					vertex.position.z = mesh->mVertices[j].z;

					vertex.normal.x = mesh->mNormals[j].x;
					vertex.normal.y = mesh->mNormals[j].y;
					vertex.normal.z = mesh->mNormals[j].z;

					if (mesh->HasTextureCoords(0))
					{
						vertex.uv.x = mesh->mTextureCoords[0][j].x;
						vertex.uv.y = mesh->mTextureCoords[0][j].y;
					}
					else
					{
						vertex.uv.x = 0.0f;
						vertex.uv.y = 0.0f;
					}

					vertices.push_back(vertex);
				}

				// Populate the index buffer
				for (int j = 0 ; j < mesh->mNumFaces ; ++j)
				{
					const aiFace & Face = mesh->mFaces[j];
					triangles.push_back(base + Face.mIndices[0]);
					triangles.push_back(base + Face.mIndices[1]);
					triangles.push_back(base + Face.mIndices[2]);
				}

				base += mesh->mNumVertices;
			}

			vertexBuffer->allocate(vertices.size() * sizeof(MeshVertex), GL_STATIC_DRAW, (void *)vertices.data());
			indexBuffer->allocate(triangles.size() * sizeof(unsigned int), GL_STATIC_DRAW, (void *)triangles.data());

			std::vector<Mesh::VertexSpec> specs;

			Mesh::VertexSpec SPEC_POS;
			SPEC_POS.index = 0;
			SPEC_POS.size = 3;
			SPEC_POS.type = GL_FLOAT;
			SPEC_POS.normalized = GL_FALSE;
			SPEC_POS.stride = sizeof(MeshVertex);
			SPEC_POS.pointer = 0;

			Mesh::VertexSpec SPEC_UV;
			SPEC_UV.index = 2;
			SPEC_UV.size = 2;
			SPEC_UV.type = GL_FLOAT;
			SPEC_UV.normalized = GL_FALSE;
			SPEC_UV.stride = sizeof(MeshVertex);
			SPEC_UV.pointer = (void*)(sizeof(float)*3);

			Mesh::VertexSpec SPEC_NORMAL;
			SPEC_NORMAL.index = 1;
			SPEC_NORMAL.size = 3;
			SPEC_NORMAL.type = GL_FLOAT;
			SPEC_NORMAL.normalized = GL_FALSE;
			SPEC_NORMAL.stride = sizeof(MeshVertex);
			SPEC_NORMAL.pointer = (void*)(sizeof(float)*5);

			specs.push_back(SPEC_POS);
			specs.push_back(SPEC_UV);
			specs.push_back(SPEC_NORMAL);

			Mesh * mesh = Mesh::Create(vertexBuffer, triangles.size(), GL_TRIANGLES, specs, indexBuffer, GL_UNSIGNED_INT);

			g_Meshes.insert(std::pair<std::string, Mesh*>(filename.toStdString(), mesh));

			QDockWidget * dock = static_cast<MainWindow*>(parent())->m_pMeshListDock;
			static_cast<MeshListWidget*>(dock)->addMesh(filename);
		}
	}

	//
	// Tests success
	//
	if (!successful)
	{
		int ret = QMessageBox::warning(this, tr("Sandbox 3D"), tr("All Models were not loaded successfully !"));
	}
}

/**
 * @brief DrawableSurface::setClearColor
 */
void DrawableSurface::setClearColor(const QColor & color)
{
	qglClearColor(color);
	update();
}
