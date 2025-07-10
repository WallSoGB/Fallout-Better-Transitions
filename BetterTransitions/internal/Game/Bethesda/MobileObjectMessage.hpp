#pragma once

struct MobileObjectMessage {
	uint32_t unk00;
	uint32_t unk04;
	uint32_t unk08;
};

ASSERT_SIZE(MobileObjectMessage, 0xC);