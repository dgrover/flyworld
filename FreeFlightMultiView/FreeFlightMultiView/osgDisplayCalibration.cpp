#include <osg/TexMat>
#include <osgDB/ReadFile>
#include <osgViewer/Viewer>
#include <osg/ShapeDrawable>
#include <osg/Texture2D>
#include <osgGA/GUIEventHandler>


double imageWidth=1366;//800;
double imageHeight=768;//1580;
int xoffset = 0;//;1920;
int yoffset = 0;//-150;
int width=imageWidth/4;//800;
int height=imageHeight;//1580;

double camHorLoc=0;
double camVertLoc=0;
double distance=13.5;


int slaveNum=0;



float transInc=0.1;
float rotInc=1;


osg::Matrixd cam1View=osg::Matrixd();

osg::Matrixd cam2View=osg::Matrixd::translate(0.0, 0.0, distance)*osg::Matrixd::rotate(osg::DegreesToRadians(-90.0), osg::Vec3(0,1,0))*osg::Matrixd::translate(0.0, 0.0, -1*distance);
osg::Matrixd cam3View=osg::Matrixd::translate(0.0, 0.0, distance*2)*osg::Matrixd::rotate(osg::DegreesToRadians(180.0), osg::Vec3(0,1,0));
osg::Matrixd cam4View=osg::Matrixd::translate(0.0, 0.0, distance)*osg::Matrixd::rotate(osg::DegreesToRadians(90.0), osg::Vec3(0,1,0))*osg::Matrixd::translate(0.0, 0.0, -1*distance);




osg::Matrixd transZforward = osg::Matrixd::translate(0.0,0.0,transInc);
osg::Matrixd transZbackward = osg::Matrixd::translate(0.0,0.0,transInc*-1);
osg::Matrixd transXleft = osg::Matrixd::translate(transInc,0.0,0.0); //trans cam left
osg::Matrixd transXright = osg::Matrixd::translate(transInc*-1,0.0,0.0);
osg::Matrixd rotcw = osg::Matrixd::rotate(osg::DegreesToRadians(rotInc), osg::Vec3(0,1,0)); //rotate camera clockwise
osg::Matrixd rotccw = osg::Matrixd::rotate(osg::DegreesToRadians(rotInc*-1), osg::Vec3(0,1,0));



osg::Vec4 backgroundColor = osg::Vec4(0, 0, 0, 1);

double diam=3.14159*2;
double depth=0;

double rS= 0.1; //rotation speed

osg::Vec3d up=osg::Vec3d(0,0,1); //up vector

std::string imageFileName="numberline.gif";//"vert_stripe.bmp";



class keyboardHandler : public osgGA::GUIEventHandler
{
public:
	virtual bool handle(const osgGA::GUIEventAdapter&,osgGA::GUIActionAdapter&);
};

bool keyboardHandler::handle(const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter& aa)
{
	switch(ea.getEventType())
	{
	case(osgGA::GUIEventAdapter::KEYDOWN):
		{
			switch(ea.getKey())
			{
			case '0':
				slaveNum=0;
				break;
			case '1':
				slaveNum=1;
				break;
			case '2':
				slaveNum=2;
				break;
			case '3':
				slaveNum=3;
				break;
			case '4':
				slaveNum=4;
				break;

			case 'a':
				if(slaveNum==0 || slaveNum==1)
				{
					cam1View=cam1View*transXleft;
				}
				if(slaveNum==0 || slaveNum==2)
				{
					cam2View=cam2View*transXleft;
				}
				if(slaveNum==0 || slaveNum==3)
				{
					cam3View=cam3View*transXleft;
				}
				if(slaveNum==0 || slaveNum==4)
				{
					cam4View=cam4View*transXleft;
				}
				break;

			case 'd':
				if(slaveNum==0 || slaveNum==1)
				{
					cam1View=cam1View*transXright;
				}
				if(slaveNum==0 || slaveNum==2)
				{
					cam2View=cam2View*transXright;
				}
				if(slaveNum==0 || slaveNum==3)
				{
					cam3View=cam3View*transXright;
				}
				if(slaveNum==0 || slaveNum==4)
				{
					cam4View=cam4View*transXright;
				}

				break;

			case 'w':
				if(slaveNum==0 || slaveNum==1)
				{
					cam1View=cam1View*transZforward;
				}
				if(slaveNum==0 || slaveNum==2)
				{
					cam2View=cam2View*transZforward;
				}
				if(slaveNum==0 || slaveNum==3)
				{
					cam3View=cam3View*transZforward;
				}
				if(slaveNum==0 || slaveNum==4)
				{
					cam4View=cam4View*transZforward;
				}

				break;
			case 's':
				if(slaveNum==0 || slaveNum==1)
				{
					cam1View=cam1View*transZbackward;
				}
				if(slaveNum==0 || slaveNum==2)
				{
					cam2View=cam2View*transZbackward;
				}
				if(slaveNum==0 || slaveNum==3)
				{
					cam3View=cam3View*transZbackward;
				}
				if(slaveNum==0 || slaveNum==4)
				{
					cam4View=cam4View*transZbackward;
				}
				break;

			case 'q':
				if(slaveNum==0 || slaveNum==1)
				{
					cam1View=cam1View*rotccw;
				}
				if(slaveNum==0 || slaveNum==2)
				{
					cam2View=cam2View*rotccw;
				}
				if(slaveNum==0 || slaveNum==3)
				{
					cam3View=cam3View*rotccw;
				}
				if(slaveNum==0 || slaveNum==4)
				{
					cam4View=cam4View*rotccw;
				}
				break;


			case 'e':
				if(slaveNum==0 || slaveNum==1)
				{
					cam1View=cam1View*rotcw;
				}
				if(slaveNum==0 || slaveNum==2)
				{
					cam2View=cam2View*rotcw;
				}
				if(slaveNum==0 || slaveNum==3)
				{
					cam3View=cam3View*rotcw;
				}
				if(slaveNum==0 || slaveNum==4)
				{
					cam4View=cam4View*rotcw;
				}
				break;

			default:
				return false;
			} 
			return true;
		}
	default:
		return false;
	}
}


















