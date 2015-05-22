
#include "stdafx.h"
#include "tunnelcalib.h"
#include "FlyTunnel.h"

int xOffset = 0;
int yOffset = 0;

double viewWidth = 1920;
double viewHeight =  1200;

double defaultDistance = (10.0);
double distance = defaultDistance;


osg::ref_ptr<osg::Box> box;
double defaultRotation = 0;
double rotation = defaultRotation;
double loadedRotation = defaultRotation;
double loadedDistance = defaultDistance;


double camHorLoc = 0;
double camVertLoc = 0;
double transInc = 0.1;
double depth = 0;



osg::Vec4 backgroundColor = osg::Vec4(0, 0, 0, 1);
osg::Vec3d up = osg::Vec3d(0, 0, 1); //up vector
const char* imageFileName = "images//numberline.gif";//"vert_stripe.bmp";
const char* displayFile = "displaySettings.txt";
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

		case 'q':
			rotation = rotation - transInc*10.0;
			break;

		case 'e':
			rotation = rotation + transInc*10.0;
			break;


		case ' ':
			rotation = loadedRotation;
			distance = loadedDistance;
			break;

		case '.':
			rotation = defaultRotation;
			distance = defaultDistance;
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
	geode->setCullingActive(false);

	osg::ref_ptr<osg::TessellationHints> hints = new osg::TessellationHints;
	hints->setDetailRatio(2.0f);
	hints->setCreateBody(false);
	hints->setCreateBackFace(false);
	hints->setCreateFrontFace(false);
	hints->setCreateNormals(false);
	hints->setCreateTop(false);
	hints->setCreateBottom(true);
	
	 box = new osg::Box(osg::Vec3d(0.0, 0.0, 0.0), 3*viewWidth/viewHeight,3*viewHeight/viewWidth, 0);
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
	printf("**********\n\n");
}


void writeInfo()
{
	FILE * file = fopen(displayFile, "w");
	fprintf(file, "%f\n", distance);
	fprintf(file, "%f\n", rotation);
	fclose(file);
}

void setStartingViews()
{
	std::ifstream file(displayFile, std::ios::in);

	if (file.is_open())
	{
		file >> loadedDistance;
		file >> loadedRotation;
		distance = loadedDistance;
		rotation = loadedRotation;
		file.close();
	}

	else
	{
		distance = defaultDistance;
		rotation = defaultRotation;
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
	masterTraits->x = xOffset;
	masterTraits->y = yOffset;
	masterTraits->width = viewWidth;
	masterTraits->height = viewHeight;
	masterTraits->windowDecoration = false;
	masterTraits->doubleBuffer = true;
	masterTraits->sharedContext = 0;
	GLenum buffer = masterTraits->doubleBuffer ? GL_BACK : GL_FRONT;
	viewer.getCamera()->setDrawBuffer(buffer);
	viewer.getCamera()->setReadBuffer(buffer);
	viewer.getCamera()->setClearColor(osg::Vec4(0, 0, 0, 1)); // black background
	viewer.setCameraManipulator(NULL);
	osg::ref_ptr<osg::GraphicsContext> centerGC = osg::GraphicsContext::createGraphicsContext(masterTraits.get());
	viewer.getCamera()->setGraphicsContext(centerGC.get());
	osg::ref_ptr<osg::Viewport> centerVP = new osg::Viewport(0, 0, masterTraits->width, masterTraits->height);
	viewer.getCamera()->setViewport(centerVP);
}

void run()
{

	while (!viewer.done())
	{
		viewer.getCamera()->setViewMatrix(osg::Matrixd::rotate(osg::DegreesToRadians(rotation), osg::Vec3(1, 0, 0))*osg::Matrixd::lookAt(osg::Vec3d(camHorLoc, distance, camVertLoc), osg::Vec3d(camHorLoc, depth, camVertLoc), osg::Vec3d(0, 0, 1)));
		viewer.getCamera()->setProjectionMatrixAsPerspective(40.0, 1920.0/1200.0, distance - 50, distance+50);
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