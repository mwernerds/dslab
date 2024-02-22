#include "dslab.h"
#include<stdlib.h>
#include<stdint.h>



#if defined(_WIN32)
#include <chrono>

int gettimeofday(struct timeval* tp, struct timezone* tzp) {
  namespace sc = std::chrono;
  sc::system_clock::duration d = sc::system_clock::now().time_since_epoch();
  sc::seconds s = sc::duration_cast<sc::seconds>(d);
  tp->tv_sec = s.count();
  tp->tv_usec = sc::duration_cast<sc::microseconds>(d - s).count();

  return 0;
}
#else
#include <sys/time.h>
#include<unistd.h>
#endif // _WIN32


BasicGLPane *glPane;
BasicGLPane *getView() {return glPane;};

DataEngine *pDataEngine;
void registerDataEngine(DataEngine *pData)
{
	pDataEngine = pData;
}
DataEngine *getEngine() {return  pDataEngine;};




class MyApp: public wxApp
{
 
	
    virtual bool OnInit();
    uint64_t lastthink;
 
    bool render_loop_on;
public:
    wxFrame *frame;

	wxTextCtrl *textBox;
    
    wxStreamToTextRedirector *redirect=NULL;
	void activateRenderLoop(bool on)
	{
		if(on && !render_loop_on)
		{
			Connect( wxID_ANY, wxEVT_IDLE, wxIdleEventHandler(MyApp::onIdle) );
			render_loop_on = true;
		}
		else if(!on && render_loop_on)
		{
			Disconnect( wxEVT_IDLE, wxIdleEventHandler(MyApp::onIdle) );
			render_loop_on = false;
		}
	}
		uint64_t _ticks(void)
		{	
			struct timeval tv;
			gettimeofday(&tv, 0);
			return uint64_t( tv.tv_sec ) * 1000 + tv.tv_usec / 1000;
		}

	void activateRedirection(bool which)
	{
		if (!which)
		{
			if(redirect != NULL)
				delete redirect;
		    redirect = NULL;
		}
	}
	
	void onIdle(wxIdleEvent& evt)
	{

		getEngine()->think(_ticks() - lastthink);
		if(render_loop_on)
		{
			getView()->Refresh();
			evt.RequestMore(); // render continuously, not only once on idle
		}
		lastthink = _ticks();
	}
};


IMPLEMENT_APP(MyApp);




class myFrame :public wxFrame
{
	public:
	void menuHandler(wxCommandEvent& event )
	{
		
		switch(event.GetId())
		{
			case wxID_EXIT:
				getEngine()->beforeQuit();
				Close(true);
				break;
			case ID_ACTIVATE_LOOP:
				wxGetApp().activateRenderLoop(true);
				break;
			case ID_DEACTIVATE_LOOP:
				wxGetApp().activateRenderLoop(false);
				break;
			case ID_RENDER:
				getView()->Refresh();
				break;
				
			case wxID_ABOUT:
				 wxMessageBox( wxT("This is a pervasive data science demonstration tool written by <martin.werner@ifi.lmu.de> to showcase research results.\n\nFind and download it at https://github.com/mwernerds/dslab"), wxT("About"), wxOK | wxICON_INFORMATION );
				break;
			default:
				try{
				getEngine()->handleMenu(event.GetId());
				}catch(const std::exception& e)
			{
				std::string s = e.what();
				wxString mystring(s.c_str(), wxConvUTF8);
				wxMessageBox(mystring,_("Standard Exception"));
			}
			catch(...)
			{
				wxMessageBox(_("Unhandled and Unknown Exception (not std::exception derivative) in user code segment."),_("Fatal Error"));
			}
		}
		
	}

	myFrame(const wxString& title,
    const wxPoint& pos, const wxSize& size)
      : wxFrame((wxFrame *) NULL, -1, title, pos, size)
	{
		
		Connect(wxID_EXIT, wxID_EXIT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(myFrame::menuHandler));
		Connect(wxID_ABOUT, wxID_ABOUT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(myFrame::menuHandler));
		Connect(MIN_Menu, MAX_Menu, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(myFrame::menuHandler));
		Connect(ID_CORE_MENU_MIN, ID_CORE_MENU_MAX, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(myFrame::menuHandler));
	}
	
		
};

#define TEXT_ID wxID_HIGHEST + 1
 
