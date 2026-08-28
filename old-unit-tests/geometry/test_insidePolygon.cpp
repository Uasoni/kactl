#include "../UnitTest.h"
#include "../../content/geometry/insidePolygon.h"

const int TWEAKS = 5;

class TestInsidePolygon :
	public UnitTest
{
public:
	vector<vector<Point<int> > > polys;
	vector<string> strings;
	vector<Point<int> > points;
	vector<int> answers;

	TestInsidePolygon() : UnitTest("TestInsidePolygon") {
		ifstream in("inside_polygon.in");
		int n;
		while (in >> n) {
			vector<Point<int> > poly(n);
			stringstream ss;
			for (int i = 0; i < (n); ++i) {
				in >> poly[i];
				ss << poly[i] << " ";
			}
			int q;
			in >> q;
			Point<int> p;
			for (int i = 0; i < (q); ++i) {
				in >> p;
				polys.push_back(poly);
				strings.push_back(ss.str());
				points.push_back(p);
			}
		}
		in.close();

		ifstream out("inside_polygon.out");
		answers.resize(points.size());
		for (int i = 0; i < (answers.size()); ++i) {
			out >> answers[i];
		}
	}

	virtual ~TestInsidePolygon()
	{
	}

	virtual void run(int c)
	{
		if (c < points.size()) {
			bool a = inside_polygon(polys[c].begin(),
					polys[c].end(),points[c],false);
			stringstream ss;
			ss << strings[c] << " : " << points[c];
			check(a, (bool)answers[c], ss.str());
		} else {
			vector<Point<double> > poly(2+rand()%20);
			for(auto &i:poly) i = Point<double>(rand()%100000,rand()%100000);
			Point<double> p(rand()%100000,rand()%100000);
			bool a = inside_polygon(poly.begin(),poly.end(),p,true);
			for (int i = 0; i < 10; ++i) {
				double alpha = rand()%1000/1000.0;
				Point<double> D(rand()%1000,rand()%1000);
				for(auto &i:poly) i = i.rotate(alpha)+D;
				p = p.rotate(alpha)+D;
				check(inside_polygon(poly.begin(),poly.end(),p,true),a);
			}
		}
	}

	virtual int get_count() const
	{
		return points.size() + 20;
	}
};

KACTL_AUTOREGISTER_TEST(TestInsidePolygon);
