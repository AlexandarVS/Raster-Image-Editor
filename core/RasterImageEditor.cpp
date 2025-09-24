#include "RasterImageEditor.h"
#include "../transforms/color/GrayscaleTransform.h"
#include "../transforms/color/MonochromeTransform.h"
#include "../transforms/color/NegativeTransform.h"
#include "../transforms/orientation/FlipLeftTransform.h"
#include "../transforms/orientation/FlipTopTransform.h"
#include "../transforms/orientation/RotateLeftTransform.h"
#include "../transforms/orientation/RotateRightTransform.h"

RasterImageEditor& RasterImageEditor::getInstance() {
	static RasterImageEditor instance;
	return instance;
}

RasterImageEditor::RasterImageEditor() : currentSessionIndex(-1), isRunning(false), hasUnsavedWork(false) {}

RasterImageEditor::~RasterImageEditor() noexcept {
	for (size_t i = 0; i < sessions.size(); i++) {
		delete sessions[i];
	}
	sessions.clear();
}

void RasterImageEditor::run()
{
	isRunning = true;
	showMenu();
	inputCommand();
}

void RasterImageEditor::inputCommand()
{
	std::string input;
	do {
		try {
			std::cout << "> ";
			std::getline(std::cin, input);
			handleCommand(input);
		}
		catch (const std::exception& e) {
			std::cout << e.what() << " ";
		}
	} while (isRunning);
}

void RasterImageEditor::showMenu() const
{
	std::cout << " =============================== RASTER IMAGE EDITOR =============================== " << std::endl;
	std::cout << "!!! ------ To start a new session enter command 'load' ------ !!!" << std::endl;
	std::cout << "!!! ------ This redactor works only with extensions {ppm,pbm,pgm} ------ !!!" << std::endl;
	std::cout << std::endl;
	std::cout << "> load <image01> <image02> ...         | Start a session and load images" << std::endl;
	std::cout << "> switch <session ID>                  | Switches sessions" << std::endl;
	std::cout << "> grayscale                            | Turn all loaded images into grayscale" << std::endl;
	std::cout << "> monochrome                           | Turn all loaded images into monochrome" << std::endl;
	std::cout << "> negative                             | Turn all loaded images into negative" << std::endl;
	std::cout << "> rotate left                          | Rotate images 90 degrees to the left" << std::endl;
	std::cout << "> rotate right                         | Rotate images 90 degrees to the right" << std::endl;
	std::cout << "> flip top                             | Flip images top to bottom" << std::endl;
	std::cout << "> flip left                            | Flip images left to right" << std::endl;
	std::cout << "> undo                                 | Undo the last applied command" << std::endl;
	std::cout << "> redo                                 | Redo the last undone command" << std::endl;
	std::cout << "> add <image>                          | Add a new image to the session" << std::endl;
	std::cout << "> save                                 | Save current images in the session" << std::endl;
	std::cout << "> saveas <image01> <image02> ...       | Save images with specific names" << std::endl;
	std::cout << "> session info                         | Show information about the current session" << std::endl;
	std::cout << "> exit                                 | Exit the session" << std::endl;

}

void RasterImageEditor::handleCommand(const std::string& input)
{
	if (isRunning == false) {
		throw std::runtime_error("Editor is in invalid state");
	}

	tokens = tokenize(input);

	const std::string& command = tokens[0];

	if (command == "load") {
		handleLoad(input);
	}
	else if (command == "exit") {
		handleExit(input);
	}
	else if (command == "switch") {
		handleSwitch(input);
	}
	else if (command == "grayscale" || command == "monochrome" || command == "negative" 
			|| command == "rotate" || command == "flip" || command == "paste") {
		handleTransformations(input);
	}
	else if (command == "add") {
		handleAdd(input);
	}
	else if (command == "undo") {
		handleUndo(input);
	}
	else if (command == "redo") {
		handleRedo(input);
	}
	else if (command == "save") {
		handleSave(input);
	}
	else if (command == "saveas") {
		handleSaveAs(input);
	}
	else if (command == "session") {
		handleSessionInfo(input);
	}
	else if (command == "exit") {
		handleExit(input);
	}
	else {
		tokens.clear();
		throw std::runtime_error("Invalid command!");
	}

	tokens.clear();
}


std::vector<std::string> RasterImageEditor::tokenize(const std::string& input)
{
	std::vector<std::string> tokens;
	std::string token;

	for (size_t i = 0; i < input.size(); i++) {
		char currentSymbol = input[i];
		if (currentSymbol == ' ' || currentSymbol == '\t') {
			if (!token.empty()) {
				tokens.push_back(token);
				token.clear();
			}
		}
		else {
			token += currentSymbol;
		}
	}

	if (!token.empty()) {
		tokens.push_back(token);
	}

	if (tokens.empty()) {
		throw std::runtime_error("Invalid Command!");
	}

	return tokens;
}

void RasterImageEditor::handleLoad(const std::string& input) {
	tokens = tokenize(input);
	if (tokens.size() <= 1 || tokens[0] != "load") {
		throw std::runtime_error("Invalid command!");
	}

	tokens.erase(tokens.begin());
	Session* newSession = new Session(tokens);
	int sessionID = newSession->getSessionID();
	sessions.push_back(newSession);
	currentSessionIndex = sessions.size() - 1;

	std::cout << "Session with ID: " << sessionID << " started and activated." << std::endl;
	hasUnsavedWork = true;
	tokens.clear();
}


