// spherecalib.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

double imageWidth = 912;
double imageHeight = 1140;

int xoffset = 0;
int yoffset = 0;

double camHorLoc = 0;
double camVertLoc = 0;

float cradius = 2.0;
float pdist = 11.0;

float distance = cradius + pdist;

float fov = 25.0;

float transInc = 0.1;
float rotInc = 1;
double depth = 0;

osg::Matrixd transZforward = osg::Matrixd::translate(0.0, 0.0, transInc);
osg::Matrixd transZbackward = osg::Matrixd::translate(0.0, 0.0, transInc*-1);
osg::Matrixd transXleft = osg::Matrixd::translate(transInc, 0.0, 0.0);
osg::Matrixd transXright = osg::Matrixd::translate(transInc*-1, 0.0, 0.0);
osg::Matrixd rotcw = osg::Matrixd::rotate(osg::DegreesToRadians(rotInc), osg::Vec3(0, 1, 0));
osg::Matrixd rotccw = osg::Matrixd::rotate(osg::DegreesToRadians(rotInc*-1), osg::Vec3(0, 1, 0));

osg::Vec4 backgroundColor = osg::Vec4(0, 0, 0, 1);
osg::Vec3d up = osg::Vec3d(0, 0, 1); //up vector

const char* imageFileName = "image.bmp";

osg::Matrixd sphereView;
osgViewer::Viewer viewer;

void printInfo()
{
	osg::Quat::value_type  angle = *new osg::Quat::value_type();
	osg::Quat::value_type  rotVecX = *new osg::Quat::value_type();
	osg::Quat::value_type  rotVecY = *new osg::Quat::value_type();
	osg::Quat::value_type  rotVecZ = *new osg::Quat::value_type();

	printf("**********\n");

	printf("Camera X Position: %f\n", viewer.getSlave(0)._viewOffset.getTrans().x());
	printf("Camera Z Position: %f\n", viewer.getSlave(0)._viewOffset.getTrans().z());

	viewer.getSlave(0)._viewOffset.getRotate().getRotate(angle, rotVecX, rotVecY, rotVecZ);

	printf("Camera Rotation: %f\n", osg::RadiansToDegrees(angle));
	printf("Camera Rotation Vector X: %f\n", rotVecX);
	printf("Camera Rotation Vector Y: %f\n", rotVecY);
	printf("Camera Rotation Vector Z: %f\n", rotVecZ);
	printf("\n");

	printf("**********\n\n");
}

class keyboardHandler : public osgGA::GUIEventHandler
{
	public:
		virtual bool handle(const osgGA::GUIEventAdapter&, osgGA::GUIActionAdapter&);
};

bool keyboardHandler::handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa)
{
	if (ea.getEventType() == osgGA::GUIEventAdapter::KEYDOWN)
	{
		switch (ea.getKey())
		{
			case 'a':
				sphereView = sphereView*transXleft;
				break;

			case 'd':
				sphereView = sphereView*transXright;
				break;

			case 'w':
				sphereView = sphereView*transZforward;
				break;

			case 's':
				sphereView = sphereView*transZbackward;
				break;

			case 'q':
				sphereView = sphereView*rotccw;
				break;

			case 'e':
				sphereView = sphereView*rotcw;
				break;

			case 'p':
				printInfo();
				break;

			default:
				return false;
		}

		return true;
	}
	else
		return false;
}


osg::Geode* createShapes()
{
	osg::Geode* geode = new osg::Geode();

	osg::StateSet* stateset = new osg::StateSet();
	stateset->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
	osg::Image* image = new osg::Image();

	image = osgDB::readImageFile(imageFileName);

	if (image)
	{
		osg::Texture2D* texture = new osg::Texture2D(image);
		texture->setWrap(osg::Texture::WRAP_S, osg::Texture::REPEAT);
		
		osg::TexMat* texmat = new osg::TexMat;
		stateset->setTextureAttributeAndModes(0, texture, osg::StateAttribute::ON);
		stateset->setTextureAttributeAndModes(0, texmat, osg::StateAttribute::ON);
		texture->setUnRefImageDataAfterApply(true);
	}

	geode->setStateSet(stateset);
	geode->setCullingActive(false);

	osg::TessellationHints* hints = new osg::TessellationHints;
	hints->setDetailRatio(0.8f);

	hints->setCreateBackFace(true);
	hints->setCreateFrontFace(false);
	hints->setCreateNormals(false);
	hints->setCreateTop(false);
	hints->setCreateBottom(false);

	osg::Sphere* sph = new osg::Sphere(osg::Vec3(0.0f, 0.0f, 0.0f), cradius);
	osg::ShapeDrawable* sphere = new osg::ShapeDrawable(sph, hints);
	sphere->setUseDisplayList(false);
	geode->addDrawable(sphere);

	osg::TexMat* texmat = (osg::TexMat*)(stateset->getTextureAttribute(0, osg::StateAttribute::TEXMAT));

	return geode;
}

int _tmain(int argc, _TCHAR* argv[])
{
	osg::Group* root = new osg::Group;
	root->addChild(createShapes());
	
	viewer.setSceneData(root);

	osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits;
	traits->x = xoffset;
	traits->y = yoffset;
	traits->width = imageWidth;
	traits->height = imageHeight;
	traits->windowDecoration = false;
	traits->doubleBuffer = true;
	traits->sharedContext = 0;

	osg::ref_ptr<osg::GraphicsContext> gc = osg::GraphicsContext::createGraphicsContext(traits.get());

	osg::ref_ptr<osg::Camera> camera = new osg::Camera;
	camera->setGraphicsContext(gc.get());
	camera->setViewport(new osg::Viewport(0, 0, traits->width, traits->height));
	GLenum buffer = traits->doubleBuffer ? GL_BACK : GL_FRONT;
	camera->setDrawBuffer(buffer);
	camera->setReadBuffer(buffer);
	viewer.addSlave(camera.get(), osg::Matrixd(), osg::Matrixd());

	keyboardHandler* handler = new keyboardHandler();
	viewer.addEventHandler(handler);
	viewer.getCamera()->setClearColor(backgroundColor); // black background
	viewer.getCamera()->setViewMatrixAsLookAt(osg::Vec3d(camHorLoc, distance, camVertLoc), osg::Vec3d(camHorLoc, depth, camVertLoc), up);
	viewer.getCamera()->setProjectionMatrixAsPerspective(fov, imageWidth / imageHeight, 0.1, 5);
	
	viewer.setCameraManipulator(NULL);

	while (!viewer.done())
	{
		viewer.getSlave(0)._viewOffset = sphereView;
		viewer.frame();
	}

	return 0;
}

