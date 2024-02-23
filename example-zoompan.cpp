#include<dslab.h>
#include<iostream>
using namespace std;

enum {
	MENU_ZOOMFIT=500,
	MENU_ZOOMIN,
	MENU_ZOOMOUT
};

class MyDataEngine :public DataEngine
{
	private:
	double theta[3];
	dsOrthoZoomPan zoompan;
	public:
	
	virtual void Init()
	{
		cout << "Initialization" << endl;
		zoompan.setMBR(0,10,0,10);
		glEnable(GL_DEPTH_TEST);
		/*wxToolBar *toolbar;
		toolbar = new wxToolBar(getFrame(),-1);

				toolbar->AddTool(MENU_ZOOMIN,wxT("Zoom In"),wxBitmap(&xpm_viewmagp));
		toolbar->AddTool(MENU_ZOOMOUT,wxT("Zoom Out"),wxBitmap( &xpm_viewmagm));
		toolbar->AddTool(MENU_ZOOMFIT,wxT("Zoom Fit"),wxBitmap(&xpm_viewmag1));
		
		
		toolbar->AddSeparator();
		toolbar->AddTool(wxID_ABOUT,wxT("About"),wxBitmap((&xpm_mdsg)));
		toolbar->Realize();
		getFrame()->SetToolBar(toolbar);
		*/
		
		
		
	}
	virtual void think(double iElapsed)
	{
		theta[0] += 0.1*3.1415926*(iElapsed / 1E12);
		theta[1] += 0.06*3.1415926*(iElapsed / 1E12);
	}
	virtual void createMenu(wxMenu *menuDataEngine)
	{
		// start of from 500
		menuDataEngine->Append(500,wxT("First"),wxT("Help"));
		menuDataEngine->Append(501,wxT("Second"),wxT("Help2"));
	}
	virtual void extendViewMenu(wxMenu *menuDataEngine)
	{
		// start of from 500
		menuDataEngine->Append(502,wxT("First View"),wxT("Help"));
		menuDataEngine->Append(503,wxT("Second View"),wxT("Help2"));
		DS_start_rendering();
	}
		
		
		virtual void handleMenu(int id)
		{
			switch (id)
			{
				case MENU_ZOOMFIT:
					zoompan.zoomFit(); break;
				case MENU_ZOOMIN:
					zoompan.zoom(1); break;
				case MENU_ZOOMOUT:
					zoompan.zoom(-1); break;
				default:
					cout << "Menu Item " << id << " ignored" << endl;
					break;
			}
		}

		virtual void beforeQuit(){
			// Do some data cleanup / file sync / close
			cout << "Preparing DataEngine for exit" << endl;
		};
		
				
		virtual void render(size_t width, size_t height)
		{
		  
		  zoompan.setViewport(0,0,width,height);
		  zoompan.glViewport();
		  
		  glMatrixMode( GL_PROJECTION );
		  glLoadIdentity( );
		  zoompan.glProject();

			glMatrixMode( GL_MODELVIEW );
			glClearColor( 0.0, 0.0, 0.0, 0.0 );
			glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ); 

   		glColor3f( 1.0, 0.0, 0.0 );

		glBegin(GL_POLYGON);
			glColor3f(   1.0,  1.0, 0 );
			glVertex2f(  1,1  );
			glVertex2f(  8,1 );
			glVertex2f( 8,8  );
		glEnd();
	
					
		}
		
		virtual void mouseClick(size_t x, size_t y)
		{
		}
		
		virtual void mouseDragging(size_t from_x,size_t from_y, size_t to_x,size_t to_y)
		{
			auto from = zoompan.clientProject(from_x,from_y);
			auto to = zoompan.clientProject(to_x,to_y);
			zoompan.x -=  (to.first - from.first);
			zoompan.y -=  (to.second - from.second);
		}
		virtual void mouseWheel(size_t x, size_t y, int steps) {
			zoompan.fixZoom(x,y,steps);
		};
};

/*Instantiate it and make it available to the library*/
MyDataEngine eng;
DataEngine *getDataEngineImplementation()
{
   return (DataEngine*) &eng;
}

