#include "NodeTextureCreationWindow.h"
#include "ui_NodeTextureCreationWindow.h"

#include <GL/glcorearb.h>

/**
 * @brief NodeTextureCreationWindow::NodeTextureCreationWindow
 * @param pParent
 */
NodeTextureCreationWindow::NodeTextureCreationWindow(QWidget * pParent)
: QDialog(pParent)
, ui(new Ui::NodeTextureCreationWindow)
{
	ui->setupUi(this);

	// Unsigned Normalized
	ui->comboBox->addItem(QString("GL_R8"), QVariant::fromValue(GL_R8));
	ui->comboBox->addItem(QString("GL_RG8"), QVariant::fromValue(GL_RG8));
	ui->comboBox->addItem(QString("GL_RGBA8"), QVariant::fromValue(GL_RGBA8));

	ui->comboBox->addItem(QString("GL_R16"), QVariant::fromValue(GL_R16));
	ui->comboBox->addItem(QString("GL_RG16"), QVariant::fromValue(GL_RG16));
	ui->comboBox->addItem(QString("GL_RGBA16"), QVariant::fromValue(GL_RGBA16));

	// Float
	ui->comboBox->addItem(QString("GL_R16F"), QVariant::fromValue(GL_R16F));
	ui->comboBox->addItem(QString("GL_RG16F"), QVariant::fromValue(GL_RG16F));
	ui->comboBox->addItem(QString("GL_RGBA16F"), QVariant::fromValue(GL_RGBA16F));

	ui->comboBox->addItem(QString("GL_R32F"), QVariant::fromValue(GL_R32F));
	ui->comboBox->addItem(QString("GL_RG32F"), QVariant::fromValue(GL_RG32F));
	ui->comboBox->addItem(QString("GL_RGBA32F"), QVariant::fromValue(GL_RGBA32F));

	// Signed Integral
	ui->comboBox->addItem(QString("GL_R8I"), QVariant::fromValue(GL_R8I));
	ui->comboBox->addItem(QString("GL_RG8I"), QVariant::fromValue(GL_RG8I));
	ui->comboBox->addItem(QString("GL_RGBA8I"), QVariant::fromValue(GL_RGBA8I));

	ui->comboBox->addItem(QString("GL_R16I"), QVariant::fromValue(GL_R16I));
	ui->comboBox->addItem(QString("GL_RG16I"), QVariant::fromValue(GL_RG16I));
	ui->comboBox->addItem(QString("GL_RGBA16I"), QVariant::fromValue(GL_RGBA16I));

	ui->comboBox->addItem(QString("GL_R32I"), QVariant::fromValue(GL_R32I));
	ui->comboBox->addItem(QString("GL_RG32I"), QVariant::fromValue(GL_RG32I));
	ui->comboBox->addItem(QString("GL_RGBA32I"), QVariant::fromValue(GL_RGBA32I));

	// Unsigned Integral
	ui->comboBox->addItem(QString("GL_R8UI"), QVariant::fromValue(GL_R8UI));
	ui->comboBox->addItem(QString("GL_RG8UI"), QVariant::fromValue(GL_RG8UI));
	ui->comboBox->addItem(QString("GL_RGBA8UI"), QVariant::fromValue(GL_RGBA8UI));

	ui->comboBox->addItem(QString("GL_R16UI"), QVariant::fromValue(GL_R16UI));
	ui->comboBox->addItem(QString("GL_RG16UI"), QVariant::fromValue(GL_RG16UI));
	ui->comboBox->addItem(QString("GL_RGBA16UI"), QVariant::fromValue(GL_RGBA16UI));

	ui->comboBox->addItem(QString("GL_R32UI"), QVariant::fromValue(GL_R32UI));
	ui->comboBox->addItem(QString("GL_RG32UI"), QVariant::fromValue(GL_RG32UI));
	ui->comboBox->addItem(QString("GL_RGBA32UI"), QVariant::fromValue(GL_RGBA32UI));

	// Special
	ui->comboBox->addItem(QString("GL_RGB10_A2"), QVariant::fromValue(GL_RGB10_A2));
	ui->comboBox->addItem(QString("GL_RGB10_A2UI"), QVariant::fromValue(GL_RGB10_A2UI));
	ui->comboBox->addItem(QString("GL_R11F_G11F_B10F"), QVariant::fromValue(GL_R11F_G11F_B10F));
	ui->comboBox->addItem(QString("GL_SRGB8_ALPHA8"), QVariant::fromValue(GL_SRGB8_ALPHA8));

	ui->comboBox->addItem(QString("GL_DEPTH_COMPONENT16"), QVariant::fromValue(GL_DEPTH_COMPONENT16));
	ui->comboBox->addItem(QString("GL_DEPTH_COMPONENT24"), QVariant::fromValue(GL_DEPTH_COMPONENT24));
	ui->comboBox->addItem(QString("GL_DEPTH_COMPONENT32F"), QVariant::fromValue(GL_DEPTH_COMPONENT32F));
	ui->comboBox->addItem(QString("GL_DEPTH24_STENCIL8"), QVariant::fromValue(GL_DEPTH24_STENCIL8));
	ui->comboBox->addItem(QString("GL_DEPTH32F_STENCIL8"), QVariant::fromValue(GL_DEPTH32F_STENCIL8));
	ui->comboBox->addItem(QString("GL_STENCIL_INDEX8"), QVariant::fromValue(GL_STENCIL_INDEX8));
}

/**
 * @brief NodeTextureCreationWindow::~NodeTextureCreationWindow
 */
NodeTextureCreationWindow::~NodeTextureCreationWindow()
{
	// nothing here
}

/**
 * @brief NodeTextureCreationWindow::getCurrentFormat
 * @return
 */
unsigned int NodeTextureCreationWindow::getCurrentFormat(void) const
{
	return(0);
}
