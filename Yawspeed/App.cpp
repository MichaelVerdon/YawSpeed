#include "App.h"
#include "MainFrame.h"
#include <wx/wx.h>

wxIMPLEMENT_APP(App); //Generate main function

//Entry point of app
bool App::OnInit() {
	MainFrame* mainFrame = new MainFrame("YawSpeed");
	//Resize window
	mainFrame->SetClientSize(500, 300);
	mainFrame->Center();
	mainFrame->Show();
	return true; //Tells wxwidgets processing continues, false is closes
}
