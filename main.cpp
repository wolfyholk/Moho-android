#include <QApplication>
#include <QMainWindow>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QGraphicsPolygonItem>
#include <QSlider>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QToolBar>
#include <QAction>
#include <QLabel>
#include <QPushButton>
#include <QPolygonF>
#include <QMap>
#include <QPointF>

class AnimationEditor : public QMainWindow {
public:
    AnimationEditor() {
        QWidget *central = new QWidget(this);
        QVBoxLayout *mainLayout = new QVBoxLayout(central);
        mainLayout->setContentsMargins(4, 4, 4, 4);

        // Верхняя панель инструментов (в стиле Moho)
        QToolBar *toolbar = addToolBar("Tools");
        toolbar->setMovable(false);
        toolbar->setStyleSheet("QToolBar { background: #2b2b2b; spacing: 8px; padding: 4px; } "
                               "QToolButton { color: white; background: #3c3f41; padding: 8px; border-radius: 4px; }");

        QAction *addBoneAct = toolbar->addAction("+ Кость");
        QAction *addShapeAct = toolbar->addAction("+ Фигура");
        QAction *keyframeAct = toolbar->addAction("Ключевой кадр");
        QAction *clearAct = toolbar->addAction("Очистить");

        // Холст (рабочая область)
        scene = new QGraphicsScene(0, 0, 1280, 720, this);
        scene->setBackgroundBrush(QColor("#e0e0e0"));
        
        // Рамка кадра (камера)
        scene->addRect(0, 0, 1280, 720, QPen(Qt::blue, 3));

        view = new QGraphicsView(scene);
        view->setRenderHint(QPainter::Antialiasing);
        view->setStyleSheet("background: #1e1e1e; border: none;");
        mainLayout->addWidget(view, 1);

        // Нижняя панель таймлайна
        QWidget *timelinePanel = new QWidget();
        timelinePanel->setStyleSheet("background: #2b2b2b; color: white;");
        QHBoxLayout *timelineLayout = new QHBoxLayout(timelinePanel);

        frameLabel = new QLabel("Кадр: 1 / 120");
        frameLabel->setMinimumWidth(110);

        timelineSlider = new QSlider(Qt::Horizontal);
        timelineSlider->setRange(1, 120);
        timelineSlider->setValue(1);

        timelineLayout->addWidget(frameLabel);
        timelineLayout->addWidget(timelineSlider);
        mainLayout->addWidget(timelinePanel);

        setCentralWidget(central);

        // Создаем стартовый объект для теста
        spawnCharacterBlock();

        // Подключаем кнопки
        connect(addBoneAct, &QAction::triggered, this, &AnimationEditor::spawnBone);
        connect(addShapeAct, &QAction::triggered, this, &AnimationEditor::spawnCharacterBlock);
        connect(keyframeAct, &QAction::triggered, this, &AnimationEditor::saveKeyframe);
        connect(clearAct, &QAction::triggered, this, [this]() {
            scene->clear();
            scene->addRect(0, 0, 1280, 720, QPen(Qt::blue, 3));
            activeItem = nullptr;
            keyframes.clear();
        });

        connect(timelineSlider, &QSlider::valueChanged, this, &AnimationEditor::onFrameChanged);
    }

private:
    QGraphicsScene *scene;
    QGraphicsView *view;
    QSlider *timelineSlider;
    QLabel *frameLabel;
    QGraphicsItem *activeItem = nullptr;
    QMap<int, QPointF> keyframes;

    void spawnCharacterBlock() {
        QGraphicsRectItem *rect = scene->addRect(-50, -50, 100, 100, QPen(Qt::black, 2), QBrush(QColor("#4caf50")));
        rect->setPos(640, 360);
        rect->setFlag(QGraphicsItem::ItemIsMovable);
        rect->setFlag(QGraphicsItem::ItemIsSelectable);
        activeItem = rect;
        keyframes[1] = rect->pos();
    }

    void spawnBone() {
        // Треугольная форма кости как в Moho
        QPolygonF bonePoly;
        bonePoly << QPointF(0, 0) << QPointF(15, -15) << QPointF(120, 0) << QPointF(15, 15);
        
        QGraphicsPolygonItem *bone = scene->addPolygon(bonePoly, QPen(Qt::darkRed, 2), QBrush(QColor(255, 80, 80, 180)));
        bone->setPos(600, 360);
        bone->setFlag(QGraphicsItem::ItemIsMovable);
        bone->setFlag(QGraphicsItem::ItemIsSelectable);
        activeItem = bone;
    }

    void saveKeyframe() {
        if (!activeItem) return;
        int currentFrame = timelineSlider->value();
        keyframes[currentFrame] = activeItem->pos();
    }

    void onFrameChanged(int frame) {
        frameLabel->setText(QString("Кадр: %1 / 120").arg(frame));
        if (!activeItem || keyframes.isEmpty()) return;

        // Линейная интерполяция (плавное движение между ключами)
        int prevFrame = 1;
        int nextFrame = 120;
        bool hasNext = false;

        for (auto it = keyframes.begin(); it != keyframes.end(); ++it) {
            if (it.key() <= frame) {
                prevFrame = it.key();
            }
            if (it.key() >= frame && !hasNext) {
                nextFrame = it.key();
                hasNext = true;
            }
        }

        if (prevFrame == nextFrame || !hasNext) {
            activeItem->setPos(keyframes[prevFrame]);
        } else {
            double t = double(frame - prevFrame) / double(nextFrame - prevFrame);
            QPointF p1 = keyframes[prevFrame];
            QPointF p2 = keyframes[nextFrame];
            activeItem->setPos(p1 + (p2 - p1) * t);
        }
    }
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    AnimationEditor editor;
    editor.show();
    return app.exec();
}
