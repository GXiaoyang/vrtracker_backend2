#pragma once
#include <vr_types.h>

struct IndexerObserver
{
	virtual void NewVRConfigEvent(const VRConfigEvent &e) = 0;
};

struct ObservableIndexer
{
	virtual void RegisterObserver(IndexerObserver *) = 0;
	virtual void UnRegisterObserver(IndexerObserver *) = 0;
};

struct BasicObservableIndexer : public ObservableIndexer
{
	virtual void RegisterObserver(IndexerObserver *observer) override
	{
		observers.push_back(observer);
	}
	virtual void UnRegisterObserver(IndexerObserver *observer) override
	{
		std::remove(observers.begin(), observers.end(), observer);
	}
protected:
	void NotifyObservers(const VRConfigEvent &e)
	{
		for (auto observer : observers)
		{
			observer->NewVRConfigEvent(e);
		}
	}
	std::vector<IndexerObserver *> observers;
};
