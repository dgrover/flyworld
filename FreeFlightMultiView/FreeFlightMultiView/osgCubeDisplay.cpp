
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
#include <osgUtil/Optimizer>
#include <osg/ShapeDrawable>
#include <osg/Material>
#include <osg/Texture2D>
#include <osg/Math>
//#include <osgSim>

double imageWidth=1200;
double imageHeight=800;

double fovy=30;

double boxHorMin=0;
double boxVertMin=0;
double boxHorMax=boxHorMin + imageWidth;
double boxVertMax=boxVertMin + imageHeight;

osg::Vec4 backgroundColor = osg::Vec4(0, 0, 0, 1);

double numBoxes=1;
double depth=0;
double xS=0.0; //speed of camera
double yS=0;
double zS=0.0;
double rS=0; //rotation speed
//double camHorLoc=boxHorMin+imageWidth/2*numBoxes; //location of camera
double camHorLoc=0;
double camVertLoc=0;
double distance=20;
osg::Vec3d up=osg::Vec3d(0,0,1); //up vector

std::string imageFileName="color2.jpg";










class TextureUpdateCallback : public osg::NodeCallback
{
public:
    TextureUpdateCallback(osg::TexMat* texmat,
                       double delay = 0.01) :
        _texmat(texmat),
        _phaseS(35.0f),
        _phaseT(18.0f),
        _phaseScale(5.0f),
        _delay(delay),
        _prevTime(0.0)
    {
    }

    virtual void operator()(osg::Node*, osg::NodeVisitor* nv)
    {
        if (!_texmat)
            return;

        if (nv->getFrameStamp()) {
            double currTime = nv->getFrameStamp()->getSimulationTime();
            if (currTime - _prevTime > _delay) {

                float rad = osg::DegreesToRadians(currTime);

                // zoom scale (0.2 - 1.0)
                float scale = sin(rad * _phaseScale) * 0.4f + 0.6f;
                float scaleR = 1.0f - scale;

                // calculate new texture coordinates
                float s, t;
                s = ((sin(rad * _phaseS) + 1) * 0.5f) * (scaleR);
                t = ((sin(rad * _phaseT) + 1) * 0.5f) * (scaleR);


                _texmat->setMatrix(osg::Matrix::translate(s,t,1.0)*osg::Matrix::scale(scale,scale,1.0));

                // record time
                _prevTime = currTime;
            }
        }
	}
	
private:
    osg::TexMat* _texmat;

    float _phaseS, _phaseT, _phaseScale;

    double _delay;
    double _prevTime;
};












//	setViewMatrixAsLookAt(osg::Vec3d(move,distance,camVertLoc+yS*currTime), osg::Vec3d(move,0.0,camVertLoc+yS*currTime), up);



  /*class rootDataType : public osg::Referenced
    {
   public:
    rootDataType::rootDataType(osg::Node* n)
    {
       rotation = 0;

       findNodeVisitor findCylinder("cylinder"); 
       n->accept(findCylinder);
       cylinderNode = 
		   dynamic_cast <osg::MatrixTransform> (findCylinder.getFirst());
    }
	 
	     void rootDataType::updateCylinderRotation()
    { 
       rotation += 0.01;
       cylinderNode->setCurrentHPR( osg::Vec3(rotation,0,0) );
    }
      
    protected:
       osgSim::Transform* cylinderNode;
       double rotation; // (radians)  
    };*/



  /* class GeodeUpdateCallback : public osg::NodeCallback 
    {
    public:
       virtual void operator()(osg::Node* node, osg::NodeVisitor* nv)
       {
          //osg::ref_ptr<tankDataType> tankData = 
            // dynamic_cast<tankDataType*> (node->getUserData() );
		  osg::Geode* geode = static_cast<osg::Geode*>( node ); 
		  geode->getDrawable(0);
          //root->getChild(0);
          traverse(node, nv); 
       }
    };*/

/*class CylinderUpdateCallback : public osg::Drawable::UpdateCallback 
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
osg::Quat* quat = new osg::Quat(30*currTime, *rotationVec);

osg::ShapeDrawable * geom = dynamic_cast< osg::ShapeDrawable * >( node ) ; 

osg::Shape* cyl = geom->getShape();
((osg::Cylinder*)cyl)->setRotation(*quat);
geom->setShape(cyl);


} 


}; 
*/







