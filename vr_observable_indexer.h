#pragma once
#include <vr_types.h>

struct KeysObserver
{
	virtual void NewVRKeysUpdate(const VRKeysUpdate &e) = 0;
};

struct ObservableKeysIndexer
{
	virtual void RegisterObserver(KeysObserver *) = 0;
	virtual void UnRegisterObserver(KeysObserver *) = 0;
};

struct BasicObservableKeysIndexer : public ObservableKeysIndexer
{
	virtual void RegisterObserver(KeysObserver *observer) override
	{
		observers.push_back(observer);
	}
	virtual void UnRegisterObserver(KeysObserver *observer) override
	{
		std::remove(observers.begin(), observers.end(), observer);
	}
protected:
	void NotifyObservers(const VRKeysUpdate &e)
	{
		for (auto observer : observers)
		{
			observer->NewVRKeysUpdate(e);
		}
	}
	std::vector<KeysObserver *> observers;
};
