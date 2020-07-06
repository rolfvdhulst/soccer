//
// Created by rolf on 12-04-20.
//

#include <QtWidgets/QFileDialog>
#include "ReplayWidget.h"
#include <QDateTime>

ReplayWidget::ReplayWidget(QWidget* parent) : QWidget(parent){
    mainLayout = new QVBoxLayout();
    sliderLayout = new QHBoxLayout();

    fileName = new QLabel();
    QHBoxLayout * fileNameLayout = new QHBoxLayout();
    fileNameLayout->addWidget(fileName);
    exitButton = new QPushButton();
    exitButton->setMaximumSize(24,24);
    exitButton->setIcon(QIcon::fromTheme("application-exit"));
    connect(exitButton,&QPushButton::clicked,this,&QWidget::close);
    fileNameLayout->addWidget(exitButton,0,Qt::AlignRight);
    mainLayout->addLayout(fileNameLayout);
    pausePlayShortcut = new QShortcut(this);
    leftShortcut = new QShortcut(this);
    rightShortcut = new QShortcut(this);
    pausePlayShortcut->setKey(Qt::Key_Space);
    leftShortcut->setKey(Qt::Key_Left);
    rightShortcut->setKey(Qt::Key_Right);


    beforeInfo = new QVBoxLayout();
    currentTime = new QLabel(stringFromTime(Time(0.0)));
    currentPacket = new QLabel("0");
    beforeInfo->addWidget(currentTime);
    beforeInfo->addWidget(currentPacket);

    afterInfo = new QVBoxLayout();
    totalTime = new QLabel(stringFromTime(Time(lastTime-firstTime)));
    totalPacket = new QLabel("0");
    afterInfo->addWidget(totalTime);
    afterInfo->addWidget(totalPacket);

    timeSlider = new QSlider(Qt::Horizontal);
    timeSlider->setValue(0);
    timeSlider->setMaximum(0);
    sliderLayout->addLayout(beforeInfo);
    sliderLayout->addWidget(timeSlider);
    sliderLayout->addLayout(afterInfo);
    connect(timeSlider,&QSlider::sliderReleased,this,&ReplayWidget::seekFrame);

    mainLayout->addLayout(sliderLayout);
    toolBar = new QToolBar();

    playButton = new QToolButton();

    playButton->setIcon(QIcon::fromTheme("media-playback-start"));
    toolBar->addWidget(playButton);
    connect(playButton,&QToolButton::pressed,this,&ReplayWidget::togglePlay);

    stepBackwardButton = new QToolButton();
    stepBackwardButton->setIcon(QIcon::fromTheme("media-skip-backward"));
    toolBar->addWidget(stepBackwardButton);
    connect(stepBackwardButton,&QToolButton::pressed, this, &ReplayWidget::stepBackward);

    stepForwardButton = new QToolButton();
    stepForwardButton->setIcon(QIcon::fromTheme("media-skip-forward"));
    toolBar->addWidget(stepForwardButton);
    connect(stepForwardButton,&QToolButton::pressed, this, &ReplayWidget::stepForward);

    playTimer = new QTimer();
    connect(playTimer,&QTimer::timeout,this,&ReplayWidget::updateInformation);

    playSpeed = new QComboBox();
    playSpeed->setMaximumWidth(80);

    QVector<QVariant> speeds ={0.05,0.1,0.25,0.5,1.0,2.0,5.0,10.0};
    for (const auto& speed : speeds) {
        playSpeed->addItem(speed.toString(),speed);
    }

    playSpeed->setCurrentIndex(4);
    connect(playSpeed,SIGNAL(currentIndexChanged(int)),this,SLOT(setSpeed(int)));
    toolBar->addWidget(playSpeed);

    mainLayout->addWidget(toolBar);
    groupBox = new QGroupBox("Replay");
    groupBox->setLayout(mainLayout);

    totalLayout = new QHBoxLayout();
    totalLayout->addWidget(groupBox);
    setLayout(totalLayout);//Bind the main layout to the widget
    close(); // Not open by default
}
ReplayWidget::~ReplayWidget() {

}
void ReplayWidget::openFile() {
    QFileDialog dialog(this);

    QDir directory = dialog.directory();
    while(directory.dirName() != "soccer"){
        bool success = directory.cdUp();
        if(!success){
            std::cerr<<"Could not find application root folder, did you rename or are you not running from root?"<<std::endl;
            break;
        }
    }
    if(!directory.cd("logfiles")){
        std::cout<<"Could not find logfiles directory!"<<std::endl;
    }
    dialog.setDirectory(directory);

    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setNameFilter(tr("Logfiles (*.log)"));
    dialog.setViewMode(QFileDialog::Detail);
    if (dialog.exec()){
        QString filePath = dialog.selectedFiles().first();
        openFile(filePath);
    }
}
void ReplayWidget::openRecentFile() {
    QDir directory = QDir::current();
    while(directory.dirName() != "soccer"){
        bool success = directory.cdUp();
        if(!success){
            std::cerr<<"Could not find application root folder, did you rename or are you not running from root?"<<std::endl;
            std::cerr<<"Could not open log file"<<std::endl;
            return;
        }
    }
    if(!directory.cd("logfiles")){
        std::cerr<<"Could not find logfiles directory!"<<std::endl;
        return;
    }
    QFileInfoList fileInfos= directory.entryInfoList(QDir::Files, QDir::Time);
    if(!fileInfos.empty()){
        openFile(fileInfos.first().absoluteFilePath());
    }else{
        std::cerr<<"Could not find any recent files in your logfiles directory!"<<std::endl;
    }
}
void ReplayWidget::openFile(const QString &filePath) {
    if(fileIsOpen){
        closeFile();
    }
    fileIsOpen = logReader.open(filePath);
    if(fileIsOpen){
        //We opened the file succesfully and indexed it
        //Setup visualizations correctly
        emit replayActive(true);
        show();
        fileName->setText(filePath);
        totalPacket->setText(QString::number(logReader.fileMessageCount()-1));

        lastTime = logReader.frameAt(logReader.fileMessageCount()-1).first;//index starts at 0
        firstTime = logReader.frameAt(0).first;
        Time totalDuration = Time(lastTime-firstTime);
        totalTime->setText(stringFromTime(totalDuration));

        timeSlider->setMaximum((int) logReader.fileMessageCount()-1);
        timeSlider->setValue(0);
        auto frame = logReader.frameAt(0);
        emit gotLogFrame(frame.second);//Send the first tick to all widgets
        logReader.resetToStartOfFile();
        currentReplayTime = 0;
        currentPacketNumber = 0;
        updateTimerInfo(frame.first);
        connectShortcuts();
    } else{
        //We could not succesfully open the file
    }
}
QString ReplayWidget::stringFromTime(const Time &time) {
    return QString("%1:%2.%3")
            .arg(time.asIntegerSeconds() /60)
            .arg(time.asIntegerSeconds()%60,2,10,QChar('0'))
            .arg(time.asIntegerMilliSeconds()%1000,3,10,QChar('0'));
}
void ReplayWidget::togglePlay() {
    //Toggle playing/pause
    isPlaying = !isPlaying;
    if(isPlaying){
        playButton->setIcon(QIcon::fromTheme("media-playback-pause"));
        playTimer->start(STEP_MS);
    }else{
        playButton->setIcon(QIcon::fromTheme("media-playback-start"));
        playTimer->stop();
    }

}
void ReplayWidget::setSpeed(int index) {
    speedFactor = playSpeed->itemData(index).toDouble();
}
void ReplayWidget::updateInformation() {
    currentReplayTime += (long)(STEP_MS*1000000*speedFactor);
    while(currentPacketNumber<logReader.fileMessageCount()-1 &&
    logReader.getNextTime()<firstTime+currentReplayTime){
        auto frame = logReader.nextFrame();
        currentPacketNumber ++;
        updateTimerInfo(frame.first); //TODO: somehow is still wrong at times... not sure what's going wrong
        emit gotLogFrame(frame.second);
    }
    //we reached end-of file, pause and also correct packet number
    if(currentPacketNumber == logReader.fileMessageCount()-1){
        togglePlay();
    }
}
void ReplayWidget::updateTimerInfo(long long int time) {

    if(!timeSlider->isSliderDown()){ //Check if user has grabbed slider to change time
        timeSlider->setValue(currentPacketNumber);
    }
    currentPacket->setText(QString::number(currentPacketNumber));
    currentTime->setText(stringFromTime(Time((long) time -firstTime)));
}
void ReplayWidget::stepForward() {
    if(currentPacketNumber<logReader.fileMessageCount()-1){
        auto frame = logReader.nextFrame();
        currentPacketNumber ++;
        currentReplayTime = frame.first - firstTime;
        updateTimerInfo(frame.first);
        emit gotLogFrame(frame.second);
    }
}

