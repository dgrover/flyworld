
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
#include <iostream>
#include <fstream>
#include <osg/ShapeDrawable>
#include <osg/Material>
#include <osg/Texture2D>
#include <osg/Math>
#include <vector>



osg::Vec4 backgroundColor = osg::Vec4(0, 0, 0, 1); //black background
float cylinderRadius = 2.0f; 
float cylinderHeight = 4.0f;
float cylinderX = 0.0f; //location
float cylinderY = 0.0f;
float cylinderZ = 0.0f;
std::string imageFileName="test.jpg";

double depth=1;
double xS=0.0; //speed of camera
double yS=0;
double zS=0.0;
double rS=0.0; //rotation speed
//double camHorLoc=boxHorMin+imageWidth/2*numBoxes; //location of camera
double camHorLoc=0;
double camVertLoc=0;
double distance=13.5;
osg::Vec3d up=osg::Vec3d(0,0,1); //up vector


double imageWidth=1200;
double imageHeight=800;

double fovy=30;

double boxHorMin=0;
double boxVertMin=0;
double boxHorMax=boxHorMin + imageWidth;
double boxVertMax=boxVertMin + imageHeight;






class CameraUpdateCallback : public osg::NodeCallback
{
public:
	CameraUpdateCallback(
		double delay = 0.0001):


	_delay(delay),
		_prevTime(0.0),
		timeToSubtract(0.0)
	{
	}

	virtual void operator()(osg::Node* node, osg::NodeVisitor* nv)
	{
		osg::Camera* cam = static_cast<osg::Camera*>( node ); 
		//double move;

		if (nv->getFrameStamp()) {
			double currTime = nv->getFrameStamp()->getSimulationTime();
			//if (currTime - _prevTime > _delay) {

//use timeToSubtract..........................
//boxHorMin+imageWidth/2*3 switching point to boxHorMin+imageWidth/2 (or camHorLoc)

				//boxHorMin+imageWidth/2*numBoxes is middle of boxes
				//move=camHorLoc+xS*(currTime-timeToSubtract);
/*if(move>=boxHorMin+imageWidth/2*3)
{
	
timeToSubtract=currTime;
move=camHorLoc;
}*/
//cam->setProjectionMatrixAsPerspective(30, 3.0 / 3.0, 0.1, 3000);
//cam->setProjectionMatrix(osg::Matrix::ortho(-1,1,-1,1,-1, 1));
				//cam->setViewMatrixAsLookAt(osg::Vec3d(move,distance,camVertLoc+yS*currTime), osg::Vec3d(move,0.0,camVertLoc+yS*currTime), up);
//cam->setViewMatrixAsLookAt(osg::Vec3d(camHorLoc,distance,camVertLoc+yS*currTime), osg::Vec3d(move,depth+zS*currTime,camVertLoc+yS*currTime), up);
//makeRotate (value_type angle, const Vec3f &axis)
osg::Matrixd cameraRotation;
osg::Matrixd cameraTrans;
cameraRotation.makeRotate(
			osg::DegreesToRadians(rS), osg::Vec3(0,1,0), // roll
          osg::DegreesToRadians(0.0), osg::Vec3(1,0,0) , // pitch
          osg::DegreesToRadians( 0.0), osg::Vec3(0,0,1) ); // heading 

// cameraTrans.makeTranslate( 0,0,0 );
cam->setViewMatrix(cam->getViewMatrix()*cameraRotation);


				_prevTime = currTime;
			}
		//}
		std::cout<<"Camera update callback - pre traverse"<<node<<std::endl;
		traverse(node,nv);
		std::cout<<"Camera update callback - post traverse"<<node<<std::endl;
	}
private:
	double _delay;
	double _prevTime;
	double  timeToSubtract;

};





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
	//viewer.setCameraManipulator(new osgGA::FlightManipulator);
	//starting position for manipulator
	//viewer.getCameraManipulator()->setHomePosition(osg::Vec3d(0,0,0), osg::Vec3d(1,1,0), osg::Vec3d(0,0,1), false); //eye, center, up
	
