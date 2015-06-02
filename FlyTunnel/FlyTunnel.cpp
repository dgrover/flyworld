#include "stdafx.h"
#include "FlyTunnel.h"

static osgDB::DirectoryContents getImages(std::string directory)
{
	osgDB::DirectoryContents images;

	if (osgDB::fileType(directory) == osgDB::DIRECTORY)
	{
		osgDB::DirectoryContents dc = osgDB::getSortedDirectoryContents(directory);

		for (osgDB::DirectoryContents::iterator itr = dc.begin(); itr != dc.end(); ++itr)
		{
			std::string filename = directory + "/" + (*itr);
			std::string ext = osgDB::getLowerCaseFileExtension(filename);
			if ((ext == "jpg") || (ext == "png") || (ext == "gif") || (ext == "rgb") || (ext == "bmp"))
			{
				images.push_back(filename);

			}
		}
	}

	else
	{
		images.push_back(directory);
	}

	return images;
}

osg::ref_ptr<osg::Geode> FlyTunnel::createShapes()
{
	osg::ref_ptr<osg::Geode> geode = new osg::Geode();
	osg::ref_ptr<osg::StateSet> stateset = new osg::StateSet();
	stateset->setMode(GL_LIGHTING, osg::StateAttribute::OFF);


	osgDB::DirectoryContents images = getImages(imageFileName);
	imageSequence = new osg::ImageSequence;
	imageSequence->setMode(osg::ImageSequence::Mode::PRE_LOAD_ALL_IMAGES);

	if (!images.empty())
	{
		for (osgDB::DirectoryContents::iterator itr = images.begin(); itr != images.end(); ++itr)
		{
			const std::string& filename = *itr;
			osg::ref_ptr<osg::Image> image = osgDB::readImageFile(filename);

			if (image.valid())
			{
				imageSequence->addImage(image.get());
			}
		}

		numImages = imageSequence->getNumImageData();
	}

	if (imageSequence)
	{
		osg::ref_ptr<osg::Texture2D> texture = new osg::Texture2D(imageSequence.get());
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

	osg::ref_ptr<osg::Box> box = new osg::Box(osg::Vec3d(0.0, 0.0, 0.0), boxWidth, boxHeight, 0);
	box->setRotation(osg::Quat(osg::DegreesToRadians(90.0), osg::Vec3d(1, 0, 0)));
	osg::ref_ptr<osg::ShapeDrawable> rect = new osg::ShapeDrawable(box, hints);
	rect->setUseDisplayList(false);
	geode->addDrawable(rect);

	return geode;
}




void FlyTunnel::setView()
{
	std::ifstream file(displayFile, std::ios::in);


	if (file.is_open())
	{
		file >> distance;
		file >> rotation;
		file >> camHorLoc;
		file.close();
	}
}

void FlyTunnel::setSequence()
{
	double number;
	std::ifstream file(sequenceFile, std::ios::in);

	if (file.is_open())
	{
		while (file >> number)
			sequence.push_back(number);

		file.close();
	}
	else
	{
		for (unsigned int i = 0; i < numImages; i++)
			sequence.push_back(1);
	}
}

void FlyTunnel::setup()
{
	osg::ref_ptr<osg::Group> root = new osg::Group;
	root->addChild(createShapes());	
	setSequence();
	viewer->setSceneData(root);
	setView();

	osg::ref_ptr<osg::GraphicsContext::Traits> masterTraits = new osg::GraphicsContext::Traits;
	GLenum buffer = masterTraits->doubleBuffer ? GL_BACK : GL_FRONT;
	viewer->getCamera()->setDrawBuffer(buffer);
	viewer->getCamera()->setReadBuffer(buffer);
	viewer->getCamera()->setClearColor(osg::Vec4(0, 0, 0, 1)); // black background
	viewer->setCameraManipulator(NULL);

	osg::ref_ptr<osg::Camera> leftCam = new osg::Camera;
	osg::ref_ptr<osg::GraphicsContext::Traits> leftTraits = new osg::GraphicsContext::Traits;
	leftTraits->x = xOffset;
	leftTraits->y = yOffset;
	leftTraits->width = viewWidth / 2;
	leftTraits->height = viewHeight;
	leftTraits->windowDecoration = false;
	leftTraits->doubleBuffer = true;
	leftTraits->sharedContext = 0;
	osg::ref_ptr<osg::GraphicsContext> leftGC = osg::GraphicsContext::createGraphicsContext(leftTraits.get());
	leftCam->setGraphicsContext(leftGC.get());
	osg::ref_ptr<osg::Viewport> leftVP = new osg::Viewport(0, 0, leftTraits->width, leftTraits->height);
	leftCam->setViewport(leftVP);
	viewer->addSlave(leftCam);

	osg::ref_ptr<osg::Camera> rightCam = new osg::Camera;
	osg::ref_ptr<osg::GraphicsContext::Traits> rightTraits = new osg::GraphicsContext::Traits;
	rightTraits->x = xOffset + viewWidth / 2;
	rightTraits->y = yOffset;
	rightTraits->width = viewWidth / 2;
	rightTraits->height = viewHeight;
	rightTraits->windowDecoration = false;
	rightTraits->doubleBuffer = true;
	rightTraits->sharedContext = 0;
	osg::ref_ptr<osg::GraphicsContext> rightGC = osg::GraphicsContext::createGraphicsContext(rightTraits.get());
	rightCam->setGraphicsContext(rightGC.get());
	osg::ref_ptr<osg::Viewport> rightVP = new osg::Viewport(0, 0, rightTraits->width, rightTraits->height);
	rightCam->setViewport(rightVP);
	viewer->addSlave(rightCam);

	viewer->getCamera()->setProjectionMatrixAsPerspective(40.0, 1280.0 / 800.0, distance - 50, distance + 50);
	viewer->getSlave(0)._viewOffset = osg::Matrixd::rotate(osg::DegreesToRadians(rotation), osg::Vec3(1, 0, 0))*osg::Matrixd::lookAt(osg::Vec3d(camHorLoc, distance, camVertLoc), osg::Vec3d(camHorLoc, depth, camVertLoc), osg::Vec3d(0, 0, 1));
	viewer->getSlave(1)._viewOffset = osg::Matrixd::rotate(osg::DegreesToRadians(rotation), osg::Vec3(1, 0, 0))*osg::Matrixd::lookAt(osg::Vec3d(-camHorLoc, distance, camVertLoc), osg::Vec3d(-camHorLoc, depth, camVertLoc), osg::Vec3d(0, 0, 1));
}

osg::ref_ptr<osgViewer::Viewer> FlyTunnel::getViewer()
{
	return viewer;
}