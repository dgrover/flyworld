#ifndef FLYTUNNEL_H
#define FLYTUNNEL_H

#include <osgDB/ReadFile>
#include <osgViewer/Viewer>
#include <osg/ShapeDrawable>
#include <osg/Texture2D>
#include <osg/TexMat>
#include <osg/ImageSequence>
#include <osgDB/Registry>
#include <osgDB/FileNameUtils>
#include <osgDB/FileUtils>

class FlyTunnel
{
private:
	double viewWidth,
	 viewHeight,
	 boxWidth,
	 boxHeight,
	 xOffset,
	 yOffset,
	 depth,
	 camHorLoc, camVertLoc, distance, rotation,
	 recLength, recWidth;

	osg::ref_ptr<osgViewer::Viewer> viewer;

	const char* imageFileName;
	const char* sequenceFile;
	const char* displayFile;

	osg::ref_ptr<osg::Geode> createShapes();
	void setView();
	void setSequence();
	void setup();

public:

	osg::ref_ptr<osg::ImageSequence> imageSequence;
	std::vector<unsigned int> sequence;
	unsigned int numImages;

	osg::ref_ptr<osgViewer::Viewer> getViewer();

	FlyTunnel(char *imgFile, char *sequence, char *settings, double w, double h, double x) :
		imageFileName(imgFile), sequenceFile(sequence), displayFile(settings), viewWidth(w), viewHeight(h), xOffset(x), yOffset(0),
		camHorLoc(0), camVertLoc(0.0), depth(0), distance(10), rotation(0), recLength(10), recWidth(10), boxWidth(viewWidth / 100.0), boxHeight(viewHeight/100.0)
	{
		viewer = new osgViewer::Viewer();
		setup();
	}
};
#endif