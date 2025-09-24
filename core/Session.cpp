#include "Session.h"
#include "../images/PBMImage.h"
#include "../images/PGMImage.h"
#include "../images/PPMImage.h"
#include "../images/ImageFactory.h"
#include <iostream>
#include <fstream>
#pragma warning(disable : 4996)
size_t Session::nextID = 1;

Session::Session(const std::vector<std::string>& filenames) {
    load(filenames);
    sessionID = getNextID();
}

Session::Session(const Session& other)
{
    sessionID = getNextID();
    copyFrom(other);
}

Session& Session::operator=(const Session& other)
{
    if (this != &other) {
        free();
        copyFrom(other);
    }
    return *this;
}

Session::~Session() noexcept
{
    free();
}

size_t Session::getSessionID() const
{
    return sessionID;
}

void Session::getSessionInformation() const
{
    std::cout << "Files in session with ID " << sessionID << ":" << std::endl;
    for (size_t i = 0; i < images.size(); i++) {
        std::cout << images[i]->getFilePath() << " - " << images[i]->getFormat() << std::endl;
    }
}

void Session::load(const std::vector<std::string>& filenames)
{
    for (size_t i = 0; i < filenames.size(); i++) {
        NetPBMImage* currentImage = ImageFactory::createImage(filenames[i]);
        images.push_back(currentImage);
        transformStartIndex.push_back(0);
    }
}

void Session::applyTransformation(const ImageTransformer& operation)
{
    undoTransformations.push_back(pendingTransformations);
    redoTransformations.clear();
    pendingTransformations.push_back(operation.clone());
}

void Session::undo()
{
    if (undoTransformations.empty()) {
        return;
    }

    redoTransformations.push_back(cloneTransformations(pendingTransformations));
    pendingTransformations = undoTransformations.back();
    undoTransformations.pop_back();
}

void Session::redo()
{
    if (redoTransformations.empty()) {
        return;
    }
    
    undoTransformations.push_back(cloneTransformations(pendingTransformations));
    clearTransformations(pendingTransformations);

    const std::vector<ImageTransformer*>& toRedo = redoTransformations.back();
    pendingTransformations = cloneTransformations(toRedo);
    redoTransformations.pop_back();
}

void Session::add(const std::string& filePath)
{
    NetPBMImage* newImage = ImageFactory::createImage(filePath);
    images.push_back(newImage);
    transformStartIndex.push_back(pendingTransformations.size());
}


void Session::save() {
    for (size_t i = 0; i < images.size(); i++) {
        try {
            NetPBMImage* copy = images[i]->clone();
            
            for (size_t j = transformStartIndex[i]; j < pendingTransformations.size(); j++) {
                pendingTransformations[j]->apply(*copy);
            }
            copy->save();
            delete copy;
        }
        catch (const std::exception& e) {
            std::cout << e.what();
        }
   }
}

void Session::saveas(const std::vector<std::string>& filePaths)
{
    if (filePaths.size() > images.size()) {
        throw std::runtime_error("Size of images is larger then images in session");
    }

    for (size_t i = 0; i < filePaths.size(); i++) {
        try {
            NetPBMImage* copy = images[i]->clone();

            for (size_t j = transformStartIndex[i]; j < pendingTransformations.size(); j++) {
                pendingTransformations[j]->apply(*copy);
            }

            copy->saveas(filePaths[i]);
            delete copy;
        }
        catch (const std::exception& e) {
            std::cout << "Failed to save as " << filePaths[i] << ": " << e.what() << std::endl;
        }
    }

    for (size_t i = filePaths.size(); i < images.size(); i++) {
        try {
            NetPBMImage* copy = images[i]->clone();

            for (size_t j = 0; j < pendingTransformations.size(); j++) {
                pendingTransformations[j]->apply(*copy);
            }
            copy->save();
            delete copy;
        }
        catch (const std::exception& e) {
            std::cout << e.what();
        }
    }
}

void Session::closeSession()
{
    free();
}

std::vector<ImageTransformer*> Session::cloneTransformations(const std::vector<ImageTransformer*>& original)
{
    std::vector<ImageTransformer*> copy;
    for (size_t i = 0; i < original.size(); i++) {
        copy.push_back(original[i]->clone());
    }
    return copy;
}

void Session::clearTransformations(std::vector<ImageTransformer*>& transformations)
{
    for (size_t i = 0; i < transformations.size(); i++) {
        delete transformations[i];
    }
    transformations.clear();
}

void Session::free()
{
    for (size_t i = 0; i < images.size(); i++) {
        delete images[i];
    }
    images.clear();

    clearTransformations(pendingTransformations);

    for (size_t i = 0; i < undoTransformations.size(); i++) {
        clearTransformations(undoTransformations[i]);
    }
    undoTransformations.clear();

    for (size_t i = 0; i < redoTransformations.size(); i++) {
        clearTransformations(redoTransformations[i]);
    }
    redoTransformations.clear();

    transformStartIndex.clear();
}

void Session::copyFrom(const Session& other)
{
    for (size_t i = 0; i < other.images.size(); i++) {
        images.push_back(other.images[i]->clone());
    }

    pendingTransformations = cloneTransformations(other.pendingTransformations);

    for (size_t i = 0; i < other.undoTransformations.size(); i++) {
        undoTransformations.push_back(cloneTransformations(other.undoTransformations[i]));
    }

    for (size_t i = 0; i < other.redoTransformations.size(); i++) {
        redoTransformations.push_back(cloneTransformations(other.redoTransformations[i]));
    }
}

size_t Session::getNextID()
{
    return nextID++;
}
