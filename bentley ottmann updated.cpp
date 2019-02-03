#include<bits/stdc++.h>
using namespace std;

#define epsilon (1.0e-4)


enum Type{REGULAR, INTERSECTION, HORIZONTAL_START, HORIZONTAL_END,NONE};

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

    bool operator < (const Point &p2) const{
        if( fabs(y - p2.y) < epsilon){
            return x < p2.x;
        }
        else{
            return y > p2.y;
        }
    }


};

struct Event{
    Point point;
    Type event_type;

    Event(){

    }

    Event(Point p){
        point = p;
        event_type = NONE;
    }
    Event(Point pp, Type type){
        point = pp;
        event_type = type;
    }



    void print(){
        cout << "\n---Event---" << endl;
        cout << point.x << " " << point.y << endl;
        cout << endl;
    }

    bool operator != (const Event &e2)const{
        return point.x != e2.point.x || point.y != e2.point.y;
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
            /*if( x2 - start_point.x > epsilon && x2 - end_point.x < epsilon){
                return 0;
            }
            else{*/

                if( x2 > end_point.x) return 1;
                else  return 0;
                //else return 0;
            //}
            //double x1 = 1000000.0;
            //return x1 < x2;
        }
        else if(fabs(sweep_line_y - s2.start_point.y) < epsilon &&  fabs(sweep_line_y - s2.end_point.y) < epsilon){
            /// s2 horizontal
            double x1 = get_x(start_point.x, start_point.y, end_point.x, end_point.y, sweep_line_y);
            /*if( x1 - s2.start_point.x > epsilon && x1 - s2.end_point.x < epsilon){
                return 1;
            }
            else{*/
                if( x1 < s2.start_point.x) return 1;
                else if(x1 > s2.end_point.x) return 0;
                else return 1;
            //}
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


int n;
int point_count,intersection_count;
set<Event>Q;
set<int>intersection_segments[305];
set<Point>cur_points;
multiset<Segment>T;
map<pair<double,double>,int>Map,intersection_map;
map<pair<double,double>, pair<pair<double,double> ,int>>horizontal_start_point;
vector<Segment>ans[305];
vector<Segment>segments[305];
int ans_cnt;
Point which_segment[305];
double lowest_y = FLT_MAX;
int end_points_cnt;
Segment left_dummy_segment = Segment(Point(-FLT_MAX / 2.0,FLT_MAX / 2.0), Point(-FLT_MAX / 2.0, -FLT_MAX / 2.0));
Segment right_dummy_segment = Segment(Point(FLT_MAX / 2.0 ,FLT_MAX / 2.0), Point(FLT_MAX / 2.0,-FLT_MAX / 2.0));





/// -------------- code segment for intersection  -------------------- ///

Segment getBoundingBox(Segment p) {
        Point a = Point(min(p.start_point.x, p.end_point.x), min(p.start_point.y,p.end_point.y));
        Point b = Point(max(p.start_point.x, p.end_point.x), max(p.start_point.y,p.end_point.y));
        return Segment(a,b);
}

double crossProduct(Point a, Point b) {
        return a.x * b.y - b.x * a.y;
}

bool doBoundingBoxesIntersect(Segment a, Segment b) {
    return a.start_point.x <= b.end_point.x && a.end_point.x >= b.start_point.x && a.start_point.y <= b.end_point.y && a.end_point.y >= b.start_point.y;

}

bool isPointOnLine(Segment a, Point b) {

        Segment aTmp = Segment(Point(0, 0), Point(a.end_point.x - a.start_point.x, a.end_point.y - a.start_point.y));
        Point bTmp = Point(b.x - a.start_point.x, b.y - a.start_point.y);
        double r = crossProduct(aTmp.end_point , bTmp);
        return fabs(r) < epsilon;
}

bool isPointRightOfLine(Segment a, Point b) {

        Segment aTmp = Segment( Point(0, 0), Point(a.end_point.x - a.start_point.x, a.end_point.y - a.start_point.y));
        Point bTmp = Point(b.x - a.start_point.x, b.y - a.start_point.y);
        return crossProduct(aTmp.end_point, bTmp) < 0.0;
}

bool lineSegmentTouchesOrCrossesLine(Segment a, Segment b) {
        return isPointOnLine(a, b.start_point) || isPointOnLine(a, b.end_point) || (isPointRightOfLine(a, b.start_point) ^ isPointRightOfLine(a, b.end_point));
}

bool doLinesIntersect(Segment a, Segment b) {
        Segment box1 = getBoundingBox(a);
        Segment box2 = getBoundingBox(b);
        return doBoundingBoxesIntersect(box1, box2) && lineSegmentTouchesOrCrossesLine(a, b) && lineSegmentTouchesOrCrossesLine(b, a);
}



Point get_intersection_point(Segment P, Segment Q)
{

    Point A = P.start_point;
    Point B = P.end_point;
    Point C = Q.start_point;
    Point D = Q.end_point;

    double a1 = B.y - A.y;
    double b1 = A.x - B.x;
    double c1 = a1*(A.x) + b1*(A.y);


    double a2 = D.y - C.y;
    double b2 = C.x - D.x;
    double c2 = a2*(C.x)+ b2*(C.y);

    double determinant = a1*b2 - a2*b1;

    if (determinant == 0)
    {

        return Point(FLT_MAX, FLT_MAX);
    }
    else
    {
        double x = (b2*c1 - b1*c2)/determinant;
        double y = (a1*c2 - a2*c1)/determinant;
        return Point(x,y);


    }
}

/// -------------- intersection code segment end ---------------------- ///



double get_precision(double value, double precision)
{
    return (floor((value * pow(10, precision) + 0.5)) / pow(10, precision));

}

Point get_double_precision_point(Point p)
{
    double x = floor(p.x) + get_precision(p.x - floor(p.x), 6.0);
    double y = floor(p.y) + get_precision(p.y - floor(p.y), 6.0);
    //cout << "precision " << x << " " << y << endl;
    return Point(x,y);
}

double get_double_precision(double val)
{
    double x = floor(val) + get_precision(val - floor(val), 6.0);
    //cout << "precision " << x << " " << y << endl;
    return x;
}


void find_new_event(Segment sl, Segment sr, Point p)
{
    if(doLinesIntersect(sl,sr)){
        Point intersection_point = get_intersection_point(sl,sr);
        intersection_point = get_double_precision_point(intersection_point);
        if( intersection_point.y - sweep_line_y < epsilon || ( fabs(intersection_point.y - sweep_line_y)  < epsilon && intersection_point.x - p.x > epsilon) ){

            if(cur_points.find(intersection_point) == cur_points.end()){
                cur_points.insert(intersection_point);
                Q.insert(Event(intersection_point,INTERSECTION));
                cout << "yes paisi" << endl;
            }
        }
    }



}

void vector_print(vector<Segment>r, string name)
{
    cout << "\n-- vector segment : " << name << "---" << endl;

    for(Segment s : r){
        s.print(name);
    }
    cout << name << " ----end----\n" << endl;
}

bool is_valid(Segment p)
{
    return p != left_dummy_segment && p != right_dummy_segment;
}

void handle_event_point(Event e)
{
    Point p = e.point;
    //Segment g = Segment(p,p);

    multiset<Segment>::iterator it;
    vector<Segment>Up,Lp,Cp;

    for(Segment s : segments[Map[{p.x,p.y}]]){
        Up.push_back(s);
    }

    vector_print(Up, "UP");



    ///----------------////
    it = T.upper_bound(Segment(Point(p.x+.01,p.y),Point(p.x+.01,p.y)));
    Segment s = *it;
    s.print("after upper-bound");
    it--;
    s = *it;



    /// horizontal r jnno eksathe kaaj korbo


    while(s != left_dummy_segment){
        bool dhukse = 0;
        //s.print("while");
        if(fabs(s.start_point.y - s.end_point.y) < epsilon  ){
            if( p.x - s.start_point.x > epsilon && p.x - s.end_point.x < epsilon){
                s.print("parallel");
                dhukse = 1;
                Cp.push_back(s);
            }
            else if( p.x - s.start_point.x < epsilon ){
                s.print("baal parallel");
                dhukse = 1;
            }

        }
        else{
            double x = get_x(s.start_point.x, s.start_point.y, s.end_point.x, s.end_point.y, sweep_line_y);
            x = get_double_precision(x);

            if(fabs(x - p.x) < epsilon){
                s.print("regular");
                dhukse = 1;
                Point intersect = Point(x, sweep_line_y);
                if(fabs(intersect.x - s.end_point.x) < epsilon && fabs(intersect.y - s.end_point.y) < epsilon){
                    /// intersection is lower end-point
                    Lp.push_back(s);
                }
                else{
                    Cp.push_back(s);
                }

            }
            else if(x - p.x > epsilon){
                s.print("baal regular");
                dhukse = 1;
            }

        }
        if(!dhukse) break;
        //s.print("while");
        it--;
        s = *it;
    }


    vector_print(Lp, "LP");
    vector_print(Cp, "CP");


    if(Up.size() + Lp.size() + Cp.size() > 1){
        /// got an intersection
        cout << "GOT INTERSECTION" << endl;
        if(intersection_map.find({p.x,p.y}) == intersection_map.end()){
            intersection_map[{p.x,p.y}] = ++intersection_count;
        }

        int cnt = intersection_map[{p.x,p.y}];

        for(Segment s : Up){
            intersection_segments[cnt].insert(s.segment_num);
        }
        for(Segment s : Lp){
            intersection_segments[cnt].insert(s.segment_num);
        }
        for(Segment s : Cp){
            intersection_segments[cnt].insert(s.segment_num);
        }
    }

    /// for only horizontal case
    if(e.event_type == HORIZONTAL_END){
        cout << "\nHORIZONTAL CASE\n" << endl;
        it = T.upper_bound(Segment(Point(p.x+.01,p.y),Point(p.x+.01,p.y)));
        Segment s = *it;
        s.print("after upper-bound");
        it--;
        s = *it;




        while(s != left_dummy_segment){
            bool dhukse = 0;
            //s.print("while");
            if(fabs(s.start_point.y - s.end_point.y) < epsilon  ){
                dhukse = 1;

            }
            else{
                double x = get_x(s.start_point.x, s.start_point.y, s.end_point.x, s.end_point.y, sweep_line_y);
                x = get_double_precision(x);
                pair<pair<double,double>,int>pre_point = horizontal_start_point[{p.x,p.y}];



                if(x - pre_point.first.first > epsilon && x - p.x < epsilon){
                    dhukse = 1;

                    if(intersection_map.find({x,p.y}) == intersection_map.end()){
                        intersection_map[{x,p.y}] = ++intersection_count;
                    }
                    int cnt = intersection_map[{x,p.y}];

                    intersection_segments[cnt].insert(s.segment_num);
                    intersection_segments[cnt].insert(pre_point.second);

                }
                else if(x - p.x > epsilon){
                    dhukse = 1;
                }

            }
            if(!dhukse) break;
            //s.print("while");
            it--;
            s = *it;
        }


        ///
    }



    for(Segment s : Lp){
        T.erase(s);
    }

    for(Segment s : Cp){
        T.erase(s);
    }

    /*if(fabs(p.y - lowest_y) < epsilon){
        /// lowest end poin touch kore felese
        /// r kaaj nai :P
        return;
    }*/

    cout << "baaaaal" <<endl;
    for(Segment s : Up){
        T.insert(s);
    }
    for(Segment s : Cp){
        T.insert(s);
    }


    cout << "Insert r pore segments in T : " << endl;

    for(it = T.begin(); it != T.end(); it++){
        Segment s = *it;
        s.print("After insert");
    }


    if(Up.size() + Cp.size() == 0){


        it = T.upper_bound(Segment(Point(p.x+.01,p.y),Point(p.x+.01,p.y)));
        Segment right_neighbour = *it;
        cout << "\n--------UP + CP = 0 -------\n";
        it--;
        Segment s = *it;



        Segment left_neighbour = left_dummy_segment;


        bool is_left_neighbour_paise = 0;
        while(s != left_dummy_segment){
            bool dhukse = 0;

            if(fabs(s.start_point.y - s.end_point.y) < epsilon  ){
                if( p.x - s.start_point.x > epsilon && p.x - s.end_point.x < epsilon){
                    dhukse = 1;
                }
                else if( p.x - s.start_point.x < epsilon ){
                    dhukse = 1;
                    right_neighbour = s;
                }

            }
            else{
                double x = get_x(s.start_point.x, s.start_point.y, s.end_point.x, s.end_point.y, sweep_line_y);

                if(fabs(x - p.x) < epsilon){
                    dhukse = 1;
                }
                else if(x - p.x > epsilon){
                    right_neighbour = s;
                    dhukse = 1;
                }

            }
            if(!dhukse){
                left_neighbour = s;
                is_left_neighbour_paise = 1;
                break;
            }
            //s.print("while");
            it--;
            s = *it;
        }

        if(!is_left_neighbour_paise) left_neighbour = *it;

        if(is_valid(left_neighbour) && is_valid(right_neighbour)){

            left_neighbour.print("left");
            right_neighbour.print("right");

            find_new_event(left_neighbour,right_neighbour,p);
        }

        cout << "\n----- END ----\n";

    }
    else{
        it = T.upper_bound(Segment(Point(p.x+.01,p.y),Point(p.x+.01,p.y)));
        Segment right_neighbour = *it;
        cout << "\n--------UP + CP > 0 -------\n";
        it--;
        Segment s = *it;


        Segment rightmost = right_dummy_segment;
        Segment leftmost = right_dummy_segment;



        Segment left_neighbour = left_dummy_segment;


        bool is_left_neighbour_paise = 0;
        while(s != left_dummy_segment){
            bool dhukse = 0;

            if(fabs(s.start_point.y - s.end_point.y) < epsilon  ){
                if( p.x - s.start_point.x > epsilon && p.x - s.end_point.x < epsilon){

                    if(rightmost == right_dummy_segment){
                        rightmost = s;
                    }

                    leftmost = s;

                    dhukse = 1;
                }
                else if( p.x - s.start_point.x < epsilon ){
                    dhukse = 1;
                    right_neighbour = s;
                }

            }
            else{
                double x = get_x(s.start_point.x, s.start_point.y, s.end_point.x, s.end_point.y, sweep_line_y);

                if(fabs(x - p.x) < epsilon){
                    dhukse = 1;

                    if(rightmost == right_dummy_segment){
                        rightmost = s;
                    }

                    leftmost = s;

                }
                else if(x - p.x > epsilon){
                    right_neighbour = s;
                    dhukse = 1;
                }

            }
            if(!dhukse){
                left_neighbour = s;
                is_left_neighbour_paise = 1;
                break;
            }
            //s.print("while");
            it--;
            s = *it;
        }

        if(!is_left_neighbour_paise) left_neighbour = *it;

        if(is_valid(left_neighbour) && is_valid(leftmost)){
            find_new_event(left_neighbour, leftmost, p);
            left_neighbour.print("left neighbour");
            leftmost.print("leftmost");
        }

        if(is_valid(right_neighbour) && is_valid(rightmost)){
            find_new_event(rightmost, right_neighbour, p);
            right_neighbour.print("right neighbour");
            rightmost.print("rightmost");
        }



        cout << "\n----- END ----\n";



    }





    ///----------------////



}


int main()
{
    freopen("in.txt","r",stdin);
    freopen("out.txt", "w", stdout);

    scanf("%d",&n);

    sweep_line_y = 6;

    for(int i = 0; i < n; i++){
        double x1,y1,x2,y2;
        scanf("%lf%lf%lf%lf",&x1,&y1,&x2,&y2);


        Point a = Point(x1,y1);
        Point b = Point(x2,y2);

        lowest_y = min(y1,min(y2,lowest_y));

        ///a --- b

        if(fabs(a.y - b.y) < epsilon && b.x - a.x < epsilon){
            swap(a,b);
        }
        else if( b.y > a.y){
            swap(a,b);
        }

        //cout << a.x << " " << a.y << " | " << b.x <<" " <<b.y << endl;

        if(Map.find({a.x,a.y}) == Map.end()){
            Map[{a.x,a.y}] = ++point_count;
            //Endpoint_Map[{a.x,a.y}] = ++end_points_cnt;
            segments[Map[{a.x,a.y}]].push_back(Segment(a,b,i+1));

            //cout << "yes" << endl;
        }
        else{
            segments[Map[{a.x,a.y}]].push_back(Segment(a,b,i+1));
        }

        if(fabs(a.y - b.y) < epsilon){
            Q.insert(Event(a,HORIZONTAL_START));
            Q.insert(Event(b,HORIZONTAL_END));
            horizontal_start_point[{b.x,b.y}] = {{a.x,a.y},i+1};
        }
        else{
            Q.insert(Event(a,REGULAR));
            Q.insert(Event(b,REGULAR));
        }

        cur_points.insert(a);
        cur_points.insert(b);



       //T.insert(Segment(a,b));




    }


    /// dummy segment insert
    T.insert(left_dummy_segment);
    T.insert(right_dummy_segment);

    while(!Q.empty()){
        cout << "\n////////----- event shuru ----///////////\n" << endl;
        Event e = *Q.begin();
        Q.erase(e);
        e.print();
        sweep_line_y = e.point.y;
        handle_event_point(e);
        //cout << e.point.x << " " << e.point.y << endl;

        //

        cout << "\n////////----- event shesh---------//////\n\n" << endl;


    }

    cout << "\n\n----- INTERSECTIONS ----\n\n";

    map<pair<double,double>,int>::iterator it;

    for(it = intersection_map.begin(); it != intersection_map.end(); it++){
        pair<double,double> point = it->first;
        int num = it->second;

        cout << "Point : " << point.first << " " << point.second << " , Lines : ";
        for(int seg_num : intersection_segments[num]){
            cout << seg_num << " ";
        }
        cout << endl;
    }




    return 0;
}
