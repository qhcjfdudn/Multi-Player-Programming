#include <unordered_map>

#include "GameObject.h"

// uint32_t Ÿ���� �ĺ��ڴ� ȣ��Ʈ�� �����ϰ� �����Ѵ�.
// GameObject* Ÿ���� ȣ��Ʈ�� �޸� �ּҰ� �ٸ��� �����ȴ�.
class LinkingContext
{
public:
	LinkingContext() : mNextNetworkId(1) {}

	void AddGameObject(GameObject* inGameObject, uint32_t inNetworkId)
	{
		mGameObjectToNetworkIdMap[inGameObject] = inNetworkId;
		mNetworkIdToGameObjectMap[inNetworkId] = inGameObject;
	}
	void RemoveGameObject(GameObject* const inGameObject)
	{
		uint32_t networkId = mGameObjectToNetworkIdMap[inGameObject];
		mGameObjectToNetworkIdMap.erase(inGameObject);
		mNetworkIdToGameObjectMap.erase(networkId);
	}

	uint32_t GetNetworkId(
		GameObject* const inGameObject,
		bool inShouldCreateIfNotFound)
	{
		auto it = mGameObjectToNetworkIdMap.find(inGameObject);
		if (it != mGameObjectToNetworkIdMap.end())
			return it->second;
		else if (inShouldCreateIfNotFound)
		{
			uint32_t networkId = mNextNetworkId++;
			AddGameObject(inGameObject, networkId);
			return networkId;
		}
		else return 0;
	}
	uint32_t GetNetworkId(GameObject* const inGameObject)
	{
		auto it = mGameObjectToNetworkIdMap.find(inGameObject);
		if (it != mGameObjectToNetworkIdMap.end())
			return it->second;
		else
			return 0;
	}

	GameObject* GetGameObject(uint32_t inNetworkId)
	{
		auto it = mNetworkIdToGameObjectMap.find(inNetworkId);
		if (it != mNetworkIdToGameObjectMap.end())
			return it->second;
		else
			return nullptr;
	}

private:
	std::unordered_map<uint32_t, GameObject*> mNetworkIdToGameObjectMap;
	std::unordered_map<GameObject*, uint32_t> mGameObjectToNetworkIdMap;

	uint32_t mNextNetworkId;
};