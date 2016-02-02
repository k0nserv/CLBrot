#include "App.h"

LRESULT CALLBACK WndProc (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    static App* app = 0;

    switch (message) {
        case WM_CREATE:
        {
            // Get the 'this' pointer we passed to CreateWindow via the lpParam parameter.
            CREATESTRUCT* cs = (CREATESTRUCT*) lParam;
            app = (App*) cs->lpCreateParams;
            return 0;
        }
    }

    // Don't start processing messages until after WM_CREATE.
    if (app)
        return app->msgProc(message, wParam, lParam);
    else
        return DefWindowProc(hWnd, message, wParam, lParam);
}

/* Class definitions */

/* Public */
App::App (HINSTANCE i, uint w, uint h) {
	hInstance = i;
	fullscreen = false;
    lowIteration = false;
    width = w;
	height = h;

	init();
}

App::~App () {
	delete console;
	delete settingsDirectory;
}

int App::run () {
    MSG msg = {0};

    while (msg.message != WM_QUIT) {
        // If there are Window messages then process them.
        if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
            // Otherwise, do animation
        else {
            render();
        }
    }
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(hRC);
    DestroyWindow(mainWnd);
    return (int) msg.wParam;
}

/* Private */


/* Rendering */
void App::render () {
	if ( engine == NULL )
		return;

	Color* colors;
	colors = engine->getPixels();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	glBegin(GL_POINTS);
	for(uint i = 0; i < width*height; i++){
		int x = i%width;
		int y = i/width;
		Color current = colors[i];

		glColor3ub(current.r, current.g, current.b);
		glVertex2i(x,y);
		}
	glEnd();	


	glEnable(GL_BLEND);
	console->draw(width, height);
	glDisable(GL_BLEND);
	// Swap Buffers (Double Buffering)
	SwapBuffers(hDC);
}

/* Input detection and response */
void App::keyDown (WPARAM wParam, short int repeat) {
	/* Retive max and min coordiantes */
	Complex max, min;
	settings->getMaxAndMin(max, min);


    Complex newMax = max;
    Complex newMin = min;
	double zoomFactor = settings->getZoomFactor();
    
	POINT p;
    double zRe = repeat * zoomFactor * 1 * (min.re - max.re);
    double zIm = repeat * zoomFactor * 1 * (min.im - max.im);


    //get cursor pos and translate to window coordinates
    GetCursorPos(&p);
    ScreenToClient(mainWnd, &p);

    switch (wParam) {
        case VK_LEFT:
            newMax.re += zRe;
            newMin.re += zRe;
            break;
        case VK_RIGHT:
            newMax.re -= zRe;
            newMin.re -= zRe;
            break;
        case VK_UP:
			if(console->isVisible())
				console->keyDown(wParam);
			else{
				newMax.im += zIm;
				newMin.im += zIm;
			}
            break;

		case VK_DOWN:
			if(console->isVisible())
				console->keyDown(wParam);
			else{
				newMax.im -= zIm;
				newMin.im -= zIm;
			}
			break;
            
        case VK_PRIOR:
            if(console->isVisible())
                console->keyDown(wParam);
            break;

        case VK_NEXT:
            if(console->isVisible())
                console->keyDown(wParam);
            break;

        //Zoom around point
		case VK_F1:
			GetCursorPos(&p);
			zoomAroundMouse(120, p.x, p.y);
            break;

		case VK_F2:
			console->toggleVisible();
			break;

        case VK_F11:
            fullscreen = !fullscreen;
            DWORD w;
            DWORD h;
            if (fullscreen) {
				previousWidth = width;
				previousHeight = height;
                w = GetSystemMetrics(SM_CXSCREEN);
                h = GetSystemMetrics(SM_CYSCREEN);
            }
            else {
				w = previousWidth;
                h = previousHeight;
            }
            changeSize(w, h);
            return;

        case VK_F12:
            captureScreen();
			break;
    }

	if ( newMax.re != max.re || newMax.im != max.im || 
		newMin.re != min.re || newMin.im != min.im ) {
			Mandelbrot m = settings->getMandelBrot();
			m.max = newMax;
			m.min = newMin;
			settings->setMandelbrot(m);
	}

}

void App::charInput(WPARAM wParam) {
	/* send input to console */
	if(console->isVisible())
		console->keyDown(wParam);

}