void ReplayWidget::stepBackward() {
    if(currentPacketNumber>0){
        currentPacketNumber--;
        auto frame = logReader.frameAt(currentPacketNumber);
        currentReplayTime = frame.first-firstTime;
        updateTimerInfo(frame.first);
        emit gotLogFrame(frame.second);
    }
}

void ReplayWidget::seekFrame() {
    int index = timeSlider->value();
    currentPacketNumber = index;
    auto frame = logReader.frameAt(index);
    currentReplayTime = frame.first-firstTime;
    updateTimerInfo(frame.first);
    emit gotLogFrame(frame.second);
}
bool ReplayWidget::isRunning() const {
    return fileIsOpen;
}
void ReplayWidget::closeEvent(QCloseEvent * event) {
    if(fileIsOpen){
        closeFile();
        emit replayActive(false);
    }
}
void ReplayWidget::connectShortcuts() {
    connect(pausePlayShortcut,&QShortcut::activated,this,&ReplayWidget::togglePlay);
    connect(leftShortcut,&QShortcut::activated,this,&ReplayWidget::stepBackward);
    connect(rightShortcut,&QShortcut::activated,this,&ReplayWidget::stepForward);
}
void ReplayWidget::disconnectShortcuts() {
    disconnect(pausePlayShortcut,&QShortcut::activated,this,&ReplayWidget::togglePlay);
    disconnect(leftShortcut,&QShortcut::activated,this,&ReplayWidget::stepBackward);
    disconnect(rightShortcut,&QShortcut::activated,this,&ReplayWidget::stepForward);
}
void ReplayWidget::closeFile() {
    disconnectShortcuts();
    if(isPlaying){
        togglePlay();
    }
    logReader.close();
    fileIsOpen = false;

}
