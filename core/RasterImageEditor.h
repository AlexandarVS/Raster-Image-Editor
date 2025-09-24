#pragma once
#include "Session.h"
#include "../transforms/ImageTransformer.h"
#include <string>
#include <vector>

class RasterImageEditor {
public:
	static RasterImageEditor& getInstance();
	RasterImageEditor(const RasterImageEditor& other) = delete;
	RasterImageEditor& operator=(const RasterImageEditor& other) = delete;

	void run();

private:
	RasterImageEditor();
	~RasterImageEditor() noexcept;
private:
	void inputCommand();
	void showMenu() const;
	void handleCommand(const std::string& input);
	std::vector<std::string> tokenize(const std::string& input);

private:
	void handleLoad(const std::string& input);
	void handleTransformations(const std::string& input);
	void handleUndo(const std::string& input);
	void handleRedo(const std::string& input);
	void handleSave(const std::string& input);
	void handleSaveAs(const std::string& input);
	void handleAdd(const std::string& input);
	void handleSessionInfo(const std::string& input);
	void handleExit(const std::string& input);
	void handleSwitch(const std::string& input);

private:
	ImageTransformer* transformationFactory(const std::vector<std::string>& tokens);
private:
	std::vector<Session*> sessions;
	int currentSessionIndex;
	std::vector<std::string> tokens;
	bool isRunning;
	bool hasUnsavedWork;
};
