
#include "stdafx.h"
#include "tunnelcalib.h"

int xOffset = 1920;
int yOffset = 0;

double viewWidth = 1280*2;
double viewHeight =  800;

double boxWidth = viewWidth / 100.0;
double boxHeight = viewHeight / 100.0;

double defaultDistance = 10.0;
double distance = defaultDistance;


osg::ref_ptr<osg::Box> box;
double defaultRotation = 0;
double rotation = defaultRotation;
double loadedRotation = defaultRotation;
double loadedDistance = defaultDistance;


double defaultCamHorLoc = boxWidth / 2.0;
double loadedCamHorLoc = defaultCamHorLoc;
double camHorLoc = defaultCamHorLoc;
double camVertLoc = 0;
double transInc = 0.1;
double depth = 0;



osg::Vec4 backgroundColor = osg::Vec4(0, 0, 0, 1);
osg::Vec3d up = osg::Vec3d(0, 0, 1); //up vector
const char* imageFileName = "images//numberline.gif";//"vert_stripe.bmp";
const char* displayFile = "..//tunnelDisplaySettings.txt";
osgViewer::Viewer viewer;

void printInfo();
osg::ref_ptr<osg::Geode> createShapes();
void setStartingViews();
void setup();
void run();


class keyboardHandler : public osgGA::GUIEventHandler
{
	public:
		virtual bool handle(const osgGA::GUIEventAdapter&, osgGA::GUIActionAdapter&);
};

bool keyboardHandler::handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa)
{
	switch (ea.getEventType())
	{
	case(osgGA::GUIEventAdapter::KEYDOWN) :
	{
		switch (ea.getKey())
		{
		case 'w':
			distance = distance - transInc;
			break;

		case 's':
			distance = distance + transInc;
			break;

		case 'a':
			camHorLoc=camHorLoc-transInc;
			break;

		case 'd':
			camHorLoc = camHorLoc + transInc;;
			break;

		case 'q':
			rotation = rotation - transInc*10.0;
			break;

		case 'e':
			rotation = rotation + transInc*10.0;
			break;


		case ' ':
			rotation = loadedRotation;
			distance = loadedDistance;
			camHorLoc= loadedCamHorLoc;
			break;

		case '.':
			rotation = defaultRotation;
			distance = defaultDistance;
			camHorLoc = defaultCamHorLoc;
			break;

		case '+':
			transInc = transInc*2.0;
			break;

		case '-':
			transInc = transInc / 2.0;
			break;

		case 'p':
			printInfo();
			break;



		default:
			return false;
			break;
		}
		return true;
	}
	default:
		return false;
		break;
	}
}



osg::ref_ptr<osg::Geode> createShapes()
{
	osg::ref_ptr<osg::Geode> geode = new osg::Geode();
	osg::ref_ptr<osg::StateSet> stateset = new osg::StateSet();
	stateset->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
	osg::ref_ptr<osg::Image> image = new osg::Image();
	image = osgDB::readImageFile(imageFileName);

	if (image)
	{
		osg::ref_ptr<osg::Texture2D> texture = new osg::Texture2D(image);
		texture->setWrap(osg::Texture::WRAP_S, osg::Texture::REPEAT);
		osg::ref_ptr<osg::TexMat> texmat = new osg::TexMat;
		stateset->setTextureAttributeAndModes(0, texture, osg::StateAttribute::ON);
		stateset->setTextureAttributeAndModes(0, texmat, osg::StateAttribute::ON);
		texture->setUnRefImageDataAfterApply(true);
	}

	geode->setStateSet(stateset);
	geode->setCullingActive(true);

	osg::ref_ptr<osg::TessellationHints> hints = new osg::TessellationHints;
	hints->setDetailRatio(2.0f);
	hints->setCreateBody(false);
	hints->setCreateBackFace(false);
	hints->setCreateFrontFace(false);
	hints->setCreateNormals(false);
	hints->setCreateTop(false);
	hints->setCreateBottom(true);
	
	 box = new osg::Box(osg::Vec3d(0.0, 0.0, 0.0), boxWidth,boxHeight, 0);
	box->setRotation(osg::Quat(osg::DegreesToRadians(90.0), osg::Vec3d(1,0,0)));
	osg::ref_ptr<osg::ShapeDrawable> rect = new osg::ShapeDrawable(box, hints);
	rect->setUseDisplayList(false);
	geode->addDrawable(rect);

	return geode;
}

void printInfo()
{
	printf("**********\n");
	printf("Distance: %f\n", distance);
	printf("Rotation: %f\n", rotation);
	printf("Hor Loc: %f\n", camHorLoc);
	printf("**********\n\n");
}


void writeInfo()
{
	FILE * file = fopen(displayFile, "w");
	fprintf(file, "%f\n", distance);
	fprintf(file, "%f\n", rotation);
	fprintf(file, "%f\n", camHorLoc);
	fclose(file);
}

