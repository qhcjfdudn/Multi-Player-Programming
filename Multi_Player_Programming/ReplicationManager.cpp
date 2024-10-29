#include"ReplicationManager.h"
#include "GetRequiredBits.h"

void ReplicationManager::ReplicateWorldState(OutputMemoryBitStream& inStream,
	const std::vector<GameObject*>& inAllObjects) {
	// 1. ��ü �����͸� replication�ϴ� ������ ���
	inStream.WriteBits(PT_ReplicationData, GetRequiredBits<PT_MAX>::value);
	// 2. ��� ��ü�� ���
	for (auto go : inAllObjects) {
		ReplicateIntoStream(inStream, go);
	}
}

void ReplicationManager::ReplicateIntoStream(OutputMemoryBitStream& inStream,
	GameObject* inGameObject) {
	// 1. Network ID ���
	inStream.Write(mLinkingContext->GetNetworkId(inGameObject));
	// 2. Class ID ���
	inStream.Write(inGameObject->GetClassId());
	// 3. ��ü ����ȭ ���
	inGameObject->Write(inStream);
}

void ReplicationManager::ReceiveWorld(InputMemoryBitStream& inStream) {
	std::unordered_set<GameObject*> receivedObjects;

	while (inStream.GetRemainingBitCount() > 0) {
		GameObject* receivedGameObject = ReceiveReplicatedObject(inStream);
		receivedObjects.insert(receivedGameObject);
	}

	// ���� ���� replicatedGameObject �� ������ object�� ���ٸ� ������ ����
	for (auto go : mObjectsReplicatedToMe) {
		if (receivedObjects.find(go) == receivedObjects.end()) {
			mLinkingContext->RemoveGameObject(go);
			go->Destroy();
		}
	}

	mObjectsReplicatedToMe = receivedObjects;
}

GameObject* ReplicationManager::ReceiveReplicatedObject(InputMemoryBitStream& inStream) {
	uint32_t networkId;
	uint32_t classId;
	inStream.Read(networkId);
	inStream.Read(classId);

	GameObject* go = mLinkingContext->GetGameObject(networkId);
	if (!go) {
		// ��ü�� ���� ������ networkId�� mLinkingContext�� ����
	}

	go->Read(inStream);

	return go;
}