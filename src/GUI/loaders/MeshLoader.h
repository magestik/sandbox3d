﻿#pragma once

#include "../../Rendering/Mesh/Mesh.h"
#include "../../Rendering/Mesh/SubMesh.h"

#include <string>

#include <QDir>
#include <QString>

Mesh loadMesh(const QDir & dir, const QString & filepath);
