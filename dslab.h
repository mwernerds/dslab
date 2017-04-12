#ifndef _glpane_
#define _glpane_

#define DEBUG_OUTPUT 1
#define USE_OMP 1
#define VISUAL_STREAMS 0

/*Image ressources*/
extern const char *xpm_mdsg;
extern const char *xpm_viewmagp;
extern const char *xpm_viewmagm;
extern const char *xpm_viewmag1;

#include "wx/wx.h"
#include "wx/glcanvas.h"
#include "wx/intl.h"
#include "wx/event.h"
#include "wx/progdlg.h"

#if defined(__WXMSW__)
	#include "guicon.h"
#endif

#if USE_OPENMP
#include<omp.h>
#endif

 #include<cmath>
 
// include OpenGL
#ifdef __WXMAC__
#include "OpenGL/glu.h"
#include "OpenGL/gl.h"
#else
#include <GL/glu.h>
#include <GL/gl.h>
//#include <GL/glut.h>
#endif
#include<string>

#define DATA_ENGINE_MENU 500
using namespace std; 
/*Helper Class for 2D Ortho*/
 
#define ORTHO_ZOOM_DEFAULT_SCALE 0.9
 class dsOrthoZoomPan
 {
	 double mbr[4];
	 double near, far;
	 double scale;
	 int zoomlevel;
     size_t viewport[4];	 
	 public:
	 double x,y;
	 
	 dsOrthoZoomPan()
	 {
		 x = y  = zoomlevel = 0;
		 scale = ORTHO_ZOOM_DEFAULT_SCALE;
		 near = -10; far = 10;
	 }
	 void   setMBR(double l,double r, double t, double b){
			mbr[0] = l;
			mbr[1] = r;
			mbr[2] = t;
			mbr[3] = b;
		}
	void setPos(double x, double y)
	{
		this->x = x;
		this->y = y;
	}

	double animate_source_zoomlevel;
	double animate_source_x;
	double animate_source_y;
	double animate_target_zoomlevel;
	double animate_target_x;
	double animate_target_y;
	double animate_time_remaining;
	double animate_duration;
	
	void animate(double iElapsed)
	{
		// progress animation if active
		if (animate_time_remaining > 0)
		{
			animate_time_remaining -= iElapsed;
			if (animate_time_remaining < 0) 
				animate_time_remaining = 0;
			double tau = animate_time_remaining / animate_duration; 
			x = tau * animate_source_x + (1-tau) * animate_target_x;
			y = tau * animate_source_y + (1-tau) * animate_target_y;
			zoomlevel = tau * animate_source_zoomlevel + (1-tau) * animate_target_zoomlevel;
		}
	}
	

	void zoomFitAnimated(double duration=500)
	{
		animate_source_zoomlevel = zoomlevel;
		animate_source_x = x;
		animate_source_y = y;
		animate_target_zoomlevel = 0;
		animate_target_x = (mbr[1]+mbr[0]) / 2;
		animate_target_y = (mbr[3]+mbr[2]) / 2;
		animate_time_remaining = duration;
		animate_duration = duration; // default 1/2 second
	}

	void zoomFit()
	{
		zoomlevel = 0;
		x = (mbr[1]+mbr[0]) / 2;
		y = (mbr[3]+mbr[2]) / 2;
	}
		
	void setViewport(size_t left, size_t top, size_t width, size_t height)
	{
		viewport[0] = left;
		viewport[1] = top;
		viewport[2] = width;
		viewport[3] = height;
	}
	
	void setClipping (double _near, double _far)
	{
		near = _near;
		far = _far;
	};
	
	void fixZoom(size_t x, size_t y, size_t amount)
	{
		
		std::pair<double,double> fixedpoint = clientProject(x,y);
		zoomlevel += amount;
		std::pair<double,double> newpoint = clientProject(x,y);
		this->x -= (newpoint.first - fixedpoint.first);
		this->y -= (newpoint.second - fixedpoint.second);
	}
	
	void zoom(int amount)
	{
		zoomlevel += amount;
	}
	
	void glViewport()
	{
		::glViewport( viewport[0],viewport[1],viewport[2],viewport[3] );
	}
	
	void glProject()
	{
		// first move, then scale
		//double l,r,t,b;
		
		// calculate based on x y 
		double view_width = mbr[1]-mbr[0];
		double view_height = mbr[3]-mbr[2];
		// add scale
		double thescale = pow(scale, zoomlevel);
		view_width *= thescale;
		view_height *= thescale;
		// project
		glOrtho(x-view_width / 2, x+ view_width/2,
				y-view_height/2, y+view_height/2,
				near,far);	
		//glOrtho(mbr[2],mbr[3],mbr[0],mbr[1],-10,10); // y x rendering
		
	}

	
	
	std::pair<double,double> clientProject(double vx, double vy)
	{
		double xrel = (vx - viewport[0]) / viewport[2];
		double yrel = (vy - viewport[1]) / viewport[3];
		
		double view_width = mbr[1]-mbr[0];
		double view_height = mbr[3]-mbr[2];
	//	cout << xrel << "/" << yrel << endl;
		// add scale
		double thescale = pow(scale, zoomlevel);
		view_width *= thescale;
		view_height *= thescale;
		
		return std::pair<double,double>(
				x + ((xrel-0.5)  * view_width),
				y - ((yrel-0.5)  * view_height));
				//mbr[0] + xrel*(mbr[1]-mbr[0]),
				//mbr[2]+ (1-yrel)*(mbr[3]-mbr[2]));
		//return std::pair<double,double>(mbr[0] + xrel*(mbr[1]-mbr[0]),mbr[2]+ (1-yrel)*(mbr[3]-mbr[2]));
	}
};
 
 
class DataEngine
{
	protected:
		
