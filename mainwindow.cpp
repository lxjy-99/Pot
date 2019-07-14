#include "mainwindow.h"
#include "ui_mainwindow.h"

#include<math.h>
#include<stdlib.h>
#include<time.h>
#include<QDebug>
#include<QString>

#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
    #if defined(_MSC_VER) && (_MSC_VER > 1600)
        // Coding: UTF-8
        #pragma execution_character_set("utf-8")
    #endif
#endif //预编译头，解决中文乱码问题

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setFixedSize(700, 600);    //设置窗口大小
    setWindowTitle(tr("疯狂海底捞"));
    QPixmap pixmap(":/images/iii/ccc.png");
    QCursor cursor= QCursor(pixmap,-1,-1);
    setCursor(cursor);

    interval_angle=15;
    interval_miner=200;

    food=new Food;
    map=new Map;

    QPalette pal;
    pal.setColor(QPalette::WindowText, Qt::yellow);
    ui->label_final->setPalette(pal);
    //ui->lcdNumber->setPalette(pal);
    //ui->lcdNumber_sum->setPalette(pal);
   // ui->lcdNumber_speed->setPalette(pal);

    this->setStyleSheet("#MainWindow{border-image:url(:/images/iii/background.jpg);}");
    pix.load("://images//iii//hook1.png");

    timer_angle.setInterval(interval_angle);//设置触发间隔为15ms
    connect(&timer_angle,&QTimer::timeout,this,&MainWindow::rotate);   //时间到了窗口就旋转
    connect(&timer_angle,&QTimer::timeout,this,&MainWindow::ss);

    timer_s.setInterval(interval_miner);    //s控制运动
    connect(&timer_s,&QTimer::timeout,this,&MainWindow::mm);     //随机生成

    timer_clock.setInterval(1000);  //计时器时间间隔为 1 S
    connect(&timer_clock,&QTimer::timeout,this,&MainWindow::timereduce);

    rad=180/3.1415926;//弧度
    angle_change=1;//角度转变量为1°

    lable[0]=ui->label_1;lable[1]=ui->label_2;lable[2]=ui->label_3;
    lable[3]=ui->label_4;lable[4]=ui->label_5;lable[5]=ui->label_6;
    lable[6]=ui->label_7;lable[7]=ui->label_8;lable[8]=ui->label_9;
    lable[9]=ui->label_10;//鸡腿
    lable[10]=ui->label_11;lable[11]=ui->label_12;lable[12]=ui->label_13;//11为加速，12减速，13加时间

    lable[13]=ui->label_14;//随机

    init();
    do_stop();//暂停，跳出帮助菜单
    ui->label_pause->setVisible(false);//暂停图案
    ui->toolButton_ppause->setVisible(false);

    ui->label_begin->setVisible(true);
    ui->toolButton_start->setVisible(true);
}
void MainWindow::init()
{
    ui->label_pause->setVisible(false);//暂停图案
    ui->toolButton_ppause->setVisible(false);

    ui->label_finish->setVisible(false);//结束图案
    ui->toolButton_restart->setVisible(false);
    ui->toolButton_cancel->setVisible(false);

    ui->label_final->setVisible(false);
    ui->lcdNumber_speed->setVisible(false);

    food->init();
    map->init();//地图初始化设置

    time_left=60;//剩余时间为60s
    time=time_up=time_down=0;
    k=1;
    for(int i=0;i<6;i++)
    {
        lable[i]->setVisible(true);
        lable[i]->move(map->get_point(i));
        if(i<2)
            lable[i]->resize(140,120);
        else
            lable[i]->resize(100,100);
    }
    for(int i=6;i<14;i++)//让随机不可见
        lable[i]->setVisible(false);

    ui->toolButton->setStyleSheet("#toolButton{border-image:url(:/images/iii/1.png);}");
    ui->lcdNumber->display(time_left);
    ui->lcdNumber_sum->display(food->get_sum());
    timer_angle.start();
    timer_clock.start();
    timer_s.start();
}

void MainWindow::paintEvent(QPaintEvent *)
{

    QPainter painter(this);
    painter.translate(350,120-20);
    painter.rotate(food->get_angle());
    painter.translate(-350,-100);

    int length=food->get_length();
    if(food->isrotating()==false||food->isstopped())
    {
        QPen pen;
        pen.setWidth(6);
        painter.setPen(pen);
        painter.setRenderHint(QPainter::Antialiasing, true);
        painter.drawLine(350,95,350,95+length);
        painter.drawPixmap(320,95+length,pix);//绘图的起点
    }
    else
       painter.drawPixmap(320,95,pix);//绘图的起点
    update();
}

