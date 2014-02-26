
#include <osg/Notify>
#include <osg/TextureRectangle>
#include <osg/Geometry>
#include <osg/Geode>
#include <osg/TexMat>
#include <osg/Group>
#include <osg/Projection>
#include <osg/MatrixTransform>
#include <osgText/Text>
#include <osgDB/Registry>
#include <osgDB/ReadFile>
#include <osgViewer/Viewer>

#include <osg/ShapeDrawable>
#include <osg/Material>
#include <osg/Texture2D>
#include <osg/Math>
#include <osgGA/FlightManipulator>
#include <osgGA/FirstPersonManipulator>
#include <osgGA/TerrainManipulator>
#include <osgGA/DriveManipulator>
#include <osgGA/SphericalManipulator>
#include <osgGA/UFOManipulator>
#include <osgGA/AnimationPathManipulator>
#include <osgGA/TrackballManipulator>


osg::Vec4 backgroundColor = osg::Vec4(0, 0, 0, 1); //black background
float cylinderRadius = 2.0f; 
float cylinderHeight = 4.0f;
float cylinderX = 0.0f; //location
float cylinderY = 0.0f;
float cylinderZ = 0.0f;
std::string imageFileName="rand_cb.jpg";

osg::Geode* createCylinder()
{
	osg::Geode* geode = new osg::Geode();

	osg::StateSet* stateset = new osg::StateSet();
	osg::Image* image = new osg::Image();
	image = osgDB::readImageFile(imageFileName);

	if (image)
	{
		osg::Texture2D* texture = new osg::Texture2D;
		texture->setImage(image);
		stateset->setTextureAttributeAndModes(0,texture,osg::StateAttribute::ON);
	}

	geode->setStateSet( stateset );
	stateset->setMode(GL_LIGHTING, osg::StateAttribute::OFF);


	osg::TessellationHints* hints = new osg::TessellationHints;
	hints->setDetailRatio(15.0f);

	hints->setCreateBackFace(true);
	hints->setCreateFrontFace(false);
	hints->setCreateNormals(false);
	hints->setCreateTop(false);
	hints->setCreateBottom(false);
	geode->addDrawable(new osg::ShapeDrawable(new osg::Cylinder(osg::Vec3(cylinderX,cylinderY,cylinderZ),cylinderRadius,cylinderHeight),hints));


	return geode;
}


osg::Geode* createShapes()
{
	osg::Geode* geode = new osg::Geode();

	osg::StateSet* stateset = new osg::StateSet();

	//stateset->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
	geode->setStateSet( stateset );

	float radius = 0.8f;
	float height = 1.0f;

	osg::TessellationHints* hints = new osg::TessellationHints;
	hints->setDetailRatio(15.0f);

	//hints->setCreateBackFace(true);
	//hints->setCreateFrontFace(false);
	//hints->setCreateNormals(false);
	//hints->setCreateTop(false);
	//hints->setCreateBottom(false);


	osg::ShapeDrawable* box = new osg::ShapeDrawable(new osg::Box(osg::Vec3(1.0f,0.0f,0.0f),radius/8), hints);
	box->setColor(osg::Vec4(1,0,1,1));

	osg::ShapeDrawable* cone = new osg::ShapeDrawable(new osg::Cone(osg::Vec3(0.5f,1.0f,0.0f),radius/10,height/2),hints);
	cone->setColor(osg::Vec4(1,0,0,1));

	osg::ShapeDrawable* capsule = new osg::ShapeDrawable(new osg::Capsule(osg::Vec3(1.0f,1.0f,0.0f),radius/8,height/3),hints);
	capsule->setColor(osg::Vec4(0,0,1,1));

	osg::ShapeDrawable* sphere = new osg::ShapeDrawable(new osg::Sphere(osg::Vec3(0.0f,-1.0f,0.0f),radius/10),hints);
	sphere->setColor(osg::Vec4(0,1,1,1));

	geode->addDrawable(sphere);
	geode->addDrawable(box);
	geode->addDrawable(cone);
	geode->addDrawable(capsule);
	return geode;
}


int main( int argc, char **argv )
{
	// create the model
	osg::Group* root = new osg::Group;
	root->addChild( createCylinder() );
	root->addChild( createShapes() );
	osgViewer::Viewer viewer;

	// add model to viewer.
	viewer.setSceneData( root );
	viewer.getCamera()->setClearColor(backgroundColor);
	viewer.setCameraManipulator(new osgGA::TrackballManipulator);
	//starting position for manipulator
	viewer.getCameraManipulator()->setHomePosition(osg::Vec3d(0,0,0), osg::Vec3d(1,1,0), osg::Vec3d(0,0,1), false); //eye, center, up
	
	return viewer.run();

	return 0;
}