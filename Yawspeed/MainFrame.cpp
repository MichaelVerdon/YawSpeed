#include "MainFrame.h"
#include <wx/wx.h>
#include <string>
#include "Profile.h"
#include "Spinner.h"

MainFrame::MainFrame(const wxString& title) : wxFrame(nullptr, wxID_ANY, title) { //Null pointer as it is top level window
	// In windows gui application we use a winMain func instead of int Main.
	createGUI();
	bindEventHandlers();
	initCfg();

}

void MainFrame::createGUI() {

	const std::string bindDefault = "Press Any Key";
	const std::string unbound = "Unbound";

	panel = new wxPanel(this);

	yawSpeedText = new wxStaticText(panel, wxID_ANY, "Set Yaw Speed", wxPoint(105, 25));
	yawSpeedText->SetBackgroundColour(*wxLIGHT_GREY);

	slider = new wxSlider(panel, wxID_ANY, 50, 0, 100, wxPoint(50, 50), wxSize(200, -1), wxSL_VALUE_LABEL);

	bindsText = new wxStaticText(panel, wxID_ANY, "Binds", wxPoint(135, 125));
	bindsText->SetBackgroundColour(*wxLIGHT_GREY);

	leftCtrl = new wxTextCtrl(panel, wxID_ANY, unbound, wxPoint(50, 150), wxSize(100, -1), wxTE_READONLY); // -1 = default
	bindLeft = new wxButton(panel, wxID_ANY, "Bind Left", wxPoint(150, 150), wxSize(100, -1), wxBU_EXACTFIT);

	rightCtrl = new wxTextCtrl(panel, wxID_ANY, unbound, wxPoint(50, 180), wxSize(100, -1), wxTE_READONLY);
	bindRight = new wxButton(panel, wxID_ANY, "Bind Right", wxPoint(150, 180), wxSize(100, -1), wxBU_EXACTFIT);

	saveButton = new wxButton(panel, wxID_ANY, "Save Settings", wxPoint(100, 250), wxSize(100, -1), wxBU_EXACTFIT);

	startButton = new wxButton(panel, wxID_ANY, "Start", wxPoint(350, 50), wxSize(100, -1), wxBU_EXACTFIT);

	stopButton = new wxButton(panel, wxID_ANY, "Stop", wxPoint(350, 90), wxSize(100, -1), wxBU_EXACTFIT);

	// evt.skip() for handling multiple events together in each method

	CreateStatusBar();
	wxLogStatus("Not Running");

}

void MainFrame::bindEventHandlers() {

	Bind(wxEVT_CHAR_HOOK, &MainFrame::onKeyBindPress, this);
	slider->Bind(wxEVT_SLIDER, &MainFrame::onSliderChanged, this);
	startButton->Bind(wxEVT_BUTTON, &MainFrame::startClicked, this);	
	stopButton->Bind(wxEVT_BUTTON, &MainFrame::stopClicked, this);
	bindLeft->Bind(wxEVT_BUTTON, &MainFrame::onLeftBind, this);
	bindRight->Bind(wxEVT_BUTTON, &MainFrame::onRightBind, this);
	saveButton->Bind(wxEVT_BUTTON, &MainFrame::onSaveClicked, this);

}

// Disable and enable buttons based on binding keys
void MainFrame::toggleButtons(bool buttonsEnabled) {
	if (!buttonsEnabled) {

		slider->Disable();
		startButton->Disable();
		stopButton->Disable();
		bindLeft->Disable();
		bindRight->Disable();
		saveButton->Disable();

	}
	else {

		slider->Enable();
		startButton->Enable();
		stopButton->Enable();
		bindLeft->Enable();
		bindRight->Enable();
		saveButton->Enable();

	}
}

