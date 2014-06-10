#include <osgDB/ReadFile>
#include <osgViewer/Viewer>
#include <osg/ShapeDrawable>
#include <osg/Texture2D>

//#include <osg/Notify>
//#include <osg/TextureRectangle>
//#include <osg/Geometry>
//#include <osg/Geode>
//#include <osg/TexMat>
//#include <osg/Group>
//#include <osg/Projection>
//#include <osg/MatrixTransform>
//#include <osgText/Text>
//#include <osgDB/Registry>
//#include <iostream>
//#include <fstream>
//#include <osg/Material>
//#include <osg/Math>
//#include <vector>

//lib files included
//osg.lib osgDB.lib osgViewer.lib 

//lib files removed
//OpenThreads.lib osgAnimation.lib osgFX.lib osgGA.lib osgManipulator.lib osgParticle.lib osgShadow.lib osgPresentation.lib osgSim.lib osgTerrain.lib osgText.lib osgUtil.lib osgVolume.lib osgWidget.lib

using namespace std;

osg::Vec4 backgroundColor = osg::Vec4(0, 0, 0, 1); //black background

float cylinderRadius = 5.0f; 
float cylinderHeight = 5.0f;

float cylinderX = 0.0f; //location
float cylinderY = 0.0f;
float cylinderZ = 0.0f;

string imageFileName = "..//..//images//test.jpg";

double depth = 1;
double xS = 0.0; //speed of camera
double yS = 0;
double zS = 0.0;
double rS = 0.0; //rotation speed

double camHorLoc = 0;
double camVertLoc = 0;
//double distance = 13.5;

osg::Vec3d up = osg::Vec3d(0,0,1); //up vector

double imageWidth = 1280;
double imageHeight = 800;

//double fovy = 30;

double boxHorMin = 0;
double boxVertMin = 0;
double boxHorMax = boxHorMin + imageWidth;
double boxVertMax = boxVertMin + imageHeight;

//osg::BoundingBox bb;

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
	hints->setCreateTop(true);
	hints->setCreateBottom(true);

	geode->addDrawable(new osg::ShapeDrawable(new osg::Cylinder(osg::Vec3(cylinderX,cylinderY,cylinderZ),cylinderRadius,cylinderHeight),hints));
	
	return geode;
}

//osg::Geode* createShapes()
//{
//	osg::Geode* geode = new osg::Geode();
//
//	osg::StateSet* stateset = new osg::StateSet();
//
//	//stateset->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
//	geode->setStateSet( stateset );
//
//	float radius = 0.8f;
//	float height = 1.0f;
//
//	osg::TessellationHints* hints = new osg::TessellationHints;
//	hints->setDetailRatio(15.0f);
//
//	//hints->setCreateBackFace(true);
//	//hints->setCreateFrontFace(false);
//	//hints->setCreateNormals(false);
//	//hints->setCreateTop(false);
//	//hints->setCreateBottom(false);
//
//
//	//osg::ShapeDrawable* box = new osg::ShapeDrawable(new osg::Box(osg::Vec3(1.0f,0.0f,0.0f),radius/8), hints);
//	//box->setColor(osg::Vec4(1,0,1,1));
//
//	//osg::ShapeDrawable* box = new osg::ShapeDrawable(new osg::Box(osg::Vec3(-1.65f,0.0f,0.0f),0.5, 0.5, 4), hints);
//	//box->setColor(osg::Vec4(1,0,1,1));
//
//	//osg::ShapeDrawable* cone = new osg::ShapeDrawable(new osg::Cone(osg::Vec3(0.5f,1.0f,0.0f),radius/10,height/2),hints);
//	//cone->setColor(osg::Vec4(1,0,0,1));
//
//	//osg::ShapeDrawable* capsule = new osg::ShapeDrawable(new osg::Capsule(osg::Vec3(1.0f,1.0f,0.0f),radius/8,height/3),hints);
//	//capsule->setColor(osg::Vec4(0,0,1,1));
//
//	//osg::ShapeDrawable* sphere = new osg::ShapeDrawable(new osg::Sphere(osg::Vec3(0.0f,-1.0f,0.0f),radius/10),hints);
//	//sphere->setColor(osg::Vec4(0,1,1,1));
//
//	//geode->addDrawable(sphere);
//	//geode->addDrawable(box);
//	//geode->addDrawable(cone);
//	//geode->addDrawable(capsule);
//
//	//bb=box->computeBound();
//
//	return geode;
//}


int main( int argc, char **argv )
{
	// create the model
	osg::Group* root = new osg::Group;
	root->addChild( createCylinder() );
	//root->addChild( createShapes() );
	osgViewer::Viewer viewer;

	// add model to viewer.
	viewer.setSceneData( root );
	viewer.getCamera()->setClearColor(backgroundColor);

	//viewer.setCameraManipulator(NULL);
	viewer.getCamera()->setViewMatrixAsLookAt(osg::Vec3d(camHorLoc,0,camVertLoc), osg::Vec3d(camHorLoc,depth,camVertLoc), up);

	int index=0;
	int maxIndex=0;

	vector<float> rotationNumbers;
	vector<float> translationNumbers;
	
	float number;
	ifstream rot("rotationNumbers.txt",ios::in);
	
	while (rot >> number) 
		rotationNumbers.push_back(number);

	rot.close();
	
	ifstream trans("translationNumbers.txt",ios::in);

	while (trans >> number) 
		translationNumbers.push_back(number);
	
	trans.close();
	
	while(!viewer.done())
	{

		osg::Matrixd currentView = viewer.getCamera()->getViewMatrix();
		osg::Matrixd cameraRotation, cameraTrans;
		
		osg::Vec3d camEye, camCenter, camUp;

		// Three axis rotation and translation

		//cameraRotation.makeRotate(
		//	osg::DegreesToRadians(rotationNumbers[index]), osg::Vec3(0,1,0), // yaw
		//	osg::DegreesToRadians(rotationNumbers[index+1]*0), osg::Vec3(1,0,0) , // pitch
		//	osg::DegreesToRadians( rotationNumbers[index+2]*0), osg::Vec3(0,0,1) ); // roll
		//cameraTrans.makeTranslate( translationNumbers[index],translationNumbers[index+1],translationNumbers[index+2] );

		// Yaw axis rotation and z-axis translation
		cameraRotation.makeRotate(osg::DegreesToRadians(rotationNumbers[index]), osg::Vec3(0,1,0)); // yaw
		cameraTrans.makeTranslate( 0,0,translationNumbers[index] );

		viewer.getCamera()->setViewMatrix(currentView*cameraRotation*cameraTrans);
		viewer.getCamera()->getViewMatrixAsLookAt(camEye, camCenter, camUp, 1.0);

		// Cylinder boundary detection
		if(camEye.z()>=cylinderHeight/2 || camEye.z()<=-cylinderHeight/2 || pow(camEye.x(), 2) + pow(camEye.y(), 2) >=pow(cylinderRadius, 2))
			viewer.getCamera()->setViewMatrix(currentView*cameraRotation);
				
		//if(bb.contains(camEye))
		//	viewer.getCamera()->setViewMatrix(currentView*cameraRotation);
		
		viewer.frame();
		index++;

		if (index > maxIndex)
			index=0;
	}

	return 0;
}
