#pragma once
#include <vector>
#include <string>
#include "../images/NetPBMImage.h"
#include "../transforms/ImageTransformer.h"

class Session {
public:
	Session(const std::vector<std::string>& filenames);
	Session(const Session& other);
	Session& operator=(const Session& other);
	~Session() noexcept;

	size_t getSessionID() const;
	void getSessionInformation() const;

	void load(const std::vector<std::string>& filenames);
	void applyTransformation(const ImageTransformer& operation);
	void undo();
	void redo();
	void add(const std::string& filePath);
	void save();
	void saveas(const std::vector<std::string>& filenames);
	void closeSession();
private:
	std::vector<ImageTransformer*> cloneTransformations(const std::vector<ImageTransformer*>& original);
	void clearTransformations(std::vector<ImageTransformer*>& transformations);
	void free();
	void copyFrom(const Session& other);
	static size_t getNextID();
private:
	std::vector<NetPBMImage*> images;
	std::vector<ImageTransformer*> pendingTransformations;
	std::vector<size_t> transformStartIndex;
	std::vector<std::vector<ImageTransformer*>> undoTransformations;
	std::vector<std::vector<ImageTransformer*>> redoTransformations;
	size_t sessionID;
	static size_t nextID;
};