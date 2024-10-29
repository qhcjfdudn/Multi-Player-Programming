#include "LinkingContext.h"

// uint32_t Ÿ���� �ĺ��ڴ� ȣ��Ʈ�� �����ϰ� �����Ѵ�.
// GameObject* Ÿ���� ȣ��Ʈ�� �޸� �ּҰ� �ٸ��� �����ȴ�.

LinkingContext::LinkingContext() : mNextNetworkId(1) {}

void LinkingContext::AddGameObject(GameObject* inGameObject, uint32_t inNetworkId)
{
	mGameObjectToNetworkIdMap[inGameObject] = inNetworkId;
	mNetworkIdToGameObjectMap[inNetworkId] = inGameObject;
}
void LinkingContext::RemoveGameObject(GameObject* const inGameObject)
{
	uint32_t networkId = mGameObjectToNetworkIdMap[inGameObject];
	mGameObjectToNetworkIdMap.erase(inGameObject);
	mNetworkIdToGameObjectMap.erase(networkId);
}

uint32_t LinkingContext::GetNetworkId(
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
uint32_t LinkingContext::GetNetworkId(GameObject* const inGameObject)
{
	auto it = mGameObjectToNetworkIdMap.find(inGameObject);
	if (it != mGameObjectToNetworkIdMap.end())
		return it->second;
	else
		return 0;
}

GameObject* LinkingContext::GetGameObject(uint32_t inNetworkId)
{
	auto it = mNetworkIdToGameObjectMap.find(inNetworkId);
	if (it != mNetworkIdToGameObjectMap.end())
		return it->second;
	else
		return nullptr;
}