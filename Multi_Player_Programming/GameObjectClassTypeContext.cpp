#include "OutputMemoryBitStream.h"
#include "RoboCat.h"

class GameObjectClassTypeContext {
public:
	void WriteClassType(
		OutputMemoryBitStream& inStream,
		const GameObject* inGameObject)
	{
		if (dynamic_cast<const RoboCat*>(inGameObject))
			// 'RBCT' == 1380074324
			// '0' == 48
			// '00' == 48 << 8 + 48 = 12336
			// ��������ǥ ���� char 4�ڱ��� biject�� int�� ���� �ٲ۴�.
			// ���� �Ʒ��� ���� static_cast�� �����Ѵ�.
			inStream.Write(static_cast<uint32_t>('RBCT'));
		// else if ...
	}
};