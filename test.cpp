#include<bits/stdc++.h>
using namespace std;

#define epsilon (1.0e-4)

double sweep_line_y;


double get_x(double x0, double y0, double x1, double y1, double y)
{
    if(fabs(y1 - y0) < epsilon) return FLT_MAX;
    //if(fabs(y1 - y0) < epsilon) return x1;
    double m1 = (x1 - x0) / (y1 - y0);
    return x0 + m1 * (y - y0);
}

double get_y(double x0, double y0, double x1, double y1, double x)
{
    if(fabs(x1 - x0) < epsilon) return FLT_MAX;
    //if(fabs(x1 - x0) < epsilon) return y0;
    double m1 = (y1 - y0) / (x1 - x0);
    return y0 + m1 * (x - x0);
}

struct Point{
    double x,y;

    Point(){
        x = y = 0;

    }

    Point(double xx ,double yy){
        x = xx;
        y = yy;
    }




};

struct Event{
    Point point;

    Event(){

    }

    Event(Point pp){
        point = pp;
    }

    void print(){
        cout << "\n---Event---" << endl;
        cout << point.x << " " << point.y << endl;
        cout << endl;
    }

    bool operator < (const Event &e2) const{
        if( fabs(point.y - e2.point.y) < epsilon){
            return point.x < e2.point.x;
        }
        else{
            return point.y > e2.point.y;
        }
    }
};

struct Segment{
    Point start_point,end_point;
    int segment_num = -1;

    Segment(){
    }
    Segment(Point a, Point b){
        start_point = a;
        end_point = b;
    }

    Segment(Point a, Point b, int num){
        start_point = a;
        end_point = b;
        segment_num = num;
    }

    void print(string name){
        cout << "\n---segment : "<< name <<  " ---\n";
        cout << start_point.x << " " << start_point.y << endl;
        cout << end_point.x << " " << end_point.y << endl;
    }

    bool operator != (const Segment &s2) const{
        return fabs(start_point.x - s2.start_point.x) > epsilon ||
            fabs(start_point.y - s2.start_point.y) > epsilon ||
            fabs(end_point.x - s2.end_point.x) > epsilon ||
            fabs(end_point.y - s2.end_point.y) > epsilon;
    }

    bool operator == (const Segment &s2) const{
        return fabs(start_point.x - s2.start_point.x) < epsilon &&
            fabs(start_point.y - s2.start_point.y) < epsilon &&
            fabs(end_point.x - s2.end_point.x) < epsilon &&
            fabs(end_point.y - s2.end_point.y) < epsilon;
    }
    /// horizontal segment goes last is still baki
    bool operator < (const Segment &s2) const{
        double x1,x2;

        if(fabs(sweep_line_y - start_point.y) < epsilon &&  fabs(sweep_line_y - end_point.y) < epsilon && fabs(sweep_line_y - s2.start_point.y) < epsilon && fabs(sweep_line_y - s2.end_point.y) < epsilon){
            /// both line horizontal along sweep line
            return start_point.x <  s2.start_point.x;
        }
        else if(fabs(sweep_line_y - start_point.y) < epsilon &&  fabs(sweep_line_y - end_point.y) < epsilon){
            /// s horizontal
            double x2 = get_x(s2.start_point.x, s2.start_point.y, s2.end_point.x, s2.end_point.y, sweep_line_y);
            if( x2 - start_point.x > epsilon && x2 - end_point.x < epsilon){
                return 0;
            }
            else{
                if( x2 > end_point.x) return 1;
                else return 0;
            }
            //double x1 = 1000000.0;
            //return x1 < x2;
        }
        else if(fabs(sweep_line_y - s2.start_point.y) < epsilon &&  fabs(sweep_line_y - s2.end_point.y) < epsilon){
            /// s2 horizontal
            double x1 = get_x(start_point.x, start_point.y, end_point.x, end_point.y, sweep_line_y);
            if( x1 - s2.start_point.x > epsilon && x1 - s2.end_point.x < epsilon){
                return 1;
            }
            else{
                if( x1 < s2.start_point.x) return 1;
                else return 0;
            }
        }
        else{
            /// neither
            double x1 = get_x(start_point.x, start_point.y, end_point.x, end_point.y, sweep_line_y);
            double x2 = get_x(s2.start_point.x, s2.start_point.y, s2.end_point.x, s2.end_point.y, sweep_line_y);

            if(fabs(x1 - x2) < epsilon){
                if(fabs(end_point.x - s2.end_point.x) < epsilon){
                    return start_point.x < s2.start_point.x;
                }
                return end_point.x < s2.end_point.x;
            }
            return x1 < x2;
        }



    }
};



multiset<Segment>T;







Point get_intersection_point(Segment P, Segment Q)
{
    // Line AB represented as a1x + b1y = c1
    Point A = P.start_point;
    Point B = P.end_point;
    Point C = Q.start_point;
    Point D = Q.end_point;

    double a1 = B.y - A.y;
    double b1 = A.x - B.x;
    double c1 = a1*(A.x) + b1*(A.y);

    // Line CD represented as a2x + b2y = c2
    double a2 = D.y - C.y;
    double b2 = C.x - D.x;
    double c2 = a2*(C.x)+ b2*(C.y);

    double determinant = a1*b2 - a2*b1;

    if (determinant == 0)
    {
        // The lines are parallel. This is simplified
        // by returning a pair of FLT_MAX
        return Point(FLT_MAX, FLT_MAX);
    }
    else
    {
        double x = (b2*c1 - b1*c2)/determinant;
        double y = (a1*c2 - a2*c1)/determinant;

       // if((min(P.start_point.x, Q.start_point.x) - x) < epsilon  && (x - max(P.end_point.x, Q.end_point.x)) < epsilon  && (max(P.end_point.y, Q.end_point.y) - y) < epsilon  && (y - min(P.start_point.y, Q.start_point.y)) < epsilon ) return Point(x, y);
        //else Point(FLT_MAX, FLT_MAX);
        return Point(x,y);


    }
}

