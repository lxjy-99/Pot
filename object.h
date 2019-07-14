#ifndef OBJECT_H
#define OBJECT_H

#include<stdlib.h>
#include<time.h>
#include<QtGui>
#include<QImage>

class Object
{
private:
    int x,y;
    int height,width;
    int price;
    int kind;//0表示普通物质，1为加速，2为减速
    double speed;
    bool visible;
public:

    Object(){
        visible=true;
    }
    void spo(const int &xx,const int &yy);
    void ssz(const int &wwidth,const int &hheight);
    void spr(const int &pprice);
    void ssp(const double &sspeed);
    void skd(const int &kkind);
    void svisible();
    void sinvisible();

    int get_kind(){return kind;}
    int get_price(){return price;}
    int get_x(){return x;}
    int get_y(){return y;}
    double get_speed(){return speed;}

    bool is_visible(){return visible;}


    void init();

    bool detection(const int &xx,const int &yy);//碰撞检测
};


class Map
{
private:
    Object *object[14];
    int obj;
public:
    Map();
    int get_obj(){return obj;}
    double get_obj_sp(){return object[obj]->get_speed();}
    int get_obj_pr(){return object[obj]->get_price();}
    int get_obj_kd(){return object[obj]->get_kind();}
    QPoint get_point(const int &i);
    int get_kind(const int &i);
    bool is_visible(const int &i);

    bool detection(const int &x,const int &y);//碰撞检测

    void invisible();
    void rrand();//随机生成

    void init();
};

class Food
{
private:
    int angle;
    double speed;
    double length;
    double rlength;
    int sum;

    bool is_rotating;
    bool is_grab;
    bool stopped;
    bool fflag;

public:
    void change_angle(const int &aangle);
    void change_length(const double &llength);
    void change_rlength(const double &rrlength);
    void change_sum(const double &ssum);

    void slength(const double &llength);
    void srlength(const double &rrlength);
    void ssp(const double &sspeed);

    void rotating(bool c);
    void grab(bool c);
    void set_stopped(bool c);

    double get_speed(){return speed;}
    int get_angle(){return angle;}
    double get_length(){return length;}
    double get_rlength(){return rlength;}
    int get_sum(){return sum;}

    bool isrotating(){return is_rotating;}
    bool isgrab(){return is_grab;}
    bool isstopped(){return stopped;}
    bool isfflag()
    {
        fflag=!fflag;
        return fflag;
    }
    //初始化
    void init();


};

#endif // ITEM_H
