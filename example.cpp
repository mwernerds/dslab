#include<dslab.h>
#include<iostream>
using namespace std;

class MyDataEngine :public DataEngine
{
	private:
	double theta[3];
	public:
	
	virtual void Init()
	{
		cout << "Initialization" << endl;
		glEnable(GL_DEPTH_TEST);
		
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
			cout << "Menu " << id << endl;
			wxMessageBox( wxT("Hello there..."), wxT("About"), wxOK | wxICON_INFORMATION );
		}

		virtual void beforeQuit(){
			// Do some data cleanup / file sync / close
			cout << "Preparing DataEngine for exit" << endl;
		};
		
				
		virtual void render(size_t width, size_t height)
		{
		  glViewport( 0, 0, width, height );
		  glMatrixMode( GL_PROJECTION );
		  glLoadIdentity( );

			if ( width <= height ) {
				glOrtho( -2.0, 2.0, -2.0 * (GLfloat) height / (GLfloat) width,
				2.0 * (GLfloat) height / (GLfloat) width, -10.0, 10.0 );
			} else {
				glOrtho( -2.0 * (GLfloat) width / (GLfloat) height,
				2.0 * (GLfloat) width / (GLfloat) height, -2.0, 2.0, -10.0, 10.0 );
			}

			glMatrixMode( GL_MODELVIEW );
			GLdouble 	 size = 0.75;

			glClearColor( 0.0, 0.0, 0.0, 0.0 );

			glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ); 

			glRotatef( theta[0], 1.0, 0.0, 0.0 );
			glRotatef( theta[1], 0.0, 1.0, 0.0 );
			glRotatef( theta[2], 0.0, 0.0, 1.0 );

   		glColor3f( 1.0, 0.0, 0.0 );

		glBegin(GL_POLYGON);
			glColor3f(   1.0,  1.0, 1.0 );
			glVertex3f(  0.5, -0.5, 0.5 );
			glVertex3f(  0.5,  0.5, 0.5 );
			glVertex3f( -0.5,  0.5, 0.5 );
			glVertex3f( -0.5, -0.5, 0.5 );
		glEnd();
	
		glBegin(GL_POLYGON);
			glColor3f(  1.0,  0.0,  1.0 );
			glVertex3f( 0.5, -0.5, -0.5 );
			glVertex3f( 0.5,  0.5, -0.5 );
			glVertex3f( 0.5,  0.5,  0.5 );
			glVertex3f( 0.5, -0.5,  0.5 );
		glEnd();
 

		glBegin(GL_POLYGON);
			glColor3f(   0.0,  1.0,  0.0 );
			glVertex3f( -0.5, -0.5,  0.5 );
			glVertex3f( -0.5,  0.5,  0.5 );
			glVertex3f( -0.5,  0.5, -0.5 );
			glVertex3f( -0.5, -0.5, -0.5 );
		glEnd();
 

		glBegin(GL_POLYGON);
			glColor3f(   0.0,  0.0,  1.0 );
			glVertex3f(  0.5,  0.5,  0.5 );
			glVertex3f(  0.5,  0.5, -0.5 );
			glVertex3f( -0.5,  0.5, -0.5 );
			glVertex3f( -0.5,  0.5,  0.5 );
		glEnd();
 
		glBegin(GL_POLYGON);
			glColor3f(   1.0,  0.0,  0.0 );
			glVertex3f(  0.5, -0.5, -0.5 );
			glVertex3f(  0.5, -0.5,  0.5 );
			glVertex3f( -0.5, -0.5,  0.5 );
			glVertex3f( -0.5, -0.5, -0.5 );
		glEnd();
					
		}
		
		virtual void mouseClick(size_t x, size_t y)
		{
			cout << "Click @" << x <<","<< y << endl;
		}
};

/*Instantiate it and make it available to the library*/
MyDataEngine eng;
DataEngine *getDataEngineImplementation()
{
   return (DataEngine*) &eng;
}