/// ---------------------------- ///




Point get_point_for_upper_bound_delete(Point p)
{
    /// > >
    //printf("p.x %0.8lf\n",p.x);
    double x = p.x;
    double y = p.y;
    double new_x;
    double new_y;
    double dif = 100000;

    double cur = .9;
    double check_val = p.x - floor(p.x);

    //printf("check val %0.8lf\n",check_val);

    while(cur - check_val > epsilon && cur > epsilon){
        if(cur - check_val < dif){
            //cout << "yes" << endl;
            dif = cur - check_val;
            new_x = cur;
        }

        cur -= 0.005;
    }

    new_x = new_x + floor(p.x);

    //cout << new_x << endl;


    dif = 100000;
    cur = .9;
    check_val = p.y - floor(p.y);

    //printf("check val %0.8lf\n",check_val);

    while(cur - check_val > epsilon && cur > epsilon){
        if(cur - check_val < dif){
            //cout << "yes" << endl;
            dif = cur - check_val;
            new_y = cur;
        }

        cur -= 0.005;
    }

    new_y = new_y + floor(p.y);



    //printf("%.8lf\n",new_x);
    //cout << new_y;

    return Point(new_x,new_y);

}

double get_precision(double value, double precision)
{
    return (floor((value * pow(10, precision) + 0.5)) / pow(10, precision));

}

Point get_double_precision_point(Point p)
{
    double x = floor(p.x) + get_precision(p.x - floor(p.x), 6.0);
    double y = floor(p.y) + get_precision(p.y - floor(p.y), 6.0);
    cout << "precision " << x << " " << y << endl;
    return Point(x,y);
}
/*
Point get_for_upper_bound_insert(Point p)
{

}
*/
int main()
{

    //get_point_for_upper_bound_delete(Point(37.0/7.0,23.0/7.0));

    //Point p = Point(5.28571, 3.28572);
    Point p = Point(5.5, 5);
    cout << "p " << p.x << " " <<p.y << endl;
    sweep_line_y = p.y;

    Segment left_dummy_segment = Segment(Point(-FLT_MAX / 2.0,FLT_MAX / 2.0), Point(-FLT_MAX / 2.0, -FLT_MAX / 2.0));
    T.insert(left_dummy_segment);
    Segment right_dummy_segment = Segment(Point(FLT_MAX / 2.0 ,FLT_MAX / 2.0), Point(FLT_MAX / 2.0,-FLT_MAX / 2.0));
    T.insert(right_dummy_segment);

    T.insert(Segment(Point(1,5),Point(10,5)));
    T.insert(Segment(Point(6,9),Point(5,1)));
    T.insert(Segment(Point(3,8),Point(2,2)));
    T.insert(Segment(Point(8,8),Point(3,2)));
    T.insert(Segment(Point(-2,9),Point(-4,1)));
    T.insert(Segment(Point(15,8),Point(13,1)));
    T.insert(Segment(Point(12,5),Point(17,5)));
    T.insert(Segment(Point(5.5,5),Point(6,3)));


    /*T.insert(Segment( get_double_precision_point(Point(6,9)), get_double_precision_point(Point(5,1))));
    T.insert(Segment( get_double_precision_point(Point(1,23.0/7.0)), get_double_precision_point(Point(10,23.0/7.0)) ));
    T.insert(Segment( get_double_precision_point(Point(15,9)), get_double_precision_point(Point(12,2))));
    T.insert(Segment( get_double_precision_point(Point(4,10)), get_double_precision_point(Point(46.0/7.0,-24.0/7.0))));
    T.insert(Segment( get_double_precision_point(Point(11,23.0/7.0)), get_double_precision_point(Point(17,23.0/7.0))));*/

    for(Segment s : T){
        s.print("test");
    }

    cout << "---end---\n" << endl;
    multiset<Segment>::iterator it;


    it = T.lower_bound(Segment(Point(p.x+.001,p.y),Point(p.x+.001,p.y)));
    Segment s = *it;
    s.print("first");
    it--;
    s = *it;





    while(s != left_dummy_segment){
        bool dhukse = 0;
        //s.print("while");
        if(fabs(s.start_point.y - s.end_point.y) < epsilon && p.x - s.start_point.x > epsilon && p.x - s.end_point.x < epsilon){
            s.print("parallel");
            dhukse = 1;
            cout << "Here" << endl;
        }
        else{
            double x = get_x(s.start_point.x, s.start_point.y, s.end_point.x, s.end_point.y, sweep_line_y);
            cout << "x " << x << endl;
            if(fabs(x - p.x) < epsilon){
                s.print("regular");
                dhukse = 1;
            }

        }
        if(!dhukse) break;
        //s.print("while");
        it--;
        s = *it;
    }

    //Point pp = get_intersection_point(Segment(Point(6,9),Point(5,1)), Segment(Point(4,10),Point(6.57143,-3.42857)));
    //cout << pp.x << " " << pp.y << endl;
    /*double x = get_x(6,9,5,1,23.0/7.0);
    cout << x;*/


    //printf("%0.6lf",x);






    return 0;
}