osg::Geode* createShapes()
{
    osg::Geode* geode = new osg::Geode();
/*
    // ---------------------------------------
    // Set up a StateSet to texture the objects
    // ---------------------------------------
    osg::StateSet* stateset = new osg::StateSet();
	stateset->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
	osg::Image* image = new osg::Image();
	 image = osgDB::readImageFile(imageFileName);
	
    if (image)
    {
		
	osg::Texture2D* texture = new osg::Texture2D;
	//texture->setTextureSize(5000000, 5000);
	texture->setImage(image);
	
	stateset->setTextureAttributeAndModes(0,texture,osg::StateAttribute::ON);
	
    }
    
    geode->setStateSet( stateset );
	geode->setCullingActive(false);
    
    //float radius = 4.8;//5; //0.8f;
    //float height = 3.8;//3.6; //5.02654824574*8/12;
    

   osg::TessellationHints* hints = new osg::TessellationHints;
    hints->setDetailRatio(0.1f);
	

   hints->setCreateBackFace(true);
hints->setCreateFrontFace(false);
hints->setCreateNormals(false);
hints->setCreateTop(false);
hints->setCreateBottom(false);
hints->setTessellationMode(osg::TessellationHints::TessellationMode::USE_SHAPE_DEFAULTS);

osg::Cylinder* box=new osg::Cylinder(osg::Vec3(0.0,0,0),8,8);

//osg::Box* box = new osg::Box(osg::Vec3(0.0f,0.0f,0.0f),10.0f,10.0f,10.0f);

osg::ShapeDrawable* cube = new osg::ShapeDrawable(box ,hints);
  cube->setUseDisplayList(false);
  //cube->setStateSet( stateset );
//cylinder->setUpdateCallback(new CylinderUpdateCallback());
*/


osg::StateSet* stateset = new osg::StateSet();
	stateset->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
	osg::Image* image = new osg::Image();
	 image = osgDB::readImageFile(imageFileName);
	
    if (image)
    {
	osg::Texture2D* texture = new osg::Texture2D;
	//texture->setTextureSize(5000000, 5000);
	texture->setImage(image);
	stateset->setTextureAttributeAndModes(0,texture,osg::StateAttribute::ON);
    }
    
    











	osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array(10);
	(*vertices)[0].set(-3, -3, -3); //left front bottom
	(*vertices)[1].set(3, -3, -3); //right front bottom
	(*vertices)[2].set(3, -3, 3); //right front top
	(*vertices)[3].set(-3, -3, 3); //left front top
	(*vertices)[4].set(-3, 3, -3); //left back bottom
	(*vertices)[5].set(3, 3, -3); //right back bottom
	(*vertices)[6].set(3, 3, 3); //right back top
	(*vertices)[7].set(-3, 3, 3); //left back top
	(*vertices)[8].set(-3, -3, -3); //left front bottom
	(*vertices)[9].set(-3, -3, 3); //left front top




	osg::ref_ptr<osg::DrawElementsUInt> indices = new osg::DrawElementsUInt(GL_TRIANGLES, 24);
	(*indices) [0] = 0; (*indices) [1] = 1; (*indices) [2] = 2; //front
	(*indices) [3] = 0; (*indices) [4] = 2; (*indices) [5] = 3; //front
	(*indices) [6] = 8; (*indices) [7] = 9; (*indices) [8] = 7; //left
	(*indices) [9] = 8; (*indices) [10] = 7; (*indices) [11] = 4; //left
	(*indices) [12] = 4; (*indices) [13] = 7; (*indices) [14] = 6; //back
	(*indices) [15] = 4; (*indices) [16] = 6; (*indices) [17] = 5; //back
	(*indices) [18] = 5; (*indices) [19] = 6; (*indices) [20] = 2; //right
	(*indices) [21] = 5; (*indices) [22] = 2; (*indices) [23] = 1;  //right
	/*(*indices) [24] = 0; (*indices) [25] = 1; (*indices) [26] = 2;
	(*indices) [27] = 0; (*indices) [28] = 1; (*indices) [29] = 2;
	(*indices) [30] = 0; (*indices) [31] = 1; (*indices) [32] = 2;
	(*indices) [33] = 0; (*indices) [34] = 1; (*indices) [35] = 2;*/

osg::ref_ptr<osg::Geometry> cube = new osg::Geometry;


float texWidth=1;
float texHeight=1;
	osg::Vec2Array* texcoords = new osg::Vec2Array(10);
(*texcoords)[0].set(0.0f, 0.0f);
(*texcoords)[1].set(texWidth/4, 0.0f);
(*texcoords)[2].set(texWidth/4, texHeight);
(*texcoords)[3].set(0.0f, texHeight);
(*texcoords)[4].set(3*texWidth/4, 0.0f);
(*texcoords)[5].set(texWidth/2, 0.0f);
(*texcoords)[6].set(texWidth/2, texHeight);
(*texcoords)[7].set(3*texWidth/4, texHeight);
(*texcoords)[8].set(texWidth, 0);
(*texcoords)[9].set(texWidth, texHeight);
///8///9

cube->setTexCoordArray(0, texcoords /*,osg::Geometry::BIND_OVERALL*/);







	

	cube->setVertexArray(vertices.get());
	cube->addPrimitiveSet(indices.get());

cube->setStateSet( stateset );
	geode->setCullingActive(false);

	//osg::ref_ptr<osg::Geode> geode = new osg::Geode();


    geode->addDrawable(cube.get());
//   geode->setUpdateCallback(new TextureUpdateCallback(texmat));

    return geode;
}

