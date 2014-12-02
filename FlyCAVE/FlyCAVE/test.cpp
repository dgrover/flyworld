#include "OpenLoopSphere.h"



int main( int argc, char **argv )
{

	osg::ref_ptr<osgViewer::Viewer> viewer;
	OpenLoopSphere ols (1, 1366, 768, 0, 0, 0, 0, 7.5/2.0, 7.5/2.0+10.5, 0.5, 0.2, "verticalLine.jpg", "displaySettings.txt");

	viewer=ols.setup();

	while(!viewer->done())
	{
		viewer->getSlave(0)._viewOffset=ols.getView(1);
		viewer->frame();
	}
}