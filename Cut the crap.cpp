#include<stdlib.h>
#include<ctime>
#include<simplecpp>
#include<cmath>

inline int random()
{
    return (((unsigned)rand()*time(0))%10);
}

struct Point
{
    int x;
    int y;
};



inline bool gameover(int x,int y)
{
    if(x<125 || x>625 || y<125 || y>625) return true;
    return false;
}

bool onSegment(Point p, Point q, Point r)
{
    if (q.x <= max(p.x, r.x) && q.x >= min(p.x, r.x) &&
            q.y <= max(p.y, r.y) && q.y >= min(p.y, r.y))
        return true;

    return false;
}

int orientation(Point p, Point q, Point r)
{

    int val = (q.y - p.y) * (r.x - q.x) -
              (q.x - p.x) * (r.y - q.y);

    if (val == 0) return 0;
    return (val > 0)? 1: 2;
}

bool doIntersect(Point p1, Point q1, Point p2, Point q2)
{
    int o1 = orientation(p1, q1, p2);
    int o2 = orientation(p1, q1, q2);
    int o3 = orientation(p2, q2, p1);
    int o4 = orientation(p2, q2, q1);


    if (o1 != o2 && o3 != o4)
        return true;


    if ( o1 == 0 && onSegment(p1, p2, q1)) return true;

    if (o2 == 0 &&  onSegment(p1, q2, q1)) return true;

    if (o3 == 0 &&  onSegment(p2, p1, q2)) return true;

    if (o4 == 0 &&  onSegment(p2, q1, q2)) return true;

    return false;
}


struct line_segment
{
    Point a,b;
    line_segment*next;
    line_segment*prev;
};



bool self_intersection(line_segment*head,line_segment*ptr1)
{
    line_segment *ptr2;
    ptr2=ptr1->prev;


    while(head!=ptr2)
    {
        if(doIntersect(head->a,head->b,ptr1->a,ptr1->b)==true) return true;
        head=head->next;
    }
    return false;
}


bool bot_intersect_me(line_segment*head,line_segment*bot_ptr1)
{

    while(head->next!= NULL)
    {

        if(doIntersect(head->a,head->b,bot_ptr1->a,bot_ptr1->b)==true) return true;
        head=head->next;
    }

    return false;
}
bool me_intersect_bot(line_segment*ptr1,line_segment*bot_head)
{

    while(bot_head->next!= NULL)
    {
        if(doIntersect(bot_head->a,bot_head->b,ptr1->a,ptr1->b)==true) return true;
        bot_head=bot_head->next;
    }


    return false;
}



void bot(Point to_reach,int*bot_locationx,int*bot_locationy)
{
    static Color bot_color=COLOR(255,0,0);

    int m1,m2,distance,length=(random()%7+3)*10;

    to_reach.x=to_reach.x-3+random()%6;;
    to_reach.y=to_reach.y-3+random()%6;;

    distance=sqrt(pow((to_reach.x-*bot_locationx),2)+pow((to_reach.y-*bot_locationy),2));
    m1=length;
    m2=distance-length;

    to_reach.x=(m2*(*bot_locationx)+m1*to_reach.x)/distance;//section formula
    to_reach.y=(m2*(*bot_locationy)+m1*to_reach.y)/distance;
    imprintLine(*bot_locationx,*bot_locationy,to_reach.x,to_reach.y,bot_color);//line towards point clicked
    *bot_locationx=to_reach.x;
    *bot_locationy=to_reach.y;


}






int main()
{
    int length;
    Point player;
    player.x=375,player.y=625;
    initCanvas("cut the crap",750,750);
    Rectangle r1(375,375,500,500);
    Rectangle r2(375,650,100,50);
    Rectangle r3(375,375,500,500);


    r1.setFill(true);
    Color r=COLOR(233, 245, 66);
    r1.setColor(r);

    Color mine_color=COLOR(0, 0, 0);

    r3.setFill(false);
    r=COLOR(0, 0, 0);
    r3.setColor(r);




    int pt_clicked,m1,m2,distance;
    Point pt_reached,bot_location;
    bot_location.x=375;
    bot_location.y=125;



    line_segment*head,*ptr1,*ptr2;
    head=new line_segment;
    ptr1=head;


    line_segment*bot_head,*bot_ptr1,*bot_ptr2;
    bot_head=new line_segment;
    bot_ptr1=bot_head;


    for(int i=0; i<1000; i++)
    {
        length=(random()%7+3)*10;// Generates length(30-100)
        pt_clicked=getClick();
        pt_reached.x=pt_clicked/65536;//x cordinate of clicked pt
        pt_reached.y=pt_clicked%65536;//y cordinate of clicked pt
        distance=sqrt(pow((pt_reached.x-player.x),2)+pow((pt_reached.y-player.y),2));
        m1=length;
        m2=distance-m1;
        pt_reached.x=(m2*player.x+m1*pt_reached.x)/distance;//section formula
        pt_reached.y=(m2*player.y+m1*pt_reached.y)/distance;
        imprintLine(player.x,player.y,pt_reached.x,pt_reached.y,mine_color);//line towards point clicked


        ptr1->a=player;
        ptr1->b=pt_reached;
        ptr2=new line_segment;
        ptr2->next=NULL;
        ptr2->prev=ptr1;
        ptr1->next=ptr2;
        ptr1=ptr2;
        if(i>1)
        {

            if(self_intersection(head,ptr1->prev)==true)
            {
                cout<<"Self harm";
                wait(2);
                return 0;
            }

            if( me_intersect_bot(ptr1->prev,bot_head)==true)
            {
                cout<<"You Won";
                wait(2);
                return 0;
            }

        }



        bot_ptr1->a=bot_location;
        bot(player,&bot_location.x,&bot_location.y);
        bot_ptr1->b=bot_location;

        bot_ptr2=new line_segment;
        bot_ptr2->next=NULL;
        bot_ptr2->prev=bot_ptr1;
        bot_ptr1->next=bot_ptr2;
        bot_ptr1=bot_ptr2;


        if(i>2)
            if(bot_intersect_me(head,bot_ptr1->prev)==true)
            {
                cout<<"bot wins";
                wait(2);
                return 0;

            };





        player.x=pt_reached.x;
        player.y=pt_reached.y;




        if(gameover(player.x,player.y)==true)
        {
            cout<<"Gone Outside";
            wait(2);
            return 0;
        }
    }




   // wait(3);

    return 0;
}