void RasterImageEditor::handleSwitch(const std::string& input) {
	tokens = tokenize(input);
	if (tokens.size() != 2 || tokens[0] != "switch") {
		throw std::runtime_error("Invalid command!");
	}

	int newID = std::stoi(tokens[1]);
	bool found = false;

	for (size_t i = 0; i < sessions.size(); i++) {
		if (sessions[i]->getSessionID() == newID) {
			currentSessionIndex = i;
			std::cout << "Switched to session ID: " << newID << std::endl;
			found = true;
			break;
		}
	}

	if (!found) {
		throw std::runtime_error("No session with that ID exists!");
	}

	tokens.clear();
}


void RasterImageEditor::handleTransformations(const std::string& input)
{
	tokens = tokenize(input);
	if (tokens.empty() || currentSessionIndex == -1) {
		throw std::runtime_error("Invalid command!");
	}

	ImageTransformer* transformation = transformationFactory(tokens);
	if (!transformation) {
		throw std::runtime_error("Invalid command!");
	}

	Session* currentSession = sessions[currentSessionIndex];
	currentSession->applyTransformation(*transformation);
	hasUnsavedWork = true;
	tokens.clear();
}

void RasterImageEditor::handleUndo(const std::string& input) {
	tokens = tokenize(input);
	if (tokens.size() != 1 || tokens[0] != "undo" || currentSessionIndex == -1) {
		throw std::runtime_error("Invalid command!");
	}

	Session* currentSession = sessions[currentSessionIndex];
	currentSession->undo();
	hasUnsavedWork = true;
	tokens.clear();
}


void RasterImageEditor::handleRedo(const std::string& input) {
	tokens = tokenize(input);
	if (tokens.size() != 1 || tokens[0] != "redo" || currentSessionIndex == -1) {
		throw std::runtime_error("Invalid command!");
	}

	Session* currentSession = sessions[currentSessionIndex];
	currentSession->redo();
	hasUnsavedWork = true;
	tokens.clear();
}


void RasterImageEditor::handleSave(const std::string& input) {
	tokens = tokenize(input);
	if (tokens.size() != 1 || tokens[0] != "save" || currentSessionIndex == -1) {
		throw std::runtime_error("Invalid command!");
	}

	Session* currentSession = sessions[currentSessionIndex];
	currentSession->save();
	hasUnsavedWork = false;
	tokens.clear();
}


void RasterImageEditor::handleSaveAs(const std::string& input) {
	tokens = tokenize(input);
	if (tokens.size() < 2 || tokens[0] != "saveas" || currentSessionIndex == -1) {
		throw std::runtime_error("Invalid command!");
	}

	Session* currentSession = sessions[currentSessionIndex];
	tokens.erase(tokens.begin());
	currentSession->saveas(tokens);
	hasUnsavedWork = false;
	tokens.clear();
}


void RasterImageEditor::handleAdd(const std::string& input) {
	tokens = tokenize(input);
	if (tokens.size() != 2 || tokens[0] != "add" || currentSessionIndex == -1) {
		throw std::runtime_error("Invalid command!");
	}

	Session* currentSession = sessions[currentSessionIndex];
	currentSession->add(tokens[1]);
	hasUnsavedWork = true;
	tokens.clear();
}


void RasterImageEditor::handleSessionInfo(const std::string& input) {
	tokens = tokenize(input);
	if (tokens.size() != 2 || tokens[0] != "session" || tokens[1] != "info" || currentSessionIndex == -1) {
		throw std::runtime_error("Invalid command!");
	}

	Session* currentSession = sessions[currentSessionIndex];
	currentSession->getSessionInformation();
	tokens.clear();
}


void RasterImageEditor::handleExit(const std::string& input) {
	tokens = tokenize(input);
	if (tokens.size() != 1 || tokens[0] != "exit" || currentSessionIndex == -1) {
		throw std::runtime_error("Invalid command!");
	}

	Session* currentSession = sessions[currentSessionIndex];

	if (hasUnsavedWork) {
		std::cout << "Session contains unsaved work. Save it now (y/n)?" << std::endl;

		char answer;
		std::cin >> answer;
		std::cin.ignore();

		if (answer == 'y' || answer == 'Y') {
			currentSession->save();
			hasUnsavedWork = false;
		}
	}

	currentSession->closeSession();
	delete currentSession;
	sessions.erase(sessions.begin() + currentSessionIndex);

	std::cout << "Session closed." << std::endl;

	if (sessions.empty()) {
		currentSessionIndex = -1;
	}
	else {
		currentSessionIndex = 0;
		std::cout << "Switched to session ID: " << sessions[0]->getSessionID() << std::endl;
	}

	tokens.clear();
}



ImageTransformer* RasterImageEditor::transformationFactory(const std::vector<std::string>& tokens)
{
	if (tokens.empty()) return nullptr;

	const std::string& command = tokens[0];

	if (command == "grayscale") {
		return new GrayscaleTransform();
	}
	else if (command == "monochrome") {
		return new MonochromeTransform();
	}
	else if (command == "negative") {
		return new NegativeTransform();
	}
	else if (command == "rotate" && tokens.size() == 2) {
		if (tokens[1] == "left") return new RotateLeftTransform();
		if (tokens[1] == "right") return new RotateRightTransform();
	}
	else if (command == "flip" && tokens.size() == 2) {
		if (tokens[1] == "left") return new FlipLeftTransform();
		if (tokens[1] == "top") return new FlipTopTransform();
	}
	return nullptr;
}