	public:
		virtual std::string getTitle(){return std::string("DSLAB-based Data Science Demonstrator");};
		virtual void Init(){};
		virtual void Update(){};
		virtual void stopRendering(){};
		virtual void think(double iElapsed){};
		virtual void createMenu(wxMenu *menuDataEngine){};
		virtual void extendViewMenu(wxMenu *menuDataEngine){};		
		virtual void handleMenu(int id){};
		virtual void beforeQuit(){};
		virtual void render(size_t width, size_t height){};
		//virtual void moseMoved(size_t x, size_t y)
		virtual void mouseClick(size_t x, size_t y) {};
		virtual void rightClick(size_t x, size_t y) {};
		virtual void mouseDragging(size_t from_x,size_t from_y, size_t to_x,size_t to_y){};
		virtual void mouseMoved(size_t x,size_t y){};
		virtual void mouseWheel(size_t x, size_t y, int steps) {};
		virtual void key(int keycode, bool ctrl, bool shift, bool alt){};
		
		
};

 
class BasicGLPane : public wxGLCanvas
{
    wxGLContext*	m_context;
    size_t last_known_mouse_x,last_known_mouse_y; // for dragging
    bool dragging;
    
public:
	BasicGLPane(wxFrame* parent, int* args);
	virtual ~BasicGLPane();
 
	void resized(wxSizeEvent& evt);
 
	int getWidth();
	int getHeight();
 
	void render(wxPaintEvent& evt);
	void prepare3DViewport(int topleft_x, int topleft_y, int bottomrigth_x, int bottomrigth_y);
	void prepare2DViewport(int topleft_x, int topleft_y, int bottomrigth_x, int bottomrigth_y);
 
	// events
	void mouseMoved(wxMouseEvent& event);
	void mouseDown(wxMouseEvent& event);
	void mouseWheelMoved(wxMouseEvent& event);
	void mouseReleased(wxMouseEvent& event);
	void rightClick(wxMouseEvent& event);
	void mouseLeftWindow(wxMouseEvent& event);
	void keyPressed(wxKeyEvent& event);
	void keyReleased(wxKeyEvent& event);
		
		
	void writeScreenshot(std::string filename);
 
	DECLARE_EVENT_TABLE()
};



#define ID_CORE_MENU_MIN 100
#define ID_ACTIVATE_LOOP 101
#define ID_DEACTIVATE_LOOP 102
#define ID_RENDER 103

#define ID_CORE_MENU_MAX 200


#define MIN_Menu 500
#define MAX_Menu 700

extern BasicGLPane *glPane;

extern DataEngine *gEngine;

void DS_stop_redirection();
void DS_start_rendering();
void DS_stop_rendering();
void DS_render_once();
void DS_screenshot(std::string filename);

void registerDataEngine(DataEngine *pData);
DataEngine *getDataEngineImplementation();



/*#define IMPLEMENT_DATAENGINE(CustomDataEngine) \
CustomDataEngine de;\
 DataEngine *getDataEngineImplementation()\
 {\
 	return (DataEngine *) &de;\
 }
*/
wxFrame *getFrame();


#endif


