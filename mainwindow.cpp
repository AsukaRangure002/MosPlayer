/*
视频播放器MosPlayer

已含内容：
    样式美化
    多视频文件载入
    列表选择文件
待添加：
    载入网络视频
    循环播放
*/

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QGridLayout>
#include <QUrl>
#include <QDir>
#include <QFileDialog>
#include <QPixmap>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 组件初始化
    QWidget *m_ctrl = new QWidget(this);
    m_player        = new QMediaPlayer(this);
    m_videowidget   = new QVideoWidget(this);
    m_playList      = new QListWidget(this);
    m_playBtn       = new QPushButton(m_ctrl);
    m_openBtn       = new QPushButton(m_ctrl);
    m_voiceSlider   = new QSlider(Qt::Vertical, m_ctrl);
    m_processSlider = new QSlider(Qt::Horizontal, this);

    m_voiceSlider->setValue(40);    //音量初值
    m_processSlider->setRange(0, 0);

    // 音视频输出
    m_player      = new QMediaPlayer(this);
    m_audioOutput = new QAudioOutput(this);
    m_audioOutput->setVolume(0.4);
    m_player->setVideoOutput(m_videowidget);
    m_player->setAudioOutput(m_audioOutput);

    // 样式
    m_playBtn->setFixedSize(100, 100);
    m_openBtn->setFixedHeight(50);
    m_videowidget->setMinimumSize(400, 300);

    m_playList->setMinimumWidth(100);
    m_playList->setMaximumWidth(200);
    m_voiceSlider->setFixedHeight(76);

    setButtonImage(m_playBtn, play_icon);
    m_playBtn->setIconSize(QSize(80, 80));
    m_playBtn->setFlat(true);

    setWindowIcon(QIcon(":/Icon/Icon.jpg"));
    setWindowTitle("MosPlayer");


    m_openBtn->setObjectName("m_openBtn");  //设置控件对象名
    m_playBtn->setObjectName("m_playBtn");
    m_ctrl->setObjectName("m_ctrl");

    // 布局
    QGridLayout *ctrlLayout = new QGridLayout(m_ctrl);
    QGridLayout *mainLayout = new QGridLayout(this->centralWidget());
    ctrlLayout->addWidget(m_playBtn, 0, 0, 1, 1);

    ctrlLayout->addWidget(m_voiceSlider, 0, 1, 1, 1);
    ctrlLayout->addWidget(m_openBtn, 1, 0, 1, 2);
    mainLayout->addWidget(m_videowidget, 0, 0, 4, 5);
    mainLayout->addWidget(m_processSlider, 4, 0, 1, 5);
    mainLayout->addWidget(m_playList, 0, 5, 3, 1);
    mainLayout->addWidget(m_ctrl, 3, 5, 2, 1);



    // 信号槽
    connect(m_openBtn, &QPushButton::clicked, this, &MainWindow::openBtn_clicked);
    connect(m_playBtn, &QPushButton::clicked, this, &MainWindow::do_stateChanged);
    connect(m_voiceSlider, &QSlider::valueChanged, this, &MainWindow::voiceSlider_changed);
    connect(m_player, &QMediaPlayer::durationChanged, this, &MainWindow::do_durationChanged);
    connect(m_player, &QMediaPlayer::positionChanged, this, &MainWindow::do_positionChanged);
    connect(m_player, &QMediaPlayer::durationChanged, this, &MainWindow::do_durationChanged);
    connect(m_processSlider, &QSlider::sliderMoved, this, &MainWindow::do_sliderMoved);
    connect(m_playList, &QListWidget::doubleClicked, this, &MainWindow::do_listWidget_doubleClicked);
//    qDebug()<<"能接收信息";
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::openBtn_clicked(){
    m_curPath         = QDir::homePath();
    m_dlgTitle        = "请选择视频文件";
    m_filter          = "视频文件(*wmv, *.mp4);;所有文件(*.*)";   // 文件过滤器
    m_fileList        = QFileDialog::getOpenFileNames(this, m_dlgTitle, m_curPath, m_filter);

    if(m_fileList.isEmpty()){
        return;
    }

    // 添加文件到播放列表
    for(int i = 0; i < m_fileList.size(); i++){
        QFileInfo fileInfo(m_fileList[i]);
        QListWidgetItem *m_Item = new QListWidgetItem(fileInfo.fileName());
        m_Item->setData(Qt::UserRole, QUrl::fromLocalFile(m_fileList[i]));
        m_playList->addItem(m_Item);
    }

    // 播放第一个视频
    if(m_player->playbackState() != QMediaPlayer::PlayingState){
        m_playList->setCurrentRow(0);
        QUrl source = getUrlFromItem(m_playList->currentItem());
        m_player->setSource(source);
    }

    m_player->play();
    setButtonImage(m_playBtn, pause_icon);
}

QUrl MainWindow::getUrlFromItem(QListWidgetItem * item){
    // 返回列表播放数据
    QVariant itemData = item->data(Qt::UserRole);
    QUrl source       = itemData.value<QUrl>();
    return source;
}

void MainWindow::do_stateChanged(){
    if(m_player->playbackState() == QMediaPlayer::PlayingState){
        m_player->pause();      // 暂停
        setButtonImage(m_playBtn, play_icon);
    }else if(m_player->playbackState() == QMediaPlayer::PausedState){
        m_player->play();
        setButtonImage(m_playBtn, pause_icon);
    }
}

void MainWindow::setButtonImage(QPushButton *btn, QString Image){
    QPixmap pix    = QPixmap(Image);
    btn->setIcon(QIcon(pix));
    btn->setFlat(true);
}

void MainWindow::voiceSlider_changed(int value){
    m_player->audioOutput()->setVolume(value/100.0);
//    qDebug()<<QString::asprintf("%d", value);
}

void MainWindow::do_listWidget_doubleClicked(const QModelIndex &index){
    Q_UNUSED(index);
    m_loopPlay = false;     // 防止do_stateChanged()切换视频
    m_player->setSource(getUrlFromItem(m_playList->currentItem()));
    m_player->play();
    m_loopPlay = true;
}

void MainWindow::do_positionChanged(qint64 position)
{
    m_processSlider->setValue(position);
}

void MainWindow::do_durationChanged(qint64 duration)
{
    m_processSlider->setRange(0, duration);
}

void MainWindow::do_sliderMoved(int position)
{
    m_player->setPosition(position);
}

