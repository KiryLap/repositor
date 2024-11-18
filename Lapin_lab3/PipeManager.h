#ifndef PIPEMANAGER_H
#define PIPEMANAGER_H

#include <iostream>
#include <map>
#include <vector>
#include "Pipe.h"
#include "utils.h"

class PipeManager {
private:
    int nextId = 1;

public:
    int getNextId() const { return nextId; }
    int getLastPipeId();
    std::string savePipe(const Pipe &pipe) const;
    void toggleRepair(Pipe &pipe, int id);
    int addPipeGTN();
    void addPipe();
    void editPipes();
    void deletePipes();
    void displayAllPipes();
    void editPipesFound(const vector<int>& foundIds);
    void deletePipesFound(const vector<int>& foundIds);
    void displayPipeMenuFound();
    void handleFoundPipeMenu(const vector<int>& foundPipeIds);
    void searchPipesName();
    void searchPipesRepair();
    void searchPipeMenu();
    void searchPipeSwitch();
    void displayPipeMenu();
    void PipeSwitch();
};

#endif // PIPEMANAGER_H