// Disable and enable buttons when starting the program.
void MainFrame::toggleButtonsOnInit(bool running) {
	if (running) {

		slider->Disable();
		startButton->Disable();
		bindLeft->Disable();
		bindRight->Disable();
		saveButton->Disable();

	}
	else {

		slider->Enable();
		startButton->Enable();
		bindLeft->Enable();
		bindRight->Enable();
		saveButton->Enable();

	}
}

void MainFrame::initCfg() {

	loadFile(profile);
	slider->SetValue(profile.yawSpeed);
	leftCtrl->SetValue(profile.leftBind);
	rightCtrl->SetValue(profile.rightBind);

}

void MainFrame::startClicked(wxCommandEvent& evt) {

	//Connect(wxEVT_CHAR_HOOK, wxKeyEventHandler(MainFrame::running), nullptr, this);
	isRunning = true;

	toggleButtonsOnInit(true);
	spinner.initSpinner(profile);
	spinner.spinnerOn = true;
	spinner.keyListener();

	wxLogStatus("Running");

	//evt.Skip();

}

void MainFrame::stopClicked(wxCommandEvent& evt) {

	isRunning = false;
	spinner.spinnerOn = false;
	spinner.spinning = false;
	toggleButtonsOnInit(false);

	wxLogStatus("Terminated");

}

void MainFrame::onSliderChanged(wxCommandEvent& evt) {

	profile.setYawSpeed(slider->GetValue());
	
}

void MainFrame::onLeftBind(wxCommandEvent& evt) {

	if (!awaitingKeyPress) {

		leftCtrl->ChangeValue("Press Any Key");
		awaitingKeyPress = true;
		bindLeft->Disable();
		toBind = 1;

		toggleButtons(false);
		
	}
	evt.Skip();
}

void MainFrame::onRightBind(wxCommandEvent& evt) {

	if (!awaitingKeyPress) {

		rightCtrl->ChangeValue("Press Any Key");
		awaitingKeyPress = true;
		bindRight->Disable();
		toBind = 2;

		toggleButtons(false);

	}
	evt.Skip();
}

void MainFrame::onKeyBindPress(wxKeyEvent& evt) {

	if (awaitingKeyPress) {

		int keyCode = evt.GetKeyCode();

		wxLogStatus(wxString::Format("Key pressed: %d", keyCode));

		switch (toBind) {

		case 1:
			profile.setLeftBind(std::to_string(keyCode));
			leftCtrl->ChangeValue(std::to_string(keyCode));
			bindLeft->Enable();
			break;

		case 2:
			profile.setRightBind(std::to_string(keyCode));
			rightCtrl->ChangeValue(std::to_string(keyCode));
			bindRight->Enable();
			break;

		default:
			break;
		}
		awaitingKeyPress = false;
		toggleButtons(true);

	}
	evt.Skip();
}

// Honestly just keeping this for future reference
//void MainFrame::running(wxKeyEvent& evt) {
//
//	// Handling left spins
//	int key = evt.GetKeyCode();
//	wxLogStatus(wxString::Format("Key Pressed: %d", key));
//	if (key == std::stoi(profile.leftBind)) {
//		if (spinner.direction == 1 && spinner.spinning) {
//			wxLogStatus("Stopped Spinning");
//			spinner.centerCursor();
//			spinner.spinning = false;
//		}
//		else {
//			wxLogStatus("Spinning Left");
//			spinner.centerCursor();
//			spinner.direction = 1;
//			spinner.spinning = true;
//			spinner.spin();
//		}
//	}
//		// Handling right spins
//	else if (key == std::stoi(profile.rightBind)) {
//		if (spinner.direction == -1 && spinner.spinning) {
//			wxLogStatus("Stopped Spinning");
//			spinner.centerCursor();
//			spinner.spinning = false;
//		}
//		else {
//			wxLogStatus("Spinning Right");
//			spinner.centerCursor();
//			spinner.direction = -1;
//			spinner.spinning = true;
//			spinner.spin();
//		}
//	}
//}

void MainFrame::onSaveClicked(wxCommandEvent& evt) {

	saveFile(profile);
}
