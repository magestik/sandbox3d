#include "TextureLoader.h"

#include <QImage>
#include <QGLWidget>

#include "../../Rendering/Rendering.h"

GPU::Texture<GL_TEXTURE_2D> * loadTexture(const QDir & dir, const QString & filename)
{
	QImage img(dir.filePath(filename));

	QImage tex = QGLWidget::convertToGLFormat(img);

	size_t size = (tex.width() * tex.height() * 4 * sizeof(char));

	if (size > 0)
	{
		GPU::Texture<GL_TEXTURE_2D> * pTexture = new GPU::Texture<GL_TEXTURE_2D>();

		pTexture->init<GL_RGBA8>(tex.width(), tex.height());

		glBindTexture(GL_TEXTURE_2D, pTexture->GetObject());

		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, tex.width(), tex.height(), GL_RGBA, GL_UNSIGNED_BYTE, tex.bits());

		glBindTexture(GL_TEXTURE_2D, 0);

		return(pTexture);
	}

	return(nullptr);

}
