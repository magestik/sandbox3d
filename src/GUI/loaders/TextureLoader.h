#pragma once

#include "../../Rendering/Mesh/Mesh.h"
#include "../../Rendering/Mesh/SubMesh.h"

#include <string>

#include <QDir>
#include <QString>

GPU::Texture<GL_TEXTURE_2D> * loadTexture(const QDir & dir, const QString & filepath);
