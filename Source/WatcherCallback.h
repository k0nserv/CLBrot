#ifndef WATCHER_CALLBACK_H
#define WATCHER_CALLBACK_H

class WatcherCallback {
public:
	virtual void dataChanged() = 0; 
};
#endif