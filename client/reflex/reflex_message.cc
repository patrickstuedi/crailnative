#include "reflex_message.h"

ReflexMessage::ReflexMessage(short magic, short type, long long ticket,
                             long long lba, int count)
    : magic_(magic), type_(type), ticket_(ticket), lba_(lba), count_(count) {}

ReflexMessage::ReflexMessage() {}

ReflexMessage::~ReflexMessage() {}

int ReflexMessage::Write(ByteBuffer &buf) const { return 0; }

int ReflexMessage::Update(ByteBuffer &buf) { return 0; }
