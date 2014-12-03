// andno037 anden259


#include <QApplication>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <vector>
#include <chrono>
#include "Point.h"
#include <map>

// constants
static const int SCENE_WIDTH = 512;
static const int SCENE_HEIGHT = 512;

void render_points(QGraphicsScene* scene, const vector<Point>& points)
{
    for (const auto& point : points) {
        point.draw(scene);
    }
}

void render_line(QGraphicsScene* scene, const Point& p1, const Point& p2)
{
    p1.lineTo(scene, p2);
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // open file
    string filename = "input400.txt";
    ifstream input;
    input.open(filename);

    // the vector of points
    vector<Point> points;

    // read points from file
    int N;
    int x;
    int y;

    input >> N;

    for (int i = 0; i < N; ++i) {
        input >> x >> y;
        points.push_back(Point(x, y));
    }
    input.close();

    // setup graphical window
    QGraphicsView *view = new QGraphicsView();
    QGraphicsScene *scene = new QGraphicsScene(0, 0, SCENE_WIDTH, SCENE_HEIGHT);
    view->setScene(scene);
    // draw points to screen all at once
    render_points(scene, points);
    view->scale(1, -1); //screen y-axis is inverted
    view->resize(view->sizeHint());
    view->setWindowTitle("Fast Pattern Recognition - andno037 anden259");
    view->show();

    // sort points by natural order
    // makes finding endpoints of line segments easy
    sort(points.begin(), points.end());
    auto begin = chrono::high_resolution_clock::now();

// ---------------------------------------------------------------------
// this section is the only code we wrote, everything else is from the original code.

    // for each point in sorted order
    for (size_t p = 0; p < points.size(); ++p) {
        // indexed with the sloap from the start point to the other point we are checking
        // and store the point in a vector
        map<double , vector<Point> > slop_map;
        for (size_t index = p + 1; index < points.size(); ++index) {
            // use the sloap as index and push back the point in the vector
            slop_map[points.at(p).slopeTo(points.at(index))].push_back(points.at(index));
        }

        // go through all stored points
        for (auto pair : slop_map) {
            // if it's 3 points or more in the vector (the vector does NOT contain the start point),
            // render the line from start to the last point (since the points are sorted already,
            // the full line will be draw.
            if (pair.second.size() >= 3) {
                render_line(scene, points.at(p), *(pair.second.end() - 1));
                a.processEvents(); // show rendered line
            }
        }
    }
// ---------------------------------------------------------------------

    auto end = chrono::high_resolution_clock::now();
    cout << "Computing line segments took "
         << std::chrono::duration_cast<chrono::milliseconds>(end - begin).count()
         << " milliseconds." << endl;

    return a.exec(); // start Qt event loop
}
