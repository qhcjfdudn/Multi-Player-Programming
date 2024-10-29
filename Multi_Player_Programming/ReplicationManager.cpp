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