int main( int argc, char **argv )
{
// construct the viewer.
	osgViewer::Viewer viewer;


 // create the model
    osg::Group* root = new osg::Group;
    root->addChild( createShapes() );

   // root->setUpdateCallback(new RootUpdateCallback());
    // add model to viewer.
    viewer.setSceneData( root );
//osg::ref_ptr<osg::Camera> myCam = new osg::Camera;
//myCam->setClearColor(backgroundColor); // black background
	// set dimensions of the view volume
	//myCam->setProjectionMatrixAsPerspective(fovy, 3.0 / 3.0, 0.1, 3000);
	//viewer.setCamera(myCam); // attach camera to the viewer
   
//return viewer.run();
	//viewer.getCamera()->setUpdateCallback(new CameraUpdateCallback());




  int xoffset = 20;//1920;
    int yoffset = 20;
	int width=300;//800;
	int height=600;//1280;

    // front
    {
        osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits;
        traits->x = xoffset + 0;
        traits->y = yoffset + 0;
        traits->width = width;
        traits->height = height;
        traits->windowDecoration = true;
        traits->doubleBuffer = true;
        traits->sharedContext = 0;

        osg::ref_ptr<osg::GraphicsContext> gc = osg::GraphicsContext::createGraphicsContext(traits.get());

        osg::ref_ptr<osg::Camera> camera = new osg::Camera;
        camera->setGraphicsContext(gc.get());
        camera->setViewport(new osg::Viewport(0,0, traits->width, traits->height));
        GLenum buffer = traits->doubleBuffer ? GL_BACK : GL_FRONT;
        camera->setDrawBuffer(buffer);
        camera->setReadBuffer(buffer);
		//camera->setClearColor(backgroundColor); // black background
		//camera->setViewMatrixAsLookAt(osg::Vec3d(camHorLoc,distance,camVertLoc), osg::Vec3d(camHorLoc,depth,camVertLoc), up);
	// set dimensions of the view volume
	//camera->setProjectionMatrixAsPerspective(fovy, 3.0 / 3.0, 0.1, 3000);

        // add this slave camera to the viewer, with a shift left of the projection matrix
       // viewer.addSlave(camera.get(), osg::Matrixd::translate(1.0,0.0,0.0), osg::Matrixd());
		viewer.addSlave(camera.get(), osg::Matrixd(), osg::Matrixd());
    }
    
    // back
    {
        osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits;
        traits->x = xoffset + width*2;
        traits->y = yoffset;
        traits->width = width;
        traits->height = height;
        traits->windowDecoration = true;
        traits->doubleBuffer = true;
        traits->sharedContext = 0;

        osg::ref_ptr<osg::GraphicsContext> gc = osg::GraphicsContext::createGraphicsContext(traits.get());

        osg::ref_ptr<osg::Camera> camera = new osg::Camera;
        camera->setGraphicsContext(gc.get());
        camera->setViewport(new osg::Viewport(0,0, traits->width, traits->height));
        GLenum buffer = traits->doubleBuffer ? GL_BACK : GL_FRONT;
        camera->setDrawBuffer(buffer);
        camera->setReadBuffer(buffer);
			//camera->setClearColor(backgroundColor); // black background
	// set dimensions of the view volume
//	camera->setProjectionMatrixAsPerspective(fovy, 3.0 / 3.0, 0.1, 3000);
//camera->setViewMatrixAsLookAt(osg::Vec3d(camHorLoc,distance,camVertLoc), osg::Vec3d(camHorLoc,depth,camVertLoc), up);
        // add this slave camera to the viewer, with a shift right of the projection matrix
      //  viewer.addSlave(camera.get(), osg::Matrixd::translate(-1.0,0.0,0.0), osg::Matrixd()); //original

osg::Matrixd camRot;
osg::Matrixd camTrans;
camRot.makeRotate(osg::DegreesToRadians(180.0), osg::Vec3(0,1,0));
camTrans.makeTranslate(0.0,0.0,distance*2);

//viewer.addSlave(camera.get(), osg::Matrixd(), osg::Matrixd::translate(0.0,0.0,distance*2));
	viewer.addSlave(camera.get(), osg::Matrixd(),camTrans*camRot);
	//viewer.addSlave(camera.get(), osg::Matrixd(),osg::Matrixd::rotate(osg::DegreesToRadians(180.0), osg::Vec3(0,1,0)).translate(0.0,0.0,distance*2));
    }


//left side
	
 {
        osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits;
        traits->x = xoffset+width*3;
        traits->y = yoffset;
        traits->width = width;
        traits->height = height;
        traits->windowDecoration = true;
        traits->doubleBuffer = true;
        traits->sharedContext = 0;

        osg::ref_ptr<osg::GraphicsContext> gc = osg::GraphicsContext::createGraphicsContext(traits.get());

        osg::ref_ptr<osg::Camera> camera = new osg::Camera;
        camera->setGraphicsContext(gc.get());
        camera->setViewport(new osg::Viewport(0,0, traits->width, traits->height));
        GLenum buffer = traits->doubleBuffer ? GL_BACK : GL_FRONT;
        camera->setDrawBuffer(buffer);
        camera->setReadBuffer(buffer);
			//camera->setClearColor(backgroundColor); // black background
	// set dimensions of the view volume
//	camera->setProjectionMatrixAsPerspective(fovy, 3.0 / 3.0, 0.1, 3000);
//camera->setViewMatrixAsLookAt(osg::Vec3d(camHorLoc,distance,camVertLoc), osg::Vec3d(camHorLoc,depth,camVertLoc), up);
        // add this slave camera to the viewer, with a shift right of the projection matrix
      //  viewer.addSlave(camera.get(), osg::Matrixd::translate(-1.0,0.0,0.0), osg::Matrixd()); //original

osg::Matrixd camRot;
osg::Matrixd camTrans1;
osg::Matrixd camTrans2;
camTrans1.makeTranslate(0.0,0.0,distance);
camRot.makeRotate(osg::DegreesToRadians(90.0), osg::Vec3(0,1,0));
camTrans2.makeTranslate(0.0,0.0,-1*distance);

//viewer.addSlave(camera.get(), osg::Matrixd(), osg::Matrixd::translate(0.0,0.0,distance*2));
	viewer.addSlave(camera.get(), osg::Matrixd(),camTrans1*camRot*camTrans2);
	//viewer.addSlave(camera.get(), osg::Matrixd(),osg::Matrixd::rotate(osg::DegreesToRadians(180.0), osg::Vec3(0,1,0)).translate(0.0,0.0,distance*2));
    }








//right side
	
 {
        osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits;
        traits->x = xoffset+width;
        traits->y = yoffset;
        traits->width = width;
        traits->height = height;
        traits->windowDecoration = true;
        traits->doubleBuffer = true;
        traits->sharedContext = 0;

        osg::ref_ptr<osg::GraphicsContext> gc = osg::GraphicsContext::createGraphicsContext(traits.get());

        osg::ref_ptr<osg::Camera> camera = new osg::Camera;
        camera->setGraphicsContext(gc.get());
        camera->setViewport(new osg::Viewport(0,0, traits->width, traits->height));
        GLenum buffer = traits->doubleBuffer ? GL_BACK : GL_FRONT;
        camera->setDrawBuffer(buffer);
        camera->setReadBuffer(buffer);
			//camera->setClearColor(backgroundColor); // black background
	// set dimensions of the view volume
//	camera->setProjectionMatrixAsPerspective(fovy, 3.0 / 3.0, 0.1, 3000);
//camera->setViewMatrixAsLookAt(osg::Vec3d(camHorLoc,distance,camVertLoc), osg::Vec3d(camHorLoc,depth,camVertLoc), up);
        // add this slave camera to the viewer, with a shift right of the projection matrix
      //  viewer.addSlave(camera.get(), osg::Matrixd::translate(-1.0,0.0,0.0), osg::Matrixd()); //original

osg::Matrixd camRot;
osg::Matrixd camTrans1;
osg::Matrixd camTrans2;
camTrans1.makeTranslate(0.0,0.0,distance);
camRot.makeRotate(osg::DegreesToRadians(-90.0), osg::Vec3(0,1,0));
camTrans2.makeTranslate(0.0,0.0,-1*distance);

//viewer.addSlave(camera.get(), osg::Matrixd(), osg::Matrixd::translate(0.0,0.0,distance*2));
	viewer.addSlave(camera.get(), osg::Matrixd(),camTrans1*camRot*camTrans2);
	//viewer.addSlave(camera.get(), osg::Matrixd(),osg::Matrixd::rotate(osg::DegreesToRadians(180.0), osg::Vec3(0,1,0)).translate(0.0,0.0,distance*2));
    }







viewer.getCamera()->setClearColor(backgroundColor); // black background

viewer.getCamera()->setViewMatrixAsLookAt(osg::Vec3d(camHorLoc,distance,camVertLoc), osg::Vec3d(camHorLoc,depth,camVertLoc), up);


 
	viewer.setCameraManipulator(NULL);

	while(!viewer.done())
	{


		viewer.frame();
	}
	return 0;
}





