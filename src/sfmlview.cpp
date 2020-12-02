#include "sfmlview.h"

#include <QQuickWindow>
#include <QSurface>
#include <SFML/Config.hpp>
#include <GL/glx.h>
#include <GL/glu.h>
#include <QtPlatformHeaders/QGLXNativeContext>
#include <QOpenGLDebugLogger>
#include <QOpenGLFunctions>
SFMLView::SFMLView()
{
    setFlag(ItemHasContents, true);
}

QSGNode *SFMLView::updatePaintNode(QSGNode *oldNode, QQuickItem::UpdatePaintNodeData *)
{
   QSGSimpleTextureNode *node = static_cast<QSGSimpleTextureNode *>(oldNode);

    if (!node && m_start) {
        _initContext();
        node = m_node.get();
    }
    if (node)
    {
        _generateTexture();
        node->setRect(boundingRect());
        node->markDirty(QSGNode::DirtyForceUpdate);
        node->markDirty(QSGNode::DirtyGeometry);
        node->markDirty(QSGNode::DirtyMaterial);
    }

    return node;
}

void SFMLView::_initContext()
{
    QOpenGLContext *m_mainContext = QOpenGLContext::currentContext();
    _displayCurrentContext();
    m_mainContext->doneCurrent();

    m_contextSFML = new sf::Context();
    qDebug() << "MAJOR " << m_contextSFML->getSettings().majorVersion;
    qDebug() << "MINOR " << m_contextSFML->getSettings().minorVersion;

    GLXContext glxContext;
    glxContext = glXGetCurrentContext();
    assert(glxContext);

    m_context = new QOpenGLContext();
    m_context->setNativeHandle(QVariant::fromValue(QGLXNativeContext(glxContext)));
    m_context->setShareContext(m_mainContext);

    QSurfaceFormat format = m_context->format();
    format.setOption(QSurfaceFormat::DebugContext);
    m_context->setFormat(format);

    assert(m_context->create());
    assert(m_context->isValid());

    m_offscreenSurface = new QOffscreenSurface();
    m_offscreenSurface->setFormat(m_context->format());
    m_offscreenSurface->create();
    assert(m_offscreenSurface->isValid());

    m_context->makeCurrent(m_offscreenSurface);


    m_renderTexture = new sf::RenderTexture();
    if (m_renderTexture->create(800, 600) == false)
        qDebug() << "Fail to init render texture";

    m_node = std::make_shared<QSGSimpleTextureNode>();

    _displayCurrentContext();
}

void SFMLView::_generateTexture()
{
    QOpenGLContext *m_mainContext = QOpenGLContext::currentContext();
    _displayCurrentContext();
    m_mainContext->doneCurrent();

    m_context->makeCurrent(m_offscreenSurface);

    _displayCurrentContext();

    m_renderTexture->setActive(true);

    m_renderTexture->clear(sf::Color::Red);
    m_renderTexture->display();

    m_renderTexture->getTexture().copyToImage().saveToFile("/home/anthony/try.png");
    GLuint texture = static_cast<GLuint>(m_renderTexture->getTexture().getNativeHandle());

    qDebug() << "NATIVE HANDLE " << texture;


    GLenum error = glGetError();
    qDebug() << "GL ERROR " << error;


    QImage image(m_renderTexture->getTexture().copyToImage().getPixelsPtr(), 800, 600, QImage::Format_RGBA8888);

    QSGTexture *wrapper = window()->createTextureFromImage(image);

    QQuickWindow::CreateTextureOptions opts;
    /*
    opts.setFlag(QQuickWindow::TextureHasAlphaChannel, true);
    opts.setFlag(QQuickWindow::TextureOwnsGLTexture, true);
    opts.setFlag(QQuickWindow::TextureHasMipmaps, false);*/

    wrapper =   window()->createTextureFromNativeObject(QQuickWindow::NativeObjectTexture,
                                                   &texture,
                                                   0,
                                                   QSize(800, 600), opts);

    if (wrapper == nullptr)
        qDebug() << "wraper not init";

    m_node->setTexture(wrapper);

    qDebug() << "TEXTURE SIZE " << wrapper->textureSize();

    m_context->doneCurrent();
}

void SFMLView::_displayCurrentContext()
{
    qDebug() << "Current Context INFO " << QOpenGLContext::currentContext()->surface()->format().version();
}
