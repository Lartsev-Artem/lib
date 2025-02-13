#include "convex_hull_2d.h"

typedef double Type;

int main() {

	const std::string FNameGrahamSpanPoints = "GSpan.txt";
	const std::string FNameJarvisSpanPoints = "JSpan.txt";
	const std::string FNameChanSpanPoints = "CSpan.txt";
	const std::string FNameAllPoints = "Rand2.txt";



	ConvexHull2D<Type> convex_hull;
	convex_hull.RandCoord(47 , 63);
	convex_hull.Show();

	std::ofstream ofs(FNameAllPoints);
	ofs << convex_hull.GetData();
	ofs.close();

	
	convex_hull.SetCore(ConvexHull2D<Type>::e_jarvis);
	convex_hull.Run();
	convex_hull.OutFile(FNameJarvisSpanPoints);
	
	convex_hull.SetCore(ConvexHull2D<Type>::e_graham);
	convex_hull.Run();
	convex_hull.OutFile(FNameGrahamSpanPoints);

	convex_hull.SetCore(ConvexHull2D<Type>::e_chan);
	convex_hull.Run();
	convex_hull.OutFile(FNameChanSpanPoints);

	return 0;
};
