#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <QMediaPlayer>
#include <QVideoWidget>
#include <QAudioOutput>
#include <QListWidget>
#include <QPushButton>



QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void openBtn_clicked();                              // 打开文件
    void do_stateChanged();                              // 播放状态切换
    void voiceSlider_changed(int value);                 // 音量调节
    void do_durationChanged(qint64 duration);            // 播放时长变化
    void do_positionChanged(qint64 position);            // 进度条变化
    void do_listWidget_doubleClicked(const QModelIndex &index);     //播放列表双击
    void do_sliderMoved(int position);                   // 调节进度条

private:
    Ui::MainWindow *ui;

    QMediaPlayer* m_player;             // 播放器
    QVideoWidget* m_videowidget;        // 视频输出
    QAudioOutput* m_audioOutput;        // 音频输出
    QListWidget*  m_playList;           // 播放列表
    QPushButton*  m_playBtn;            // 暂停/播放按钮
    QPushButton*  m_openBtn;            // 资源打开按钮
    QSlider*      m_voiceSlider;        // 音量条
    QSlider*      m_processSlider;      // 进度条
    qint64        m_duration;           // 视频资源总时长
    qint64        m_position;           // 视频当前播放位置
    QString       m_curPath;            // 文件路径
    QString       m_dlgTitle;           // 文件对话框标题栏
    QString       m_filter;             // 文件过滤器
    QStringList   m_fileList;           // 播放列表
    QString       m_curPlay;            // 当前文件名

    bool          m_loopPlay = true;    // 是否循环播放

    QString play_icon  = ":/Icon/play.png";
    QString pause_icon = ":/Icon/pause.png";
    QString open_icon  = ":/Icon/open.png";

    void setButtonImage(QPushButton *btn, QString Image);
    QUrl getUrlFromItem(QListWidgetItem * item);        // 返回item播放数据
};
#endif // MAINWINDOW_H
