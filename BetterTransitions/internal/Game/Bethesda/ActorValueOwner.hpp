#pragma once

#include "ActorValue.hpp"

class Actor;

class ActorValueOwner {
public:
	virtual int32_t		GetBaseActorValueI(ActorValue::Index aeIndex);
	virtual float		GetBaseActorValueF(ActorValue::Index aeIndex);
	virtual int32_t		GetActorValueI(ActorValue::Index aeIndex);
	virtual float		GetActorValueF(ActorValue::Index aeIndex);
	virtual float		Fn_04(ActorValue::Index aeIndex);
	virtual float		GetActorValueDamage(ActorValue::Index aeIndex);
	virtual float		GetPermActorValue(ActorValue::Index aeIndex);
	virtual int32_t		GetPermanentActorValueI(ActorValue::Index aeIndex);
	virtual float		GetPermanentActorValueF(ActorValue::Index aeIndex);
	virtual Actor*		GetActor() const;
	virtual uint16_t	GetLevel() const;

	static bool GetDerivedValue(ActorValueOwner* apOwner, ActorValue::Index aeIndex, float& arVal);
};

ASSERT_SIZE(ActorValueOwner, 0x4);