#include "Watcher.h"


Watcher::Watcher() {
	listeners = std::vector<WatcherCallback*>();
}

void Watcher::addWatcher(WatcherCallback* w) {
	listeners.push_back(w);
}

void Watcher::fire() {
	for ( uint i = 0; i < listeners.size() ; i++ )
		listeners.at(i)->dataChanged();
}








