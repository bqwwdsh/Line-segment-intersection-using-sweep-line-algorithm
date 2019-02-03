#include<bits/stdc++.h>
using namespace std;

#define epsilon (1.0e-6)


enum Type{REGULAR, INTERSECTION};

double sweep_line_y;
int point_count;


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
    Type event_type;

    Event(){

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

int n;
set<Event>Q,erased_event;
multiset<Segment>T;
vector<Segment>segments[305];
map<pair<double,double>,int>Map,Endpoint_Map;
vector<Segment>ans[305];
int ans_cnt;
Point which_segment[305];
double lowest_y = FLT_MAX;
int end_points_cnt;
Segment left_dummy_segment = Segment(Point(-FLT_MAX,FLT_MAX), Point(-FLT_MAX,-FLT_MAX));
Segment right_dummy_segment = Segment(Point(FLT_MAX,FLT_MAX), Point(FLT_MAX,-FLT_MAX));


/*
5
10 2 10 9
14 9 10 9
10 9 6 13
13 6 10 9
6 4 10 9
*/
/*
6
13 6 9 9
15 4 13 6
8 5 18 7
5 4 2 10
13 6 16 9
13 2 13 6
*/

/*
8
13 6 9 9
8 5 18 7
5 4 2 10
13 6 16 9
20 2 25 8
22 3 27 7
26 7 18 4
0 2 6 8
*/

/*
4
13 6 9 9
8 5 18 7
13 6 16 9
20 2 25 8
*/

/*
8
9.8 11 8 2
1 10 3 8
8 10 6 6
4 9 1 6
7 8 7 5
6 10 10 6
5 13 7 8
3 8 2 5
*/

/*
double get_intersection_x(Segment a, double y)
{
    if(fabs(a.start_point.y - a.end_point.y) < epsilon){
        return a.start_point.x;
    }
    else{
        double m1 = (a.end_point.x - a.start_point.x) / (a.end_point.y - a.start_point.y);
        double val = a.start_point.x + m1 * (y - a.start_point.y);
        return val;
    }
}
*/

/// -------------- intersection -------------------- ///

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

        //if((min(P.start_point.x, Q.start_point.x) - x) < epsilon  && (x - max(P.end_point.x, Q.end_point.x)) < epsilon  && (max(P.end_point.y, Q.end_point.y) - y) < epsilon  && (y - min(P.start_point.y, Q.start_point.y)) < epsilon ) return Point(x, y);
        //else Point(FLT_MAX, FLT_MAX);
        return Point(x,y);


    }
}

/// -------------- end ---------------------- ///


void find_new_event(Segment sl, Segment sr, Point p)
{
    if(doLinesIntersect(sl,sr)){
        Point intersection_point = get_intersection_point(sl,sr);
        if( intersection_point.y < sweep_line_y || ( fabs(intersection_point.y - sweep_line_y)  < epsilon && intersection_point.x > p.x) ){
            if(erased_event.find(Event(intersection_point)) == erased_event.end()){
                Q.insert(Event(intersection_point));
            }
        }
    }



}

void vector_print(vector<Segment>r, string name)
{
    cout << "-- vector segment : " << name << "---" << endl;

    for(Segment s : r){
        s.print(name);
    }
    cout << "----end----" << endl;
}

/*
3
0 0 5 5
2 0 2 5
-1 0 10 0
*/