void setStartingViews()
{
	std::ifstream file(displayFile, std::ios::in);

	if (file.is_open())
	{
		file >> loadedDistance;
		file >> loadedRotation;
		file >> loadedCamHorLoc;
		distance = loadedDistance;
		rotation = loadedRotation;
		camHorLoc = loadedCamHorLoc;
		file.close();
	}

	else
	{
		distance = defaultDistance;
		rotation = defaultRotation;
		camHorLoc = defaultCamHorLoc;
	}
}


void setup()
{
	osg::ref_ptr<osg::Group> root = new osg::Group;
	root->addChild(createShapes());
	viewer.addEventHandler(new keyboardHandler());
	viewer.setSceneData(root);
	setStartingViews();

	osg::ref_ptr<osg::GraphicsContext::Traits> masterTraits = new osg::GraphicsContext::Traits;
	GLenum buffer = masterTraits->doubleBuffer ? GL_BACK : GL_FRONT;
	viewer.getCamera()->setDrawBuffer(buffer);
	viewer.getCamera()->setReadBuffer(buffer);
	viewer.getCamera()->setClearColor(osg::Vec4(0, 0, 0, 1)); // black background
	viewer.setCameraManipulator(NULL);


	osg::ref_ptr<osg::Camera> leftCam = new osg::Camera;
	osg::ref_ptr<osg::GraphicsContext::Traits> leftTraits = new osg::GraphicsContext::Traits;
	leftTraits->x = xOffset;
	leftTraits->y = yOffset;
	leftTraits->width = viewWidth/2;
	leftTraits->height = viewHeight;
	leftTraits->windowDecoration = false;
	leftTraits->doubleBuffer = true;
	leftTraits->sharedContext = 0;
	osg::ref_ptr<osg::GraphicsContext> leftGC = osg::GraphicsContext::createGraphicsContext(leftTraits.get());
	leftCam->setGraphicsContext(leftGC.get());
	osg::ref_ptr<osg::Viewport> leftVP = new osg::Viewport(0, 0, leftTraits->width, leftTraits->height);
	leftCam->setViewport(leftVP);
	viewer.addSlave(leftCam);




	osg::ref_ptr<osg::Camera> rightCam = new osg::Camera;
	osg::ref_ptr<osg::GraphicsContext::Traits> rightTraits = new osg::GraphicsContext::Traits;
	rightTraits->x = xOffset+viewWidth/2;
	rightTraits->y = yOffset;
	rightTraits->width = viewWidth/2;
	rightTraits->height = viewHeight ;
	rightTraits->windowDecoration = false;
	rightTraits->doubleBuffer = true;
	rightTraits->sharedContext = 0;
	osg::ref_ptr<osg::GraphicsContext> rightGC = osg::GraphicsContext::createGraphicsContext(rightTraits.get());
	rightCam->setGraphicsContext(rightGC.get());
	osg::ref_ptr<osg::Viewport> rightVP = new osg::Viewport(0, 0, rightTraits->width, rightTraits->height);
	rightCam->setViewport(rightVP);
	viewer.addSlave(rightCam);
}

void run()
{

	while (!viewer.done())
	{

		viewer.getCamera()->setProjectionMatrixAsPerspective(40.0, 1280.0 / 800.0 , distance - 50, distance + 50);

		viewer.getSlave(0)._viewOffset = osg::Matrixd::rotate(osg::DegreesToRadians(rotation), osg::Vec3(1, 0, 0))*osg::Matrixd::lookAt(osg::Vec3d(camHorLoc, distance, camVertLoc), osg::Vec3d(camHorLoc, depth, camVertLoc), osg::Vec3d(0, 0, 1));
		viewer.getSlave(1)._viewOffset = osg::Matrixd::rotate(osg::DegreesToRadians(rotation), osg::Vec3(1, 0, 0))*osg::Matrixd::lookAt(osg::Vec3d(-camHorLoc, distance, camVertLoc), osg::Vec3d(-camHorLoc, depth, camVertLoc), osg::Vec3d(0, 0, 1));

	

		//viewer.getSlave(0)._viewOffset = osg::Matrixd::lookAt(osg::Vec3d(camHorLoc, distance, camVertLoc), osg::Vec3d(camHorLoc, depth, camVertLoc), osg::Vec3d(0, 0, 1));
		//viewer.getSlave(0)._projectionOffset = osg::Matrixd::perspective(40.0, 1280.0 / 4800.0, centerDistance - radius, centerDistance - centerCull);


		viewer.frame();
	}

	writeInfo();
}

int _tmain(int argc, _TCHAR* argv[])
{


	//FlyTunnel mov("images", "sequence.txt", "displaySettings.txt", 1920, 1200 , 0);
	//osg::ref_ptr<osgViewer::Viewer> viewer = mov.getViewer();

	/*while (true)
	{
		for (unsigned int i = 0; i < mov.numImages; i++)
		{
			for (unsigned int j = 0; j < mov.sequence[i]; j++)
			{
				mov.imageSequence->seek(((double)i) / ((double)(mov.numImages - 1)));
				viewer->frame();
			}
		}
	}*/

	setup();
	run();
	return 0;
}