LRESULT App::msgProc (UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_ACTIVATE:
            return 0;

        // WM_SIZE is sent when the user resizes the window.  
        case WM_SIZE:
            // Save the new client area dimensions.
            width = LOWORD(lParam);
            height = HIWORD(lParam);
            resize(width, height);
            return 0;

        case WM_MOUSEWHEEL:
            zoomAroundMouse(GET_WHEEL_DELTA_WPARAM(wParam), LOWORD(lParam), HIWORD(lParam));
            return 0;

		case WM_MBUTTONDOWN:
			zoomAroundMouse(LOWORD(wParam), GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			return 0;

        case WM_KEYDOWN:
            keyDown(wParam, (short) lParam);
            return 0;
	
		case WM_CHAR:
			charInput(wParam);
			return 0;

        // WM_DESTROY is sent when the window is being destroyed.
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;

        // Catch this message so to prevent the window from becoming too small.
        case WM_GETMINMAXINFO:
            ((MINMAXINFO*) lParam)->ptMinTrackSize.x = 200;
            ((MINMAXINFO*) lParam)->ptMinTrackSize.y = 200;
            return 0;
    }

    return DefWindowProc(mainWnd, msg, wParam, lParam);
}

void App::zoomAroundMouse (int dz, int x, int y) {
	POINT p;
	p.x = x;
	p.y = y;
	double xC, yC;

	ScreenToClient(mainWnd, &p);
	settings->pixelsToCoordinates(p.x, p.y, xC, yC);

	engine->zoomAroundPoint(dz, xC, yC);
}


/* Size updates */
void App::resize (uint w, uint h) {
	/* Retive max and min coordiantes */
	Complex max, min;
	Mandelbrot m = settings->getMandelBrot();
	settings->getMaxAndMin(max, min);

    width = w;
	height = h;

	m.height = h;
	m.width  = w;
	settings->setMandelbrot(m);
	settings->calculateCoordinateScale();

    glMatrixMode(GL_PROJECTION);
    glViewport(0, 0, width, height); // Reset The Current Viewport
    glLoadIdentity();
    glOrtho(0, width, height, 0, -1, 1);
    glDisable(GL_DEPTH);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();	

	console->resize(w,h);
}

void App::changeSize (uint w, uint h) {
    long style = !fullscreen ? (FULLSCREEN_STYLE) : WS_OVERLAPPEDWINDOW;
    LONG lStyle = GetWindowLong(mainWnd, GWL_STYLE);
    lStyle &= ~style;
    SetWindowLong(mainWnd, GWL_STYLE, lStyle);

    GetWindowLong(mainWnd, GWL_STYLE);
    lStyle |= (fullscreen ? (FULLSCREEN_STYLE) : WS_OVERLAPPEDWINDOW);
    SetWindowLong(mainWnd, GWL_STYLE, lStyle);
    /*
    bool result = SetWindowPos(mainWnd,HWND_TOPMOST,ScreenToClient(mainWnd,0),ScreenToClient(mainWnd,0),w,h,
                                                       SWP_FRAMECHANGED | SWP_NOMOVE);
     */
    MoveWindow(mainWnd, 0, 0, w, h, true);
    UpdateWindow(mainWnd);
    resize(w, h);
}


/* Take a screenshot and save it in the programs folder under %APPDATA%*/
void App::captureScreen () {
    CImage c = CImage();
	std::string filename;
	std::stringstream ss;
	HRESULT result;
	Color* colors = engine->getPixels();

    //Create 8 bit image with resulotion width*height
    c.Create(width, height, 24);

    //Build the image
    for (uint i = 0; i < width * height; i++) {
        int x = i % width;
        int y = i / width;
        Color color = colors[i];
        if (color.r != 0 && color.g != 0 && color.b != 0) {
            color;
        }

        COLORREF colorRef = RGB(color.r, color.g, color.b);

        c.SetPixel(x, y, colorRef);
    }

    /* Save the image */
	//Build the filename
	ss << SettingsDirectory::getScreenshotPrefix() << "-" << settingsDirectory->getScreenshotIndex() << ".png";
	filename = ss.str();
	/* Clear the stringstream, both calls required */
	ss.clear();
	ss.str("");
	ss << settingsDirectory->getScreenshotPath() << "\\" << filename;
	result = c.Save(ss.str().c_str(), Gdiplus::ImageFormatPNG);
	settingsDirectory->incrementScreenshotIndex();
	console->log("Screen captured");
}


