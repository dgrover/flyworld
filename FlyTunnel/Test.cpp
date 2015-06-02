#include "stdafx.h"
#include "FlyTunnel.h"
int _tmain(int argc, _TCHAR* argv[])
{


	FlyTunnel mov("images", "sequence.txt", "..//tunnelDisplaySettings.txt", 1280*2, 800, 1920);
	osg::ref_ptr<osgViewer::Viewer> viewer = mov.getViewer();

	while (true)
	{
		for (unsigned int i = 0; i < mov.numImages; i++)
		{
			for (unsigned int j = 0; j < mov.sequence[i]; j++)
			{
				mov.imageSequence->seek(((double)i) / ((double)(mov.numImages - 1)));
				viewer->frame();
			}
		}

		if (GetAsyncKeyState(VK_ESCAPE))
			break;
	}


	return 0;
}