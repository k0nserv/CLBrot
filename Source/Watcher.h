#ifndef WATCHER_H
#define WATCHER_H

#include <vector>

#include "WatcherCallback.h"
#include "Definitions.h"


class Watcher {
public:
	Watcher();

	void addWatcher(WatcherCallback*);
	void fire();

private:
	std::vector<WatcherCallback*> listeners; 

};
#endif