class GeodeUpdateCallback : public osg::NodeCallback 
{ 
public : 

	void operator()( osg::Node * node , osg::NodeVisitor * nv) 
	{ 
		double currTime;
		if (nv->getFrameStamp())
		{
			currTime= nv->getFrameStamp()->getSimulationTime();
		}
		osg::Vec3f* rotationVec=new osg::Vec3(0,0,1);
		osg::Quat* quat = new osg::Quat(rS*diam*currTime, *rotationVec);


		osg::Geode * geode = dynamic_cast< osg::Geode * >( node ); 
		osg::Drawable* drawable = geode->getDrawable(0);
		//osg::ShapeDrawable * geom = dynamic_cast< osg::ShapeDrawable * >( node ) ; 

		osg::Shape* cyl = ((osg::ShapeDrawable*) drawable)->getShape();
		((osg::Cylinder*)cyl)->setRotation(*quat);
		((osg::ShapeDrawable*) drawable)->setShape(cyl);


	} 
}; 





class TextureUpdateCallback : public osg::NodeCallback
{
private:
	osg::TexMat* texMat;
public:
	TextureUpdateCallback(osg::TexMat* tm)
	{
		texMat=tm;
	}

	virtual void operator()(osg::Node*, osg::NodeVisitor* nv)
	{
		if (!texMat)
		{
			return;
		}

		if (nv->getFrameStamp())
		{
			double currTime = nv->getFrameStamp()->getSimulationTime();
			float s=currTime*rS;
			texMat->setMatrix(osg::Matrix::translate(s,0.0,0.0));
		}
	}
};







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
		stateset->setTextureAttributeAndModes(0,texture,osg::StateAttribute::ON);
		stateset->setTextureAttributeAndModes(0, texmat, osg::StateAttribute::ON);
		texture->setUnRefImageDataAfterApply(true);
	}

	geode->setStateSet( stateset );
	geode->setCullingActive(false);

	float radius = 3;//5; //0.8f;
	float height = 3.8;//3.6; //5.02654824574*8/12;

	osg::TessellationHints* hints = new osg::TessellationHints;
	hints->setDetailRatio(0.8f);


	hints->setCreateBackFace(true);
	hints->setCreateFrontFace(false);
	hints->setCreateNormals(false);
	hints->setCreateTop(false);
	hints->setCreateBottom(false);
	osg::Vec3f* rotationVec=new osg::Vec3(0,0,1);
	osg::Quat* quat = new osg::Quat(30, *rotationVec);
	osg::Cylinder* cyl = new osg::Cylinder(osg::Vec3(0.0f,0.0f,0.0f),radius,height);
	cyl->setRotation(*quat);
	osg::ShapeDrawable* cylinder = new osg::ShapeDrawable(cyl ,hints);
	cylinder->setUseDisplayList(false);
	geode->addDrawable(cylinder);

	//    geode->setUpdateCallback(new GeodeUpdateCallback());

	//osg::TexMat* texmat = (osg::TexMat*)(stateset->getTextureAttribute(0,osg::StateAttribute::TEXMAT));	
	//geode->setUpdateCallback(new TextureUpdateCallback(texmat));
	return geode;
}

