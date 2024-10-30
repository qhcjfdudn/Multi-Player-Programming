#include"ReplicationManager.h"
#include "GetRequiredBits.h"
#include "ReplicationHeader.h"

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

void ReplicationManager::ReplicateCreate(OutputMemoryBitStream& inStream,
	GameObject* inGameObject)
{
	ReplicationHeader rh(ReplicationAction::RA_Create,
		mLinkingContext->GetNetworkId(inGameObject, true),
		inGameObject->GetClassId());

	rh.Write(inStream);
	inGameObject->Write(inStream);
}

void ReplicationManager::ReplicateUpdate(OutputMemoryBitStream& inStream,
	GameObject* inGameObject)
{
	ReplicationHeader rh(ReplicationAction::RA_Update,
		mLinkingContext->GetNetworkId(inGameObject, false),
		inGameObject->GetClassId());

	rh.Write(inStream);
	inGameObject->Write(inStream);
}

void ReplicationManager::ReplicateDestroy(OutputMemoryBitStream& inStream,
	GameObject* inGameObject)
{
	ReplicationHeader rh(ReplicationAction::RA_Destroy,
		mLinkingContext->GetNetworkId(inGameObject, false));

	rh.Write(inStream);
}

void ReplicationManager::ProcessReplicationAction(InputMemoryBitStream& inStream)
{
	ReplicationHeader rh;
	rh.Read(inStream);

	switch (rh.mReplicationAction)
	{
	case ReplicationAction::RA_Create:
		// CreateRegistry::Get() �޼��尡 �ִٸ� ��ü ����. �ƴ϶�� rh.mClassId�� ���� ����.
		GameObject* go = nullptr;

		mLinkingContext->AddGameObject(go, rh.mNetworkId);
		go->Read(inStream);
		
		break;

	case ReplicationAction::RA_Update:
		GameObject* go = mLinkingContext->GetGameObject(rh.mNetworkId);

		if (go) {
			go->Read(inStream);
		}
		else {
			// ���� ReplicateCreate�� ���� ���� ���·� ���޵Ǿ��� �� �ִ�.
			// ���� stream�� �б� ���� ��·�� stream�� head�� �������Ѿ� �ϱ� ������, 
			// �������� ��ó�� ������ �� ��ü ����Ѵ�.
			// CreateRegistry::Get() �޼��尡 �ִٸ� ��ü ����. �ƴ϶�� rh.mClassId�� ���� ����.
			go = nullptr;
			go->Read(inStream);
			delete go;
		}
		
		break;

	case ReplicationAction::RA_Destroy:
		GameObject* go = mLinkingContext->GetGameObject(rh.mNetworkId);
		mLinkingContext->RemoveGameObject(go);
		go->Destroy();
		
		break;
		
	default:
		break;
	}
}