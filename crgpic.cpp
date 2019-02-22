#include "crgpic.h"

bool CRgPic::loadFile(const QString &fileName)
{
    QImage image(fileName);
    if (image.isNull()) {
        QMessageBox::information(this, QGuiApplication::applicationDisplayName(),
                                 tr("Cannot load %1.").arg(QDir::toNativeSeparators(fileName)));
        return false;
    }
    QPixmap *pixmap = new QPixmap(QPixmap::fromImage(image.scaled(scene->width(), scene->height())));
//    QGraphicsPixmapItem* pixmapitem = scene->addPixmap(*pixmap);
    scene->addPixmap(*pixmap);
    return true;
}


void CRgPic::loadPic()
{
    QStringList mimeTypeFilters;
    foreach (const QByteArray &mimeTypeName, QImageReader::supportedMimeTypes())
        mimeTypeFilters.append(mimeTypeName);
    mimeTypeFilters.sort();
    const QStringList picturesLocations = QStandardPaths::standardLocations(QStandardPaths::PicturesLocation);
    QFileDialog dialog(this, tr("Open File"), picturesLocations.isEmpty() ? QDir::currentPath() : picturesLocations.first());
    dialog.setAcceptMode(QFileDialog::AcceptOpen);
    dialog.setMimeTypeFilters(mimeTypeFilters);
    dialog.selectMimeTypeFilter("image/jpeg");
    while (dialog.exec() == QDialog::Accepted && !loadFile(dialog.selectedFiles().first())) {}
}


void CRgPic::clearAll()
{
    scene->clearContours();
}


CRgPic::CRgPic(QWidget *parent)
    : QWidget(parent)
{
    scene = new QMyScene(QRectF(0,0,300,400));
    view = new QGraphicsView(scene, this);

    open_bt = new QPushButton(tr("&Open"));
    clear_bt = new QPushButton(tr("&Clear"));
    QHBoxLayout *h_layout = new QHBoxLayout;
    h_layout->addWidget(open_bt);
    h_layout->addWidget(clear_bt);

    QVBoxLayout *v_layout = new QVBoxLayout;
    v_layout->addWidget(view);
    v_layout->addLayout(h_layout);

    setLayout(v_layout);

    connect(open_bt, SIGNAL(clicked()), this, SLOT(loadPic()));
    connect(clear_bt, SIGNAL(clicked()), this, SLOT(clearAll()));
}


CRgPic::~CRgPic(){}