bool MyApp::OnInit()
{
	// first register the foreign class
	registerDataEngine(getDataEngineImplementation());

#if defined(DSLAB_USE_WINDOWS_CONSOLE)
	RedirectIOToConsole();
#endif
	 //wxMessageBox( wxT("This is a pervasive data science demonstration tool written by <martin.werner@ifi.lmu.de> to showcase research results.\n\n (https://github.com/mwernerds/dslab), wxT("About"), wxOK | wxICON_INFORMATION );
 		
    wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
    wxString s(getDataEngineImplementation()->getTitle().c_str(),wxConvUTF8);
    frame = new myFrame(  s, wxPoint(50,50), wxSize(400,200));
 
    int args[] = {WX_GL_RGBA, WX_GL_DOUBLEBUFFER, WX_GL_DEPTH_SIZE, 16, WX_GL_STENCIL_SIZE, 8, 0};
 
    glPane = new BasicGLPane( (wxFrame*) frame, args);
    sizer->Add(glPane, 5, wxEXPAND);
    
    textBox = new wxTextCtrl( (wxFrame*) frame, TEXT_ID, _(""), wxDefaultPosition, wxDefaultSize,  wxTE_MULTILINE | wxTE_READONLY | wxTE_RICH2 , wxDefaultValidator, wxTextCtrlNameStr);
    
    sizer->Add(textBox,1, wxEXPAND);
    
    
    
	 /*The menu*/
	wxMenu *menuFile = new wxMenu;
	//menuFile->Append(500, wxT("&Hello...\tCtrl-H"), wxT("Help string shown in status bar for this menu item"));
	//	menuFile->AppendSeparator();
		menuFile->Append(wxID_EXIT);
	
		wxMenu *menuRendering = new wxMenu();
		menuRendering->Append(ID_ACTIVATE_LOOP,wxT("&Loop Renderer"),wxT("Activates Loop Rendering. This is the default"));
		menuRendering->Append(ID_DEACTIVATE_LOOP,wxT("&Stop Renderer"),wxT("Deactivates Loop Rendering. This frees up resources, if rendering is exceptionally hard"));
		menuRendering->Append(ID_RENDER,wxT("&Render View"),wxT("Render Single View"));
		menuRendering->AppendSeparator();
		getEngine()->extendViewMenu(menuRendering);
	
		wxMenu *menuDataEngine = new wxMenu();
		getEngine()->createMenu(menuDataEngine);
		
	
		wxMenu *menuHelp = new wxMenu;
		menuHelp->Append(wxID_ABOUT);
			
		wxMenuBar *menuBar = new wxMenuBar;
		menuBar->Append( menuFile, wxT("&File" ));
		menuBar->Append( menuRendering, wxT("&Rendering" ));
		menuBar->Append( menuDataEngine, wxT("&Data Engine" ));
		menuBar->Append( menuHelp, wxT("&Help" ));
		frame->SetMenuBar( menuBar );
		frame->CreateStatusBar();
		frame->SetStatusText(wxT( "Welcome to DSLAB <see www.martinwerner.de for more information>!" ));
		// Start off with the default toolbar. I expect this to be overwritten in many cases

		frame->Maximize(true);
 
    frame->SetSizer(sizer);
    frame->SetAutoLayout(true);
 
    frame->Show();
#ifndef DISABLE_GUI_CONSOLE    
    redirect = new wxStreamToTextRedirector(textBox);
#endif
    
   /* if (wxGetApp().argc >1)
      if(wxStrcmp(wxGetApp().argv[1],wxT("--console")) != 0)
		console = true;
	if (!console) 
		redirect = new wxStreamToTextRedirector(textBox);	*/
	//RedirectIOToConsole();
	
//    activateRenderLoop(true);
	getEngine()->Init();
    return true;
} 
 
BEGIN_EVENT_TABLE(BasicGLPane, wxGLCanvas)
EVT_MOTION(BasicGLPane::mouseMoved)
EVT_LEFT_DOWN(BasicGLPane::mouseDown)
EVT_LEFT_UP(BasicGLPane::mouseReleased)
EVT_RIGHT_DOWN(BasicGLPane::rightClick)
EVT_LEAVE_WINDOW(BasicGLPane::mouseLeftWindow)
EVT_SIZE(BasicGLPane::resized)
EVT_CHAR_HOOK(BasicGLPane::keyPressed) // was keydown, now works
EVT_KEY_UP(BasicGLPane::keyReleased)
EVT_MOUSEWHEEL(BasicGLPane::mouseWheelMoved)
EVT_PAINT(BasicGLPane::render)
END_EVENT_TABLE()
 
 
// some useful events to use
void BasicGLPane::mouseMoved(wxMouseEvent& event)
{
   if (event.Dragging())
   {
	   dragging = true;
	   getEngine()->mouseDragging(last_known_mouse_x,last_known_mouse_y,event.GetX(), event.GetY());
   }else{
	   getEngine()->mouseMoved(event.GetX(), event.GetY());
	}
    // BOOK-KEEPING
   last_known_mouse_x=event.GetX();
   last_known_mouse_y=event.GetY();
   this->Refresh();
	
	
}
void BasicGLPane::mouseDown(wxMouseEvent& event) {
	   
	}
void BasicGLPane::mouseWheelMoved(wxMouseEvent& event) {
	getEngine()->mouseWheel(event.GetX(),event.GetY(), event.GetWheelRotation() / event.GetWheelDelta());
	}
void BasicGLPane::mouseReleased(wxMouseEvent& event) {
	if (dragging)
	{
		dragging = false;
		return;
	}
	getEngine()->mouseClick(event.GetX(), event.GetY());
	}