viewer.setCameraManipulator(NULL);
//viewer.getCamera()->setUpdateCallback(new CameraUpdateCallback());
viewer.getCamera()->setViewMatrixAsLookAt(osg::Vec3d(camHorLoc,0,camVertLoc), osg::Vec3d(camHorLoc,depth,camVertLoc), up);

int maxIndex=98;
int index=0;
std::vector<float> rotationNumbers;
std::vector<float> translationNumbers;
{
using namespace std;

ifstream in("rotationNumbers.txt",ios::in);
float number;
while (in >> number) {
		rotationNumbers.push_back(number);
	}
in.close();
}


{
using namespace std;

ifstream in("translationNumbers.txt",ios::in);
float number;
while (in >> number) {
		translationNumbers.push_back(number);
	}
in.close();
}




/*
rotationNumbers.push_back(0.1);
rotationNumbers.push_back(0.2);
rotationNumbers.push_back(0.13);
rotationNumbers.push_back(-0.1);
rotationNumbers.push_back(0.2);
rotationNumbers.push_back(0.13);
rotationNumbers.push_back(-0.1);
rotationNumbers.push_back(0.2);
rotationNumbers.push_back(-0.13);*/
osg::Vec3d camEye, camCenter, camUp;
	while(!viewer.done())
	{



		osg::Matrixd currentView = viewer.getCamera()->getViewMatrix();
osg::Matrixd cameraRotation;
osg::Matrixd cameraTrans;
/*cameraRotation.makeRotate(
			osg::DegreesToRadians(rotationNumbers[index]*2), osg::Vec3(0,1,0), // heading
          osg::DegreesToRadians(0.0), osg::Vec3(1,0,0) , // pitch
          osg::DegreesToRadians( 0.0), osg::Vec3(0,0,1) ); // roll*/

cameraRotation.makeRotate(
			osg::DegreesToRadians(rotationNumbers[index]), osg::Vec3(0,1,0), // roll
          osg::DegreesToRadians(rotationNumbers[index+1]), osg::Vec3(1,0,0) , // pitch
          osg::DegreesToRadians( rotationNumbers[index+2]), osg::Vec3(0,0,1) ); // heading

 cameraTrans.makeTranslate( 0,0,translationNumbers[index] );

 //cameraTrans.makeTranslate( translationNumbers[index],translationNumbers[index+1],translationNumbers[index+2] );
 viewer.getCamera()->setViewMatrix(currentView*cameraRotation*cameraTrans);


viewer.getCamera()->getViewMatrixAsLookAt(camEye, camCenter, camUp, 1.0);
cameraRotation.makeRotate(
			osg::DegreesToRadians(rotationNumbers[index]*2), osg::Vec3(0,1,0), // roll
          osg::DegreesToRadians(rotationNumbers[index+1]*2), osg::Vec3(1,0,0) , // pitch
          osg::DegreesToRadians( rotationNumbers[index+2]*2), osg::Vec3(0,0,1) ); // heading
if(camEye.z()>=cylinderHeight/2-0.5)
{
	viewer.getCamera()->setViewMatrix(currentView*cameraRotation);
}

if(camEye.z()<=cylinderHeight/2*-1+0.5)
{
	viewer.getCamera()->setViewMatrix(currentView*cameraRotation);
}

if(pow(camEye.x(), 2) + pow(camEye.y(), 2) >=pow(cylinderRadius, 2)-0.5)
{
viewer.getCamera()->setViewMatrix(currentView*cameraRotation);
}
//viewer.getCamera()->setViewMatrixAsLookAt(osg::Vec3d(camHorLoc,0,camVertLoc), osg::Vec3d(camHorLoc,depth,camVertLoc), up);
		viewer.frame();
		index+=3;
		if (index>maxIndex)
		{
index=0;
		}
	}
	return 0;
}


//cylinderRadius=2

//x^2+y^2=radius^2