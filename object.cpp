#include"object.h"
#include<QDebug>
void Object::spo(const int &xx, const int &yy)   //设置中心位置
{
    x=xx;
    y=yy;
}
void Object::ssz(const int &wwidth, const int &hheight)    //设置大小
{
    width=wwidth;
    height=hheight;
}
void Object::spr(const int &pprice)    //设置矿的价值
{
    price=pprice;
}
void Object::ssp(const double &sspeed)     //设置钩子速度
{
    speed=sspeed;
}
void Object::skd(const int &kkind)    //？？？？？
{
    kind=kkind;
}

void Object::svisible()//设置为可见
{
    visible=true;
}

void Object::sinvisible()//设置为不可见
{
    visible=false;
}

void Object::init()//初始化
{
    x=y=height=width=kind=0;
    visible=true;
}

bool Object::detection(const int &xx,const int &yy)//item碰撞检测
{
    if(visible==true&&xx>x+5&&xx<(x+width-5)&&yy>y+2&&yy<(y+height-5))//+5、-5是为了缩小碰撞检测范围
        return true;
    return false;
}

class Crab:public Object
{
public:
    Crab();
    void init();
};

class Vegetable:public Object
{
public:
    Vegetable();
    void init();
};


Map::Map()
{
    for(int i=0;i<14;i++)//初始化item数组
       object[i]=new Object;
}
QPoint Map::get_point(const int &i)
{
    QPoint p(object[i]->get_x(),object[i]->get_y());
    return p;
}
int Map::get_kind(const int &i)
{
    return object[i]->get_kind();
}

bool Map::is_visible(const int &i)
{
    return object[i]->is_visible();
}

bool Map::detection(const int &x,const int &y)
{
    for(int i=0;i<14;i++){
        if(object[i]->detection(x,y))//如果碰到了任意一个物体
        {
            obj=i;
            return true;
        }
    }
}
void Map::invisible()
{
    object[obj]->sinvisible();
}
void Map::rrand()
{
    srand(time(NULL));
    int times=1+rand()%3;//随机生成1-3个
    int count=0;
    while(count<times)
    {
        int i=6+rand()%4;
        if(object[i]->is_visible()==false)
        {
            object[i]->svisible();
            int w=50+rand()%300;
            int h=136+rand()%300;
            object[i]->spo(w,h);
            if(i==6)//如果是大矿石
            {
                object[i]->ssz(140,120);
                object[i]->spr(500);
                object[i]->ssp(-2);
                object[i]->skd(0);
            }
            else if(i==9)//是钻石
            {
                object[i]->ssz(120,100);
                object[i]->spr(700);
                object[i]->ssp(-3);
                object[i]->skd(0);
            }
            else{//如果是小矿石
                object[i]->ssz(120,100);
                object[i]->spr(200);
                object[i]->ssp(-4);
                object[i]->skd(0);
            }
            count++;
        }
        //只要不是所有随机矿石都被占满，就继续进行循环
        int flag=1;
        for(int i=6;i<10;i++)
            if(object[i]->is_visible()==false)
                flag=0;
        if(flag)
            break;
    }
    //生成随机物质
    times=rand()%3;//随机生成0-2个
    count=0;
    while(count<times)
    {
        int i=10+rand()%4;
        if(object[i]->is_visible()==false)
        {
            object[i]->svisible();
            int w=50+rand()%300;
            int h=136+rand()%300;
            object[i]->spo(w,h);
            object[i]->ssz(60,50);
            object[i]->spr(0);
            object[i]->ssp(-4);
            if(i==13)
                object[i]->skd(1+rand()%3);
            else
                object[i]->skd(i-9);
            count++;
        }
        //只要不是所有随机矿石都被占满，就继续进行循环
        int flag=1;
        for(int i=10;i<14;i++)
            if(object[i]->is_visible()==false)
                flag=0;
        if(flag)
            break;
    }

}

void Map::init()
{
    srand(time(NULL));
    for(int i=0;i<6;i++)
    {
        object[i]->init();
        int w=50+rand()%300;
        int h=136+rand()%300;
        object[i]->spo(w,h);
        if(i<2)
        {
            object[i]->ssz(140,120);
            object[i]->spr(500);
            object[i]->ssp(-2);
        }
        else{
            object[i]->ssz(60,50);
            object[i]->spr(200);
            object[i]->ssp(-4);
        }
    }
    for(int i=6;i<14;i++)//让随机矿石不可见
    {
        object[i]->sinvisible();
    }
}


//关于各项数值的更改
void Food::change_angle(const int &aangle)
{
    angle+=aangle;
}
void Food::change_length(const double &llength)
{
    length+=llength;
}
void Food::change_rlength(const double &rrlength)
{
    rlength+=rrlength;
}
void Food::change_sum(const double &ssum)
{
    sum+=ssum;
}
//关于各项数值的设置
void Food::ssp(const double &sspeed)
{
    speed=sspeed;
}
void Food::slength(const double &llength)
{
    length=llength;
}
void Food::srlength(const double &rrlength)
{
    rlength=rrlength;
}

//关于各布尔值的设置
void Food::rotating(bool c)
{
    is_rotating=c;
}
void Food::grab(bool c)
{
    is_grab=c;
}
void Food::set_stopped(bool c)
{
    stopped=c;
}

//初始化
void  Food::init()
{
    angle=0;//起始旋转角度为0
    speed=4;//钩子初始速度为2
    length=0;
    rlength=0;
    sum=0;

    is_rotating=true;//判断是否正在进行旋转
    is_grab=false;//当前没有抓取任何物体
    stopped=false;
    fflag=true;
}

