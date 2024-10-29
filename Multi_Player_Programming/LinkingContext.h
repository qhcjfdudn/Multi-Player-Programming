#pragma once

#include <unordered_map>

#include "GameObject.h"

// uint32_t Ÿ���� �ĺ��ڴ� ȣ��Ʈ�� �����ϰ� �����Ѵ�.
// GameObject* Ÿ���� ȣ��Ʈ�� �޸� �ּҰ� �ٸ��� �����ȴ�.
class LinkingContext
{
public:
	LinkingContext();

	void AddGameObject(GameObject* inGameObject, uint32_t inNetworkId);
	void RemoveGameObject(GameObject* const inGameObject);

	uint32_t GetNetworkId(
		GameObject* const inGameObject,
		bool inShouldCreateIfNotFound);
	uint32_t GetNetworkId(GameObject* const inGameObject);

	GameObject* GetGameObject(uint32_t inNetworkId);

private:
	std::unordered_map<uint32_t, GameObject*> mNetworkIdToGameObjectMap;
	std::unordered_map<GameObject*, uint32_t> mGameObjectToNetworkIdMap;

	uint32_t mNextNetworkId;
};