void handle_event_point(Event e)
{
    Point p = e.point;
    Segment g = Segment(p,p);

    multiset<Segment>::iterator it;
    vector<Segment>Up,Lp,Cp;

    for(Segment s : segments[Map[{p.x,p.y}]]){
        Up.push_back(s);
    }

    vector_print(Up, "UP");

    if(Endpoint_Map.find({p.x, p.y}) == Endpoint_Map.end()){
        cout << "yes map a nai 1" << endl;
        double xx = floor(p.x - 1.0);
        double yy ;
        if(p.y - floor(p.y) < 0.5) yy = floor(p.y) + 0.5;
        else yy = ceil(p.y);
        Point new_p = Point(xx,yy);
        g = Segment(new_p,new_p);
        sweep_line_y = yy;


        /*
        Point pp = Point(floor(p.x) - 1, ceil(p.y));
        g = Segment(pp,pp);
        //if(p.y - floor(p.y) < 0.5) sweep_line_y = floor(p.y) + 0.5;
        //else sweep_line_y = ceil(p.y);
        sweep_line_y = ceil(p.y);*/
    }

    bool is_any_segment = 1;
    if(T.size() == 0) is_any_segment = 0;

    /// containing p
    it = T.lower_bound(g);
    if(it == T.end()) is_any_segment = 0;
    //it--;

    cout << "sweep line : " << sweep_line_y << endl;
    if(is_any_segment){
        while(it != T.end()){
            Segment s = *it;

            double xx = get_x(s.start_point.x,s.start_point.y,s.end_point.x,s.end_point.y,p.y);
            s.print("equal");
            cout << "xx " << xx << endl;
            if(fabs(xx - p.x) < epsilon  ){
                /// if contains p

                cout << "yes contains" << endl;
                Point intersect = Point(xx, sweep_line_y);
                if(fabs(intersect.x - s.end_point.x) < epsilon && fabs(intersect.y - s.end_point.y) < epsilon){
                    /// intersection is lower end-point
                    Lp.push_back(s);
                }
                else{
                    Cp.push_back(s);
                }
            }
            else if(p.x - xx > epsilon){

            }
            /*else if( fabs(xx - FLT_MAX) < epsilon &&  ( s.start_point.x - p.x) < epsilon && ( p.x - s.end_point.x ) < epsilon ){
                Point intersect = Point(p.x, p.y);
                if(fabs(intersect.x - s.end_point.x) < epsilon && fabs(intersect.y - s.end_point.y) < epsilon){
                    /// intersection is lower end-point
                    Lp.push_back(s);
                }
                else{
                    Cp.push_back(s);
                }
            }*/
            else{
                break;
            }

            it++;

        }


    }

    vector_print(Lp, "LP");
    vector_print(Cp, "CP");



    if(Up.size() + Lp.size() + Cp.size() > 1){
        /// found intersection point
        /// kaaj korte hobe
        cout << "Intersection found " << endl;
        cout << "Intersect point : " << p.x << " " << p.y << endl;
        ans[ans_cnt].push_back(Segment(p,p,-1));

        for(Segment s : Up){
            //s.print("UP");
            ans[ans_cnt].push_back(s);
        }
        for(Segment s : Lp){
            //s.print("LP");
            ans[ans_cnt].push_back(s);
        }
        for(Segment s : Cp){
            //s.print("CP");
            ans[ans_cnt].push_back(s);
        }


        //cout << "----end----"<<endl;
        ans_cnt++;


    }

    /// delete Lp Cp

    for(Segment s : Lp){
        T.erase(s);
    }

    for(Segment s : Cp){
        T.erase(s);
    }

    if(fabs(p.y - lowest_y) < epsilon){
        /// lowest end poin touch kore felese
        return;
    }

    /// just below p
    /*set<Event>::iterator it1;
    double ager_sweep_line_y = sweep_line_y;
    it1 = Q.begin();
    sweep_line_y -= ( (*it1).point.y - sweep_line_y) / 2.0;*/

    if(Endpoint_Map.find({p.x, p.y}) == Endpoint_Map.end()){
        cout << "yes map a nai 2" << endl;
        sweep_line_y = floor(p.y);
        //if(p.y - floor(p.y) < 0.5) sweep_line_y = floor(p.y);
        //else sweep_line_y = floor(p.y) + 0.5;
    }

    /// insert Up Cp
    for(Segment s : Up){
        T.insert(s);
    }
    for(Segment s : Cp){
        T.insert(s);
    }





    //sweep_line_y = ager_sweep_line_y;

    cout << "Insert r pore segments in T : " << endl;

    for(it = T.begin(); it != T.end(); it++){
        Segment s = *it;
        s.print("After insert segment");
    }




    if(Up.size() + Cp.size() == 0){

        bool is = 0;
        if(Endpoint_Map.find({p.x, p.y}) == Endpoint_Map.end()){
            cout << "yes map a nai 3" << endl;
            Point pp = Point(floor(p.x), floor(p.y));
            g = Segment(pp,pp);
            is = 1;
        }

        /// for immediate left neighbour
        it = T.lower_bound(g);

        if(is || it != T.begin() ){
            if(!is) it--;
            Segment left_neighbour = *it;
            left_neighbour.print("left neighbour 1");

            ///for immediate right neighbour
            if(Endpoint_Map.find({p.x, p.y}) == Endpoint_Map.end()){
                Point pp = Point(ceil(p.x), floor(p.y));
                g = Segment(pp,pp);
                it = T.lower_bound(g);
            }
            else{
                g = Segment(p,p);
                it = T.upper_bound(g);
            }





            if(it != T.end()){
                Segment right_neighbour = *it;
                right_neighbour.print("right neighbour 1") ;
                find_new_event(left_neighbour, right_neighbour, p);
            }
            else{
                /// no right neighbour
            }
        }
        else{
            /// no left neighbour
        }

    }
    else{

        double xx1 = floor(p.x - 1.0);
        double yy1 ;
        if(p.y - floor(p.y) < 0.5) yy1 = floor(p.y);
        else yy1 = floor(p.y) + 0.5;
        Point new_p = Point(xx1,yy1);
        g = Segment(new_p,new_p);
        sweep_line_y = yy1;

        /*
        Point pp = Point(floor(p.x), floor(p.y));
        g = Segment(pp,pp);*/



        ///-----------------///
        it = T.lower_bound(g);

        bool is_leftmost = 0, is_rightmost = 0, is_left_neighbour = 0, is_right_neighbour = 0;
        Segment leftmost, rightmost, left_neighbour, right_neighbour;

        Segment check_seg = *it;
        double xx = get_x(check_seg.start_point.x, check_seg.start_point.y, check_seg.end_point.x, check_seg.end_point.y, p.y);
        if(p.x - xx > epsilon){
            cout << "yes left neighbour " << endl;
            cout << "xx " << xx << " " << p.x <<endl;
            is_left_neighbour = 1;
            left_neighbour = *it;
        }
        else{
            if(it != T.begin()) it--;
        }


        cout << "\n--- left to right ---\n" << endl;
        while(it != T.end()){
            Segment s = *it;

            double xx = get_x(s.start_point.x,s.start_point.y,s.end_point.x,s.end_point.y,p.y);
            s.print("L to R");
            cout << "xx " << xx << endl;
            if(fabs(xx - p.x) < epsilon){
                /// if contains p
                cout << "rightmost yes" << endl;
                if(!is_leftmost){
                    cout << "leftmost yes" << endl;
                    is_leftmost = 1;
                    leftmost = *it;

                }

                is_rightmost = 1;
                rightmost = *it;
            }
            else if( p.x - xx > epsilon){
                cout << "left neighbour yes" << endl;
                is_left_neighbour = 1;
                left_neighbour = *it;
            }
            else{
                break;
            }

            it++;

        }

        if(it != T.end()){
            cout << "right neighbour yes" <<endl;
            is_right_neighbour = 1;
            right_neighbour = *it;
        }

        cout << "\n---END left to right END ---\n" << endl;



        if(is_leftmost && is_left_neighbour){
            leftmost.print("leftmost");
            left_neighbour.print("left neighbour");
            find_new_event(left_neighbour, leftmost, p);
        }

        if(is_rightmost && is_right_neighbour){
            rightmost.print("rightmost");
            right_neighbour.print("right neighbour");
            find_new_event(rightmost, right_neighbour, p);
        }

        ///-----------------///
        /// for leftmost segment of Up Cp
        /*it = T.lower_bound(g);



        if(it != T.end()){
            Segment leftmost_segment = *it;
            leftmost_segment.print("leftmost segment");
            /// for immediate left neighbour
            it = T.lower_bound(g);

            if(it != T.begin()){
                it--;
                Segment left_neighbour = *it;
                left_neighbour.print("left neighbour");
                find_new_event(left_neighbour, leftmost_segment, p);
            }
            else{
                /// nothing
            }


        }
        else{
            /// nothing
        }


        /// for rightmost segment of Up Cp
        it = T.upper_bound(g);
        it--;
        if(it != T.end()){





            Segment rightmost_segment = *it;
            /// for immediate right neighbour
            rightmost_segment.print("rightmost segment");
            it = T.upper_bound(g);
            if(it != T.end()){
                Segment right_neighbour = *it;
                right_neighbour.print("right neighbour");
                find_new_event(rightmost_segment, right_neighbour, p);
            }
            else{
                /// nothing
            }


        }
        else{
            /// nothing
        }*/

    }







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

        if(fabs(a.y - b.y) < epsilon && b.x < a.x){
            swap(a,b);
        }
        else if( b.y > a.y){
            swap(a,b);
        }

        //cout << a.x << " " << a.y << " | " << b.x <<" " <<b.y << endl;

        if(Map.find({a.x,a.y}) == Map.end()){
            Map[{a.x,a.y}] = ++point_count;
            Endpoint_Map[{a.x,a.y}] = ++end_points_cnt;
            segments[Map[{a.x,a.y}]].push_back(Segment(a,b,i+1));
            //cout << "yes" << endl;
        }
        else{
            segments[Map[{a.x,a.y}]].push_back(Segment(a,b,i+1));
        }

        if(Map.find({b.x,b.y}) == Map.end()){
            Endpoint_Map[{b.x,b.y}] = ++end_points_cnt;

        }

        Q.insert(Event(a));
        Q.insert(Event(b));


       //T.insert(Segment(a,b));




    }


    /// dummy segment insert
    T.insert(left_dummy_segment);
    T.insert(right_dummy_segment);

    while(!Q.empty()){
        cout << "////////----- event shuru ----///////////" << endl;
        Event e = *Q.begin();
        Q.erase(e);
        e.print();
        erased_event.insert(e);
        sweep_line_y = e.point.y;
        handle_event_point(e);
        //cout << e.point.x << " " << e.point.y << endl;

        //

        cout << "////////----- event shesh---------//////" << endl;


    }

    /*T.erase(Segment(Point(9,9),Point(13,6)));

    multiset<Segment>::iterator it;

    for(it = T.begin(); it != T.end(); it++){
        Segment s = *it;
        s.print();
    }

    cout << "\n-----check-----------" << endl;
    Point p = Point(13,6);

    Segment g = Segment(p,p);

    /// for left side
    //it = T.upper_bound(g);
    //it--;
    /// for immediate left neighbour
    it = T.lower_bound(g);
    it--;
    ///for immediate right neighbour
    //it = T.upper_bound(g);

    if(it == T.begin()){
        cout << "yes"<< endl;

    }

    while(true){
        Segment s = *it;
        if(fabs(get_intersection_x(s,p.y) - p.x) < epsilon){
            s.print();
        }
        else{
            break;
        }
        if(it == T.begin()) break;
        it--;

    }

    if(it == T.end()){
        cout << "yes" << endl;
        return 0;
    }

    Segment s = *it;
    s.print();*/



    /*for(int i = 1; i <= point_count; i++){
        cout << "i " << i << endl;
        for(Segment s : segments[i]){
            cout << "segment" << endl;
            //printf("%.10lf %.10lf\n",s.start_point.x ,s.start_point.y);
            //printf("%.10lf %.10lf\n",s.end_point.x ,s.end_point.y);
            cout << s.start_point.x << " " << s.start_point.y << endl;
            cout << s.end_point.x << " " << s.end_point.y << endl;
        }
    }*/

    return 0;
}
