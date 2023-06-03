#pragma once
#include <wx/wx.h>
#include "Profile.h"
#include "Spinner.h"

class MainFrame : public wxFrame //Class inheritance
{
public:
	MainFrame(const wxString& title); //Pass by reference
private:
	void createGUI();
	void bindEventHandlers();
	void initCfg();

	void toggleButtons(bool buttonsEnabled);
	void toggleButtonsOnInit(bool running);

	bool awaitingKeyPress = false;
	bool isRunning = false;

	int toBind = 0;

	Profile profile;
	Spinner spinner;

	wxPanel* panel;

	wxStaticText* yawSpeedText;
	wxSlider* slider;

	wxStaticText* bindsText;
	wxTextCtrl* leftCtrl;
	wxButton* bindLeft;
	wxTextCtrl* rightCtrl;
	wxButton* bindRight;
	wxButton* saveButton;

	wxButton* startButton;
	wxButton* stopButton;

	void startClicked(wxCommandEvent& evt);
	void stopClicked(wxCommandEvent& evt);
	void onSliderChanged(wxCommandEvent& evt);
	void onLeftBind(wxCommandEvent& evt);
	void onRightBind(wxCommandEvent& evt);
	void onKeyBindPress(wxKeyEvent& evt);
	void onSaveClicked(wxCommandEvent& evt);

	//void running(wxKeyEvent& evt);
};

