#ifndef SFMLVIEW_H
#define SFMLVIEW_H

#include <QQuickItem>
#include <QSGSimpleTextureNode>
#include <QOpenGLContext>
#include <SFML/Graphics.hpp>
#include <QOffscreenSurface>

class SFMLView : public QQuickItem
{
    Q_OBJECT
    public:
        SFMLView();
        QSGNode *updatePaintNode(QSGNode *oldNode, QQuickItem::UpdatePaintNodeData *) override;
        Q_INVOKABLE void updateItem()
        {
            qDebug() << __FUNCTION__;
            m_start = true;
            emit update();
        };

    signals:
    private:
        bool m_start{false};
        void _initContext();
        void _generateTexture();
        void _displayCurrentContext();
        std::shared_ptr<QSGSimpleTextureNode> m_node;
        QOpenGLContext *m_context;
        sf::Context *m_contextSFML;
        QOffscreenSurface *m_offscreenSurface;
        sf::RenderTexture *m_renderTexture;
};

#endif // SFMLVIEW_H
