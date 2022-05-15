#include <iostream>
#include <stack>
#include <stdlib.h>
using namespace std;

struct point_2d
{
	int x, y;
};

/* A global point needed for sorting points with reference
   to the first point Used in compare function of qsort()
*/
point_2d p0;

// A utility function to find next to top in a stack. i.e. Get the second top element
point_2d get_second_top_ele(stack<point_2d> &my_stack)
{
	point_2d p = my_stack.top();
	my_stack.pop();
	point_2d res = my_stack.top();
	my_stack.push(p);
	return res;
}

// A utility function to swap two points
void swap(point_2d &p1, point_2d &p2)
{
	point_2d temp = p1;
	p1 = p2;
	p2 = temp;
}

// A utility function to return square of distance between p1 and p2
int get_dist_sq(point_2d p1, point_2d p2)
{
	return (p1.x - p2.x)*(p1.x - p2.x) +
		(p1.y - p2.y)*(p1.y - p2.y);
}

/* To find get_orientation of ordered triplet (p, q, r).
   return values:
   0 --> p, q and r are collinear
   1 --> Clockwise
   2 --> Counterclockwise (Anticlockwise)
*/
int get_orientation(point_2d p, point_2d q, point_2d r)
{
	int val = (q.y - p.y) * (r.x - q.x) -
			(q.x - p.x) * (r.y - q.y);

	if (val == 0) return 0; // collinear
	return (val > 0)? 1: 2; // clock or counterclock wise
}

/* A function used by library function qsort() to sort an array of
   points with respect to the first point
*/
int compare(const void *vp1, const void *vp2)
{
	point_2d *p1 = (point_2d *)vp1;
	point_2d *p2 = (point_2d *)vp2;

	// Find get_orientation
	int o = get_orientation(p0, *p1, *p2);
	if (o == 0)
		return (get_dist_sq(p0, *p2) >= get_dist_sq(p0, *p1))? -1 : 1;

	return (o == 2)? -1: 1;
}

// Prints convex hull of a set of n points.
void convex_hull(point_2d points[], int n)
{
	// Find the bottommost point
	int ymin = points[0].y, min = 0;
	for (int i = 1; i < n; i++)
	{
		int y = points[i].y;

		// Pick the bottom-most and choose the left most point in case of tie
		if ((y < ymin) || (ymin == y &&
			points[i].x < points[min].x))
			ymin = points[i].y, min = i;
	}

	// Place the bottom-most point at first position
	swap(points[0], points[min]);

	/* sort n-1 points with respect to the first point.
	   A point p1 comes before p2 in sorted output if p2
	   has larger polar angle (in counterclockwise
	   direction) than p1
	*/
	p0 = points[0];
	qsort(&points[1], n-1, sizeof(point_2d), compare);

#if 0
	// Enable this code if you want to see the sorted output
	for(int z=0; z<n; z++) {
		point_2d p = points[z];
		cout<<endl<< "x y " << p.x << " " << p.y <<endl;
	}
#endif

	/* Get the farthest collinear point. Ignore other collinear point.
	   i.e. do not get the all colliniear points. Only get the farthest collinear point.
	 */
	int m = 1; // Initialize size of modified array
	for (int i=1; i<n; i++)
	{
		while (i < n-1 && get_orientation(p0, points[i],
									points[i+1]) == 0)
			i++;

		points[m++] = points[i];
	}

#if 0
	// Enable this code if you want to see the m output of colliniar points
	cout << "The m ponts" << endl;
	for(int z=0; z<m; z++) {
		point_2d p = points[z];
		cout<<endl<< "x y " << p.x << " " << p.y <<endl;
	}
#endif

	// If modified array of points has less than 3 points, convex hull is not possible
	if (m < 3) return;

	// Create an empty stack and push first three points
	stack<point_2d> my_stack;
	my_stack.push(points[0]);
	my_stack.push(points[1]);
	my_stack.push(points[2]);

	// Process remaining n-3 points. i.e You have to process 3 to m points.
	for (int i = 3; i < m; i++)
	{
		/* Not anticlockwiise then pop from the stack. Else push into the stack. */
		while (my_stack.size()>1 && get_orientation(get_second_top_ele(my_stack), my_stack.top(), points[i]) != 2)
			my_stack.pop();
		my_stack.push(points[i]);
	}

	/* Now stack has the output points, print contents of stack */
	cout<< endl << "Convex Hull is"<<endl;
	while (!my_stack.empty())
	{
		point_2d p = my_stack.top();
		cout << "(" << p.x << ", " << p.y <<")" << endl;
		my_stack.pop();
	}
}

// Driver program to test above functions
int main()
{
	int n, x, y, i = 0;

	cout <<endl<< "Enter the number of points"<<endl;
	cin >> n;
	cout << endl << "Enter the x and y co-ordinate value"<< endl;

	point_2d points[n];

	int n_it = n;
	while (n_it != 0) {
		cout << endl << "Enter x" << endl;
		cin >> x;
		cout << endl << "Enter y" << endl;
		cin >> y;
		points[i].x = x;
		points[i].y = y;

		i++;
		n_it--;
	}
	convex_hull(points, n);
	return 0;
}