int main( int argc, char **argv )
{
	osgViewer::Viewer viewer;


	osg::Group* root = new osg::Group;
	root->addChild( createShapes() );


	viewer.setSceneData( root );



	// front
	{
		osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits;
		traits->x = xoffset + 0;
		traits->y = yoffset + 0;
		traits->width = width;
		traits->height = height;
		traits->windowDecoration = false;
		traits->doubleBuffer = true;
		traits->sharedContext = 0;

		osg::ref_ptr<osg::GraphicsContext> gc = osg::GraphicsContext::createGraphicsContext(traits.get());

		osg::ref_ptr<osg::Camera> camera = new osg::Camera;
		camera->setGraphicsContext(gc.get());
		camera->setViewport(new osg::Viewport(0,0, traits->width, traits->height));
		GLenum buffer = traits->doubleBuffer ? GL_BACK : GL_FRONT;
		camera->setDrawBuffer(buffer);
		camera->setReadBuffer(buffer);
		viewer.addSlave(camera.get(), osg::Matrixd(), osg::Matrixd());
	}



	//right side

	{
		osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits;
		traits->x = xoffset+width;
		traits->y = yoffset;
		traits->width = width;
		traits->height = height;
		traits->windowDecoration = false;
		traits->doubleBuffer = true;
		traits->sharedContext = 0;

		osg::ref_ptr<osg::GraphicsContext> gc = osg::GraphicsContext::createGraphicsContext(traits.get());

		osg::ref_ptr<osg::Camera> camera = new osg::Camera;
		camera->setGraphicsContext(gc.get());
		camera->setViewport(new osg::Viewport(0,0, traits->width, traits->height));
		GLenum buffer = traits->doubleBuffer ? GL_BACK : GL_FRONT;
		camera->setDrawBuffer(buffer);
		camera->setReadBuffer(buffer);
		viewer.addSlave(camera.get(), osg::Matrixd(),cam2View);	
	}

	// back
	{
		osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits;
		traits->x = xoffset + width*2;
		traits->y = yoffset + 0;
		traits->width = width;
		traits->height = height;
		traits->windowDecoration = false;
		traits->doubleBuffer = true;
		traits->sharedContext = 0;

		osg::ref_ptr<osg::GraphicsContext> gc = osg::GraphicsContext::createGraphicsContext(traits.get());

		osg::ref_ptr<osg::Camera> camera = new osg::Camera;
		camera->setGraphicsContext(gc.get());
		camera->setViewport(new osg::Viewport(0,0, traits->width, traits->height));
		GLenum buffer = traits->doubleBuffer ? GL_BACK : GL_FRONT;
		camera->setDrawBuffer(buffer);
		camera->setReadBuffer(buffer);
		viewer.addSlave(camera.get(), osg::Matrixd(),cam3View);
	}


	//left side

	{
		osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits;
		traits->x = xoffset+width*3;
		traits->y = yoffset;
		traits->width = width;
		traits->height = height;
		traits->windowDecoration = false;
		traits->doubleBuffer = true;
		traits->sharedContext = 0;

		osg::ref_ptr<osg::GraphicsContext> gc = osg::GraphicsContext::createGraphicsContext(traits.get());

		osg::ref_ptr<osg::Camera> camera = new osg::Camera;
		camera->setGraphicsContext(gc.get());
		camera->setViewport(new osg::Viewport(0,0, traits->width, traits->height));
		GLenum buffer = traits->doubleBuffer ? GL_BACK : GL_FRONT;
		camera->setDrawBuffer(buffer);
		camera->setReadBuffer(buffer);

		viewer.addSlave(camera.get(), osg::Matrixd(),cam4View);
	}











	keyboardHandler* handler = new keyboardHandler();

	viewer.addEventHandler(handler); 


	viewer.getCamera()->setClearColor(backgroundColor); // black background

	viewer.getCamera()->setViewMatrixAsLookAt(osg::Vec3d(camHorLoc,distance,camVertLoc), osg::Vec3d(camHorLoc,depth,camVertLoc), up);



	viewer.setCameraManipulator(NULL);

	while(!viewer.done())
	{
		viewer.getSlave(0)._viewOffset=cam1View;
		viewer.getSlave(1)._viewOffset=cam2View;
		viewer.getSlave(2)._viewOffset=cam3View;
		viewer.getSlave(3)._viewOffset=cam4View;

		viewer.frame();
	}

	
	osg::Quat::value_type  angle=* new osg::Quat::value_type();
	for(int i=0; i<4; i++)
	{
	printf("Camera %i X Position: %f\n", i+1, viewer.getSlave(i)._viewOffset.getTrans().x());
	printf("Camera %i Z Position: %f\n", i+1, viewer.getSlave(i)._viewOffset.getTrans().z());
viewer.getSlave(i)._viewOffset.getRotate().getRotate(angle, * new osg::Quat::value_type(),* new osg::Quat::value_type(),* new osg::Quat::value_type());
printf("Camera %i Rotation: %f\n", i+1, osg::RadiansToDegrees(angle));
	}


	return 0;
}