/* Init functions */
void App::init(){
	settings = Settings::getInstance();
	console = new Console();
	initWindow();
	initGL();

	
	settingsDirectory = new SettingsDirectory(mainWnd);
    settings->setSettingsDirectory(settingsDirectory);
	string kernelPath = settingsDirectory->getRootPath().append("\\").append(KERNEL_SRC);
	engine = new Engine(kernelPath);
	console->log(engine->getBuildResult());
	
}

void App::initGL () {
	glEnable(GL_BLEND); //Enable alpha blending
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Black Background		
    glMatrixMode(GL_PROJECTION);
    glViewport(0, 0, width, height); // Reset The Current Viewport
    glLoadIdentity();
    glOrtho(0, width, height, 0, -1, 1);
    glEnable( GL_TEXTURE_2D );
	glDisable(GL_COLOR_MATERIAL);
	glDisable(GL_DEPTH);
    glMatrixMode(GL_MODELVIEW);

    //glPushMatrix ();
    glLoadIdentity();
	/* Init Fonts after opengl is done initializing */
	console->setFont(new Font("tahoma.ttf", 16));
	console->log("CLBrot by Hugo Tunius ( hugotunius.se )!");
    std::stringstream ss;
    ss << "Number of colors: " << settings->g->numberOfColors();
    console->log(ss.str());
    #ifdef _DEBUG
        console->log("Debug build");
    #endif
}

void App::initWindow () {
    TCHAR szWindowClass[] = _T("win32app");
    TCHAR szTitle[] = _T("CLBrot");
    GLuint PixelFormat; // Holds The Results After Searching For A Match

    WNDCLASSEX wcex;

    wcex.cbSize = sizeof (WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH) (COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL;
    wcex.lpszMenuName = 0;
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));

    if (!RegisterClassEx(&wcex)) {
        MessageBox(NULL,
                   _T("Call to RegisterClassEx failed!"),
                   _T("Error"),
                   NULL);
        return;
    }

    mainWnd = CreateWindow(
                           szWindowClass,
                           szTitle,
                           WS_OVERLAPPEDWINDOW,
                           CW_USEDEFAULT, CW_USEDEFAULT,
                           width, height,
                           NULL,
                           NULL,
                           hInstance,
                           this
                           );

    if (!mainWnd) {
        MessageBox(NULL,
                   _T("Call to CreateWindow failed!"),
                   _T("Win32 Guided Tour"),
                   NULL);

        return;
    }
    static PIXELFORMATDESCRIPTOR pfd = // pfd Tells Windows How We Want Things To Be
    {
        sizeof (PIXELFORMATDESCRIPTOR), // Size Of This Pixel Format Descriptor
        1, // Version Number
        PFD_DRAW_TO_WINDOW | // Format Must Support Window
        PFD_SUPPORT_OPENGL | // Format Must Support OpenGL
        PFD_DOUBLEBUFFER, // Must Support Double Buffering
        PFD_TYPE_RGBA, // Request An RGBA Format
        16, // Select Our Color Depth
        0, 0, 0, 0, 0, 0, // Color Bits Ignored
        1, // No Alpha Buffer
        0, // Shift Bit Ignored
        0, // No Accumulation Buffer
        0, 0, 0, 0, // Accumulation Bits Ignored
        16, // 16Bit Z-Buffer (Depth Buffer)  
        0, // No Stencil Buffer
        0, // No Auxiliary Buffer
        PFD_MAIN_PLANE, // Main Drawing Layer
        0, // Reserved
        0, 0, 0 // Layer Masks Ignored
    };

    hDC = GetDC(mainWnd);
    PixelFormat = ChoosePixelFormat(hDC, &pfd);
    SetPixelFormat(hDC, PixelFormat, &pfd);
    wglMakeCurrent(hDC, hRC);
    hRC = wglCreateContext(hDC);
    wglMakeCurrent(hDC, hRC);
    UpdateWindow(mainWnd);
    resize(width, height);
    ShowWindow(mainWnd,
               SW_SHOWNORMAL);
}