void BasicGLPane::rightClick(wxMouseEvent& event) {
	if (dragging)
	{
		dragging = false;
		return;
	}
	getEngine()->rightClick(event.GetX(), event.GetY());
}
void BasicGLPane::mouseLeftWindow(wxMouseEvent& event) {}
void BasicGLPane::keyPressed(wxKeyEvent& event) {
    	getEngine()->key_down(event.GetKeyCode(),event.ControlDown(),event.ShiftDown(),event.AltDown());
}
void BasicGLPane::keyReleased(wxKeyEvent& event) {
	getEngine()->key(event.GetKeyCode(),event.ControlDown(),event.ShiftDown(),event.AltDown());
	
}
 
// Vertices and faces of a simple cube to demonstrate 3D render
// source: http://www.opengl.org/resources/code/samples/glut_examples/examples/cube.c
GLfloat v[8][3];
GLint faces[6][4] = {  /* Vertex indices for the 6 faces of a cube. */
    {0, 1, 2, 3}, {3, 2, 6, 7}, {7, 6, 5, 4},
    {4, 5, 1, 0}, {5, 6, 2, 1}, {7, 4, 0, 3} };
 
 
 
BasicGLPane::BasicGLPane(wxFrame* parent, int* args) :
    wxGLCanvas(parent, wxID_ANY, args, wxDefaultPosition, wxDefaultSize, wxFULL_REPAINT_ON_RESIZE)
{
	m_context = new wxGLContext(this);
	dragging = false;


    SetBackgroundStyle(wxBG_STYLE_CUSTOM);
}
 
BasicGLPane::~BasicGLPane()
{
	delete m_context;
}
 
void BasicGLPane::resized(wxSizeEvent& evt)
{
//    wxGLCanvas::OnSize(evt);
    wxSize size = evt.GetSize();
    
    getEngine()->resize(size.GetWidth(), size.GetHeight());
    Refresh();
}
 
/** Inits the OpenGL viewport for drawing in 3D. */
void BasicGLPane::prepare3DViewport(int topleft_x, int topleft_y, int bottomrigth_x, int bottomrigth_y)
{
 
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Black Background
    glClearDepth(1.0f);	// Depth Buffer Setup
    glEnable(GL_DEPTH_TEST); // Enables Depth Testing
    glDepthFunc(GL_LEQUAL); // The Type Of Depth Testing To Do
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
 
    glEnable(GL_COLOR_MATERIAL);
 
    glViewport(topleft_x, topleft_y, bottomrigth_x-topleft_x, bottomrigth_y-topleft_y);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
 
    float ratio_w_h = (float)(bottomrigth_x-topleft_x)/(float)(bottomrigth_y-topleft_y);
    gluPerspective(45 /*view angle*/, ratio_w_h, 0.1 /*clip close*/, 200 /*clip far*/);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
 
}
 
/** Inits the OpenGL viewport for drawing in 2D. */
void BasicGLPane::prepare2DViewport(int topleft_x, int topleft_y, int bottomrigth_x, int bottomrigth_y)
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Black Background
    glEnable(GL_TEXTURE_2D);   // textures
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
 
    glViewport(topleft_x, topleft_y, bottomrigth_x-topleft_x, bottomrigth_y-topleft_y);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
 
    gluOrtho2D(topleft_x, bottomrigth_x, bottomrigth_y, topleft_y);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}
 
int BasicGLPane::getWidth()
{
    return GetSize().x;
}
 
int BasicGLPane::getHeight()
{
    return GetSize().y;
}
 
 
void BasicGLPane::render( wxPaintEvent& evt )
{

    if(!IsShown()) return;
 
    wxGLCanvas::SetCurrent(*m_context);
    wxPaintDC(this); // only to be used in paint events. use wxClientDC to paint outside the paint event
 
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	getEngine()->render(getWidth(),getHeight());
 
    glFlush();
    SwapBuffers();

}


void BasicGLPane::writeScreenshot(std::string filename)
{
	wxInitAllImageHandlers();  
	
	//	wxPaintEvent evt;
	glDrawBuffer(GL_BACK);
	//render(evt);
	Refresh(); // not sure it works.
	glReadBuffer(GL_BACK);
	GLvoid *imageData = malloc(getWidth()*getHeight()*3);		// 3 byte for RGB
	if (imageData == NULL)
	{
		fprintf(stderr,"Memory allocation failed for screenshot\n");
		return;
    }
    glReadPixels(0,0,getWidth(),getHeight(),GL_RGB,GL_UNSIGNED_BYTE,imageData);

	wxImage img(getWidth(),getHeight(), true);
	img.SetData((unsigned char *) imageData); // This should make wxWidgets responsible for the allocation
	wxString wxFilename(filename.c_str(), wxConvUTF8);;
	img.SaveFile(wxFilename,wxBITMAP_TYPE_PNG );
}


void DS_stop_redirection()
{
	wxGetApp().activateRedirection(false);
}


void DS_stop_rendering()
{
	wxGetApp().activateRenderLoop(false);
}

void DS_start_rendering()
{
	wxGetApp().activateRenderLoop(true);
}

void DS_render_once()
{
	
	getView()->Refresh();
}

void DS_screenshot(std::string filename)
{
	getView()->writeScreenshot(filename);
}



wxFrame *getFrame() {return wxGetApp().frame;};
