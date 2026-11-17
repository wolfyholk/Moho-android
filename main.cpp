#include <QApplication>
#include <QMainWindow>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QSlider>
#include <QVBoxLayout>
#include <QToolBar>
#include <QAction>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    QMainWindow window;
    QWidget *central = new QWidget(&window);
    QVBoxLayout *layout = new QVBoxLayout(central);

    // Холст для рисования и костей
    QGraphicsScene *scene = new QGraphicsScene(0, 0, 1280, 720);
    QGraphicsView *view = new QGraphicsView(scene);
    scene->addRect(100, 100, 200, 200, QPen(Qt::black), QBrush(Qt::cyan));

    // Таймлайн (шкала кадров)
    QSlider *timeline = new QSlider(Qt::Horizontal);
    timeline->setRange(1, 120);

    // Панель инструментов
    QToolBar *toolbar = window.addToolBar("Tools");
    toolbar->addAction("Добавить кость");
    toolbar->addAction("Ключевой кадр");

    layout->addWidget(view);
    layout->addWidget(timeline);
    window.setCentralWidget(central);
    window.show();

    return app.exec();
}

