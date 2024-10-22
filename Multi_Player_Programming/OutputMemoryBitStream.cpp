#include "OutputMemoryBitStream.h"

void OutputMemoryBitStream::WriteBits(uint8_t inData, size_t inBitCount)
{
	// inBitCount�� ���� buffer�� ����� �÷����� �Ǵ�
	uint32_t nextBitHead = mBitHead + static_cast<uint32_t>(inBitCount);
	if (nextBitHead > mBitCapacity) {
		ReallocBuffer(std::max(mBitCapacity * 2, nextBitHead));
	}

	uint32_t byteOffset = mBitHead >> 3;
	uint32_t bitOffset = mBitHead & 7;

	// ���� ó�� �� ����Ʈ�� inData �� �� �� �ִ� ��ŭ �켱 ����
	uint8_t currentMask = ~(0xff << bitOffset);
	mBuffer[byteOffset] = (mBuffer[byteOffset] & currentMask) | (inData << bitOffset);

	// inData�� ���� ���� ���� �κ� ����
	uint32_t bitsFreeThisByte = 8 - bitOffset;
	if (inBitCount > bitsFreeThisByte) {
		mBuffer[byteOffset + 1] = inData >> bitsFreeThisByte;
	}

	mBitHead += nextBitHead;
}

void OutputMemoryBitStream::WriteBits(const void* inData, size_t inBitCount)
{
	const char* srcByte = static_cast<const char*>(inData);

	while (inBitCount > 8) {
		WriteBits(*srcByte, 8);
		srcByte++;
		inBitCount -= 8;
	}

	if (inBitCount > 0) {
		WriteBits(*srcByte, inBitCount);
	}
}