void MainWindow::keyPressEvent(QKeyEvent *e)
{
    int key=e->key();
    if (!food->isstopped()&&key ==Qt::Key_Down)
    {
        if(food->isrotating())
        {

           food->rotating(false);
        }
    }
    if(key==Qt::Key_Space)//暂停功能
    {
        if(food->isstopped()&&time_left>0)
            resume();
        else
            do_stop();
    }
}
bool MainWindow::detection()//碰撞和越界检测
{
    int angle=food->get_angle();
    int length=food->get_length();

    if(fabs(angle)>45){
        if(length>(350/(sin(fabs(angle/rad)))))
             return true;
    }
    else if(length>(500/(cos(angle/rad))))
             return true;
    if(map->detection(350-length*sin(angle/rad),95+length*cos(angle/rad)))
    {
        food->grab(true);
        return true;
    }
    return false;
}
void MainWindow::do_stop()
{
    if(time_left>0){
        ui->label_pause->setVisible(true);//暂停图案
        ui->toolButton_ppause->setVisible(true);
    }
    food->rotating(!(food->isrotating()));
    timer_clock.stop();
    timer_angle.stop();
    timer_s.stop();
    food->set_stopped(true);
}
void MainWindow::resume()
{
    ui->label_pause->setVisible(false);//暂停图案
    ui->toolButton_ppause->setVisible(false);

    food->rotating(!(food->isrotating()));
    timer_clock.start();
    timer_angle.start();
    timer_s.start();
    food->set_stopped(false);
}
MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::timereduce()//计时器
{
    if(time_left>=0)
    {
        time++;
        ui->lcdNumber->display(time_left--);
        if(time_down==0&&time_up==0)
        {
            k=1;
            timer_s.setInterval(interval_miner);
            timer_angle.setInterval(interval_angle);
           // ui->lcdNumber_speed->setVisible(false);
        }
        if(time_down)
        {
            k=1;
            --time_down;
            //ui->lcdNumber_speed->setVisible(true);
            //ui->lcdNumber_speed->display(time_down);
        }
        if(time_up)
        {
            --time_up;
            k=3;
           //ui->lcdNumber_speed->setVisible(true);
           // ui->lcdNumber_speed->display(time_up);
        }
        if(time%8==0)//每8秒，随机生成一次
        {
            map->rrand();
            for(int i=6;i<14;i++)
                if(map->is_visible(i))//如果该物体可见
                {
                    lable[i]->setVisible(true);
                    lable[i]->move(map->get_point(i));

                }
        }
    }
    if(time_left<0)//剩余时间为0，则停止游戏
    {
        do_stop();
        ui->label_finish->setVisible(true);
        ui->toolButton_cancel->setVisible(true);
        ui->toolButton_restart->setVisible(true);
        ui->label_final->setVisible(true);
        QString s = QString::number(food->get_sum(), 10);
        ui->label_final->setText(s);
    }
}
void MainWindow::rotate()//控制旋转
{
    if(food->isrotating())//旋转时
    {
       food->change_angle(angle_change);
        int angle=food->get_angle();
        if(angle>=90||angle<=-90)
            angle_change*=-1;
    }
    update();
}
void MainWindow::mm()//动作设置
{
    if(!food->isrotating())
    {
        if(food->isfflag())//动作设置
            ui->toolButton->setStyleSheet("#toolButton{border-image:url(:/images/iii/2.png);}");
        else
            ui->toolButton->setStyleSheet("#toolButton{border-image:url(:/images/iii/3.png);}");
    }

}

void MainWindow::ss()//伸展操作
{
    if(!food->isrotating())
    {
        if(food->isgrab())//物体的移动
        {
            food->change_rlength(k*fabs(food->get_speed()));
            int rlength=food->get_rlength();
            int angle=food->get_angle();
            QPoint p=map->get_point(map->get_obj());
            QPoint temp(rlength*sin(angle/rad),-rlength*cos(angle/rad));//temp为位移大小
            lable[map->get_obj()]->move((p+temp));
        }
        if(food->isgrab()||detection())
        {
            if(food->isgrab())
                food->ssp(map->get_obj_sp());
            else
                food->ssp(-1*(food->get_speed()));
        }

        food->change_length(k*food->get_speed());
        if(food->get_length()<=0)
        {
            if(food->isgrab())//如果抓到了东西
            {
                map->invisible();
                lable[map->get_obj()]->setVisible(false);
                food->change_sum(map->get_obj_pr());
                food->srlength(0);
                food->grab(false);
                if(map->get_obj_kd())//如果抓到了特殊物品
                {
                    int kind=map->get_obj_kd();
                    if(kind==1)
                    {
                        time_up=10;
                        time_down=0;
                        timer_s.setInterval(30);
                        k=3;
                    }else if(kind==2)
                    {
                        time_down=10;
                        time_up=0;
                        timer_s.setInterval(interval_miner*2);
                        timer_angle.setInterval(interval_angle*2);
                        k=1;
                    }
                    else
                        time_left+=15;
                }
            }
            food->rotating(true);
            food->ssp(4);
        }
    }
    ui->lcdNumber_sum->display(food->get_sum());
    update();
}
void MainWindow::on_toolButton_reset_clicked()
{
    init();
}

void MainWindow::on_toolButton_continue_clicked()
{
    if(food->isstopped()&&time_left>0)
        resume();
    else
        do_stop();
}

void MainWindow::on_toolButton_start_clicked()
{
    ui->label_begin->setVisible(false);
    ui->toolButton_start->setVisible(false);
    resume();
}

void MainWindow::on_toolButton_ppause_clicked()
{
    resume();
}

void MainWindow::on_toolButton_restart_clicked()
{
    init();
}

void MainWindow::on_toolButton_cancel_clicked()
{
    qApp->quit();
//    ui->label_finish->setVisible(false);
//    ui->toolButton_cancel->setVisible(false);
//    ui->toolButton_restart->setVisible(false);
}
