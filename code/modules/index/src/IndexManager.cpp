#include "IndexManager.h"

IndexManager::IndexManager() {
    bufPageManager = new BufPageManager(new FileManager);
}